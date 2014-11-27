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
	PongCamera::cameraAngle = "Default";
	mRenderCamera->setNearClipDistance(2);
	count = -1;
}

void
PongCamera::Think(float time)
{
	const float CAMERA_SPEED = 30;
	const float RADIANS_PER_SECOND = 0.5;

	
	Ogre::LogManager::getSingleton().logMessage("cameraAngle:  " + cameraAngle );
	Ogre::LogManager::getSingleton().logMessage("count :  " + count );
	Ogre::SceneNode *tankNode = mWorld->mPlayerTank->getTankSceneNode();
	Ogre::Vector3 cPos = tankNode->getPosition();
	Ogre::Quaternion cOrientation = tankNode->getOrientation();
	if (mInputHandler->IsKeyDown(OIS::KC_H))
		{
			count = count + 1;
			if(count > 7){
				count = 0;
				cameraAngle = "Heli";
			}
			
	} else if (mInputHandler->IsKeyDown(OIS::KC_P))
	{
		count = count + 1;
		if(count > 7){
			count = 0;
			cameraAngle = "Default";

		//	Ogre::Matrix3 rotationMatrix;

		//	cOrientation.ToRotationMatrix(rotationMatrix);

		//	Ogre::Vector3 m21 = rotationMatrix.GetColumn(1);
			//mWorld->detachCameraFromTank();
			//mWorld->attachCameraToTank("key");
			mRenderCamera->setPosition(cOrientation * cPos +  30 * Ogre::Vector3::UNIT_SCALE);
		//	mRenderCamera->setOrientation(cOrientation);
			mRenderCamera->lookAt(cPos);

		}
			
			
	}
	else if(mInputHandler->IsKeyDown(OIS::KC_M))
		{
			count = count + 1;
		if(count > 7){
			count = 0;
			cameraAngle = "Movie";
		}
	}
	if(PongCamera::cameraAngle.compare("Default") != 0){

		
		if(PongCamera::cameraAngle.compare("Movie") == 0){
		
			mRenderCamera->setPosition(cPos + (160 * Ogre::Vector3(1,1,1)) + 20);
			mRenderCamera->setOrientation(cOrientation);
			mRenderCamera->lookAt(cPos);

		} else if (PongCamera::cameraAngle.compare("Heli") == 0){
		

			mRenderCamera->setPosition( cPos + (60 * Ogre::Vector3(1,0,1)) + 20);
			mRenderCamera->setOrientation(cOrientation);
			mRenderCamera->lookAt(cPos);
		} else if(PongCamera::cameraAngle.compare("Player") == 0){
			
		}
	}
	
	// Any code needed here to move the camera about per frame
	//  (use mRenderCamera to get the actual render camera, of course!)

	//Move the camera when the arrow keys are pressed
	/*
	
		 
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
