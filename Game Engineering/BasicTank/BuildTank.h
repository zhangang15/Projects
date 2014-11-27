#ifndef __BUILDTANK_h_

#define __BUILDTANK_h_

#include "OgreMatrix3.h"
#include "OgreVector3.h"
#include "ShootProjectile.h"

class Ogre::SceneManager;
class Ogre::SceneNode;

class BuildTank
{
public:
    BuildTank(Ogre::SceneManager * sceneManager, bool ai = false);

	Ogre::SceneNode * BuildTank::getTankSceneNode();

	void BuildTank::attachCameraToTank(Ogre::Camera *mPlayerCam);

	void BuildTank::yawTank(Ogre::Radian theta);

	void BuildTank::yawTurret(Ogre::Radian theta);

	void BuildTank::gunUp(Ogre::Radian theta);

	void BuildTank::setTankPosition(Ogre::Vector3 mTankPosition);

	Ogre::Quaternion BuildTank::getGunWorldOrientation();

	Ogre::Vector3 BuildTank::getGunWorldPosition();


	void resetDirectionTimer() { directionTimer = 0.0f; };
	void resetVisibilityTimer() { visibilityTimer = 0.0f; };
	
	void setDamageState(bool state);

	float addToDirectionTimer(float time) { return directionTimer += time; };
	float addTVisibilityTimer(float time) { return visibilityTimer += time; };

	float getDirectionTimer() { return directionTimer; };
	float getVibilityTimer() { return visibilityTimer; };
	bool getDamageState() { return isDamaged; };
	int getID() { return id; };

	void setOldPos(Ogre::Vector3 oldPos) { mOldPos = oldPos; };
	Ogre::Vector3 getOldPos() { return mOldPos; };

protected:
	Ogre::SceneManager *mSceneManager;
	Ogre::SceneNode *mTankBodyNode;
	Ogre::SceneNode *mTankGunNode;
	Ogre::SceneNode *mTankTurretNode;
	Ogre::SceneNode *mTankCameraNode;

	Ogre::Matrix3 mTankOrientation;
	Ogre::Matrix3 mTurretOrientation;
	Ogre::Matrix3 mGunOrientation;
    
	float directionTimer;
	float visibilityTimer;
	bool isDamaged;
	bool isAI;
	void BuildTank::shoot();
	int id;
	static int count;
	Ogre::Vector3 mOldPos;

private:
	void BuildTank::createBody();

	void BuildTank::createTurret();

	void BuildTank::createGun();

	void BuildTank::createProjectile();

	void BuildTank::setBadGun(Ogre::Radian theta);

	void BuildTank::setBroken();
};



#endif
