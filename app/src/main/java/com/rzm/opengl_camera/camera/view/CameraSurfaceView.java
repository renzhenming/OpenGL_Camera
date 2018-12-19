package com.rzm.opengl_camera.camera.view;

import android.content.Context;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceHolder.Callback;
import android.view.SurfaceView;

public class CameraSurfaceView extends SurfaceView implements Callback {
	private static final String TAG = "CameraPreView";
	
	public CameraSurfaceView(Context context) {
		super(context);
		SurfaceHolder surfaceHolder = getHolder();
		surfaceHolder.addCallback(this);
		surfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
	}

	public void surfaceCreated(SurfaceHolder holder) {
		Surface surface = holder.getSurface();
		int width = getWidth();
		int height = getHeight();
		if(null != mCallback){
			mCallback.createSurface(surface, width, height);
		}
	}

	public void surfaceChanged(SurfaceHolder holder, int format, int width,
			int height) {
		if(null != mCallback){
			mCallback.resetRenderSize(width, height);
		}
	}

	public void surfaceDestroyed(SurfaceHolder holder) {
		if(null != mCallback){
			mCallback.destroySurface();
		}
	}
	
	private CameraPreviewListener mCallback;
	public void setCallback(CameraPreviewListener listener){
		this.mCallback = listener;
	}
	public interface CameraPreviewListener {
		void createSurface(Surface surface, int width, int height);
		void resetRenderSize(int width, int height);
		void destroySurface();
	}
}
