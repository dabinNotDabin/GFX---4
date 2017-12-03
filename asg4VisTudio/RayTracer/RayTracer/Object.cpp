#include "Object.h"
#include <iostream>

#define PI         3.141592653589793

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

	double t = (((p1 - r.origin) * normalB) / dot);

	// Point where ray intersects the triangle's supporting plane.
	Point intersection = r.origin + (r.direction * t);

	if (((intersection - r.origin) * r.direction) < 0 + EPS)
		intersection = Point::Infinite();

	// Need to check if the point is inside the triangle.
	double areaTri = 0.5*((p2 - p1).cross(p3 - p1)).length();
	double area1 = 0.5*((p2 - p1).cross(intersection - p1)).length() / areaTri;
	double area2 = 0.5*((p3 - p1).cross(intersection - p1)).length() / areaTri;
	double area3 = 0.5*((p3 - p2).cross(intersection - p2)).length() / areaTri;


	if ((area1 + area2 + area3) <= 1.0 + EPS)
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
//    return normal;
}

Point Sphere::getNormal(Point p){
    Point ret = (p-center);
    
    ret.normalize();
    return ret;
}

Point Sphere::getIntersection(Ray r)
{
	//Point a = (center - r.origin);
	//a.normalize();

	//Point b = r.direction;
	//b.normalize();

	//Point c = (center - r.origin);

	//double theta = acos(a * b);

	//double proj = c.length() * cos(theta);

	//Point d = b * proj;

	//double dist = (d - c).length();


	//if (dist == radius + EPS)
	//	return d;
	//else if (dist < radius + EPS)
	//{
	//	double cut = sqrt(radius - dist);
	//	dist = dist + cut;
	//	return Point(r.origin + b * dist);
	//}
	//else
	//	return Point::Infinite();

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
		d1 = -a + sqrt(det);
		d2 = -a - sqrt(det);
		
		Point p = Point::Infinite();

		if (d1 < EPS && d2 < EPS)
			return p;
		else if (d1 < EPS)
			p = r.origin + r.direction * d2;
		else if (d2 < EPS)
			p = r.origin + r.direction * d1;
		else
		{
			if (d1 < d2)
				p = r.origin + r.direction * d1;
			else
				p = r.origin + r.direction * d2;
		}

		if (((p - r.origin) * r.direction) < EPS)  
			p = Point::Infinite();
				
		return p;
	}

}

