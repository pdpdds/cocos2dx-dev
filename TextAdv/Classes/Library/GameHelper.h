#ifndef GAMEHELPER_H
#define GAMEHELPER_H
#include "Vector3D.h"

#include "cocos2d.h"
#include <Box2D/Box2D.h>
USING_NS_CC;

#define PI_CONSTANT 3.14159f

typedef enum {
	NO_DIRECTION = 0,
	LEFT = 1,
	UP_LEFT = 2,
	UP = 3,
	UP_RIGHT = 4,
	RIGHT = 5,
	DOWN_RIGHT = 6,
	DOWN = 7,
	DOWN_LEFT = 8
} DirectionType;

class GameHelper: public CCObject {
public:
	bool clockwiseO(Vector3D* O, Vector3D* A , Vector3D* B );
CCArray* convexHull(CCArray* P);
float distanceP1(CCPoint p1, CCPoint p2 );
float degreesToRadians(float d);
float radiansToDegrees(float r);
float vectorToRadians(CCPoint vector);
CCPoint radiansToVector(float radians);
Vector3D* quadraticA(float a, float b , float c );
float absoluteValue(float a);
CCPoint midPointP1(CCPoint p1, CCPoint p2 );
bool point(CCPoint p, CCRect r );
bool point(CCPoint p, CCPoint origin , float radius );
CCString* sanitizeString(CCString* str);

	
b2Vec2 b2ClampA(const b2Vec2& a, const b2Vec2& low , const b2Vec2& high ) {
	return b2Max(low, b2Min(a, high));

	}


};


#endif //GAMEHELPER_H