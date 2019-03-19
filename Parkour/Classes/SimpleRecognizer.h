//
//  SimpleRecognizer.h
//  PaoKu
//
//  Created by YTB on 14-2-28.
//
//

#ifndef __PaoKu__SimpleRecognizer__
#define __PaoKu__SimpleRecognizer__

#include "cocos2d.h"


typedef enum {
	SimpleGesturesRight,
	SimpleGesturesLeft,
	SimpleGesturesUp,//
	SimpleGesturesDown,//
	SimpleGesturesError,//
	SimpleGesturesNotSupport
    
} SimpleGestures;

class CPoint
{
public:
	//--- Wobbrock used doubles for these, not ints
	//int x, y;
	double x, y;
	CPoint()
	{
		this->x=0;
		this->y=0;
	}
	CPoint(double x, double y)
	{
		this->x = x;
		this->y = y;
	}
};

class SimpleRecognizer
{
protected:
	//--- These are variables because C++ doesn't (easily) allow
	//---  constants to be floating point numbers
	double X;
	double Y;
	SimpleGestures result;
    std::vector<CPoint> points;

public:
	SimpleRecognizer();
    
	void beginPoint(double x, double y);
	void movePoint(double x, double y);
	SimpleGestures endPoint();

    std::vector<CPoint>& getPoints();
    
};
#endif /* defined(__PaoKu__SimpleRecognizer__) */
