#ifndef __Projectile_h_
#define __Projectile_h_

#include "OgreMatrix3.h"
#include "OgreVector3.h"
#include "Ogre.h"
#include "btBulletDynamicsCommon.h"
#include "BaseObject.h"
#include <string>

class Ogre::SceneManager;
class Ogre::SceneNode;
class Ogre::LogManager;

class Projectile: public BaseObject
{
public:
    Projectile::Projectile(Ogre::SceneManager * sceneManager);
	~Projectile();

    //void Projectile::createProjectile();

	Ogre::SceneNode * Projectile::getProjectileSceneNode();

	//void Projectile::shootProjectile(Ogre::Quaternion orientation, Ogre::Vector3 pPos, float time);

	void Projectile::setProjectilePosition(Ogre::Vector3 pPos);

	bool Projectile::getProjectileVisibility();

	void Projectile::setProjectileVisibility(bool visibility);

	void setOrientation(Ogre::Quaternion orientation);
	
	Ogre::Quaternion getOrientation() { return mOrientation;};
	
	void setupProjectile(Ogre::Vector3 pPos, Ogre::Quaternion orientation);

	bool isFireOn() { return mFireOn;};
	void setFireOn(bool isOn) { mFireOn = isOn;};
	Ogre::Vector3 getProjectileDistanceVector() { return mProjectileDistanceVector; };
	void setProjectileDistanceVector(Ogre::Vector3 newVector) { mProjectileDistanceVector = newVector; };
	bool isGravityDown() { return mGravityDown; };
	void setGravityDown(bool isDown) { mGravityDown = isDown; };
	btRigidBody* getRigidBody() { return mRigidBody; };

	void Think(float time);

	void setWillDestroy() { willDestroy = true;};
	bool getWillDestroy() { return willDestroy; };

	void destroy();
protected:
	Ogre::SceneManager *mSceneManager;

	Ogre::SceneNode *mProjectileNode;

	Ogre::Matrix3 mProjectileOrientation;

	//Ogre::Matrix3 mGunOrientation;

	Ogre::Vector3 projectilePosition;
    
	bool mProjectileVisibility;


	btRigidBody* mRigidBody;
	btTransform mProjectileTransform;
	std::string mName;
	bool willDestroy;
	int index;
	Ogre::Entity *mEntity;

	int id;
	static int count;

	bool mFireOn;
	Ogre::Vector3 mProjectileDistanceVector;
	bool mGravityDown;
	Ogre::Quaternion mOrientation;



};

#endif


