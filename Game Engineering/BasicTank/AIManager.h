#ifndef __AIManager_h_
#define __AIManager_h_

#include "World.h"

class World;
class BuildTank;
namespace Ogre {
    class SceneNode;
    class SceneManager;

}
class AIManager 
{

public:
    // You may want to add parameters to the AI Manager constructor
	AIManager(World *world, Ogre::SceneManager *sceneManager);
	~AIManager();
	void createEnemyTanks();
    // Go through all of the AIs, and call think on each one
    void Think(float time);

protected:
	World *mWorld;
	float mRotateTime[MAX_ENEMY_TANK];
	int direction[MAX_ENEMY_TANK];
	Ogre::SceneManager *mSceneManager;

	BuildTank *enemyTanks;
    // Probablay want a list of individual AI classes here ...
};

#endif