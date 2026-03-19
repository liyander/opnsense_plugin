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
    
    // Check if the plugin is enabled
    if ($mdl->general->enabled == '1') {
        
        // --- 1. DEEP HARDENING: Block DNS Bypass Mechanisms ---
        if ($mdl->general->block_dns_bypass == '1') {
            // Block DoT (853) and known VPN tunnel ports (WireGuard, IPsec, OpenVPN, WARP)
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
            // This stops devices with hardcoded 8.8.8.8 from exiting the network.
            $fw->registerFilterRule(
                6,
                [
                    'type' => 'block',
                    'protocol' => 'tcp/udp',
                    'direction' => 'in',
                    'to_not' => true,
                    'to' => '(self)',
                    'to_port' => '53',
                    'descr' => "AppBlocker: Hardening - Force Local DNS (Block rogue port 53)"
                ]
            );
        }

        // Aggregate IPs to completely block
        $ips_to_block = [];

        $social_media = (string)$mdl->general->social_media;
        $apps = explode(",", $social_media);

        if (in_array('telegram', $apps)) {
            $ips_to_block = array_merge($ips_to_block, [
                '91.108.4.0/22', '91.108.8.0/22', '91.108.12.0/22', '91.108.16.0/22',
                '91.108.20.0/22', '91.108.56.0/22', '149.154.160.0/20', '185.76.151.0/24',
                '2001:b28::/32', '2001:67c:4e8::/48'
            ]);
        }

        if (in_array('facebook', $apps) || in_array('whatsapp', $apps) || in_array('instagram', $apps)) {
            // Comprehensive Meta/Facebook ASNs
            $ips_to_block = array_merge($ips_to_block, [
                '31.13.24.0/21', '31.13.64.0/18', '66.220.144.0/20', '69.63.176.0/20',
                '69.171.224.0/19', '74.119.76.0/22', '103.4.96.0/22', '129.253.0.0/16',
                '157.240.0.0/16', '173.252.64.0/18', '204.15.20.0/22'
            ]);
        }

        if (in_array('tiktok', $apps)) {
            $ips_to_block = array_merge($ips_to_block, [
                '161.117.0.0/16', '103.111.228.0/22', '103.136.220.0/22', '103.118.122.0/23', '137.189.0.0/16'
            ]);
        }

        if (in_array('discord', $apps)) {
            // Discord Voice and Media (Cloudflare subsets used by Discord)
            $ips_to_block = array_merge($ips_to_block, [
                '162.159.128.0/18', '162.159.135.0/24'
            ]);
        }

        if (in_array('netflix', $apps)) {
            $ips_to_block = array_merge($ips_to_block, [
                '45.57.0.0/17', '108.175.32.0/20', '198.38.96.0/19', '198.45.48.0/20'
            ]);
        }

        // --- Games IP Blocking ---
        $games_selected = (string)$mdl->general->games;
        $active_games = explode(",", $games_selected);

        if (in_array('pubg', $active_games)) {
            $ips_to_block = array_merge($ips_to_block, [
                '43.128.0.0/16', '43.129.0.0/16', '43.134.0.0/16', '43.156.0.0/16',
                '43.158.0.0/16', '119.28.0.0/16', '129.226.0.0/16', '150.109.0.0/16', '170.106.0.0/16'
            ]);
        }

        if (in_array('freefire', $active_games)) {
            $ips_to_block = array_merge($ips_to_block, [
                '13.250.0.0/16', '119.8.0.0/16', '119.28.0.0/16'
            ]);
        }

        if (in_array('cod', $active_games)) {
            $ips_to_block = array_merge($ips_to_block, [
                '185.34.104.0/22', '185.34.106.0/24', '199.204.62.0/24'
            ]);
        }

        if (in_array('roblox', $active_games)) {
            $ips_to_block = array_merge($ips_to_block, [
                '128.116.0.0/16', '170.21.64.0/22', '170.21.112.0/21', '209.20.158.0/23'
            ]);
        }

        // Write rules for all gathered IPs
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
