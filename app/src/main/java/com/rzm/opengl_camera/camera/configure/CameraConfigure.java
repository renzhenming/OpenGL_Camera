package com.rzm.opengl_camera.camera.configure;

public class CameraConfigure {
	private int cameraFacingId;
	private int degress;
	private int textureWidth;
	private int textureHeight;

	public CameraConfigure(int degress, int textureWidth, int textureHeight, int cameraFacingId){
		this.degress = degress;
		this.textureWidth = textureWidth;
		this.textureHeight = textureHeight;
		this.cameraFacingId = cameraFacingId;
	}

	public int getDegress() {
		return degress;
	}

	public int getTextureWidth() {
		return textureWidth;
	}

	public int getTextureHeight() {
		return textureHeight;
	}
	
	public int getCameraFacingId() {
		return cameraFacingId;
	}
}
