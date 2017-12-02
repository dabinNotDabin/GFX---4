#include "RayTracer.h"
#include <iostream>

RayTracer::RayTracer(Scene * s, int maxd, int sm){
  scene = s;
  maxdepth = maxd;
  samples = sm;
}

// This function determines the closest object that a ray intersects.
// The getIntersection() method is polymorphic. Please provide implementations
// for all the objects in your scene (see Object.h and Object.cpp).
Object * RayTracer::intersect(Ray r){
  scene->startIteration();
  Object * current = NULL;
  Object * minObject = NULL;
  Point inter;
  //Initialize min to infinite point;
  Point min = Point::Infinite();
  while((current = scene->getNextObject()) != NULL){
    inter = current->getIntersection(r);
    if((inter - r.origin).length() > 1E-6){
      if((inter-r.origin).length()  < (min-r.origin).length()){
        min = inter;
        minObject = current;
      }
    }
  }
  return minObject;
}

// Trace a ray recursively
Color RayTracer::trace(Ray r, int depth)
{ 
    Color rad = Color(0.0,0.0,0.0,0.0);
    Object * obj = intersect(r);
	Point intersection = Point::Infinite();

	if (obj != nullptr)
		intersection = obj->getIntersection(r);
	else
		return rad;

	if (depth >= maxdepth)
		return Phong(obj->getNormal(intersection), intersection, r, obj);

	rad = Phong(obj->getNormal(intersection), intersection, r, obj);

	if (obj->getMaterial()->type == REFLECTIVE)
	{
		r = r.reflect(obj->getNormal(intersection), intersection);
		rad = rad + trace(r, depth + 1) * obj->getMaterial()->kr;
	}
	else if (obj->getMaterial()->type == REFRACTIVE)
	{
		//Kr * reflectionColor + (1 - Kr) * refractionColor;
		r = r.refract(obj->getNormal(intersection), intersection, 1.0, obj->getMaterial()->kt);
		rad = rad + trace(r, depth + 1) * obj->getMaterial()->kt;
	}


  rad.clamp(1.0);
  
  return rad;
}

// Local Phong illumination at a point.
Color RayTracer::Phong(Point normal, Point intersection, Ray ray, Object * obj)
{
    Color lightVal = obj->getMaterial()->ambient;
	Material * mat = obj->getMaterial();

    normal.normalize();
	Point lightPos;
    for (unsigned int i = 0; i < scene->lights.size(); i++) 
    {
        lightPos = scene->lights[i];
		
        Point lightDir = lightPos - intersection;
        lightDir.normalize();

        double diffuseIntensity = 0.0; 
        double specularIntensity = 0.0;

		Object * o = intersect(Ray(intersection, lightDir));

		if (o == nullptr)
		{
			diffuseIntensity = min (abs((lightDir * normal)), 1.0);
			
			Point viewDir = *(scene->camera) - intersection;
			Point halfAngle = (lightDir + viewDir);
			halfAngle.normalize();

			double specWeight = min (abs(halfAngle * normal), 1.0);
			specularIntensity = pow(specWeight, 100.0);
		}

		double lightScalar =  1.0 / (double)(scene->lights.size());
        lightVal = lightVal + (((mat->diffuse * diffuseIntensity) + (mat->specular * specularIntensity)) * lightScalar);
    }

/*
	if (mat->type == REFLECTIVE)
	{
		ray = reflected Ray;
		lightVal = lightVal + trace(ray, );
	}

	if (mat->type == REFRACTIVE)
	{
		ray = refracted Ray;
		rad = rad + trace(ray);
	}
*/
	lightVal.clamp(1.0);
  
	return lightVal;
}


// This function generates point on the image plane and starts a trace 
// through them.
// Grid supersampling is also implemented.
Color RayTracer::calculate(int x, int y){
  Color c = Color(0.0,0.0,0.0,0.0);
  for(int i = 1; i <= samples; i++){
    for(int j = 1; j <= samples; j++){
      double one, two;
      // We subtract 0.5 so that the (u,v)-origin is at the
      //center of the image plane.
      one = double(x)-0.5+double(i)/double(samples);
      two = double(y)-0.5+double(j)/double(samples);
      Ray r = scene->makeRay(one,two);
      c = c+trace(r,0);
    }
  }
  c = c*(1.0/double(samples*samples));
  
  c.clamp(1.0);

  return c;
}

