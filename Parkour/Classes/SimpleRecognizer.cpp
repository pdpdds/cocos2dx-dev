//
//  SimpleRecognizer.cpp
//  PaoKu
//
//  Created by YTB on 14-2-28.
//
//

#include "SimpleRecognizer.h"

#define MAX_DOUBLE std::numeric_limits<double>::max();

// class define
SimpleRecognizer::SimpleRecognizer()
{
	this->result = SimpleGesturesError;
}

// be called in onTouchBegan
void SimpleRecognizer::beginPoint(double x, double y)
{
	this->result = SimpleGesturesError;
    points.clear();
	points.push_back(CPoint(x,y));
}

void SimpleRecognizer::movePoint(double x, double y)
{
	points.push_back(CPoint(x, y));
    
	if (result == SimpleGesturesNotSupport) {
		return;
	}
    
	SimpleGestures newRtn = SimpleGesturesError;
	int len = this->points.size();	
	double dx = this->points[len - 1].x - this->points[len - 2].x;
	double dy = this->points[len - 1].y - this->points[len - 2].y;
    
	if (abs(dx) > abs(dy)) 
	{
		
		if (dx > 0) {
			newRtn = SimpleGesturesRight;
		} else {
			newRtn = SimpleGesturesLeft;
		}
	} 
	else 
	{		
		if (dy > 0) {
			newRtn = SimpleGesturesUp;
		} else {
			newRtn = SimpleGesturesDown;
		}
	}
    
	// first set result
	if (result == SimpleGesturesError) {
		result = newRtn;
		return;
	}
    
	// if diretcory change, not support Recongnizer
	if (result != newRtn) {
		result = SimpleGesturesNotSupport;
	}
}

SimpleGestures SimpleRecognizer::endPoint()
{
	if (this->points.size() < 3) {
		return SimpleGesturesError;
	}
	return result;
}

std::vector<CPoint>& SimpleRecognizer::getPoints()
{
	return points;
}
