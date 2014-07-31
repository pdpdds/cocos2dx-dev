#include "SSUtil.h"

SSUtil::SSUtil(void)
{
}

SSUtil::~SSUtil(void)
{
}

float SSUtil::radiansBetweenLines(CCPoint l1p1, CCPoint l1p2, CCPoint l2p1, CCPoint l2p2) {

	//l1p1 is line1 point1, l1p2 is line 1 point2. Two points make up a line

	float dXl1 = l1p2.x - l1p1.x;
	float dYl1 = l1p2.y - l1p1.y;
	float dXl2 = l2p2.x - l2p1.x;
	float dYl2 = l2p2.y - l2p1.y;

	float gradientLine1 = (l1p2.y - l1p1.y) / (l1p2.x - l1p1.x);
	float gradientLine2 = (l2p2.y - l2p1.y) / (l2p2.x - l2p1.x);

	float degs = acosf(((dXl1*dXl2) + (dYl1*dYl2)) / ((sqrt(dXl1*dXl1 + dYl1*dYl1)) * (sqrt(dXl2*dXl2 + dYl2*dYl2))));

	return (gradientLine2 > gradientLine1) ? degs : -degs;
}