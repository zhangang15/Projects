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

float xMin, xMax, zMin, zMax;
int check = 0;

World::World(Ogre::SceneManager *sceneManager, InputHandler *input)   : mSceneManager(sceneManager), mInputHandler(input)
{
	// Global illumination for now.  Adding individual light sources will make you scene look more realistic
	mSceneManager->setAmbientLight(Ogre::ColourValue(1,1,1));

	//projectileDistanceVector = Ogre::Vector3(0,0,1000);
	fireButton = 0;
	
	Ogre::LogManager::getSingleton().logMessage("start " + Ogre::StringConverter::toString(1));

	//gravityDown = false;

	//create Player tank
	mPlayerTank = new BuildTank(mSceneManager);
	mPlayerTank->setTankPosition(Ogre::Vector3(0,5,0));
	tankList.push_back(mPlayerTank);

	//create enemy tanks
	for (int i = 1; i < MAX_ENEMY_TANK + 1; i++)
	{
		tankList.push_back(new BuildTank(mSceneManager, true));

	}

	tankList[1] -> getTankSceneNode() -> setPosition(80, 5, 80);
	tankList[1] -> yawTank(Ogre::Radian(-PI));

	tankList[2] -> getTankSceneNode() -> setPosition(-50, 5, 80);
	tankList[2] -> yawTank(Ogre::Radian(PI/2));

	tankList[3] -> getTankSceneNode() -> setPosition(-80, 5, -80);
	tankList[3] -> yawTank(Ogre::Radian(PI / 2));

	tankList[4] -> getTankSceneNode() -> setPosition(50, 5, -80);
	tankList[4] -> yawTank(Ogre::Radian(0));

	//Create Environment
	createGround();
	createWalls();
    createObstacles();
	createVirtualWalls();

	//fireOn = false;
	for(int i = 0; i < MAX_ENEMY_TANK + 1; i++)
	{

		Projectile *projectile = new Projectile(mSceneManager);
		projectile->createProjectile();

		projectileList[i] = projectile;


		resetProjectileInfo(i);
	}
}

void
World::resetProjectileInfo(int i)
{
	fireOnList[i] = false;
	projectileDistanceVectorList[i] = Ogre::Vector3(0,0,300);
	gravityDownList[i] = false;

	//projectileList[i]->mProjectilePosition(tankList[i]->getGunWorldPosition());
	projectileList[i] -> getProjectileSceneNode()->setVisible(false);

}

void
World::createGround(){
	//Create ground
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
 
    Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    plane, 200, 200, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
 
    Ogre::Entity *entGround = mSceneManager->createEntity("GroundEntity", "ground");
    mSceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);
    entGround->setMaterialName("Examples/Frost");
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

	topWallNode->setPosition(Ogre::Vector3(30, 20, -100));
	topWallNode->scale(100, 10, 2);
	topWallNode->setVisible(false);
	mWalls.push_back(topWallNode);

	Ogre::Entity *bottomWall =mSceneManager->createEntity("base.mesh");
	Ogre::SceneNode *bottomWallNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	bottomWallNode->attachObject(bottomWall);

	bottomWallNode->setPosition(Ogre::Vector3(30, 20, 100));
	bottomWallNode->scale(100, 10, 2);
	bottomWallNode->setVisible(false);
	mWalls.push_back(bottomWallNode);

	Ogre::Entity *leftwall =mSceneManager->createEntity("base.mesh");
	Ogre::SceneNode *leftWallNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	leftWallNode->attachObject(leftwall);

	leftWallNode->setPosition(Ogre::Vector3(100, 20, 15));
	leftWallNode->scale(2, 10, 100);
	leftWallNode->setVisible(false);
	mWalls.push_back(leftWallNode);

	Ogre::Entity *rightwall =mSceneManager->createEntity("base.mesh");
	Ogre::SceneNode *rightWallNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	rightWallNode->attachObject(rightwall);

	rightWallNode->setPosition(Ogre::Vector3(-100, 20, 15));
	rightWallNode->scale(2, 10, 100);
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
	////create walls
	//top
	Ogre::Plane plane1(Ogre::Vector3::UNIT_Z, Ogre::Vector3(0, 0, -100));
    Ogre::MeshManager::getSingleton().createPlane("wall1", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane1, 200, 100, 20, 10, true, 1, 5, 1, Ogre::Vector3::UNIT_Y);
    Ogre::Entity* entityTopWall = mSceneManager->createEntity("wall1");
	Ogre::SceneNode *topNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	topNode ->attachObject(entityTopWall);
    entityTopWall->setMaterialName("Examples/RustySteel");
	mWalls.push_back(topNode);

	//bottom
	Ogre::Plane plane2(-Ogre::Vector3::UNIT_Z, Ogre::Vector3(0, 0, 100));
    Ogre::MeshManager::getSingleton().createPlane("wall2", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane2, 200, 100, 20, 10, true, 1, 5, 1, Ogre::Vector3::UNIT_Y);
    Ogre::Entity* entityBottomWall = mSceneManager->createEntity("wall2");
	Ogre::SceneNode *bottomNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	bottomNode ->attachObject(entityBottomWall);
    entityBottomWall->setMaterialName("Examples/RustySteel");

	//left wall
	Ogre::Plane plane3(-Ogre::Vector3::UNIT_X, Ogre::Vector3(100, 0, 0));
    Ogre::MeshManager::getSingleton().createPlane("wall3", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane3, 100, 200, 20, 10, true, 1, 5, 1, Ogre::Vector3::UNIT_Z);
    Ogre::Entity* entityLeftWall = mSceneManager->createEntity("wall3");
	Ogre::SceneNode *leftNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	leftNode ->attachObject(entityLeftWall);
    entityLeftWall->setMaterialName("Examples/RustySteel");	

	//right wall
	Ogre::Plane plane4(Ogre::Vector3::UNIT_X, Ogre::Vector3(-100, 0, 0));
    Ogre::MeshManager::getSingleton().createPlane("wall4", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane4, 100, 200, 20, 10, true, 1, 5, 1, Ogre::Vector3::UNIT_Z);
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

//All player controls go into this method
void 
World::controlPlayer(float time)
{
	moveTurret(time);
	moveTank(time);
	//moveGun(time);
	fireShell(time);

	//move projectile
	for (int i = 0; i< MAX_ENEMY_TANK + 1; i++)
	{
		if(fireOnList[i]) 
		{
			
			bool destroyed =  handleProjectile(tankList[i]->getGunWorldOrientation(), projectileList[i], time, i);
		}
		Ogre::LogManager::getSingleton().logMessage("after index: !!!: " + Ogre::StringConverter::toString(i));
	}

	//check shell and tank collision
	for (int i = 0; i< MAX_ENEMY_TANK + 1; i++)
	{
		BuildTank *tank = tankList[i];
		if (!(tank -> getDamageState()))
		{
			checkShellTankCollision(tank);
		}
	}

}

void 
World::checkShellTankCollision(BuildTank *tank)
{
	Ogre::SceneNode *node1 = tank->getTankSceneNode();

	//check = check + 1;
	//if (check < 10)
	//{
	//	return;
	//}
	

	for(int i = 0; i < MAX_ENEMY_TANK; i++) 
	{
		if (i == tank -> getID())
		{
			continue;
		}
		
		Projectile *projectile = projectileList[i];
		Ogre::SceneNode *node2 = projectile -> getProjectileSceneNode();

		if (projectile -> getProjectileVisibility())
		{
			Ogre::AxisAlignedBox aab = node1->_getWorldAABB().intersection(node2->_getWorldAABB());
			if(!aab.isNull())
			{
				Ogre::Vector3 diff = node1->getPosition() - node2->getPosition();
				Ogre::Vector3 dir = diff.normalisedCopy();
				Ogre::Vector3 pene = aab.getMaximum() - aab.getMinimum();
				Ogre::Vector3 trans = dir * Ogre:: Math::Abs(pene.normalisedCopy().dotProduct(dir)) * pene.length() * 0.5;

				node1->translate(trans.x * 0.1, 0, trans.z *0.1);
				resetProjectileInfo(i);
				tank -> setDamageState(true);
			}
		}
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
		Ogre::LogManager::getSingleton().logMessage("wall collision: " + Ogre::StringConverter::toString(trans) + " tank: " + Ogre::StringConverter::toString(node1 ->getPosition()));

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
	if(mInputHandler->IsKeyDown(OIS::KC_SPACE)){
		Ogre::LogManager::getSingleton().logMessage("space:  " + Ogre::StringConverter::toString(fireOnList[0]));
		//if(!fireOn)
		if(!fireOnList[0] && !(tankList[0]->getDamageState()))
		{
			fireButton = fireButton + 1;
			if(fireButton > 7){
				Ogre::LogManager::getSingleton().logMessage("fire projectile called [JI] ");
			
				projectileList[0] -> setProjectileVisibility(true);
				projectileList[0]->mProjectilePosition(tankList[0]->getGunWorldPosition());
				//fireOn = true;
				fireOnList[0] = true;
				//mPlayerTank->fire(time);
				fireButton = 0;
			}
		}


	}
	Ogre::LogManager::getSingleton().logMessage("Check fireon!!! [JI] ");
}

//Handle projectile path
//Ogre::Vector3 
bool
World::handleProjectile(Ogre::Quaternion mProjectileOrientation, Projectile * projectile, float time, int index)
{

		bool shellHitWall = false;

		Ogre::Real x = projectileDistanceVectorList[index].x;
		Ogre::Real y = projectileDistanceVectorList[index].y;
		Ogre::Real z = projectileDistanceVectorList[index].z;

		if(y <  30 && !gravityDownList[index])
		{
			Ogre::LogManager::getSingleton().logMessage("Gravity up [JI] ");
			gravityDownList[index] = false;
			y = y + 2;
		} else if(y >= 30 || gravityDownList[index])
		{
			Ogre::LogManager::getSingleton().logMessage("Gravity down [JI] ");
			y = y - 2;
			gravityDownList[index] = true;
		}


		projectileDistanceVectorList[index] = Ogre::Vector3(x, y, z);
		Ogre::SceneNode * projSceneNode = projectile->getProjectileSceneNode();

		projSceneNode->translate(mProjectileOrientation * (projectileDistanceVectorList[index] * time));

		Ogre::Vector3 pos = projSceneNode->getPosition();

		if(pos.y < 5 || pos.x < -95 || pos.x > 95 || pos.z < -95 || pos.z > 95)
		{
			Ogre::LogManager::getSingleton().logMessage("Shell hit the wall [JI] ");
			y = 0;
			fireOnList[index] = false;
			shellHitWall = true;
			projectileDistanceVectorList[index] = Ogre::Vector3(x, y, z);
		}
		

		Ogre::LogManager::getSingleton().logMessage("before hitwall!!!: " + Ogre::StringConverter::toString(index));
		if(shellHitWall){
			Ogre::LogManager::getSingleton().logMessage("Destroy shell [JI] ");
			resetProjectileInfo(index);
			//projectileList[index] = NULL;
			Ogre::LogManager::getSingleton().logMessage("inside: " + Ogre::StringConverter::toString(index));
			
			destroyShell(projectile->getProjectileSceneNode());
		}

		Ogre::LogManager::getSingleton().logMessage("after hitwall!!!: " + Ogre::StringConverter::toString(index));
		return shellHitWall;
}

//Destroy shell scene node
void 
World::destroyShell(Ogre::SceneNode *shellNode){
	//shellNode->removeAndDestroyAllChildren();
	//shellNode->getCreator()->destroySceneNode(shellNode->getName());
	//delete mProjectile;
	shellNode->setVisible(false);
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
				mPlayerTankNode->translate(mPlayerTankNode->getOrientation() *   (-time * distanceToMoveVector));
				PlayerPosition = mPlayerTankNode->getPosition();

			} 
			else if(mInputHandler->IsKeyDown(OIS::KC_DOWN) && mInputHandler->IsKeyDown(OIS::KC_RIGHT))
			{
				tankReverse = true;
				mPlayerTank->yawTank(Ogre::Radian(0.5f * time));
				mPlayerTankNode->translate(mPlayerTankNode->getOrientation() *   (-time * distanceToMoveVector));
				PlayerPosition = mPlayerTankNode->getPosition();

			} 
			else if(mInputHandler->IsKeyDown(OIS::KC_DOWN))
			{
				tankReverse = true;
				mPlayerTankNode->translate(mPlayerTankNode->getOrientation() *   (-time * distanceToMoveVector));
				PlayerPosition = mPlayerTankNode->getPosition();
			}

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
	//Control Player
	controlPlayer(time);
}


