
#include <stddef.h>

#include "PhysicsWorld.h"


#include "./btogre/include/BtOgreExtras.h"
#include "./btogre/include/BtOgreGP.h"
#include "./btogre/include/BtOgreExtras.h"

#include "btBulletDynamicsCommon.h"

#include "LinearMath/btQuickprof.h"
#include "LinearMath/btIDebugDraw.h"
#include "LinearMath/btGeometryUtil.h"
#include "BulletCollision/CollisionShapes/btShapeHull.h"
#include "Ogre.h"
#include "BaseObject.h"
#include "Projectile.h"
#include "BuildTank.h"


PhysicsWorld *PhysicsWorld::m_pInstance = NULL;
BuildTank *tank;


PhysicsWorld::PhysicsWorld(void)
{
	mCollisionConfiguration = new btDefaultCollisionConfiguration();
	mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
	mOverlappingPairCache = new btDbvtBroadphase();
	mSolver = new btSequentialImpulseConstraintSolver();
	mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mOverlappingPairCache, mSolver, mCollisionConfiguration);
	mDefaultContactProcessingThreshold = BT_LARGE_FLOAT;
	mDynamicsWorld -> setGravity(btVector3(0,-50,0));
}

PhysicsWorld*
PhysicsWorld::getInstance()
{
	if(m_pInstance==NULL)
		m_pInstance=new PhysicsWorld();
	return m_pInstance;
}


btRigidBody*
PhysicsWorld::createRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape)
{
	mCollisionShapes.push_back(shape);

	btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0,0,0);
	if (isDynamic)
		shape->calculateLocalInertia(mass,localInertia);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects

#define USE_MOTIONSTATE 1
#ifdef USE_MOTIONSTATE
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo cInfo(mass,myMotionState,shape,localInertia);

	btRigidBody* body = new btRigidBody(cInfo);

	body->setContactProcessingThreshold(mDefaultContactProcessingThreshold);

#else
	btRigidBody* body = new btRigidBody(mass,0,shape,localInertia);	
	body->setWorldTransform(startTransform);
#endif//

	mDynamicsWorld->addRigidBody(body);

	return body;
}




void
PhysicsWorld::checkCollisions()
{
	////map<btCollisionObject*, CollisionInfo> newContacts;

	///* Browse all collision pairs */
	//int numManifolds = mDynamicsWorld->getDispatcher()->getNumManifolds();

	////Ogre::LogManager::getSingleton().logMessage("calling check...."  + Ogre::StringConverter::toString(numManifolds));
	////Ogre::LogManager::getSingleton().logMessage("check.... coin: "  + Ogre::StringConverter::toString(coinNode->getPosition()));

	////btTransform tr = coinBody->getWorldTransform();
	////btVector3 v =  tr.getOrigin();
	////Ogre::LogManager::getSingleton().logMessage("check.... coin: "  + Ogre::StringConverter::toString(v.getY()));

	////btTransform tr1 = tankBody->getWorldTransform();
	////btVector3 v1 =  tr1.getOrigin();
	////Ogre::LogManager::getSingleton().logMessage("check.... tank: "  + Ogre::StringConverter::toString(v1.getY()));

	//for (int i=0; i<numManifolds; i++)
	//{
	//	btPersistentManifold* contactManifold = mDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
	//	//const btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
	//	//const btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());

	//	const btCollisionObject* obA = contactManifold->getBody0();
	//	const btCollisionObject* obB = contactManifold->getBody1();

	//	BaseObject *oA = (NULL != obA->getUserPointer()) ? static_cast<BaseObject*>(obA->getUserPointer()) : NULL;
	//	BaseObject *oB = (NULL != obB->getUserPointer()) ? static_cast<BaseObject*>(obB->getUserPointer()) : NULL;

	//	if ((NULL != oA && oA->getType() == BaseObject::PROJECTILE1 && !((Projectile*)oA)->getProjectileVisibility()) ||
	//		(NULL != oB && oB->getType() == BaseObject::PROJECTILE1 && !((Projectile*)oB)->getProjectileVisibility()))
	//	{
	//		continue;
	//	}


	//	/*if (NULL != oA && oA->getType() == BaseObject::TANK )
	//	{
	//		tank = (BuildTank *)oA; 
	//		if(tank -> getID() != 0)
	//		{
	//			tank = NULL;
	//		}
	//	}

	//	if (NULL != oB && oB->getType() == BaseObject::TANK )
	//	{
	//		tank = (BuildTank *)oB; 
	//		if(tank -> getID() != 0)
	//		{
	//			tank = NULL;
	//		}
	//	}

	//	if (tank)
	//	{
	//		btRigidBody *rb = tank ->getRigidBody();

	//		btVector3 pos = rb->getWorldTransform().getOrigin();
	//		Ogre::LogManager::getSingleton().logMessage("checking tank!!: " + Ogre::StringConverter::toString((Ogre::Vector3(pos.getX(), pos.getY(), pos.getZ()))));
	//	}*/

	//	//Ogre::LogManager::getSingleton().logMessage("check i: " + Ogre::StringConverter::toString(i) + "    numManifolds: " +  Ogre::StringConverter::toString(numManifolds));
	//	/* Check all contacts points */
	//	int numContacts = contactManifold->getNumContacts();
	//	for (int j = 0; j < numContacts; j++)
	//	{
	//		//Ogre::LogManager::getSingleton().logMessage("check j: " + Ogre::StringConverter::toString(j) + "    numContacts: " + Ogre::StringConverter::toString(numContacts));
	//		btManifoldPoint& pt = contactManifold->getContactPoint(j);

	//		if (pt.getDistance()<0.f)
	//		{
	//			const btVector3& ptA = pt.getPositionWorldOnA();
	//			const btVector3& ptB = pt.getPositionWorldOnB();
	//			const btVector3& normalOnB = pt.m_normalWorldOnB;
	//			
	//			Ogre::LogManager::getSingleton().logMessage("detect a collision:  " + Ogre::StringConverter::toString((NULL != oA) ? oA->getType() : 0) + " hit "
	//				+ Ogre::StringConverter::toString((NULL != oB) ? oB->getType() : 0));
	//			if (tank)
	//			{
	//				tank->setIsCollided(true);
	//			}

	//			//if (NULL != oA && oA->getType() != BaseObject::PROJECTILE1) 
	//			//{
	//			//	if (oB->getType() != BaseObject::PROJECTILE1 || (oB->getType() == BaseObject::PROJECTILE1 && ((Projectile*)oB)->getProjectileVisibility()))
	//			//	{
	//			//		oA->setIsCollided(true);
	//			//		oA->setcollideWithType((NULL != oB)? oB->getType() : BaseObject::STATIC_OBJECT);
	//			//	}

	//			//}

	//			//if (NULL != oB && oB->getType() != BaseObject::PROJECTILE1) 
	//			//{
	//			//	if (oA->getType() != BaseObject::PROJECTILE1 || (oA->getType() == BaseObject::PROJECTILE1 && ((Projectile*)oA)->getProjectileVisibility()))
	//			//	{
	//			//		oB->setIsCollided(true);
	//			//		oB->setcollideWithType((NULL != oA)? oA->getType() : BaseObject::STATIC_OBJECT);
	//			//	}

	//			//}

	//			/*Ogre::LogManager::getSingleton().logMessage("oa collide with:  " + Ogre::StringConverter::toString(oA->getType()) + " collidewith "
	//				+ Ogre::StringConverter::toString(oA->getcollideWithType()));*/

	//			//oA->setIsCollided(true);
	//			//oB->setIsCollided(true);
	//			//oB->setcollideWithType(oA->getType());
	//			//oA->setcollideWithType(oB->getType());
	//		}
	//		//Ogre::LogManager::getSingleton().logMessage("check point");
	//	}
	//}

	//int numObjects = mDynamicsWorld->getNumCollisionObjects();
	//
	////Ogre::LogManager::getSingleton().logMessage("check numManifolds: " + Ogre::StringConverter::toString(numObjects));
	////Ogre::LogManager::getSingleton().logMessage("check bool: " + Ogre::StringConverter::toString(isCollided));
	//Ogre::LogManager::getSingleton().logMessage("[done physics check]");
}


void
PhysicsWorld::simulate(float time)
{
	mDynamicsWorld -> stepSimulation(time);
	checkCollisions();
}


PhysicsWorld::~PhysicsWorld(void)
{
}

