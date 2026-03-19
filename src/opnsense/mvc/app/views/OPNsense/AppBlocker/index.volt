{#
 # Copyright (C) 2024 Liyander
 # All rights reserved.
 #}

<script>
    $( document ).ready(function() {
        var data_get_map = {'frm_general_settings':"/api/appblocker/settings/get"};
        mapDataToFormUI(data_get_map).done(function(data){
            formatTokenizersUI();
            $('.selectpicker').selectpicker('refresh');
        });

        $("#saveAct").click(function(){
            saveFormToEndpoint(url="/api/appblocker/settings/set", formid='frm_general_settings',callback_ok=function(){
                $("#saveAct_progress").addClass("fa fa-spinner fa-pulse");
                ajaxCall(url="/api/appblocker/service/reload", sendData={}, callback=function(data,status) {
                    $("#saveAct_progress").removeClass("fa fa-spinner fa-pulse");
                });
            });
        });
    });
</script>

<div class="content-box">
    <div class="content-box-main">
        <div class="table-responsive">
            <form id="frm_general_settings">
                <table class="table table-striped table-condensed table-hover">
                    <tbody>
                        <tr>
                            <td colspan="2" valign="top" class="listtopic">General Settings</td>
                        </tr>
                        <tr>
                            <td><i class="fa fa-info-circle text-muted"></i> {{ lang._('Enable AppBlocker') }}</td>
                            <td>
                                <input type="checkbox" id="appblocker.general.enabled" value="1" />
                            </td>
                        </tr>
                        <tr>
                            <td><i class="fa fa-info-circle text-muted"></i> {{ lang._('Blocked Domains') }}</td>
                            <td>
                                <input type="text" id="appblocker.general.blocked_domains" />
                                <div class="hidden" data-for="help_for_appblocker.general.blocked_domains">
                                    <small>{{ lang._('Enter comma separated list of domains to block (e.g. example.com, test.org)') }}</small>
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td><i class="fa fa-info-circle text-muted"></i> {{ lang._('Block DoH/DoT/WARP Providers') }}</td>
                            <td>
                                <input type="checkbox" id="appblocker.general.block_dns_bypass" value="1" />
                                <div class="hidden" data-for="help_for_appblocker.general.block_dns_bypass">
                                    <small>{{ lang._('Block access to popular DNS-over-HTTPS/TLS providers (Google, Cloudflare) and VPN/WARP bootstrap domains.') }}</small>
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td><i class="fa fa-info-circle text-muted"></i> {{ lang._('Block Social Media/Apps') }}</td>
                            <td>
                                <select id="appblocker.general.social_media" class="selectpicker" multiple="multiple" data-live-search="true" data-size="10" data-selected-text-format="count > 3" data-actions-box="true">
                                   <option value="facebook">Facebook</option>
                                   <option value="twitter">Twitter</option>
                                   <option value="instagram">Instagram</option>
                                   <option value="tiktok">TikTok</option>
                                   <option value="youtube">YouTube</option>
                                   <option value="whatsapp">WhatsApp</option>
                                   <option value="telegram">Telegram</option>
                                   <option value="discord">Discord</option>
                                   <option value="reddit">Reddit</option>
                                   <option value="netflix">Netflix</option>
                                   <option value="twitch">Twitch</option>
                                   <option value="pinterest">Pinterest</option>
                                   <option value="snapchat">Snapchat</option>
                                   <option value="spotify">Spotify</option>
                                </select>
                            </td>
                        </tr>
                        <tr>
                            <td><i class="fa fa-info-circle text-muted"></i> {{ lang._('Block Gaming Platforms') }}</td>
                            <td>
                                <select id="appblocker.general.games" class="selectpicker" multiple="multiple" data-live-search="true" data-size="10" data-selected-text-format="count > 3" data-actions-box="true">
                                   <option value="roblox">Roblox</option>
                                   <option value="steam">Steam</option>
                                   <option value="epicgames">Epic Games</option>
                                   <option value="ea">EA / Origin</option>
                                   <option value="minecraft">Minecraft</option>
                                   <option value="riot">Riot Games (LoL/Valorant)</option>
                                   <option value="blizzard">Battle.net / Blizzard</option>
                                   <option value="playstation">PlayStation Network</option>
                                   <option value="xbox">Xbox Live</option>
                                   <option value="nintendo">Nintendo</option>
                                   <option value="coc">Clash of Clans / Supercell</option>
                                   <option value="freefire">Garena Free Fire</option>
                                   <option value="pubg">PUBG Mobile / Tencent</option>
                                   <option value="cod">Call of Duty Mobile</option>
                                   <option value="genshin">Genshin Impact / HoYoverse</option>
                                   <option value="mobilelegends">Mobile Legends</option>
                                </select>
                            </td>
                        </tr>
                        <tr>
                            <td>&nbsp;</td>
                            <td>
                                <button id="saveAct" type="button" class="btn btn-primary" value="Save">
                                    {{ lang._('Save Changes') }} <i id="saveAct_progress"></i>
                                </button>
                            </td>
                        </tr>
                    </tbody>
                </table>
            </form>
        </div>
    </div>
</div>
