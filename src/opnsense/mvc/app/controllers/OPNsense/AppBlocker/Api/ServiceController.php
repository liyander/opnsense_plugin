<?php

namespace OPNsense\AppBlocker\Api;

use OPNsense\Base\ApiControllerBase;
use OPNsense\Core\Backend;

class ServiceController extends ApiControllerBase
{
    public function reloadAction()
    {
        $backend = new Backend();
        // Generate the Unbound config file
        $backend->configdRun('template reload OPNsense/AppBlocker');
        // Restart Unbound to actually apply the DNS changes
        $response = $backend->configdRun("unbound restart");
        return array("status" => $response);
    }

    public function statusAction()
    {
        $backend = new Backend();
        $response = $backend->configdRun("appblocker status");
        return array("status" => $response);
    }
}
