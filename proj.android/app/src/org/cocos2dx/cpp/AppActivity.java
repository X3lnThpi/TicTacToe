/****************************************************************************
Copyright (c) 2015-2016 Chukong Technologies Inc.
Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.cpp;

import android.os.Bundle;
import org.cocos2dx.lib.Cocos2dxActivity;
import android.os.Build;
import android.support.multidex.MultiDex;
import android.view.View;
import android.view.WindowManager;
import android.view.WindowManager.LayoutParams;

import android.view.Gravity;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.util.DisplayMetrics;
import android.view.Display;
import com.google.firebase.analytics.FirebaseAnalytics;
import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.MobileAds;

public class AppActivity extends Cocos2dxActivity {

    private FirebaseAnalytics mFirebaseAnalytics;
    private AdView adView;
    private AdView mAdView;
    //private static final String AD_UNIT_ID = "ca-app-pub-7467344290953936/4394143957";
    private static final String AD_UNIT_ID = "ca-app-pub-3940256099942544/6300978111";




    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.setEnableVirtualButton(false);
        super.onCreate(savedInstanceState);


//        mAdView = findViewById(R.id.adView);
//        AdRequest adRequest = new AdRequest.Builder().build();
//        mAdView.loadAd(adRequest);

        // Make sure we're running on Pie or higher to change cutout mode
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            // Enable rendering into the cutout area
            WindowManager.LayoutParams lp = getWindow().getAttributes();
            lp.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
            getWindow().setAttributes(lp);
        }


        mFirebaseAnalytics = FirebaseAnalytics.getInstance(this);

       // MobileAds.initialize(this, "ca-app-pub-7467344290953936/7256935420");

        MobileAds.initialize(this, "ca-app-pub-7467344290953936~9036442020");

        FrameLayout.LayoutParams adParams = new FrameLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);

        adParams.gravity = Gravity.CENTER_VERTICAL | Gravity.BOTTOM;
        //adView.setLayoutParams(adParams);
        //adView.setVisibility(View.VISIBLE);

        AdView adView = new AdView(this);
        adView.setAdSize(getAdSize());
        adView.setAdUnitId(AD_UNIT_ID);
        addContentView(adView, adParams);


        AdRequest adRequest = new AdRequest.Builder()
//                .addTestDevice(AdRequest.DEVICE_ID_EMULATOR)
//                .addTestDevice("83AE15BDE9D4148E2C9A4938E8F764B5")
                .build();

        adView.loadAd(adRequest);
        adView.bringToFront();

        // Workaround in https://stackoverflow.com/questions/16283079/re-launch-of-activity-on-home-button-but-only-the-first-time/16447508
        if (!isTaskRoot()) {
            // Android launched another instance of the root activity into an existing task
            //  so just quietly finish and go away, dropping the user back into the activity
            //  at the top of the stack (ie: the last state of this task)
            // Don't need to finish it again since it's finished in super.onCreate .
            return;
        }






        // DO OTHER INITIALIZATION BELOW



//        FrameLayout.LayoutParams adParams = new FrameLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
//
//
//
//
//        adView = new AdView(this);
//        adView.setAdSize(getAdSize());
//        adView.setAdUnitId(AD_UNIT_ID);
//        adView.setVisibility(View.GONE);
//        addContentView(adView, adParams);
//
//        adParams.gravity = Gravity.CENTER_VERTICAL | Gravity.BOTTOM;
//        adView.setLayoutParams(adParams);
//        adView.setVisibility(View.VISIBLE);
////
////
//        AdRequest adRequest = new AdRequest.Builder()
//                .addTestDevice(AdRequest.DEVICE_ID_EMULATOR)
//                .addTestDevice("83AE15BDE9D4148E2C9A4938E8F764B5")
//                .build();
////
//        adView.loadAd(adRequest);
//        adView.bringToFront();




    }

    private AdSize getAdSize() {
        // Step 2 - Determine the screen width (less decorations) to use for the ad width.
        Display display = getWindowManager().getDefaultDisplay();
        DisplayMetrics outMetrics = new DisplayMetrics();
        display.getMetrics(outMetrics);
        float widthPixels = outMetrics.widthPixels;
        float density = outMetrics.density;
        int adWidth = (int) (widthPixels / density);
        // Step 3 - Get adaptive ad size and return for setting on the ad view.
        return AdSize.getCurrentOrientationAnchoredAdaptiveBannerAdSize(this, adWidth);
    }

}
