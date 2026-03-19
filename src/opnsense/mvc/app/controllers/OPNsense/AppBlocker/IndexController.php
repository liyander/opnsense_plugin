<?php

namespace OPNsense\AppBlocker;

/**
 *    Copyright (C) 2024 Liyander
 *
 *    All rights reserved.
 */

class IndexController extends \OPNsense\Base\IndexController
{
    public function indexAction()
    {
        $this->view->title = gettext("AppBlocker Settings");
        $this->view->pick('OPNsense/AppBlocker/index');
    }

    public function whitelistAction()
    {
        $this->view->title = gettext("AppBlocker Strict Whitelist");
        $this->view->pick('OPNsense/AppBlocker/whitelist');
    }
}
