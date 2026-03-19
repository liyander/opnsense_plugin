<?php

namespace OPNsense\AppBlocker\Api;

use OPNsense\Base\ApiMutableModelControllerBase;

class SettingsController extends ApiMutableModelControllerBase
{
    static protected $internalModelName = 'appblocker';
    static protected $internalModelClass = 'OPNsense\AppBlocker\AppBlocker';
}
