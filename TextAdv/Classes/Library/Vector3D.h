#ifndef VECTOR3D_H
#define VECTOR3D_H

#include "cocos2d.h"
USING_NS_CC;

//Vector3D Class
class Vector3D : CCObject {
public:
	float x;
	float y;
	float z;

	static Vector3D* create(float iX, float iY , float iZ );

	virtual bool init();
	bool initWithX(float iX, float iY , float iZ );
	
	static bool isVector(Vector3D* v1, Vector3D* v2 );

	CC_SYNTHESIZE(float, m_x, X)
    CC_SYNTHESIZE(float, m_y, Y)
	CC_SYNTHESIZE(float, m_z, Z)
};




#endif //VECTOR3D_H