
package com.Mojoc.Samples.SuperLittleRed;

import android.app.Activity;
import android.app.NativeActivity;
import android.os.Bundle;
import android.os.Handler;
import android.view.KeyEvent;
import android.view.View;
import android.widget.Toast;


/**
 * @author scott.cgi
 * @since  2015-3-2
 * Update  2019-8-4
 */
public class GameActivity extends NativeActivity {
    
    @Override
    protected void onCreate (Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }


    @Override
    protected void onResume() {
        this.immersive();
        super.onResume();
    }


    private void immersive() {
        this.getWindow().getDecorView().setSystemUiVisibility(
            View.SYSTEM_UI_FLAG_HIDE_NAVIGATION |
            View.SYSTEM_UI_FLAG_FULLSCREEN      |
            View.SYSTEM_UI_FLAG_IMMERSIVE       |
            View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
    }


    private long exitTime = 0;
    

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK && event.getRepeatCount() == 0) {

            if ((System.currentTimeMillis() - this.exitTime) > 2000) {
                Toast.makeText(GameActivity.this, this.getString(R.string.app_exit), Toast.LENGTH_SHORT).show();
                this.exitTime = System.currentTimeMillis();
            } else {
                this.onDestroy();
                final Activity local = this;

                // wait for onDestroy finish
                new Handler().postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        local.finish();
                        System.exit(0);
                    }
                }, 1000);
            }

            return true;
        }

        return super.onKeyDown(keyCode, event);
    }
}
