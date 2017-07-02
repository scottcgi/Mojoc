
package com.Mojoc.Samples.SuperLittleRed;

import android.app.NativeActivity;
import android.view.KeyEvent;
import android.view.View;

/**
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2015-03-02
 * Author : scott.cgi
 * Version: 0.0.0
 *
 */
public class Activity extends NativeActivity
{
    @Override
    protected void onResume()
    {
        this.getWindow()
            .getDecorView()
            .setSystemUiVisibility
            (
               View.SYSTEM_UI_FLAG_HIDE_NAVIGATION  |
               View.SYSTEM_UI_FLAG_FULLSCREEN       |
               View.SYSTEM_UI_FLAG_IMMERSIVE        |
               View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
            );

        super.onResume();
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event)
    {
        if (keyCode == KeyEvent.KEYCODE_BACK)
        {
            return true;
        }

        return false;
    }
}
