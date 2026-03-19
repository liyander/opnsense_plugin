<?php

function appblocker_services()
{
    $services = array();

    $services[] = array(
        'description' => gettext('AppBlocker'),
        'configd' => array(
            'restart' => array('appblocker restart'),
            'start' => array('appblocker start'),
            'stop' => array('appblocker stop'),
        ),
        'name' => 'appblocker',
    );

    return $services;
}

/**
 * Register automatic firewall rules for AppBlocker
 */
function appblocker_firewall(\OPNsense\Firewall\Plugin $fw)
{
    // Load the model
    $mdl = new \OPNsense\AppBlocker\AppBlocker();

    // Check modes
    $is_blocklist_enabled = ($mdl->general->enabled == '1');
    $is_whitelist_enabled = ($mdl->whitelistmode->enabled == '1');

    if (!$is_blocklist_enabled && !$is_whitelist_enabled) {
        return; // nothing to do
    }

    // --- 1. DEEP HARDENING: Block DNS Bypass Mechanisms ---
    // Enforce VPN/DoT drop either via toggle or strictly in Whitelist mode
    if ($mdl->general->block_dns_bypass == '1' || $is_whitelist_enabled) {
        // Block DoT (853) and known VPN tunnel ports
        $bypass_ports = ['853', '500', '4500', '1701', '51820', '2408'];
        foreach ($bypass_ports as $port) {
            $fw->registerFilterRule(
                5,
                [
                    'type' => 'block',
                    'protocol' => 'tcp/udp',
                    'direction' => 'in',
                    'to_port' => $port,
                    'descr' => "AppBlocker: Hardening - Block VPN/DoT Port {$port}"
                ]
            );
        }

        // Force Local DNS: Block outbound DNS queries on port 53 that are NOT heading to the firewall
        $fw->registerFilterRule(
            6,
            [
                'type' => 'block',
                'protocol' => 'tcp/udp',
                'direction' => 'in',
                'to_not' => true,
                'to' => '(self)',
                'to_port' => '53',
                'descr' => "AppBlocker: Hardening - Force Local DNS"
            ]
        );
    }

    // --- 2. Master list of IPs map ---
    $app_ips = [
        'telegram' => ['91.108.4.0/22', '91.108.8.0/22', '91.108.12.0/22', '91.108.16.0/22', '91.108.20.0/22', '91.108.56.0/22', '149.154.160.0/20', '185.76.151.0/24', '2001:b28::/32', '2001:67c:4e8::/48'],
        'facebook' => ['31.13.24.0/21', '31.13.64.0/18', '66.220.144.0/20', '69.63.176.0/20', '69.171.224.0/19', '74.119.76.0/22', '103.4.96.0/22', '129.253.0.0/16', '157.240.0.0/16', '173.252.64.0/18', '204.15.20.0/22'],
        'whatsapp' => ['31.13.24.0/21', '31.13.64.0/18', '66.220.144.0/20', '69.63.176.0/20', '69.171.224.0/19', '74.119.76.0/22', '103.4.96.0/22', '129.253.0.0/16', '157.240.0.0/16', '173.252.64.0/18', '204.15.20.0/22'],
        'instagram' => ['31.13.24.0/21', '31.13.64.0/18', '66.220.144.0/20', '69.63.176.0/20', '69.171.224.0/19', '74.119.76.0/22', '103.4.96.0/22', '129.253.0.0/16', '157.240.0.0/16', '173.252.64.0/18', '204.15.20.0/22'],
        'tiktok' => ['161.117.0.0/16', '103.111.228.0/22', '103.136.220.0/22', '103.118.122.0/23', '137.189.0.0/16'],
        'discord' => ['162.159.128.0/18', '162.159.135.0/24'],
        'netflix' => ['45.57.0.0/17', '108.175.32.0/20', '198.38.96.0/19', '198.45.48.0/20'],
        'youtube' => ['142.250.0.0/15', '142.251.0.0/16', '172.217.0.0/16', '173.194.0.0/16', '216.58.192.0/19', '209.85.128.0/17', '74.125.0.0/16', '204.239.164.0/22', '199.223.232.0/21'],
        'twitter' => ['104.244.42.0/21', '199.16.156.0/22', '199.59.148.0/22', '199.96.56.0/21'],
        'reddit' => ['151.101.0.0/16', '146.75.0.0/16'],
        'twitch' => ['99.181.64.0/18', '192.108.239.0/24', '52.223.192.0/18', '13.32.0.0/15'],
        'snapchat' => ['104.193.252.0/22', '35.240.0.0/13'],
        'spotify' => ['104.154.0.0/15', '35.184.0.0/13', '151.101.0.0/16'],
        'pubg' => ['43.128.0.0/16', '43.129.0.0/16', '43.134.0.0/16', '43.156.0.0/16', '43.158.0.0/16', '119.28.0.0/16', '129.226.0.0/16', '150.109.0.0/16', '170.106.0.0/16'],
        'freefire' => ['13.250.0.0/16', '119.8.0.0/16', '119.28.0.0/16'],
        'cod' => ['185.34.104.0/22', '185.34.106.0/24', '199.204.62.0/24'],
        'roblox' => ['128.116.0.0/16', '170.21.64.0/22', '170.21.112.0/21', '209.20.158.0/23']
    ];

    $ips_to_block = [];

    // --- 3. Dynamic IP logic based on Mode ---
    if ($is_whitelist_enabled) {
        // WHITELIST MODE: Block EVERYTHING that we have IPs for, EXCEPT what is explicitly allowed.
        $allowed_apps = explode(",", (string)$mdl->whitelistmode->allowed_apps);
        
        foreach ($app_ips as $app_name => $subnets) {
            // If the app is NOT in the allowed list, BLOCK IT.
            if (!in_array($app_name, $allowed_apps)) {
                $ips_to_block = array_merge($ips_to_block, $subnets);
            }
        }
    } else {
        // GENERAL BLOCKLIST MODE: Block ONLY what is explicitly checked
        $social_media = explode(",", (string)$mdl->general->social_media);
        $games_selected = explode(",", (string)$mdl->general->games);
        $all_blocked_apps = array_merge($social_media, $games_selected);

        foreach ($app_ips as $app_name => $subnets) {
            if (in_array($app_name, $all_blocked_apps)) {
                $ips_to_block = array_merge($ips_to_block, $subnets);
            }
        }
    }

    // --- 4. Write Firewall Rules ---
    foreach (array_unique($ips_to_block) as $ip) {
        if (!empty($ip)) {
            $fw->registerFilterRule(
                15,
                [
                    'type' => 'block',
                    'protocol' => 'tcp/udp',
                    'direction' => 'in',
                    'to' => $ip,
                    'descr' => "AppBlocker: Hardened IP Restrict {$ip}",
                ]
            );
        }
    }
}
