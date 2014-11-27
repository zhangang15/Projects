
#ifndef __ShootProjectile_h_
#define __ShootProjectile_h_

#include "OgreMatrix3.h"
#include "OgreVector3.h"
#include "Ogre.h"
#include "Projectile.h"

class Ogre::SceneManager;
class Ogre::SceneNode;
class Ogre::LogManager;

class ShootProjectile
{
public:
    ShootProjectile(Ogre::SceneManager * sceneManager, Ogre::SceneNode * gunSceneNode, float time);

    void ShootProjectile::createProjectile();

	void ShootProjectile::launchProjectile(Ogre::Quaternion mProjectileOrientation);

	//void Projectile::shootProjectile(Ogre::Quaternion orientation, Ogre::Vector3 pPos, float time);

	void ShootProjectile::destroyProjectile(Ogre::SceneNode *mProjectileNode);

	void ShootProjectile::handleCollision(Ogre::SceneNode *mProjectileNode, Ogre::Real x, Ogre::Real y, Ogre::Real z);

	void setGraavityDown(bool down) { gravityDown = down; };

	bool getGravityDown() { return gravityDown; };

protected:
	Ogre::SceneManager *mSceneManager;
	float mTime;
	Projectile  *mProjectile;
	Ogre::SceneNode *mProjectileNode;
	Ogre::SceneNode *mGunSceneNode;

	bool gravityDown;
	Ogre::Matrix3 mProjectileOrientation;

	Ogre::Vector3 projectilePosition, projectileDistanceVector;
    

private:

};

#endif
