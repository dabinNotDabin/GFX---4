#include "Object.h"
#include <iostream>

void Object::setMaterial(Material * mat){
    m = mat;   
}

Triangle::Triangle(Point v1,Point v2, Point v3, Point no){
    p1 = v1;
    p2 = v2;
    p3 = v3;
    
    normal = no;
    isLight = false;
}

Sphere::Sphere(Point p, double r){
    center = p;
    radius = r;
    
    isLight = false;
}
#define EPS 1E-6

Point Triangle::getIntersection(Ray r){

	r.direction.normalize();
	
	Point normalB = getNormal(p1);
	
	double dot = (normalB * r.direction);
	
	if (dot == 0)
		return Point::Infinite();
	
    double t = (  ((p1 - r.origin) * normalB) /  dot);

    // Point where ray intersects the triangle's supporting plane.
    Point intersection = r.origin + (r.direction * t);

	if (((intersection - r.origin) * r.direction) < 0 + EPS)  
		intersection = Point::Infinite();
	
    // Need to check if the point is inside the triangle.
	double areaTri = 0.5*((p2-p1).cross(p3-p1)).length();
	double area1 = 0.5*((p2-p1).cross(intersection-p1)).length()/areaTri;
	double area2 = 0.5*((p3-p1).cross(intersection-p1)).length()/areaTri;
	double area3 = 0.5*((p3-p2).cross(intersection-p2)).length()/areaTri;


	if ( (area1 + area2 + area3) <= 1.0 + EPS)
	{
		return intersection;
	}
	else
		return Point::Infinite();


}

Point Triangle::getNormal(Point p){
    Point one = p1-p2;
    Point two = p1-p3;
    Point ret = one.cross(two);
    ret.normalize();
    return ret;
}

Point Sphere::getNormal(Point p){
    Point ret = (p-center);
    
    ret.normalize();
    return ret;
}

Point Sphere::getIntersection(Ray r)
{
	double d1;
	double d2;
	
	r.direction.normalize();
	
	double a = (r.direction * (r.origin - center));

	double det = a*a - ((r.origin - center).length()) * ((r.origin - center).length()) + radius * radius;


	if (det < 0 - EPS)
		return Point::Infinite();
	else if (det == 0)
	{
		cout << "Ray grazed sphere.\n";
		d1 = -a;
		return r.origin + r.direction * d1;
	}
	else
	{
//		cout << "Ray origin, direction: " << r.origin.x << "," << r.origin.y << "," << r.origin.z << " -> " << r.direction.x << "," << r.direction.y << "," << r.direction.z << endl;

		d1 = -a + sqrt(det);
		d2 = -a - sqrt(det);
		
		Point one = r.origin + r.direction * d1;
		Point two = r.origin + r.direction * d2;

		if (((one - r.origin) * r.direction) < 0 + EPS)  
			one = Point::Infinite();
		
		if (((two - r.origin) * r.direction) < 0 + EPS)  
			two = Point::Infinite();

		if (one.length() <= two.length())
			return one;
		else
			return two;
	}

}

