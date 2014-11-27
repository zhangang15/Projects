// My header file.  This should go first!
#include "World.h"

// Ogre header files
#include "Ogre.h"
#include "OgreMath.h"
#include "OgreSceneManager.h"
#include "OgreSceneNode.h"
#include "OgreOverlayManager.h"
#include "OgreOverlay.h"
#include "OgreFontManager.h"
#include "OgreTextAreaOverlayElement.h"
// IOS (Input system) header files

#include <ois/ois.h>

// Other input files for my project
#include "Camera.h"
#include "InputHandler.h"
#include "ProjectileManger.h"
#include "PhysicsWorld.h"
#include "btBulletDynamicsCommon.h"


#include "./btogre/include/BtOgreExtras.h"
#include "./btogre/include/BtOgreGP.h"
#include "./btogre/include/BtOgreExtras.h"

float xMin, xMax, zMin, zMax;

#define Y_VALUE 5


World::World(Ogre::SceneManager *sceneManager, InputHandler *input)   : mSceneManager(sceneManager), mInputHandler(input)
{
	// Global illumination for now.  Adding individual light sources will make you scene look more realistic
	mSceneManager->setAmbientLight(Ogre::ColourValue(1,1,1));

	fireButton = 0;
	

	//create Player tank
	mPlayerTank = new BuildTank(mSceneManager);
	mPlayerTank->setTankPosition(Ogre::Vector3(0,Y_VALUE,0));
	//mPlayerTank->yawTank(Ogre::Radian(PI/2));
	tankList.push_back(mPlayerTank);

	//create enemy tanks
	for (int i = 1; i < MAX_ENEMY_TANK + 1; i++)
	{
		tankList.push_back(new BuildTank(mSceneManager, true));

	}

	tankList[1] -> setTankPosition(Ogre::Vector3(80, 5, 80));
	tankList[1] -> yawTank(Ogre::Radian(-PI));

	tankList[2] -> setTankPosition(Ogre::Vector3(-50, 5, 80));
	tankList[2] -> yawTank(Ogre::Radian(PI/2));

	tankList[3] -> setTankPosition(Ogre::Vector3(-80, 5, -80));
	tankList[3] -> yawTank(Ogre::Radian(PI / 2));

	tankList[4] -> setTankPosition(Ogre::Vector3(50, 5, -80));
	tankList[4] -> yawTank(Ogre::Radian(0));

	//Create Environment
	createGround();
	createWalls();
    createObstacles();
	createVirtualWalls();

	mProjectileManger = new ProjectileManger(mSceneManager);


	soundManger = new Sound();

}


void
World::createGround(){
	//Create ground
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
 
    Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    plane, 1000, 1000, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
 
    Ogre::Entity *entGround = mSceneManager->createEntity("GroundEntity", "ground");
    mSceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);
    entGround->setMaterialName("Examples/Frost");

	//btCollisionShape* shape = new btBoxShape(btVector3(btScalar(200.), btScalar(0.), btScalar(200.)));
	////btCollisionShape* shape = new btBoxShape(btVector3(btScalar(1.), btScalar(1.), btScalar(1.)));
	//btTransform groundTransform;

	//groundTransform.setIdentity();
	//groundTransform.setOrigin(btVector3(0,0,0));
	////shape->setLocalScaling(btVector3(100,0.1,100));
	//PhysicsWorld::getInstance()->createRigidBody(0.0f, groundTransform, shape);
}

void 
World::createObstacles(){
	//create some cubes as obstacle
	//fist
	Ogre::Entity *firstCube =mSceneManager->createEntity("base.mesh");
	firstCubeNode = mSceneManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(30, 14, 30));
	firstCubeNode->attachObject(firstCube);
	firstCubeNode->scale(8, 14, 8);


	//second
	Ogre::Entity *secondCube =mSceneManager->createEntity("base.mesh");
	secondCubeNode = mSceneManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(60, 14, 60));
	secondCubeNode->attachObject(secondCube);
	secondCubeNode->scale(8, 13, 8);


	//third
	Ogre::Entity *thirdCube =mSceneManager->createEntity("base.mesh");
	thirdCubeNode = mSceneManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(65, 14, 0));
	thirdCubeNode->attachObject(thirdCube);
	thirdCubeNode->scale(8, 17, 8);


	//fourh
	Ogre::Entity *fourthCube =mSceneManager->createEntity("base.mesh");
	fourthCubeNode = mSceneManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(-20, 14, -20));
	fourthCubeNode->attachObject(fourthCube);
	fourthCubeNode->scale(8, 18, 8);
	
	//fifth
	Ogre::Entity *fifthCube =mSceneManager->createEntity("base.mesh");
	fifthCubeNode = mSceneManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(-40, 14, -40));
	fifthCubeNode->attachObject(fifthCube);
	fifthCubeNode->scale(8, 18, 8);

	//sixth
	Ogre::Entity *sixthCube =mSceneManager->createEntity("base.mesh");
	sixthCubeNode = mSceneManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(-60, 14, 50));
	sixthCubeNode->attachObject(sixthCube);
	sixthCubeNode->scale(8, 18, 8);
}

void
World::createVirtualWalls()
{
	Ogre::Entity *topwall =mSceneManager->createEntity("base.mesh");
	Ogre::SceneNode *topWallNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	topWallNode->attachObject(topwall);

	topWallNode->setPosition(Ogre::Vector3(30, 20, -500));
	topWallNode->scale(1000, 10, 2);
	topWallNode->setVisible(false);
	mWalls.push_back(topWallNode);

	Ogre::Entity *bottomWall =mSceneManager->createEntity("base.mesh");
	Ogre::SceneNode *bottomWallNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	bottomWallNode->attachObject(bottomWall);

	bottomWallNode->setPosition(Ogre::Vector3(30, 20, 500));
	bottomWallNode->scale(1000, 10, 2);
	bottomWallNode->setVisible(false);
	mWalls.push_back(bottomWallNode);

	Ogre::Entity *leftwall =mSceneManager->createEntity("base.mesh");
	Ogre::SceneNode *leftWallNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	leftWallNode->attachObject(leftwall);

	leftWallNode->setPosition(Ogre::Vector3(500, 20, 15));
	leftWallNode->scale(2, 10, 1000);
	leftWallNode->setVisible(false);
	mWalls.push_back(leftWallNode);

	Ogre::Entity *rightwall =mSceneManager->createEntity("base.mesh");
	Ogre::SceneNode *rightWallNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	rightWallNode->attachObject(rightwall);

	rightWallNode->setPosition(Ogre::Vector3(-500, 20, 15));
	rightWallNode->scale(2, 10, 1000);
	rightWallNode->setVisible(false);
	mWalls.push_back(rightWallNode);

	xMax = leftWallNode -> getPosition() .x;
	xMin = rightWallNode -> getPosition() .x;
	
	zMax = bottomWallNode -> getPosition().z;
	zMin = topWallNode -> getPosition() .z;

}

void
World::createWalls()
{
	float height = 50.0f;

	////create walls
	//top
	Ogre::Plane plane1(Ogre::Vector3::UNIT_Z, Ogre::Vector3(0, 0, -500));
    Ogre::MeshManager::getSingleton().createPlane("wall1", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane1, 1000, 500, 20, 10, true, 1, 5, 1, Ogre::Vector3::UNIT_Y);
    Ogre::Entity* entityTopWall = mSceneManager->createEntity("wall1");
	Ogre::SceneNode *topNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	topNode ->attachObject(entityTopWall);
    entityTopWall->setMaterialName("Examples/RustySteel");
	mWalls.push_back(topNode);

	

	//bottom
	Ogre::Plane plane2(-Ogre::Vector3::UNIT_Z, Ogre::Vector3(0, 0, 500));
    Ogre::MeshManager::getSingleton().createPlane("wall2", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane2, 1000, 500, 20, 10, true, 1, 5, 1, Ogre::Vector3::UNIT_Y);
    Ogre::Entity* entityBottomWall = mSceneManager->createEntity("wall2");
	Ogre::SceneNode *bottomNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	bottomNode ->attachObject(entityBottomWall);
    entityBottomWall->setMaterialName("Examples/RustySteel");



	//left wall
	Ogre::Plane plane3(-Ogre::Vector3::UNIT_X, Ogre::Vector3(500, 0, 0));
	Ogre::MeshManager::getSingleton().createPlane("wall3", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane3, 500, 1000, 20, 10, true, 1, 5, 1, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* entityLeftWall = mSceneManager->createEntity("wall3");
	Ogre::SceneNode *leftNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	leftNode ->attachObject(entityLeftWall);
	entityLeftWall->setMaterialName("Examples/RustySteel");	


	//right wall
	Ogre::Plane plane4(Ogre::Vector3::UNIT_X, Ogre::Vector3(-500, 0, 0));
    Ogre::MeshManager::getSingleton().createPlane("wall4", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane4, 500, 1000, 20, 10, true, 1, 5, 1, Ogre::Vector3::UNIT_Z);
    Ogre::Entity* entityRightWall = mSceneManager->createEntity("wall4");
	Ogre::SceneNode *rightNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	rightNode ->attachObject(entityRightWall);
	entityRightWall->setMaterialName("Examples/RustySteel");

}

//Attach camera to tank
void
World::attachCameraToTank(Ogre::Camera *mPlayerCam){

	mPlayerTank->attachCameraToTank(mPlayerCam);
	
}

void
World::attachCameraToTank(std::string input){

	/*if(input.compare("Key") == 0){
		mPlayerTank->
	}*/
	//mPlayerTank->attachCameraToTank(mPlayerCam);
	
}

void 
World::detachCameraFromTank(){
	mPlayerTank->detachCameraFromTank();
}

//All player controls go into this method
void 
World::controlPlayer(float time)
{
	moveTurret(time);
	moveTank(time);
	moveGun(time);
	fireShell(time);

	//move projectile
	handleProjectile(time);


}

void 
World::checkShellTankCollision(BuildTank *tank)
{
	Ogre::SceneNode *node1 = tank->getTankSceneNode();

	int size = mProjectileManger -> projectileList.size();

	for(int i = 0; i < size; i++) 
	{
		//Projectile *projectile = projectileList[i];
		Projectile *projectile = mProjectileManger -> projectileList[i];
		Ogre::SceneNode *node2 = projectile -> getProjectileSceneNode();

		Ogre::AxisAlignedBox aab = node1->_getWorldAABB().intersection(node2->_getWorldAABB());
		if(!aab.isNull())
		{
			Ogre::Vector3 diff = node1->getPosition() - node2->getPosition();
			Ogre::Vector3 dir = diff.normalisedCopy();
			Ogre::Vector3 pene = aab.getMaximum() - aab.getMinimum();
			Ogre::Vector3 trans = dir * Ogre:: Math::Abs(pene.normalisedCopy().dotProduct(dir)) * pene.length() * 0.5;

			node1->translate(trans.x * 0.1, 0, trans.z *0.1);
			//should use future destory function
			projectile -> setProjectileVisibility(false);
			projectile -> setFireOn(false);
			projectile->setWillDestroy();
			tank -> setDamageState(true);
			soundManger -> playExplosionSound();
		}


		//if (projectile -> getProjectileVisibility())
		//{

		//	Ogre::AxisAlignedBox aab = node1->_getWorldAABB().intersection(node2->_getWorldAABB());
		//	if(!aab.isNull())
		//	{
		//		Ogre::Vector3 diff = node1->getPosition() - node2->getPosition();
		//		Ogre::Vector3 dir = diff.normalisedCopy();
		//		Ogre::Vector3 pene = aab.getMaximum() - aab.getMinimum();
		//		Ogre::Vector3 trans = dir * Ogre:: Math::Abs(pene.normalisedCopy().dotProduct(dir)) * pene.length() * 0.5;

		//		node1->translate(trans.x * 0.1, 0, trans.z *0.1);
		//		//should use future destory function
		//		projectile -> setProjectileVisibility(false);
		//		projectile -> setFireOn(false);
		//		projectile->setWillDestroy();
		//		tank -> setDamageState(true);
		//	}
		//}
		
	}
	
}

void
World::checkCollision(Ogre::SceneNode *node1, Ogre::SceneNode *node2){
	Ogre::AxisAlignedBox aab = node1->_getWorldAABB().intersection(node2->_getWorldAABB());
	if(!aab.isNull())
	{
		Ogre::Vector3 diff = node1->getPosition() - node2->getPosition();
		Ogre::Vector3 dir = diff.normalisedCopy();
		Ogre::Vector3 pene = aab.getMaximum() - aab.getMinimum();
		Ogre::Vector3 trans = dir * Ogre:: Math::Abs(pene.normalisedCopy().dotProduct(dir)) * pene.length() * 0.5;

		node1->translate(trans.x * 0.1, 0, trans.z *0.1);
	}
}

void 
World::checkTanksCollision(BuildTank *tank)
{

	int id = tank -> getID();
	for(int i = 0; i < tankList.size(); i++)
	{
		if(id != tankList[i] -> getID() )
		{
			checkCollision(tank -> getTankSceneNode(), tankList[i] -> getTankSceneNode());
		}	
	}

	Ogre::LogManager::getSingleton().logMessage("checkTanksCollision:  " );
}

bool World::checkWallCollision(Ogre::SceneNode *node1, Ogre::SceneNode *node2)
{
	Ogre::AxisAlignedBox aab = node1->_getWorldAABB().intersection(node2->_getWorldAABB());
	Ogre::Quaternion oldOrientation = node1 -> getOrientation();
	if(!aab.isNull())
	{
		Ogre::Vector3 diff = node1->getPosition() - node2->getPosition();
		Ogre::Vector3 dir = diff.normalisedCopy();
		Ogre::Vector3 pene = aab.getMaximum() - aab.getMinimum();
		Ogre::Vector3 trans = dir * Ogre:: Math::Abs(pene.normalisedCopy().dotProduct(dir)) * pene.length() * 0.5;

		trans = trans.normalisedCopy();
		node1 -> translate(trans.x * 0.5, 0, trans.z *0.5);
		//Ogre::LogManager::getSingleton().logMessage("wall collision: " + Ogre::StringConverter::toString(trans) + " tank: " + Ogre::StringConverter::toString(node1 ->getPosition()));

		Ogre::Vector3 pos = node1->getPosition();
		Ogre::Real x, y, z, offset;
		bool reset = false;
		x = pos.x;
		y = pos.y;
		z = pos.z;
		offset = 12.0f;
		if (pos.x + offset > xMax)
		{
			x = xMax - offset;
			reset = true;
		}
		if (pos.x - offset< xMin)
		{
			x = xMin + offset;
			reset = true;
		}
		if (pos.z + offset > zMax)
		{
			z = zMax - offset;
			reset = true;
		}
		if (pos.z - offset < zMin)
		{
			z = zMin + offset;
			reset = true;
		}
		node1 -> setPosition(x, y, z);
		return true;
	}
	return false;
}

void
World::checkObstaclesCollision(Ogre::SceneNode *node)
{
	checkCollision(node, firstCubeNode);
	checkCollision(node, secondCubeNode);
	checkCollision(node, thirdCubeNode);
	checkCollision(node, fourthCubeNode);
	checkCollision(node, fifthCubeNode);
	checkCollision(node, sixthCubeNode);
}

//File shell
void 
	World::fireShell(float time)
{
	//Fire shell
	if(mInputHandler->IsKeyDown(OIS::KC_SPACE) && !tankList[0]->getDamageState()){

		//if(!fireOn)
		//Projectile *projectile = mProjectileManger -> projectileList[0];
		//Ogre::LogManager::getSingleton().logMessage("space:  " + Ogre::StringConverter::toString(!projectile -> isFireOn()));
		//if(!projectile -> isFireOn())
		//{
		//	fireButton = fireButton + 1;
		//	if(fireButton > 7){
		//		Ogre::LogManager::getSingleton().logMessage("fire projectile called [JI] ");
		//		projectile -> setupProjectile(tankList[0] -> getGunWorldPosition(), tankList[0] -> getGunWorldOrientation());

		//		//mPlayerTank->fire(time);
		//		fireButton = 0;
		//	}
		//}
		soundManger->playFireSound();
		fireButton = fireButton + 1;
		if(fireButton > 10){
			Ogre::LogManager::getSingleton().logMessage("fire projectile called [JI] ");
			mProjectileManger->addProjectile(tankList[0] -> getGunWorldPosition(), tankList[0] -> getGunWorldOrientation());

			//mPlayerTank->fire(time);
			fireButton = 0;
		}
	}
	//Ogre::LogManager::getSingleton().logMessage("Check fireon!!! [JI] ");
}


//Handle projectile path
//Ogre::Vector3 
void
World::handleProjectile(float time)
{
	int size = mProjectileManger->projectileList.size();
	//Ogre::LogManager::getSingleton().logMessage("handleProjectile calling...  " + Ogre::StringConverter::toString(size) );
	//check shell and tank collision
	for (int i = 0; i< size; i++)
	{
		mProjectileManger -> projectileList[i] -> Think(time);

	}

	for (int i = 0; i< MAX_ENEMY_TANK+1; i++)
	{

		BuildTank *tank = tankList[i];
		
		if (!(tank -> getDamageState()))
		{
			//Ogre::LogManager::getSingleton().logMessage("check shell collision for tank ...  " + Ogre::StringConverter::toString(i) );
			checkShellTankCollision(tank);
		}
	}
	//Ogre::LogManager::getSingleton().logMessage("handleProjectile [done]...  " );
	mProjectileManger->destroyProjectile();

}



void 
World::moveGun(float time) {
	//Move gun up
	if (mInputHandler->IsKeyDown(OIS::KC_W))
	{
		mPlayerTank->gunUp(Ogre::Radian(0.005));
	}
	if (mInputHandler->IsKeyDown(OIS::KC_S))
	{
		mPlayerTank->gunUp(Ogre::Radian(-0.005));
	}
}


void
World::moveTurret(float time){
	//Turn the turret
	if(mInputHandler->IsKeyDown(OIS::KC_A))
	{
		mPlayerTank->yawTurret(Ogre::Radian(0.005));
	} else if(mInputHandler->IsKeyDown(OIS::KC_D))
	{
		mPlayerTank->yawTurret(Ogre::Radian(-0.005));
	}
}

void 
World::moveTank(float time){

	if (mPlayerTank -> getDamageState())
	{
		return;
	}

	if(mInputHandler->IsKeyDown(OIS::KC_UP) || mInputHandler->IsKeyDown(OIS::KC_DOWN) || 
	   mInputHandler->IsKeyDown(OIS::KC_RIGHT) || mInputHandler->IsKeyDown(OIS::KC_LEFT))
	{
		soundManger->playMoveSound();
	}

	bool tankReverse = false;
	Ogre::Vector3 PlayerPosition;
	Ogre::Vector3 distanceToMoveVector = Ogre::Vector3(0,0,55);
	Ogre::SceneNode *mPlayerTankNode = mPlayerTank->getTankSceneNode();
	if(mInputHandler->IsKeyDown(OIS::KC_UP) || mInputHandler->IsKeyDown(OIS::KC_DOWN))
	{

		//Move tank up, down, right & left
		if (mInputHandler->IsKeyDown(OIS::KC_UP))
		{
			mPlayerTankNode->translate(mPlayerTankNode->getOrientation() *   (time * distanceToMoveVector));
			PlayerPosition = mPlayerTankNode->getPosition();


		}
		else if(mInputHandler->IsKeyDown(OIS::KC_DOWN) && mInputHandler->IsKeyDown(OIS::KC_LEFT))
		{
			tankReverse = true;
			mPlayerTank->yawTank(Ogre::Radian(-0.5f * time));
			mPlayerTankNode->translate(mPlayerTankNode->getOrientation() * (-time * distanceToMoveVector));
			PlayerPosition = mPlayerTankNode->getPosition();

		} 
		else if(mInputHandler->IsKeyDown(OIS::KC_DOWN) && mInputHandler->IsKeyDown(OIS::KC_RIGHT))
		{
			tankReverse = true;
			mPlayerTank->yawTank(Ogre::Radian(0.5f * time));
			mPlayerTankNode->translate(mPlayerTankNode->getOrientation() * (-time * distanceToMoveVector));
			PlayerPosition = mPlayerTankNode->getPosition();

		} 
		else if(mInputHandler->IsKeyDown(OIS::KC_DOWN))
		{
			tankReverse = true;
			mPlayerTankNode->translate(mPlayerTankNode->getOrientation() *   (-time * distanceToMoveVector));
			PlayerPosition = mPlayerTankNode->getPosition();
		}

		mPlayerTank ->setTankPosition(PlayerPosition);

		for(int i = 0; i < mWalls.size(); i++)
		{
			checkWallCollision(mPlayerTankNode, mWalls[i]);
		}

		//Check for obstacles
		checkObstaclesCollision(mPlayerTankNode);
		//Check for tanks
		checkTanksCollision(mPlayerTank);

		if(!tankReverse)
		{
			//Move tank left and right
			if (mInputHandler->IsKeyDown(OIS::KC_LEFT))
			{
				mPlayerTank->yawTank(Ogre::Radian(0.5f * time));
			} else if (mInputHandler->IsKeyDown(OIS::KC_RIGHT))
			{
				mPlayerTank->yawTank(Ogre::Radian(-0.5f * time));
			}
		}
	}
}

//Think method
void 
World::Think(float time)
{
	PhysicsWorld::getInstance()->simulate(time);
	//Control Player
	controlPlayer(time);
	
}


