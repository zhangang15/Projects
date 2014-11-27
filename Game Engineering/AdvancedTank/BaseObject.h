
#ifndef __BaseObject_h_
#define __BaseObject_h_

class BaseObject
{

public:
	enum Type
	{
		STATIC_OBJECT,
		PROJECTILE1,
		PROJECTILE2,
		TANK
	};

	BaseObject(void);
	~BaseObject(void);

	void setIsCollided(bool value) { isCollided = value; };
	bool getIsCollided() { return isCollided; };

	void setcollideWithType(int value) { collideWithType = value; };
	int getcollideWithType() { return collideWithType; };

	void setType(int value) { type = value; };
	int getType() { return type; };



protected:
	bool isCollided;
	int collideWithType;
	int type;

};

#endif
