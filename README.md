# OPNsense AppBlocker Plugin

A lightweight and effective OPNsense plugin that blocks access to specific websites, domains, and popular mobile applications (e.g., Facebook, YouTube, TikTok) using Unbound DNS overrides.

---

## 🚀 Features

### 🔒 Domain Blocking

* Manually block specific domains
* Examples:

  * `ads.example.com`
  * `malware-site.org`

### 📱 App Blocking (One-click)

Block widely used platforms:

* Facebook
* Twitter (X)
* Instagram
* TikTok
* YouTube
* WhatsApp
* Telegram
* Discord
* Reddit
* Netflix
* Twitch
* Pinterest
* Snapchat
* Spotify

### 🎮 Game Blocking

Block major gaming ecosystems:

* Roblox
* Steam
* Epic Games
* EA / Origin
* Minecraft
* Riot Games (LoL / Valorant)
* Battle.net / Blizzard
* PlayStation Network
* Xbox Live
* Nintendo
* Clash of Clans / Supercell
* Garena Free Fire
* PUBG Mobile / Tencent
* Call of Duty Mobile
* Genshin Impact / HoYoverse
* Mobile Legends

### 🌟 Strict Whitelist Mode

Want maximum control? Enable **Whitelist Mode** to block **all** DNS resolution across the network, except for specifically approved domains and apps. Ideal for highly restricted environments, testing labs, or extreme parental controls.

### ⚡ Lightweight Design

* Uses native Unbound DNS service
* No external proxies
* No deep packet inspection required

---

## 🛠️ Installation (Manual / Development)

### ✅ Prerequisites

* OPNsense (tested on 24.x / 25.x)
* Root (SSH) access

### 📥 Steps

#### 1. Copy Files to OPNsense

Upload the `src` directory contents:

```bash
scp -r src/* root@<opnsense-ip>:/usr/local/
```

> This merges plugin files into:
>
> * `/usr/local/opnsense/...`
> * `/usr/local/etc/...`

#### 2. Register the Plugin

```bash
# Restart configd
service configd restart

# Reload GUI + ACLs
configctl template reload OPNsense/WebGui
configctl webgui restart

# Generate plugin config
configctl template reload Liyander/AppBlocker
```

#### 3. Verify Installation

Navigate in Web UI:

**Services → App Blocker**

> ⚠️ If you see:
> `ACL xml seems to be of wrong type`
> Ensure `ACL.xml` starts directly with `<model>` (no whitespace)

---

## ⚙️ Usage

1. Go to:
   **Services → App Blocker → Settings**
2. Enable the service
3. Add blocked domains (comma-separated)

   * Example: `gaming.com, gambling.net`
4. Select apps/services to block
5. Click **Save Changes**

### 🔄 What Happens Internally

* Generates Unbound config file
* Reloads DNS service automatically

---

## ⚙️ How It Works

Generated config path:

```
/usr/local/etc/unbound.opnsense.d/appblocker.conf
```

### Example:

```text
local-zone: "facebook.com" redirect
local-data: "facebook.com A 0.0.0.0"
local-zone: "fbcdn.net" redirect
local-data: "fbcdn.net A 0.0.0.0"
```

➡️ DNS requests are redirected to `0.0.0.0` (null routing), blocking traffic before it leaves the network.

---

## 🛡️ Hardening Against Bypass

Modern devices bypass DNS filters using DoT/DoH or hardcoded DNS.

### 1️⃣ Block DoT (DNS over TLS)

* Path: **Firewall → Rules → LAN**

| Field    | Value   |
| -------- | ------- |
| Action   | Block   |
| Protocol | TCP/UDP |
| Source   | LAN net |
| Port     | 853     |

---

### 2️⃣ Force Local DNS (Redirect)

* Path: **Firewall → NAT → Port Forward**

| Field         | Value         |
| ------------- | ------------- |
| Interface     | LAN           |
| Protocol      | TCP/UDP       |
| Source        | LAN net       |
| Destination   | ! LAN Address |
| Port          | 53            |
| Redirect IP   | 127.0.0.1     |
| Redirect Port | 53            |

➡️ Captures traffic to external DNS (8.8.8.8, 1.1.1.1)

---

### 3️⃣ Block VPN Bypass

#### Block VPN Ports

* UDP: `500, 4500, 1701, 51820`

#### Block Cloudflare WARP

* Port: `2408`

---

## 🧪 Troubleshooting

### ❌ Sites Not Blocked

* Ensure client uses OPNsense as DNS
* Flush DNS:

  * Windows: `ipconfig /flushdns`
  * Mobile: toggle airplane mode
* Check Unbound:

```bash
pluginctl -s unbound status
```

### ❌ UI Not Showing

```bash
configctl webgui restart
```

---

## 🗑️ Uninstallation

```bash
rm -rf /usr/local/opnsense/mvc/app/controllers/OPNsense/AppBlocker
rm -rf /usr/local/opnsense/mvc/app/models/OPNsense/AppBlocker
rm -rf /usr/local/opnsense/mvc/app/views/OPNsense/AppBlocker
rm -rf /usr/local/opnsense/scripts/OPNsense/AppBlocker
rm -rf /usr/local/opnsense/service/conf/actions.d/actions_appblocker.conf
rm -rf /usr/local/opnsense/service/templates/OPNsense/AppBlocker
rm -rf /usr/local/etc/inc/plugins.inc.d/appblocker.inc
rm -f /usr/local/etc/unbound.opnsense.d/appblocker.conf

# Restart services
service configd restart
configctl webgui restart
pluginctl -s unbound restart
```

---

## 📌 Summary

A minimal, efficient DNS-based blocking plugin for OPNsense that:

* Blocks apps, domains, and games
* Requires zero heavy inspection
* Integrates natively with Unbound
* Supports bypass hardening with firewall rules
