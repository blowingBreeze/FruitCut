#include "ToolFunc.h"
#include"math\CCMath.h"
USING_NS_CC;

float ToolFunc::getRotaAngle(Point beginPoint,Point touchPoint)
{
	float angle = 0.0;
	if (touchPoint.x != beginPoint.x)
	{
		angle = (touchPoint.y - beginPoint.y) / (touchPoint.x - beginPoint.x);
		angle = atan(angle) / M_PI * 180;
	}
	else
		angle = 90;

	return angle;
}

double ToolFunc::distance(Point a, Point b)
{
	return pow((b.y - a.y)*(b.y - a.y) + (b.x - a.x)*(b.x - a.x), 0.5);
}




