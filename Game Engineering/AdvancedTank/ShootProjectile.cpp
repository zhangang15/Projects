#include "ShootProjectile.h"

ShootProjectile::ShootProjectile(Ogre::SceneManager * sceneManager, Ogre::SceneNode * gunSceneNode, float time):mSceneManager(sceneManager),
	mGunSceneNode(gunSceneNode), mTime(time)
{
	projectileDistanceVector = Ogre::Vector3(0,0, 100); //Velocity of the projectile
	gravityDown = false;
	createProjectile();
	//launchProjectile(gunSceneNode->_getDerivedOrientation());
}

void 
ShootProjectile::createProjectile(){
	mProjectile = new Projectile(mSceneManager);
	//mProjectile->createProjectile();
	mProjectile->setProjectilePosition(mGunSceneNode->_getDerivedPosition());
}

void ShootProjectile::launchProjectile(Ogre::Quaternion mProjectileOrientation){

	Ogre::Real x = projectileDistanceVector.x;
	Ogre::Real y = projectileDistanceVector.y;
	Ogre::Real z = projectileDistanceVector.z;
	if(y <  30 && !gravityDown)
	{
		 Ogre::LogManager::getSingleton().logMessage("Gravity up [JI] ");
		gravityDown = false;
		y = y + 2;
	} else if(y >= 30 || gravityDown)
	{
		Ogre::LogManager::getSingleton().logMessage("Gravity down [JI] ");
		y = y - 2;
		gravityDown = true;
	}
	projectileDistanceVector = Ogre::Vector3(x, y, z);
	Ogre::SceneNode * projSceneNode = mProjectile->getProjectileSceneNode();
		
	projSceneNode->translate(mProjectileOrientation * (projectileDistanceVector * mTime));
	//Handle collision cases
	handleCollision(projSceneNode, x, y, z);

}

void ShootProjectile::handleCollision(Ogre::SceneNode *mProjectileNode, Ogre::Real x, Ogre::Real y, Ogre::Real z){

	Ogre::Vector3 pos = mProjectileNode->getPosition();
	bool shellHitWall = false;
	if(pos.y < 0 || pos.x < -995 || pos.x > 995 || pos.z < -995 || pos.z > 995){
		Ogre::LogManager::getSingleton().logMessage("Shell hit the wall [JI] ");
		y = 0;
		shellHitWall = true;
		projectileDistanceVector = Ogre::Vector3(x, y, z);
	}
	

	if(shellHitWall){
		Ogre::LogManager::getSingleton().logMessage("Destroy shell [JI] ");
		destroyProjectile(mProjectileNode);
	}
}

void 
ShootProjectile::destroyProjectile(Ogre::SceneNode *mProjectileNode)
{
	mProjectileNode->removeAndDestroyAllChildren();
	mProjectileNode->getCreator()->destroySceneNode(mProjectileNode->getName());
	delete mProjectile;
}
