#include "OgreCamera.h"
#include "Camera.h"
#include "World.h"
#include "OgreVector3.h"
#include "InputHandler.h"

PongCamera::PongCamera(Ogre::Camera *renderCamera, World *world,  InputHandler *inpuHandler) :
	mRenderCamera(renderCamera), mWorld(world), mInputHandler(inpuHandler)
{
	//mRenderCamera->setPosition(Ogre::Vector3(0,10,500));
    //mRenderCamera->lookAt(Ogre::Vector3(0,0,0));
	mRenderCamera->setNearClipDistance(2);
}

void
PongCamera::Think(float time)
{
	const float CAMERA_SPEED = 30;
	const float RADIANS_PER_SECOND = 0.5;
	// Any code needed here to move the camera about per frame
	//  (use mRenderCamera to get the actual render camera, of course!)

	//Move the camera when the arrow keys are pressed
	/*
	if (mInputHandler->IsKeyDown(OIS::KC_A))
	 {
		 
		 mRenderCamera->move(Ogre::Vector3(time * CAMERA_SPEED, 0, 0));

	 }
	 else if (mInputHandler->IsKeyDown(OIS::KC_D))
	 {
		 mRenderCamera->move(Ogre::Vector3(-time * CAMERA_SPEED, 0, 0));
	 }
	 else if (mInputHandler->IsKeyDown(OIS::KC_W))
	 {
		 mRenderCamera->move(Ogre::Vector3(0, time * CAMERA_SPEED, 0));
	 }
	 else if (mInputHandler->IsKeyDown(OIS::KC_S))
	 {
		 mRenderCamera->move(Ogre::Vector3(0, -time * CAMERA_SPEED, 0));
	 }
	 */
}
