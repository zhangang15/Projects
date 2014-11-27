#ifndef __Projectile_h_
#define __Projectile_h_

#include "OgreMatrix3.h"
#include "OgreVector3.h"
#include "Ogre.h"
class Ogre::SceneManager;
class Ogre::SceneNode;
class Ogre::LogManager;

class Projectile
{
public:
    Projectile::Projectile(Ogre::SceneManager * sceneManager);

    void Projectile::createProjectile();

	Ogre::SceneNode * Projectile::getProjectileSceneNode();

	//void Projectile::shootProjectile(Ogre::Quaternion orientation, Ogre::Vector3 pPos, float time);

	void Projectile::mProjectilePosition(Ogre::Vector3 pPos);

	bool Projectile::getProjectileVisibility();

	void Projectile::setProjectileVisibility(bool visibility);

	
	

protected:
	Ogre::SceneManager *mSceneManager;

	Ogre::SceneNode *mProjectileNode;

	Ogre::Matrix3 mProjectileOrientation;

	Ogre::Matrix3 mGunOrientation;

	Ogre::Vector3 projectilePosition;
    
	bool mProjectileVisibility;


	
};

#endif
