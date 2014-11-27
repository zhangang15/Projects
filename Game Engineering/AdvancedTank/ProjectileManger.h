
#ifndef __ProjectileManger_h_
#define __ProjectileManger_h_

#include <vector>
#include "Ogre.h"

class Projectile;
namespace Ogre {
	class SceneNode;
	class SceneManager;

}

class ProjectileManger
{
public:
	
	~ProjectileManger(void);
	ProjectileManger(Ogre::SceneManager *sceneManager);
	//static ProjectileManger *getInstance();
	std::vector<Projectile*> projectileList;
	void Think(float time);//may change
	void addProjectile(Ogre::Vector3 pPos, Ogre::Quaternion orientation);
	void destroyProjectile();

private:
	//ProjectileManger(Ogre::SceneManager *sceneManager);
	//static ProjectileManger* m_pInstance;

	Ogre::SceneManager *mSceneManager;
	//static int mProjectileCount;
};

#endif
