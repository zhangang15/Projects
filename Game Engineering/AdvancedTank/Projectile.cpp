#include "Projectile.h"

#include "./btogre/include/BtOgreExtras.h"
#include "./btogre/include/BtOgreGP.h"
#include "./btogre/include/BtOgreExtras.h"
#include "PhysicsWorld.h"


#define  DEFAULT_PROJECTILE_DISTANCE_VECTOR Ogre::Vector3(0,0,300)

int Projectile::count = -1;

Projectile::Projectile(Ogre::SceneManager * sceneManager):BaseObject(), mSceneManager(sceneManager)
{
	setType(BaseObject::PROJECTILE1);

	count++;
	id = count;
	mName = "projectile" + std::to_string(id);
	Ogre::LogManager::getSingleton().logMessage("create projectile name: " + mName);
	Ogre::Entity *shell = mSceneManager->createEntity(mName, "myShell.mesh");
	mProjectileNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	mProjectileNode->attachObject(shell);
	mProjectileNode->scale(1, 1, 1);
	setProjectileVisibility(false);
	mFireOn = false;

	Ogre::ParticleSystem* sunParticle = mSceneManager->createParticleSystem("Sun_" + mName, "Examples/Smoke");
	Ogre::SceneNode* particleNode = mProjectileNode->createChildSceneNode("Particle_" + mName);
	particleNode->attachObject(sunParticle);
	particleNode->setVisible(false);

	BtOgre::StaticMeshToShapeConverter converter(mSceneManager->getEntity(mName));
	btCollisionShape* shape = converter.createConvex();

	mProjectileTransform.setIdentity();
	//mProjectileTransform.setOrigin(btVector3(0,0,0));
	shape->setLocalScaling(btVector3(1,1,1));

	mRigidBody = PhysicsWorld::getInstance()->createRigidBody(1.0f, mProjectileTransform, shape);
	mRigidBody->setUserPointer(this);


	
	willDestroy = false;
	index = PhysicsWorld::getInstance()->getSize() - 1;
}

Ogre::SceneNode * 
Projectile::getProjectileSceneNode()
{
	return mProjectileNode;
}

void
Projectile::setProjectilePosition(Ogre::Vector3  pPos){
	Ogre::Vector3 pos = Ogre::Vector3(pPos.x,pPos.y + 7, pPos.z);
	mProjectileNode->setPosition(pos);
	Ogre::LogManager::getSingleton().logMessage("setup projectile: " + Ogre::StringConverter::toString(mProjectileNode -> getPosition()));
	//mProjectileTransform = mRigidBody->getWorldTransform();
	mProjectileTransform.setOrigin(btVector3(pos.x,pos.y,pos.z));
	mRigidBody->setWorldTransform(mProjectileTransform);
}

void 
Projectile::setProjectileVisibility(bool visibility){
	mProjectileVisibility = visibility;
	mProjectileNode->setVisible(visibility);
}

bool
Projectile::getProjectileVisibility(){
	return mProjectileVisibility;
}

void 
Projectile::setOrientation(Ogre::Quaternion orientation) 
{ 
	mOrientation = orientation ; 
	//mProjectileTransform = mRigidBody->getWorldTransform();
	mProjectileTransform.setRotation (btQuaternion(mOrientation.x, mOrientation.y, mOrientation.z, mOrientation.w));
	mRigidBody->setWorldTransform(mProjectileTransform);

	Ogre::Vector3 dir = mOrientation * Ogre::Vector3::NEGATIVE_UNIT_Z * -1;

	mRigidBody->activate();
	btVector3 velocity = btVector3(dir.x, 0.02, dir.z) * 100;
	mRigidBody->setLinearVelocity(velocity);
	Ogre::LogManager::getSingleton().logMessage("Quaternion: " + Ogre::StringConverter::toString(Ogre::Vector3(orientation.x, orientation.y, orientation.z)));
	Ogre::LogManager::getSingleton().logMessage("set velocity: " + Ogre::StringConverter::toString(Ogre::Vector3(velocity.getX(), velocity.getY(), velocity.getZ())));
	
}

void
Projectile::setupProjectile(Ogre::Vector3 pPos, Ogre::Quaternion orientation)
{
	Ogre::LogManager::getSingleton().logMessage("calling setup projectile: " + Ogre::StringConverter::toString(mProjectileNode -> getPosition()));
	mFireOn = true;
	mProjectileDistanceVector = DEFAULT_PROJECTILE_DISTANCE_VECTOR;
	mGravityDown = false;
	setProjectilePosition(pPos);
	setOrientation(orientation);
	setProjectileVisibility(true);
}


//void 
//Projectile::Think(float time)
//{
//	bool shellHitWall = false;
//	Ogre::Real x = mProjectileDistanceVector.x;
//	Ogre::Real y = mProjectileDistanceVector.x;
//	Ogre::Real z = mProjectileDistanceVector.y;
//
//	if(y <  15 && !mGravityDown)
//	{
//		Ogre::LogManager::getSingleton().logMessage("Gravity up [JI] ");
//		//gravityDownList[index] = false;
//		mGravityDown = false;
//		y = y + 2;
//	} else if(y >= 15 || mGravityDown)
//	{
//		Ogre::LogManager::getSingleton().logMessage("Gravity down [JI] ");
//		y = y - 2;
//		//gravityDownList[index] = true;
//		mGravityDown = true;
//	}
//
//	//projectileDistanceVectorList[index] = Ogre::Vector3(x, y, z);
//	mProjectileDistanceVector = (Ogre::Vector3(x, y, z));
//
//	//projSceneNode->translate(mProjectileOrientation * (projectileDistanceVectorList[index] * time));
//	mProjectileNode -> translate(mOrientation * (mProjectileDistanceVector * time) * 10);
//
//	Ogre::Vector3 pos = mProjectileNode->getPosition();
//
//	if(pos.y < 0 || pos.x < -95 || pos.x > 95 || pos.z < -95 || pos.z > 95)
//	{
//		Ogre::LogManager::getSingleton().logMessage("Shell hit the wall [JI] ");
//		y = 0;
//		//fireOnList[index] = false;
//		mFireOn = false;
//		shellHitWall = true;
//		//projectileDistanceVectorList[index] = Ogre::Vector3(x, y, z);
//		mProjectileDistanceVector = (Ogre::Vector3(x, y, z));
//	}
//
//	if(shellHitWall){
//		Ogre::LogManager::getSingleton().logMessage("Destroy shell [JI] ");
//		destroy();
//	}
//}
void 
Projectile::Think(float time)
{
	if (!mProjectileVisibility)
	{
		return;
	}
	

	bool shellHitWall = false;
	

	//projSceneNode->translate(mProjectileOrientation * (projectileDistanceVectorList[index] * time));
	//mProjectileNode -> translate(mOrientation * (mProjectileDistanceVector * time) * 10);

	btVector3 pos = mRigidBody->getCenterOfMassPosition();
	
	mProjectileNode -> setPosition(pos.getX(), pos.getY(), pos.getZ());
	btVector3 v = mRigidBody->getLinearVelocity();
	Ogre::LogManager::getSingleton().logMessage("velocity: " + Ogre::StringConverter::toString(Ogre::Vector3(v.getX(), v.getY(), v.getZ())));
	Ogre::LogManager::getSingleton().logMessage("projectile pos: " + Ogre::StringConverter::toString(mProjectileNode -> getPosition()));

	//Ogre::Vector3 pos = mProjectileNode->getPosition();

	if(pos.getY() < 0 || pos.getX() < -500 || pos.getX() > 500 || pos.getZ() < -500 || pos.getZ() > 500)
	{
		Ogre::LogManager::getSingleton().logMessage("Shell hit the wall [JI] ");
		//y = 0;
		//fireOnList[index] = false;
		mFireOn = false;
		shellHitWall = true;
		willDestroy = true;
		//projectileDistanceVectorList[index] = Ogre::Vector3(x, y, z);
		//mProjectileDistanceVector = (Ogre::Vector3(x, y, z));
	}

	if(shellHitWall){
		//Ogre::LogManager::getSingleton().logMessage("Destroy shell [JI] ");
		//destroy();
	}
}

//to-do : destroy scenenode
void 
Projectile::destroy()
{

	Ogre::LogManager::getSingleton().logMessage("calling destroy function: " + Ogre::StringConverter::toString(1));
	setProjectileVisibility(false);
	mProjectileNode->removeAndDestroyAllChildren();
	Ogre::LogManager::getSingleton().logMessage("calling destroy function: " + Ogre::StringConverter::toString(2));
	mProjectileNode->getCreator()->destroySceneNode(mProjectileNode->getName());
	//mProjectileNode->detachAllObjects();
	Ogre::LogManager::getSingleton().logMessage("calling destroy function: " + Ogre::StringConverter::toString(2));
	//mSceneManager->destroyEntity(mEntity);
	//mSceneManager->destroySceneNode(mProjectileNode);
	Ogre::LogManager::getSingleton().logMessage("calling destroy function: " + Ogre::StringConverter::toString(3));
	//Ogre::SceneNode::destroyAllAttachedMovableObjects(mProjectileNode);
	//mSceneManager->destroyAllAttachedMovableObjects(itemNode);
	//itemNode->removeAndDestroyAllChildren();
	//m_pSceneManager->destroySceneNode(itemNode);
}

Projectile::~Projectile()
{
	 destroy();
}
