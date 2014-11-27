#include "AIManager.h"
#include <cstdlib>
#include "OgreSceneNode.h"
#include "BuildTank.h"
#include "Ogre.h"
#include "Projectile.h"
#include "ProjectileManger.h"

#define DIRECTION_TIMER_MAX_VALUE 1.0f
#define ROTATE_RATE 0.5f
#define ROTATE_TIME 1.0f

AIManager::AIManager(World *world, Ogre::SceneManager *sceneManager) : mWorld(world), mSceneManager(sceneManager)
{
    // Any initialization that needs to be done ...
	//create baby tanks :)
	//createEnemyTanks();
	for (int i = 0; i < MAX_ENEMY_TANK; i++)
	{
		mRotateTime[i] = 0.0f;
	}
	for (int i = 0; i < MAX_ENEMY_TANK; i++)
	{
		direction[i] = 0;
	}
}

AIManager::~AIManager()
{
    // Clean up after yourself ...
}

void 
AIManager::Think(float time)
{
	// Do any "Global" AI thinking, and then call think for all of the actual AI's running about
	int r;
	//int direction;
	float t;

	Ogre::Vector3 distanceToMoveVector = Ogre::Vector3(0,0,40);
	
	//return;

	for (int i = 1; i < MAX_ENEMY_TANK + 1; i++)
	{
		BuildTank *tank = mWorld -> tankList[i];
		t = tank -> addToDirectionTimer(time);

		if (tank -> getDamageState())
		{
			continue;
		}

		Ogre::SceneNode *tankNode = tank->getTankSceneNode();

		Ogre::Vector3 oldPos = tankNode -> getPosition();

		if (t > DIRECTION_TIMER_MAX_VALUE)
		{			
			mRotateTime[i - 1] = mRotateTime[i - 1]+ time;

			if (0 == direction[i - 1])
			{
				r = rand() % 2;
				direction[i - 1] = (r > 0) ? 1 : -1;
				//direction[i - 1] = r - 1;
			}

			if (mRotateTime[i - 1] < ROTATE_TIME)
			{
				//tank ->yawTank(Ogre::Radian(PI));

				tank ->yawTank(Ogre::Radian(ROTATE_RATE * time * direction[i - 1]));

				//tankNode ->translate(tankNode ->getOrientation() * (time * distanceToMoveVector));
			}
			else
			{
				mRotateTime[i - 1] = 0.0f;
				direction[i - 1] = 0;
				tank -> resetDirectionTimer();
				
				mWorld ->getProjectileManger()->addProjectile(tank -> getGunWorldPosition(), tank -> getGunWorldOrientation());
				
			}
			
		}

		tankNode -> translate(tankNode ->getOrientation() * (time * distanceToMoveVector));

		//mWorld -> checkWallCollision(tankNode, tankNode -> getPosition());
		mWorld -> checkObstaclesCollision(tankNode);
		mWorld -> checkTanksCollision(tank);


		for(int i = 0; i < mWorld -> mWalls.size(); i++)
		{
			bool isDetected =  mWorld -> checkWallCollision(tankNode, mWorld -> mWalls[i]);
			if (isDetected)
			{
				mRotateTime[i - 1] = 0.0f;
			}


		}
		
	}

}

