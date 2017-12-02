#include "Geometry.h"

#define PI         3.141592653589793

// Implementation of Point and Ray classes.

Point Point::Infinite(){
    return Point(1E20,1E20,1E20);
}
Point Point::cross(Point p){
    return Point(y*p.z-z*p.y,z*p.x-x*p.z,x*p.y-y*p.x);   
}

void Point::normalize(){
    double l = this->length();
    x /= l;
    y /= l;
    z /= l;
}

Point::Point(double xo, double yo, double zo){
    x = xo;
    y = yo;
    z = zo;
}

Point Point::operator- (Point p){
    return Point(x-p.x,y-p.y,z-p.z);   
}

Point Point::operator*(double s){
    return Point(x*s,y*s,z*s);   
}

Point Point::operator+(Point p){
    return Point(p.x+x,p.y+y,p.z+z);   
}

bool Point::operator < (Point p){
    return (this->length() < (p).length());
}

// Dot product
double Point::operator* (Point p){
    return x*p.x+y*p.y+z*p.z;   
}

double Point::length(){
    return sqrt(x*x+y*y+z*z);
}

Ray::Ray(){
}

Ray::Ray(Point po, Point vo){
    origin = po;
    direction = vo;
    direction.normalize();
}

Ray Ray::reflect(Point normal, Point m){
    Ray ret;
    double tmp = -(normal*direction);
    
    Point vo = direction+(normal*tmp*2.0);
    
    Point p = Point(m.x,m.y,m.z);
    return Ray(p,vo);
}


Ray Ray::refract(Point normal, Point intersection, double ki, double kt)
{
	Ray ret;

	normal.normalize();
	direction.normalize();

	double thetaInc = abs(normal*direction);
	double ratioRefraction = ki / kt;
	
	double interim = ratioRefraction * sin(PI * thetaInc / 180.0);
	double thetaOut = asin(interim);

	double thetaOutB = pow(ratioRefraction, 2) * (1 - pow(cos(PI * thetaInc / 180.0), 2));

	Point i = direction * ratioRefraction;


	Point t = i + normal * ((ratioRefraction * cos(PI * thetaInc / 180.0)) - sqrt(1 - pow(sin(PI * thetaOutB / 180.0), 2)));
	Point p = Point(intersection.x, intersection.y, intersection.z);

	return Ray(p, t);
}
