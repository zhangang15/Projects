#include "ProjectileManger.h"
#include <stddef.h>  // defines NULL
#include "World.h"
#include "Projectile.h"
#include "Ogre.h"

//ProjectileManger* ProjectileManger::m_pInstance = NULL;  


ProjectileManger::ProjectileManger(Ogre::SceneManager *sceneManager): mSceneManager(sceneManager)
{
	
	//for(int i = 0; i < MAX_ENEMY_TANK + 1; i++)
	//{
	//	Projectile *projectile = new Projectile(mSceneManager);
	//	//projectileList[i] = projectile;
	//	projectileList.push_back(projectile);
	//}
}


void
ProjectileManger::addProjectile(Ogre::Vector3 pPos, Ogre::Quaternion orientation)
{
	Projectile *projectile = new Projectile(mSceneManager);
	projectile -> setupProjectile(pPos, orientation);

	projectileList.push_back(projectile);
}

void
ProjectileManger::destroyProjectile()
{

	
	int size = projectileList.size();
	Ogre::LogManager::getSingleton().logMessage("Destroying projectiles " + Ogre::StringConverter::toString(size));
	for (int i = 0; i < size; i++)
	{
		
		Projectile *projectile = projectileList[i];
		Ogre::LogManager::getSingleton().logMessage("Destroying : " + Ogre::StringConverter::toString(i) + "  "+ Ogre::StringConverter::toString(projectile -> getWillDestroy()));

		if(projectile -> getWillDestroy())
		{
			projectileList.erase(projectileList.begin()+i);
			//projectile->destroy();
			delete projectile;
			size--;
		}
	}
	Ogre::LogManager::getSingleton().logMessage("[Destroy projectiles done]" );
}


ProjectileManger::~ProjectileManger(void)
{
}


//ProjectileManger*
//ProjectileManger::getInstance()
//{
//	if(m_pInstance==NULL)
//		m_pInstance=new ProjectileManger();
//	return m_pInstance;
//}
