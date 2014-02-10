#ifndef _POINT_H_
#define _POINT_H_
#include<iostream>

class Point {
public:
    float x, y, z;
    Point(float xx = 0.0, float yy = 0.0, float zz = 0.0);
	void Point::toZero();
	void divide(int k);
};
Point operator+ (const Point& v1, const Point& v2);
Point operator- (const Point& v1, const Point& v2);
Point operator* (const Point& v, int k);
Point operator* (int k, const Point& v);
Point operator/ (const Point& v, int k);
std::ostream& operator<<(std::ostream &strm, const Point &v);

#endif
