#include "GameHelper.h"

//2D cross product of OA and OB vectors, i.e. z-component of their 3D cross product.
//Returns true for clockwise turn or the points are collinear, false another.
bool GameHelper::clockwiseO(Vector3D* O, Vector3D* A , Vector3D* B ) {
	return (A->x - O->x) * (B->y - O->y) - (A->y - O->y) * (B->x - O->x) <= 0;
}

//Given a set of points it returns a polygon surrounding those points (a convex hull)
//From here: http://www.algorithmist.com/index.php/Monotone_Chain_Convex_Hull.cpp
CCArray* GameHelper::convexHull(CCArray* P) {
	/*int n = P->count();
	int k = 0;
	CCArray *H = CCArray::create->initWithCapacity(n*2);
	
	//Sort points lexicographically (by X, then secondarily by Y)
	NSSortDescriptor *xDescriptor = NSSortDescriptor::alloc()->initWithKey("self.x" , true);
	NSSortDescriptor *yDescriptor = NSSortDescriptor::alloc()->initWithKey("self.y" , true);
	
	CCArray *descriptors = CCArray::arrayWithObjects(xDescriptor, yDescriptor, NULL);
	CCArray *sortedP = P::sortedArrayUsingDescriptors(descriptors);
	
	//Build lower hull
	for (int i = 0; i < n; i ++) {
		while (k >= 2 && this->clockwiseO([H , k-2) , H->objectAtIndex(k-1) , sortedP->objectAtIndex(i)]){
			k--;
		}
		H->insertObject([sortedP , i) , k++];
	};
 	
	//Build upper hull
	for (int i = n-2, t = k + 1; i >= 0; i --) {		
		while (k >= t && this->clockwiseO([H , k-2) , H->objectAtIndex(k-1) , sortedP->objectAtIndex(i)]){
			k--;
		}
		H->insertObject([sortedP , i) , k++];
	};	
	
	H->removeObjectsInRange(CCRangeMake(k, H.count-k));

	//Remove all duplicate objects
	CCArray *noDupeArray = CCArray::alloc()->init();
	for(int i = 0; i<H.count; i++){
		if(!noDupeArray->containsObject(H::objectAtIndex(i))){
			noDupeArray->addObject(H::objectAtIndex(i));
		}
	}*/

	CCArray *noDupeArray = CCArray::create();
	noDupeArray->init();
	return noDupeArray;
}
float GameHelper::distanceP1(CCPoint p1, CCPoint p2 ) {
	return sqrt( pow( (p1.x-p2.x) ,2) + pow( (p1.y-p2.y) ,2) );
}
float GameHelper::degreesToRadians(float d) {
	return d * (PI_CONSTANT/180);
}
float GameHelper::radiansToDegrees(float r) {
	return r * (180/PI_CONSTANT);
}
float GameHelper::vectorToRadians(CCPoint vector){
	if(vector.x == 0) {vector.x = 0.000001f; }
	float baseRadians = atan(vector.x/vector.y);
	if(vector.y < 0){ baseRadians += PI_CONSTANT; }	//Adjust for -Y
	return baseRadians;
}
CCPoint GameHelper::radiansToVector(float radians){
	return CCPointMake(sin(radians), cos(radians));
}
Vector3D* GameHelper::quadraticA(float a, float b , float c ) {
  Vector3D *solution = NULL;
  if (GameHelper::absoluteValue(a) < 1e-6) {
    if (GameHelper::absoluteValue(b) < 1e-6) {
      solution = GameHelper::absoluteValue(c) < 1e-6 ? Vector3D::create(0 , 0 , 0) : NULL;
    } else {
      solution = Vector3D::create(-c/b , -c/b , 0);
    }
  } else {
    float disc = b*b - 4*a*c;
    if (disc >= 0) {
      disc = sqrt(disc);
      a = 2*a;
      solution = Vector3D::create((-b-disc)/a , (-b+disc)/a , 0);
    }
  }
  return solution;
}
float GameHelper::absoluteValue(float a) {
	if(a < 0){ a*= -1; }
	return a;
}
CCPoint GameHelper::midPointP1(CCPoint p1, CCPoint p2 ) {
	return CCPointMake( (p1.x+p2.x)/2 , (p1.y+p2.y)/2 );
}


bool GameHelper::point(CCPoint p, CCRect r ) {
	bool isInRect = false;
	if( p.x < r.origin.x + r.size.width && 
	   p.x > r.origin.x &&
	   p.y < r.origin.y + r.size.height &&
	   p.y > r.origin.y )
	{
		isInRect = true;
	}
	return isInRect;
}
bool GameHelper::point(CCPoint p, CCPoint origin , float radius ) {
	bool isInCircle = false;
	if(GameHelper::distanceP1(p , origin ) <= radius){
		isInCircle = true;
	}
	return isInCircle;
}

CCString* GameHelper::sanitizeString(CCString* str) {
	/*for(CCInteger i=0; i<str->length(); i++) {
		char c = str->characterAtIndex(i);
		if(c >= kCCBMFontMaxChars) { 
			str = str->stringByReplacingCharactersInRange(CCRangeMake(i, 1) , "'");
		}
	}*/
	return str;
}


