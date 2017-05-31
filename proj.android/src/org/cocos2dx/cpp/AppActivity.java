/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
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

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import java.util.Timer;
import java.util.TimerTask;

import android.app.NativeActivity;
import android.os.Bundle;
import android.os.Handler;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.content.pm.PackageManager;

public class AppActivity extends Cocos2dxActivity implements SensorEventListener {
	private Cocos2dxGLSurfaceView glSurfaceView;
	private SensorManager mSensorManager;
	private Sensor mProximity;

	PackageManager PM;
	boolean hasProximitySensor;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		PM = this.getPackageManager();
		hasProximitySensor = PM.hasSystemFeature(PackageManager.FEATURE_SENSOR_PROXIMITY);
		if(hasProximitySensor) {
			mSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
			mProximity = mSensorManager.getDefaultSensor(Sensor.TYPE_PROXIMITY);
		}
	}

	public Cocos2dxGLSurfaceView onCreateView() {
		glSurfaceView = new Cocos2dxGLSurfaceView(this);
		this.hideSystemUI();

		glSurfaceView.setEGLConfigChooser(5,6,5,0,16,8);

		return glSurfaceView;
	}

	public void onWindowFocusChanged(boolean hasFocus) {
		super.onWindowFocusChanged(hasFocus);
		if(hasFocus) {
			this.hideSystemUI();
		}
	}

	private void hideSystemUI() {
		glSurfaceView.setSystemUiVisibility(
                Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_LAYOUT_STABLE 
                | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_HIDE_NAVIGATION // hide nav bar
                | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_FULLSCREEN // hide status bar
                | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
	}

	@Override
	protected void onResume() {
		super.onResume();
		if(hasProximitySensor) {
			mSensorManager.registerListener(this, mProximity, SensorManager.SENSOR_DELAY_NORMAL);
		}
	}

	@Override
	protected void onPause() {
		super.onPause();
		if(hasProximitySensor) {
			mSensorManager.unregisterListener(this);
		}
	}

	@Override
	public void onSensorChanged(SensorEvent event) {
		/*private void execute() {
			new Thread (new Runnable() {
				pubic void run() {*/
					//UpdateLightSensorValue(event.values[0]);
					float tmp = 1;
					UpdateLightSensorValue(event.values[0]);
				/*}
			}
		}*/
	}

	@Override
	public void onAccuracyChanged(Sensor sensor, int accuracy) {

	}

	public static native void UpdateLightSensorValue(float v);

}
