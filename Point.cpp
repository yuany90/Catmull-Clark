#include "Point.h"

Point::Point(float xx, float yy, float zz){
	x = xx;
	y = yy;
	z = zz;
}
void Point::toZero(){
	x = 0;
	y = 0;
	z = 0;
}
Point operator+ (const Point& v1, const Point& v2){
	 return Point(v1.x+v2.x,v1.y+v2.y,v1.z+v2.z);
}
Point operator- (const Point& v1, const Point& v2){
	return Point(v1.x-v2.x,v1.y-v2.y,v1.z-v2.z);
}
Point operator* (const Point& v, int k){
	return Point(v.x*k,v.y*k,v.z*k);
}
Point operator* (int k, const Point& v){
	return Point(v.x*k,v.y*k,v.z*k);
}
Point operator/ (const Point& v, int k){
	return Point(v.x/k,v.y/k,v.z/k);
}
void Point::divide(int k){
	x /= k;
	y /= k;
	z /= k;
}

std::ostream& operator<<(std::ostream &strm, const Point &v) {
	return strm << "point(" << v.x << ", " << v.y << ", " << v.z << ")";
}
