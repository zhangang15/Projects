
#include "Car.h"
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

#include <limits>


#define _USE_MATH_DEFINES
#include "math.h"

int Car::count = 0;
int Car::level = -1;

//Constructor
Car::Car(Ogre::SceneManager *sceneManager):
		BaseObject(sceneManager, 
				   std::string("Body.mesh"),					//mesh file name
				   Ogre::Vector3(3,3,3),					    //scale
				   BaseObject::CAR,								//type of object
				   20.0f,										//mass
				   std::string("car" + std::to_string(count)),	//object name
				   count										//object id
				   )										
{
	Ogre::LogManager::getSingleton().logMessage("carID: " + Ogre::StringConverter::toString(mID));	
	mTransformBeforeCollision = mRigidBody->getWorldTransform();
	mRigidBody->setAngularFactor(btVector3(0,1,0));
	count++;
	reset();
	
}


Car::Car(Ogre::SceneManager *sceneManager, std::string meshName):
		BaseObject(sceneManager, 
				   meshName,				//mesh file name
				   Ogre::Vector3(3,3,3), //scale
				   BaseObject::CAR,			//type of object
				   100.0f,					//mass
				   std::string("car" + std::to_string(count)),				//object name
				   count					//object id
				  )
{
	Ogre::LogManager::getSingleton().logMessage("carID: " + Ogre::StringConverter::toString(mID));
	mTransformBeforeCollision = mRigidBody->getWorldTransform();
	mRigidBody->setAngularFactor(btVector3(0,1,0));
	count++;
	reset();
	//getTrackNum(0);

	//collideWithType == BaseObject::GROUND;
}


////Attached the camera to the Tank
/*
void 
Car::attachCameraToCar(Ogre::Camera *playerCam){
	mCarCameraNode = mSceneNode->createChildSceneNode("Camera");
	mCarCameraNode->attachObject(playerCam);
}

void 
Car::detachCameraFromTank(){
	mCarCameraNode->removeAndDestroyAllChildren();
	mCarCameraNode->getCreator()->destroySceneNode(mCarCameraNode->getName());
}
*/
void
Car::positionBeforeCollistion(){
	if(collideWithType == BaseObject::CAR)
	{
		mTransformBeforeCollision = mRigidBody->getWorldTransform();
	}
}

//btTransform 
//Car::getTransformBeforeCollision(){
//	return mTransformBeforeCollision;
//}
void
Car::updateCarStats(bool forward)
{
	btVector3 carVelocity = mRigidBody->getLinearVelocity();
	mCurrentSpeed = carVelocity.length();
	mForward = forward;
	//Ogre::LogManager::getSingleton().logMessage("updateCarStats -> mCarSpeed " + Ogre::StringConverter::toString(mCurrentSpeed));
	//Ogre::LogManager::getSingleton().logMessage("updateCarStats -> mLastDirection " + Ogre::StringConverter::toString(mForward));
}

float 
Car::getCarSpeed()
{
	return mCurrentSpeed;
}
bool
Car::applyBreak(float value)
{
	//updateCarStats(mForward);
	if(mCurrentSpeed > 0.5){
		Ogre::Vector3 direction = mSceneNode->getOrientation() * Ogre::Vector3::UNIT_Z;
		mCurrentSpeed = mCurrentSpeed - value;
		btVector3 velocity = btVector3(0,0,0);
		if(mForward){
			
			velocity = btVector3(direction.x, 0, direction.z) * (mCurrentSpeed);
		} else {
			velocity = btVector3(direction.x, 0, direction.z) * (-mCurrentSpeed);
		}
		
		mRigidBody->activate();
		//mRigidBody->applyCentralForce(velocity);
		mRigidBody->setLinearVelocity(velocity);
		//updateCarStats(mForward);
		return true;
	} else {
		//updateCarStats(mForward);
		return false;
	}
	
}

void
Car::move(bool foward)
{
	
	if(getTrackNum(level) < 0)
	{
		mMaxSpeed = 20;
	} 
	if(getTrackNum(level) > -1){
		Document &document = Configuration::getInstance()->mDocument;
		Value &car = document["car_list"][Configuration::concatenate("car", std::to_string(mID).c_str())];
		mMaxSpeed = car["maxSpeed"].GetInt();
	}
	if(foward != mForward && mCurrentSpeed > 0.0f){
		//Stop the car first before taking any action
		bool carStillMoving = applyBreak(0.8);
		Ogre::LogManager::getSingleton().logMessage("Applying break : speed is :  " + Ogre::StringConverter::toString(mCurrentSpeed));
		if(!carStillMoving){
			Ogre::LogManager::getSingleton().logMessage("Car is not moving " );
			updateCarStats(foward);
		}
	} 
	

	if(mCurrentSpeed < mMaxSpeed){ 
		updateCarStats(foward);
		Ogre::Vector3 direction = mSceneNode->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
		direction.normalise();
		Ogre::LogManager::getSingleton().logMessage("mCarSpeed  " + Ogre::StringConverter::toString(mCurrentSpeed));
		mCurrentSpeed = mCurrentSpeed + .5;
		btVector3 velocity = btVector3(direction.x, 0, direction.z) * (foward ? mCurrentSpeed: -mCurrentSpeed);
		mRigidBody->activate();
		mRigidBody->setLinearVelocity(velocity);
	}
	else if (mCurrentSpeed > mMaxSpeed) {
		updateCarStats(foward);
		Ogre::Vector3 direction = mSceneNode->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
		direction.normalise();
		Ogre::LogManager::getSingleton().logMessage("mCarSpeed  " + Ogre::StringConverter::toString(mCurrentSpeed));
		mCurrentSpeed = mCurrentSpeed - .5;
		btVector3 velocity = btVector3(direction.x, 0, direction.z) * (foward ? mCurrentSpeed: -mCurrentSpeed);
		mRigidBody->activate();
		mRigidBody->setLinearVelocity(velocity);
	}
}

void 
Car::slowDown()
{
	mMaxSpeed =  20.f;
}

bool 
Car::isOnTheGround() 
{ 
	return Car::collideWithType == BaseObject::GROUND; 
}


void
Car::yaw(bool left)
{
	updateCarStats(mForward);
	
	if(mCurrentSpeed > 3){
		mRigidBody->activate();
		float rotate_rate = 0.2f;

		if (mLeft != left)
		{
			mRigidBody ->setAngularVelocity(btVector3(0, 0, 0));
			mLeft = !mLeft;
		}
		
		float maxAngularVelocity = 1.0f;
		if(mCurrentSpeed < 18){
			maxAngularVelocity = 0.1f;
		}
		if(mCurrentSpeed < 10){
			maxAngularVelocity = 0.07f;
		}
		if(mForward && left){
			mAngularVelocity = mAngularVelocity + 0.1f;
		} else if(mForward && !left){
			mAngularVelocity = mAngularVelocity - 0.1f;
		}else if(!mForward && left){
			//Turn right
			mAngularVelocity = mAngularVelocity - 0.1f;
		} else if(!mForward && !left){
			//Turn Left
			mAngularVelocity = mAngularVelocity + 0.1f;
		}
		//mAngularVelocity = left ? mAngularVelocity + 0.1f : mAngularVelocity - 0.1f;
		if (mAngularVelocity > maxAngularVelocity)
		{
			mAngularVelocity = maxAngularVelocity;
		}
		if (mAngularVelocity < -maxAngularVelocity)
		{
			mAngularVelocity = -maxAngularVelocity;
		}
	
		mRigidBody->setAngularVelocity(btVector3(0, mAngularVelocity, 0));
	}
}


float
Car::getDistance(btVector3 &carPos, btVector3 &point)
{
	
	return sqrt(pow(carPos.getX() - point.getX(), 2) 
			  + pow(carPos.getY() - point.getY(), 2) 
			  + pow(carPos.getZ() - point.getZ(), 2));
}

float 
Car::getDistanceFromCurveCenter(Value &track)
{
	btVector3 carPos = mRigidBody->getWorldTransform().getOrigin();
	//int trackIndex = getTrackNum(level);

	float scale = track["scale"].GetDouble();
	
	//float radius = scale * track["radius"].GetDouble();
	Value &trackPosArray = track["postion"];
	float x = scale * trackPosArray[SizeType(0)].GetDouble();
	float y = scale * trackPosArray[SizeType(1)].GetDouble();
	float z = scale * trackPosArray[SizeType(2)].GetDouble();
	//float radius = getDistance(btVector3(carPos.getX(), y, carPos.getZ()), btVector3(x,y,z));
	return getDistance(btVector3(carPos.getX(), y, carPos.getZ()), btVector3(x,y,z));;
}

float 
Car::calculateAngularVeloctiyOnCurve(Value &track, float radius)
{
	int turning = track["turning"].GetInt();
	return 1 * turning * mCurrentSpeed / radius;	
}

void 
Car::moveOnStraightTrack(Value &track)
{
	int axis = track["parallel_to"].GetInt();
	//int axis = 0;
	btVector3 carPos = mRigidBody->getWorldTransform().getOrigin();
	float distance;

	//1 == axis parallel to x;  3 == axis parallel to z
	distance = (1 == axis) ? abs(carPos.getZ()) : abs(carPos.getX());
	Ogre::LogManager::getSingleton().logMessage("check carx: " + Ogre::StringConverter::toString(carPos.getX()) );
	//parallel to x
	//if (1 == axis)
	//{
	//	distance = carPos.getZ(); //getDistance(btVector3(0, 0, carPos.getZ()), btVector3(0,0,z));
	//}

	Ogre::Vector3 direction = mSceneNode->getOrientation() * Ogre::Vector3::UNIT_Z;
	//direction.normalise();
	btVector3 velocity = btVector3(direction.x, 0, direction.z).normalize() * mCurrentSpeed;
	mRigidBody->activate();
	//mRigidBody->applyCentralForce(velocity);
	mRigidBody->setLinearVelocity(velocity);

	//float angularVelocity = 0.2f;

	//float angularVelocity =  mRigidBody->getAngularVelocity().length();
	

	float scale = track["scale"].GetDouble();
	Value &trackPosArray = track["postion"];
	float x = scale * trackPosArray[SizeType(0)].GetDouble();
	float y = scale * trackPosArray[SizeType(1)].GetDouble();
	float z = scale * trackPosArray[SizeType(2)].GetDouble();
	float controlDistance = (1 == axis) ? abs(z) : abs(x) ;


	//float controlDistance = 32 * 6;
	Ogre::LogManager::getSingleton().logMessage("controldistance: " + Ogre::StringConverter::toString(controlDistance) + "  distance: " +  Ogre::StringConverter::toString(distance));
	//if (distance > controlDistance)
	//{
	//	mAngularVelocity = mAngularVelocity - 0.0001 * mCurrentSpeed;
	//}
	//else
	//{
	//	mAngularVelocity = mAngularVelocity + 0.0001 * mCurrentSpeed;
	//}

	float d = abs(distance - controlDistance);
	if (d > 0.1)
	{
		if (distance > controlDistance)
		{
			//mAngularVelocity = mAngularVelocity - 0.00001 * mCurrentSpeed * d;
			//mAngularVelocity = mAngularVelocity - 0.00001 * d;
			//float temp = abs(mAngularVelocity)- 0.00001 * d
			mAngularVelocity =  abs(mAngularVelocity)- 0.00001 * d;
		}
		else
		{
			//mAngularVelocity = mAngularVelocity + 0.00001 * mCurrentSpeed * d;
			//mAngularVelocity = mAngularVelocity + 0.00001 * d;
			mAngularVelocity =  abs(mAngularVelocity) + 0.00001 * d;
		}

	}
	else
	{
		mAngularVelocity = 0.f;
	}
	/*if (mAngularVelocity > 2.0f)
	{
		mAngularVelocity = 0;
	}*/
	mRigidBody->setAngularVelocity(btVector3(0, mAngularVelocity, 0));

}

void 
Car::moveOnCurveTrack(Value &track)
{
	//btVector3 carPos = mRigidBody->getWorldTransform().getOrigin();
	////int trackIndex = getTrackNum(level);

	//float scale = track["scale"].GetDouble();
	//int turning = track["turning"].GetInt();
	////float radius = scale * track["radius"].GetDouble();
	//Value &trackPosArray = track["postion"];
	//float x = scale * trackPosArray[SizeType(0)].GetDouble();
	//float y = scale * trackPosArray[SizeType(1)].GetDouble();
	//float z = scale * trackPosArray[SizeType(2)].GetDouble();

	//float radius = getDistance(btVector3(carPos.getX(), y, carPos.getZ()), btVector3(x,y,z));
	//*******************************************************************



	//mCurrentSpeed = mRigidBody->getLinearVelocity().length();

	//float radius = getDistance(btVector3(carPos.getX(), 0, carPos.getZ()), btVector3(-32 * 6, 0, 0));
	//float radius = getDistance(carPos, btVector3(-32 * 6, 0, 0));
	Ogre::Vector3 direction = mSceneNode->getOrientation() * Ogre::Vector3::UNIT_Z;
	//direction.normalise();
	btVector3 velocity = btVector3(direction.x, 0, direction.z).normalize() * mCurrentSpeed;
	mRigidBody->activate();
	//mRigidBody->applyCentralForce(velocity);
	mRigidBody->setLinearVelocity(velocity);

	//float angularVelocity = 0.2f;

	//*******************************************************************
	//float angularVelocity =  1 * turning * mCurrentSpeed / radius;
	//*******************************************************************

	//mAngularVelocity =  1 * turning * mCurrentSpeed / radius;




	//*******************************************************************
	float currentRadius = getDistanceFromCurveCenter(track);
	float d = abs(currentRadius - initialRadiusForEntryPoint);
	float initialAngularVelocity = calculateAngularVeloctiyOnCurve(track, initialRadiusForEntryPoint);
	float tempAngularVelocity = calculateAngularVeloctiyOnCurve(track,currentRadius);
	float delta = abs(tempAngularVelocity - initialAngularVelocity);

	if (d > 0.1)
	{
		
		int sign = mAngularVelocity > 0 ? 1 : -1;
		if (currentRadius < initialRadiusForEntryPoint)
		{ 
			//mAngularVelocity = (abs(mAngularVelocity) - 0.0001 * d) * sign;
			if (abs(mAngularVelocity < abs(initialAngularVelocity) + delta))
			{
				mAngularVelocity = (abs(initialAngularVelocity) - 0.0001 * d) * sign;
			}
			
		}
		else
		{
			if (abs(mAngularVelocity < abs(initialAngularVelocity) + delta))
			{
				mAngularVelocity = (abs(initialAngularVelocity) + 0.0001 * d) * sign;
			}
		}
	}
	else
	{
		mAngularVelocity = initialAngularVelocity;
	}
	//*******************************************************************
	mRigidBody->setAngularVelocity(btVector3(0, mAngularVelocity, 0));
	//mAngularVelocity = 0;

	btScalar angle = mRigidBody->getWorldTransform().getRotation().getAngle();
	//mSceneNode->getOrientation().a
	//float coefficient = mMaxSpeed / angularVelocity;
	//Ogre::LogManager::getSingleton().logMessage("turning: " + Ogre::StringConverter::toString(turning) );
	Ogre::LogManager::getSingleton().logMessage("radius: " + Ogre::StringConverter::toString(initialRadiusForEntryPoint) );
	Ogre::LogManager::getSingleton().logMessage("bullet angle: " + Ogre::StringConverter::toString(angle) + "  ogre: "  + Ogre::StringConverter::toString(""));
	//Ogre::LogManager::getSingleton().logMessage("collidewith: " + Ogre::StringConverter::toString(collideWithType) + " track: " + Ogre::StringConverter::toString(BaseObject::TRACK));

}

void 
Car::autoDrive()
{
	
	mCurrentSpeed = mRigidBody->getLinearVelocity().length();
	if(collideWithType != BaseObject::TRACK)
	{
		return;
	}
	Document &document = Configuration::getInstance()->mDocument;

	char* trackListString = level == 0 ? "track_list_0" : "track_list_1";
	int num = getTrackNum(level);

	int angle = -1;


	if (num > -1)
	{
		Value &track = document[trackListString][Configuration::concatenate("track", std::to_string(num).c_str())];
		angle = track["centralAngle"].GetInt();
		angle > 0 ? moveOnCurveTrack(track) : moveOnStraightTrack(track);

		//***********************************
		if (oldTrackNum != num)
		{
			mAngularVelocity = 0.f;
			//Value &track = document[trackListString][Configuration::concatenate("track", std::to_string(num).c_str())];
			if (angle > 0)
			{

				float initialRadius;
				//initialRadiusForEntryPoint = getDistanceFromCurveCenter(track);
				initialRadiusForEntryPoint = 32 * 6;
				mAngularVelocity = calculateAngularVeloctiyOnCurve(track, initialRadiusForEntryPoint);
			}
			int carRotation = track["carOrietation"].GetInt();
			setRotation(M_PI * carRotation / 180);
			mRigidBody->setAngularVelocity(btVector3(0, mAngularVelocity, 0));
		}
		oldTrackNum = num;
	//***********************************
	}	
	
	//when game reset tracknum is -1, so move the car foward a little bit
	btVector3 carPos = mRigidBody->getWorldTransform().getOrigin();

	Ogre::Vector3 direction = mSceneNode->getOrientation() * Ogre::Vector3::UNIT_Z;
	//direction.normalise();
	btVector3 velocity = btVector3(direction.x, 0, direction.z).normalize() *  ((collideWithType == BaseObject::TRACK) ? mMaxSpeed : mMaxSpeed * 0.2);
	mRigidBody->activate();
	//mRigidBody->applyCentralForce(velocity);
	mRigidBody->setLinearVelocity(velocity);
	//mRigidBody->setAngularVelocity(btVector3(0, 0, 0));
}

bool
Car::checkCurve(Value &track, float scale, float width, int angle)
{
	float radius = scale * track["radius"].GetDouble();
	Value &trackPosArray = track["postion"];
	float x = scale * trackPosArray[SizeType(0)].GetDouble();
	float y = scale * trackPosArray[SizeType(1)].GetDouble();
	float z = scale * trackPosArray[SizeType(2)].GetDouble();

	Value &startPosArray = track["start"];
	float startX = scale * startPosArray[SizeType(0)].GetDouble();
	float startY = scale * startPosArray[SizeType(1)].GetDouble();
	float startZ = scale * startPosArray[SizeType(2)].GetDouble();

	Value &endPosArray = track["end"];
	float endX = scale * endPosArray[SizeType(0)].GetDouble();
	float endY = scale * endPosArray[SizeType(1)].GetDouble();
	float endZ = scale * endPosArray[SizeType(2)].GetDouble();

	btVector3 carPos= mRigidBody->getWorldTransform().getOrigin();
	float distance = getDistance(btVector3(carPos.getX(),y,carPos.getZ()), btVector3(x,y,z));

	//Ogre::LogManager::getSingleton().logMessage("carorigin: " +  Ogre::StringConverter::toString(Ogre::Vector3(carPos.getX(), carPos.getY(), carPos.getZ())) + " width:" + Ogre::StringConverter::toString(width));
	//Ogre::LogManager::getSingleton().logMessage("curvepos: " +  Ogre::StringConverter::toString(Ogre::Vector3(x, y, z)) + " distance: " +  Ogre::StringConverter::toString(distance));


	float xMin, xMax, zMin, zMax;
	xMin = zMin = std::numeric_limits<float>::infinity();
	xMax = zMax = -std::numeric_limits<float>::infinity();
	

	//quater circle
	if ((angle / 90) % 2 == 1)
	{
		xMax = getLargerValue(startX, endX);
		xMin = getSmallerValue(startX, endX);
		zMax = getLargerValue(startZ, endZ);
		zMin = getSmallerValue(startZ, endZ);

		//fix with width
		if (x < xMax)
		{
			xMax = xMax + width;
		}
		if (x > xMin)
		{
			xMin = xMin - width;
		}
		if (z < zMax)
		{
			zMax = zMax + width;
		}
		if (z > zMin)
		{
			zMin = zMin - width;
		}
	}

	//semi circle
	else
	{
		Value &startPosArray = track["mid"];
		float midX = scale * startPosArray[SizeType(0)].GetDouble();
		float midY = scale * startPosArray[SizeType(1)].GetDouble();
		float midZ = scale * startPosArray[SizeType(2)].GetDouble();

		xMax = getLargerValue(startX, endX);
		xMin = getSmallerValue(startX, endX);
		zMax = getLargerValue(startZ, endZ);
		zMin = getSmallerValue(startZ, endZ);

		xMax = getLargerValue(xMax, midX);
		xMin = getSmallerValue(xMin, midX);
		zMax = getLargerValue(zMax, midZ);
		zMin = getSmallerValue(zMin, midZ);

		//fix with width
		//startx == endx
		if (abs(startX - endX) < 0.0001f)
		{
			//fix with width
			if (x < xMax)
			{
				xMax = xMax + width;
			}
			if (x > xMin)
			{
				xMin = xMin - width;
			}
			zMax = zMax + width;
			zMin = zMin - width;
	
		}
		//startx != endx
		else
		{
			//fix with width
			xMax = xMax + width;
			xMin = xMin - width;

			if (z < zMax)
			{
				zMax = zMax + width;
			}
			if (z > zMin)
			{
				zMin = zMin - width;
			}
		}
	}

	/*if (distance < radius + width && distance > radius - width && 
		carPos.getX() > x - width && carPos.getX() < x + width &&
		carPos.getZ() > z - width && carPos.getZ() < z + width)*/
	//Ogre::LogManager::getSingleton().logMessage("xMax: " + Ogre::StringConverter::toString(xMax) + " xMin: " + Ogre::StringConverter::toString(xMin) + 
	//	" zMax: " + Ogre::StringConverter::toString(zMax) + " zMin: " + Ogre::StringConverter::toString(zMin));
	if (distance < radius + width && distance > radius - width &&
		carPos.getX() < xMax && carPos.getX() > xMin && 
		carPos.getZ() < zMax && carPos.getZ() > zMin)
	{
		return true;
	}
	return false;
}

bool 
Car::checkStraight(Value &track, float scale, float width, int angle)
{
	float length = track["length"].GetDouble();
	Value &trackPosArray = track["postion"];
	float x = scale * trackPosArray[SizeType(0)].GetDouble();
	float y = scale * trackPosArray[SizeType(1)].GetDouble();
	float z = scale * trackPosArray[SizeType(2)].GetDouble();

	Value &startPosArray = track["start"];
	float startX = scale * startPosArray[SizeType(0)].GetDouble();
	float startY = scale * startPosArray[SizeType(1)].GetDouble();
	float startZ = scale * startPosArray[SizeType(2)].GetDouble();

	Value &endPosArray = track["end"];
	float endX = scale * endPosArray[SizeType(0)].GetDouble();
	float endY = scale * endPosArray[SizeType(1)].GetDouble();
	float endZ = scale * endPosArray[SizeType(2)].GetDouble();

	float xMin, xMax, zMin, zMax;
	xMin = zMin = std::numeric_limits<float>::infinity();
	xMax = zMax = -std::numeric_limits<float>::infinity();

	if (abs(startX - endX) < 0.0001f)
	{
		//xMax = startX + width;
		//xMin = startX - width;

		xMax = x + width;
		xMin = x - width;
		zMax = getLargerValue(startZ, endZ);
		zMin = getSmallerValue(startZ, endZ);
	}
	else
	{
		//zMax = startZ + width;
		//zMin = startZ - width;

		zMax = z + width;
		zMin = z - width;
		xMax = getLargerValue(startX, endX);
		xMin = getSmallerValue(startX, endX);
	}

	btVector3 carPos= mRigidBody->getWorldTransform().getOrigin();

	//Ogre::LogManager::getSingleton().logMessage("straightcarorigin: " +  Ogre::StringConverter::toString(Ogre::Vector3(carPos.getX(), carPos.getY(), carPos.getZ())));
	//Ogre::LogManager::getSingleton().logMessage("straightpos: " +  Ogre::StringConverter::toString(Ogre::Vector3(x,y,z)));
	//Ogre::LogManager::getSingleton().logMessage("xMax: " + Ogre::StringConverter::toString(xMax) + " xMin: " + Ogre::StringConverter::toString(xMin) + 
	//	" zMax: " + Ogre::StringConverter::toString(zMax) + " zMin: " + Ogre::StringConverter::toString(zMin));

	if (carPos.getX() < xMax && carPos.getX() > xMin && 
		carPos.getZ() < zMax && carPos.getZ() > zMin)
	{
		return true;
	}
	return false;


}

int
Car::getTrackNum(int level) 
{
	Document &document = Configuration::getInstance()->mDocument;
	int num = -1;
	//0 level1, 1 level2
	char* trackListString = level == 0 ? "track_list_0" : "track_list_1";
	int tracksTotalNum = level == 0 ? 4 : 6;
	//char* tstr = Configuration::concatenate("track", std::to_string(0).c_str());//strcat("tarck", "0"); // "track0"; 
	//Ogre::LogManager::getSingleton().logMessage("track str: " + str);
	////int angle = document[trackListString][Configuration::concatenate("track", std::to_string(0).c_str())]["centralAngle"].GetInt();
	//Value &value = document[trackListString][Configuration::concatenate("track", std::to_string(0).c_str())];
	//int angle = value["centralAngle"].GetInt();// document["basicCarData"]["carNum"].GetInt();
	//Ogre::LogManager::getSingleton().logMessage("track angle: " + Ogre::StringConverter::toString(angle));

	for(int i = 0; i < tracksTotalNum; i++) 
	{
		Value &track = document[trackListString][Configuration::concatenate("track", std::to_string(i).c_str())];
		float scale = track["scale"].GetDouble();
		float width = scale * track["width"].GetDouble();
		int angle = track["centralAngle"].GetInt();
		bool isOnTrack = angle > 0? checkCurve(track, scale, width, angle) : checkStraight(track, scale, width, angle);
		//Ogre::LogManager::getSingleton().logMessage("level: " + Ogre::StringConverter::toString(level));
		//Ogre::LogManager::getSingleton().logMessage((angle > 0 ? "curvetrack" : "straighttrack") + Ogre::StringConverter::toString(i));
		if (isOnTrack)
		{
			num = i;
			break;
		}
	}

	Ogre::LogManager::getSingleton().logMessage("tracknum " + Ogre::StringConverter::toString(num));
	return num;
}


void 
Car::reset()
{
	//resetBasicObject();
	mCurrentLap = -1;
	mLeft = true;
	Document &document = Configuration::getInstance()->mDocument;
	Value &car = document["car_list"][Configuration::concatenate("car", std::to_string(mID).c_str())];
	mMaxSpeed = car["maxSpeed"].GetInt();
	mCurrentSpeed = 0.0f;
	mForward = true;
	//mRigidBody->setAngularFactor(btVector3(0,1,0));
	mRigidBody->setLinearVelocity(btVector3(0,0,0));
	mRigidBody->setAngularVelocity(btVector3(0,0,0));
	collideWithType = BaseObject::TRACK;
	mAngularVelocity = 0;
	carId = mID;
	lapCollision = false;
	initialRadiusForEntryPoint = 0.0f;
	oldTrackNum = - 1;
	mWinner = false;
}

