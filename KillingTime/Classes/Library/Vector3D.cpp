#include "Vector3D.h"


/*bool Vector3D::x(float iX, float iY , float iZ ) {
	//return [[self->alloc() , iX , iY , iZ] autorelease];
	return true;
}*/
Vector3D* Vector3D::create(float iX, float iY , float iZ )
{
	Vector3D* pVector = new Vector3D;
	pVector->initWithX(iX, iY, iZ);
	return pVector;
}

bool Vector3D::init() 
{
    if( (this->init()) ) 
	{

    }
    return true;
}

bool Vector3D::initWithX(float iX, float iY , float iZ ) {
	if((this->init())){
		this->setX(iX);
		this->setY(iY);
		this->setZ(iZ);
	}
	return true;
}
bool Vector3D::isVector(Vector3D* v1, Vector3D* v2 ) 
{
	if(v1->m_x == v2->m_x && v1->m_y == v2->m_y && v1->m_z == v2->m_z)
		return true;	

	return false;
}


