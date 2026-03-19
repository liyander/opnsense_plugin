{#
 # Copyright (C) 2024 Liyander
 # All rights reserved.
 #}

<script>
    $( document ).ready(function() {
        var data_get_map = {'frm_whitelist_settings':"/api/appblocker/settings/get"};
        mapDataToFormUI(data_get_map).done(function(data){
            formatTokenizersUI();
            $('.selectpicker').selectpicker('refresh');
        });

        $("#saveAct").click(function(){
            saveFormToEndpoint(url="/api/appblocker/settings/set", formid='frm_whitelist_settings',callback_ok=function(){
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
            <form id="frm_whitelist_settings">
                <table class="table table-striped table-condensed table-hover">
                    <tbody>
                        <tr>
                            <td colspan="2" valign="top" class="listtopic">Strict Whitelist Mode <small>(Danger: Blocks everything else)</small></td>
                        </tr>
                        <tr>
                            <td style="width:22%"><i class="fa fa-info-circle text-muted"></i> {{ lang._('Enable Whitelist Mode') }}</td>
                            <td>
                                <input type="checkbox" id="appblocker.whitelistmode.enabled" value="1" />
                                <div class="hidden" data-for="help_for_appblocker.whitelistmode.enabled">
                                    <small>{{ lang._('WARNING: Enabling this will block ALL regular web access and ONLY allow domains/apps explicitly approved below.') }}</small>
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td><i class="fa fa-info-circle text-muted"></i> {{ lang._('Allowed Domains') }}</td>
                            <td>
                                <input type="text" id="appblocker.whitelistmode.allowed_domains" />
                                <div class="hidden" data-for="help_for_appblocker.whitelistmode.allowed_domains">
                                    <small>{{ lang._('Enter comma separated list of domains that are ALLOWED (e.g. google.com, mycompany.com)') }}</small>
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td><i class="fa fa-info-circle text-muted"></i> {{ lang._('Allowed Apps & Services') }}</td>
                            <td>
                                <select id="appblocker.whitelistmode.allowed_apps" class="selectpicker" multiple="multiple" data-live-search="true" data-size="10" data-selected-text-format="count > 3" data-actions-box="true">
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
                                   <option value="roblox">Roblox</option>
                                   <option value="steam">Steam</option>
                                   <option value="epicgames">Epic Games</option>
                                   <option value="ea">EA / Origin</option>
                                   <option value="minecraft">Minecraft</option>
                                   <option value="riot">Riot Games</option>
                                   <option value="blizzard">Battle.net / Blizzard</option>
                                   <option value="playstation">PlayStation Network</option>
                                   <option value="xbox">Xbox Live</option>
                                   <option value="nintendo">Nintendo</option>
                                </select>
                                <div class="hidden" data-for="help_for_appblocker.whitelistmode.allowed_apps">
                                    <small>{{ lang._('Only these selected services will be allowed to connect to the internet.') }}</small>
                                </div>
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
