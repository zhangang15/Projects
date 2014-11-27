#ifndef __World_h_
 
#define __World_h_
#include "OgreVector3.h"
#include "BuildTank.h"

#define MAX_ENEMY_TANK 4
#define PI 3.1415926

// Forward delcarations of Ogre classes.  Note the Ogre namespace!
namespace Ogre {
    class SceneNode;
    class SceneManager;

}

// Forward delcarations of our own classes
class PongCamera;
class InputHandler;
class vector;
class Projectile;


class World
{
public:
	
    World(Ogre::SceneManager *sceneManager, InputHandler *input);
    
    void Think(float time);

	Ogre::SceneManager *SceneManager() { return mSceneManager; }

	void World::attachCameraToTank(Ogre::Camera *mPlayerCam);

	void createGround();

	void createWalls();

	void createObstacles();

	void checkTanksCollision(BuildTank *tank);

	bool checkWallCollision(Ogre::SceneNode *node1, Ogre::SceneNode *node2);
	
	//void checkWallCollision(Ogre::SceneNode *node1, Ogre::Vector3 oldPos);

	void checkObstaclesCollision(Ogre::SceneNode *node);

	void controlPlayer(float time);

	void World::moveTank(float time);

	void World::moveTurret(float time);

	void World::moveGun(float time);

	void World::fireShell(float time);

	void World::destroyShell(Ogre::SceneNode *shellNode);

	void resetProjectileInfo(int i);

	void checkShellTankCollision(BuildTank *tank);

	bool handleProjectile(Ogre::Quaternion mProjectileOrientation, Projectile * projectile, float time, int index);

	std::vector<BuildTank*> tankList;
	std::vector<Ogre::SceneNode *> mWalls;
	Projectile* projectileList[MAX_ENEMY_TANK + 1];
	Ogre::Vector3 projectileDistanceVectorList[MAX_ENEMY_TANK + 1];
	bool gravityDownList[MAX_ENEMY_TANK + 1];
	bool fireOnList[MAX_ENEMY_TANK + 1];

protected:

	Ogre::SceneManager *mSceneManager;
	//Ogre::Vector3 projectileDistanceVector;
	//bool fireOn;
	//bool gravityDown;
	InputHandler *mInputHandler;
	Projectile *mProjectile;
	Ogre::Camera *mPlayerCam;

	Ogre::Quaternion mProjectileOrientation;
	int fireButton;

	//several cube obstacles
	Ogre::SceneNode *firstCubeNode;
	Ogre::SceneNode *secondCubeNode;
	Ogre::SceneNode *thirdCubeNode;
	Ogre::SceneNode *fourthCubeNode;
	Ogre::SceneNode *fifthCubeNode;
	Ogre::SceneNode *sixthCubeNode;

	

	// Here is where you keep all your world data.
	//  You probably want to use containers (arrays / lists / classes / etc) to ogranize them, 
	//    instead of a whole list of variables.  

	BuildTank *mPlayerTank;

	//void checkObstacle(Ogre::SceneNode *node, Ogre::SceneNode *cubeNode);
	void checkCollision(Ogre::SceneNode *node1, Ogre::SceneNode *node2);
	void createVirtualWalls();
};

#endif
