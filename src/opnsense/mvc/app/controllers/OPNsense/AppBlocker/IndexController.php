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
        $this->view->title = gettext("AppBlocker Configuration");
        $this->view->pick('OPNsense/AppBlocker/index');
    }
}
