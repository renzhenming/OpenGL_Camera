package com.rzm.opengl_camera;

import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.RelativeLayout;
import android.widget.Toast;

import com.rzm.opengl_camera.camera.CommonCamera1;
import com.rzm.opengl_camera.camera.manager.CameraPreviewManager;
import com.rzm.opengl_camera.camera.view.CameraSurfaceView;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("reviewer");
    }

    private CameraSurfaceView mSurfaceView;
    private RelativeLayout mPreviewScreen;
    private CommonCamera1 mCamera1;
    private CameraPreviewManager mPreviewManager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mPreviewScreen = findViewById(R.id.preview_screen);
        initView();
    }

    private void initView() {
        mSurfaceView = new CameraSurfaceView(this);
        mPreviewScreen.addView(mSurfaceView,0);
        mSurfaceView.getLayoutParams().width = getWindowManager().getDefaultDisplay().getWidth();
        mSurfaceView.getLayoutParams().height = getWindowManager().getDefaultDisplay().getWidth();

        mCamera1 = new CommonCamera1(this);

        mPreviewManager = new CameraPreviewManager(mSurfaceView,mCamera1) {
            public void onPermissionDismiss(final String tip) {
                new Handler(getMainLooper()).post(new Runnable() {
                    @Override
                    public void run() {
                        Toast.makeText(MainActivity.this, tip, Toast.LENGTH_SHORT).show();
                    }
                });
            }
        };
    }

    /**
     * 切换摄像头
     * @param view
     */
    public void change(View view) {

    }
}
