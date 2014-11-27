#include "BuildTank.h"
#include "OgreSceneManager.h"
#include "OgreStringConverter.h"
#include "OgreSceneNode.h"
#include "OgreEntity.h"
#include "OgreQuaternion.h"
#include "OgreMath.h"


#include "./btogre/include/BtOgreExtras.h"
#include "./btogre/include/BtOgreGP.h"
#include "./btogre/include/BtOgreExtras.h"
#include "PhysicsWorld.h"


int BuildTank::count = -1;

int BuildTank::smokeCount = -1;
//Constructor
BuildTank::BuildTank(Ogre::SceneManager *sceneManager, bool ai): BaseObject(), mSceneManager(sceneManager), isAI(ai)
{
	damageCount = -1;
	setType(BaseObject::TANK);
	directionTimer = visibilityTimer = 0.0f;
	isDamaged = false;
	count++;
	id = count;
	createBody();
	createTurret();
	createGun();

	//disable gravity for tank to prevent constantly hitting the ground
	mRigidBody->setGravity(btVector3(0.0,0.0,0.0));
}


//Create Body of the tank
void
BuildTank::createBody(){
	mName = "tankbody" + std::to_string(id);
	Ogre::Entity *pBody = mSceneManager->createEntity(mName, "Body.mesh");
	mTankBodyNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	mTankBodyNode->attachObject(pBody);
	mTankBodyNode->scale(10,10,10);
	
	BtOgre::StaticMeshToShapeConverter converter(mSceneManager->getEntity(mName));
	btCollisionShape* shape = converter.createConvex();

	mTankTransform.setIdentity();
	mTankTransform.setOrigin(btVector3(0,5,0));
	shape->setLocalScaling(btVector3(10,10,10));

	mRigidBody = PhysicsWorld::getInstance()->createRigidBody(1.0f, mTankTransform, shape);
	mRigidBody->setUserPointer(this);
}

void 
BuildTank::setTankPosition(Ogre::Vector3 mTankPosition)
{
	mTankTransform = mRigidBody->getWorldTransform();
	Ogre::LogManager::getSingleton().logMessage("before set: " + Ogre::StringConverter::toString(Ogre::Vector3(mTankTransform.getOrigin().getX(), mTankTransform.getOrigin().getY(), mTankTransform.getOrigin().getZ())));
	
	Ogre::LogManager::getSingleton().logMessage("mTankPosition: " + Ogre::StringConverter::toString(mTankPosition));
	


	mTankBodyNode->setPosition(mTankPosition);
	mTankTransform = mRigidBody->getWorldTransform();
	mTankTransform.setOrigin(btVector3(mTankPosition.x, mTankPosition.y, mTankPosition.z));
	mRigidBody->setWorldTransform(mTankTransform);
	Ogre::LogManager::getSingleton().logMessage("mTankTransform: " + Ogre::StringConverter::toString(Ogre::Vector3(mTankTransform.getOrigin().getX(), mTankTransform.getOrigin().getY(), mTankTransform.getOrigin().getZ())));

}

void
BuildTank::createTurret(){
	Ogre::Entity *pTurret = mSceneManager->createEntity("Turret.mesh");
	mTankTurretNode = mTankBodyNode->createChildSceneNode();
	mTankTurretNode->attachObject(pTurret);

	mTankTurretNode->scale(.25,.25,.25);
	mTankTurretNode->setPosition(Ogre::Vector3(0,.5,0));
}

void
BuildTank::createGun(){
	Ogre::Entity *pGun =mSceneManager->createEntity("Gun.mesh");
	mTankGunNode = mTankTurretNode->createChildSceneNode();
	mTankGunNode->attachObject(pGun);

	mTankGunNode->scale(.7, .7, .7);
	mTankGunNode->setPosition(Ogre::Vector3(0, -1.5,2.5));
}

////Attached the camera to the Tank
void 
BuildTank::attachCameraToTank(Ogre::Camera *mPlayerCam){
	mTankCameraNode = mTankBodyNode->createChildSceneNode("Camera");
	mTankCameraNode->attachObject(mPlayerCam);
}

void 
BuildTank::detachCameraFromTank(){
	mTankCameraNode->removeAndDestroyAllChildren();
	mTankCameraNode->getCreator()->destroySceneNode(mTankCameraNode->getName());
}

//Fetches the scene node of the tank and returns it
Ogre::SceneNode *
BuildTank::getTankSceneNode(){
	return mTankBodyNode;
}

//Rotate the tank
void 
BuildTank::yawTank(Ogre::Radian theta)
{
	Ogre::Quaternion orientation = mTankBodyNode->_getDerivedOrientation() ;
	// convert orientation to a matrix
	Ogre::Matrix3 matrix3;
	orientation.ToRotationMatrix( matrix3 );

	Ogre::Vector3 xBasis = Ogre::Vector3(Ogre::Math::Cos(theta),0, - Ogre::Math::Sin(theta));
    Ogre::Vector3 yBasis = Ogre::Vector3(0,1,0);
    Ogre::Vector3 zBasis = Ogre::Vector3(Ogre::Math::Sin(theta),0,Ogre::Math::Cos(theta));
    Ogre::Matrix3 rotate;
    rotate.FromAxes(xBasis, yBasis, zBasis);
    mTankOrientation = rotate *  matrix3 ;

	mTankBodyNode->setOrientation(Ogre::Quaternion(mTankOrientation));

	//connect to physics engine
	orientation = mTankBodyNode->_getDerivedOrientation();
	mTankTransform = mRigidBody->getWorldTransform();
	mTankTransform.setRotation (btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w));
	mRigidBody->setWorldTransform(mTankTransform);

	////testing
	//btQuaternion rot = mTankTransform.getRotation();
	// mTankBodyNode ->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());
}

void 
BuildTank::yawTurret(Ogre::Radian theta)
{
	Ogre::Quaternion turretOrientation = mTankTurretNode->getOrientation() ;
	// convert orientation to a matrix
	Ogre::Matrix3 yawTMatrix3;
	turretOrientation.ToRotationMatrix(yawTMatrix3);

	Ogre::Vector3 xBasis = Ogre::Vector3(Ogre::Math::Cos(theta),0, - Ogre::Math::Sin(theta));
    Ogre::Vector3 yBasis = Ogre::Vector3(0,1,0);
    Ogre::Vector3 zBasis = Ogre::Vector3(Ogre::Math::Sin(theta),0,Ogre::Math::Cos(theta));
    Ogre::Matrix3 rotate;
    rotate.FromAxes(xBasis, yBasis, zBasis);
    mTurretOrientation = rotate *  yawTMatrix3 ;

	mTankTurretNode->setOrientation(Ogre::Quaternion(mTurretOrientation));
}

void 
BuildTank::gunUp(Ogre::Radian theta)
{
	Ogre::Quaternion gunOrientation = mTankGunNode->getOrientation() ;
	// convert orientation to a matrix
	Ogre::Matrix3 tGunMatrix3;
	gunOrientation.ToRotationMatrix( tGunMatrix3 );

	Ogre::Vector3 xBasis = Ogre::Vector3(1,0,0);
	Ogre::Vector3 yBasis = Ogre::Vector3(0,Ogre::Math::Cos(theta), Ogre::Math::Sin(theta));
	Ogre::Vector3 zBasis = Ogre::Vector3(0,-Ogre::Math::Sin(theta), Ogre::Math::Cos(theta));
	Ogre::Matrix3 rotate;
	rotate.FromAxes(xBasis, yBasis, zBasis);
	mGunOrientation = rotate * tGunMatrix3;
	
	mTankGunNode->setOrientation(Ogre::Quaternion(mGunOrientation));
}

//Fetch and return the Gun position in worldspace
Ogre::Vector3 
	BuildTank::getGunWorldPosition(){
		return mTankGunNode->_getDerivedPosition();
}

//Fetch and return the Gun orientation in world space
Ogre::Quaternion 
	BuildTank::getGunWorldOrientation()
{
	return mTankGunNode->_getDerivedOrientation();
}

void
BuildTank::setDamageState(bool state)
{ 
	
	if(state)
	{
		damageCount = damageCount + 1;

		if(damageCount == 2){
			isDamaged = state; 
			setBroken();
			smokeCount = smokeCount + 1;
			char smokeName = smokeCount;
			Ogre::ParticleSystem* sunParticle = mSceneManager->createParticleSystem("Smoke_" + smokeName, "Examples/Smoke");
			Ogre::SceneNode* particleNode = mTankBodyNode->createChildSceneNode("Particle_smoke_" + smokeName);
			particleNode->attachObject(sunParticle);
			particleNode->setVisible(true);
		}
		

	}
	
}

void
BuildTank::setBadGun(Ogre::Radian theta){
	Ogre::Quaternion gunOrientation = mTankGunNode->getOrientation() ;
	// convert orientation to a matrix
	Ogre::Matrix3 tGunMatrix3;
	gunOrientation.ToRotationMatrix( tGunMatrix3 );

	Ogre::Vector3 xBasis = Ogre::Vector3(Ogre::Math::Cos(theta),0, - Ogre::Math::Sin(theta));
	Ogre::Vector3 yBasis = Ogre::Vector3(0,1,0);
	Ogre::Vector3 zBasis = Ogre::Vector3(Ogre::Math::Sin(theta),0,Ogre::Math::Cos(theta));
	Ogre::Matrix3 rotate;
	rotate.FromAxes(xBasis, yBasis, zBasis);
	mGunOrientation = rotate * tGunMatrix3;

	mTankGunNode->setOrientation(Ogre::Quaternion(mGunOrientation));
}

void
BuildTank::setBroken(){
	this->setBadGun(Ogre::Radian (0.8));
	this->gunUp(Ogre::Radian (1.1));
	this->mTankGunNode->setPosition(Ogre::Vector3(0.75, -1.8, 1.3));
	this->mTankTurretNode->setPosition(Ogre::Vector3(1.1, 0.0, 0.0));
}


