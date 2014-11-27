#include "Projectile.h"

Projectile::Projectile(Ogre::SceneManager * sceneManager):mSceneManager(sceneManager)
{

}

void 
Projectile::createProjectile(){
	Ogre::Entity *shell =mSceneManager->createEntity("myShell.mesh");
	mProjectileNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	mProjectileNode->attachObject(shell);
	mProjectileNode->scale(1, 1, 1);
	setProjectileVisibility(false);
	mProjectileVisibility = false;
}

Ogre::SceneNode * 
Projectile::getProjectileSceneNode()
{
	return mProjectileNode;
}
void
Projectile::mProjectilePosition(Ogre::Vector3  pPos){
	mProjectileNode->setPosition(Ogre::Vector3(pPos.x,pPos.y + 7, pPos.z));
}

//void Projectile::shootProjectile(Ogre::Quaternion orientation, Ogre::Vector3 pPos, float time){
//
//	if(!getProjectileVisibility())
//	{
//		mProjectilePosition(pPos);
//		mProjectileNode->setVisible(true);
//		setProjectileVisibility(true);
//		Ogre::LogManager::getSingleton().logMessage("shootProjectile called [JI] ");
//		bool onAir = true;
//		while(onAir){
//			Ogre::Vector3 distanceToMoveVector = Ogre::Vector3(0,0,.5);
//			mProjectileNode->translate(orientation *  distanceToMoveVector);
//			projectilePosition = mProjectileNode->getPosition();
//
//			Ogre::LogManager::getSingleton().logMessage("Hello " + Ogre::StringConverter::toString(12));
//			if(projectilePosition.y < 5) {
//				Ogre::LogManager::getSingleton().logMessage("Projectile hit the floor ");
//				setProjectileVisibility(false);
//				onAir = false;
//			}
//			if(projectilePosition.x < -100 || projectilePosition.z < -100 ||
//				projectilePosition.x > 100 || projectilePosition.z > 100)
//			{
//				Ogre::LogManager::getSingleton().logMessage("Projectile hit the wall ");
//				setProjectileVisibility(false);
//				onAir = false;
//			}
//		}
//	}
//	
//
//}

void 
Projectile::setProjectileVisibility(bool visibility){
	mProjectileVisibility = visibility;
	mProjectileNode->setVisible(visibility);
}

bool
Projectile::getProjectileVisibility(){
	return mProjectileVisibility;
}