#include "Scene.h"


void Scene::startIteration(){
    indexO = 0;
}
// N is width/height
// the field of view is specified in degrees.
Scene::Scene(Point * vo, Point * no, double f, int Wo, int Ho){
    v = vo;
    n = no;
    fov = f*acos(-1)/180.0;

    // scale n to account for implicit image plane.
    *n = (*n)*(1/(2*tan(fov/2.0)));
    Point uo = v->cross(*n);
    uo.normalize();
    u = new Point(uo.x,uo.y,uo.z);
    C = Point(0,0,0);
    W = Wo;
	H = Ho;

	AR = (double)H / (double)W;
	recip = (1.0 - AR);

	recip = (abs(W - H) / 2.0) / W;
	

    indexO = 0;
    indexL = 0;
}

Object * Scene::getNextObject(){
    if(indexO == objects.size()) return NULL;
    return objects[indexO++];
}

Point Scene::getNextLight(){
    indexL++;
    return lights[indexL%lights.size()];    
}

void Scene::addObject(Object * o){
    objects.push_back(o);   
}

void Scene::addLight(Point l){
    lights.push_back(l);
}

// Sets the origin of the camera
void Scene::setCamera(Point * c){
    camera = c;
}

// A ray through a point on the image plane.
// The location of the image plane in the n-direction is implicitly taken to be the point where
// the width and height of the image plane are both 1. Please see the constructor above.
Ray Scene::makeRay(double xo, double yo){
    // calculate point on image plane

	double normx = (xo / H) - 0.5 -recip;
	double normy = (yo / H) - 0.5;

    Point po = (*u)*normx + (*v)*normy + (* camera) + (*n);
    
    Ray r = Ray(po, po - (*camera));
    return r;
}

// A test scene based on description handed out with the assignment.
// Please modify material properties as necessary to test your ray tracer.
Scene * Scene::initTestScene(int W, int H, double FOV, Point * lookat, Point * position){


	//// Scene

	//// make new scene with up vector, direction vector and fov
	Scene * ret = new Scene(new Point(0.0, 1.0, 0), new Point(0.0, 0.0, 1.0), FOV, W, H);

	// Material Properties

	Material * perfectMirror = new Material();
	perfectMirror->type = REFLECTIVE;
	perfectMirror->kr = 0.9;
	perfectMirror->kd = 0.5;
	perfectMirror->kt = 0.0;
	perfectMirror->ambient = Color(0.0, 0.0, 0.0, 1.0);
	perfectMirror->diffuse = Color(0.5, 0.5, 0.5, 1.0);
	perfectMirror->specular = Color(0.8, 0.8, 0.8, 1.0);



	Material * mirror = new Material();
	mirror->type = REFLECTIVE;
	mirror->kr = 1.0;
	mirror->kd = 0.1;
	mirror->kt = 0.0;
	mirror->ambient = Color(0.0, 0.0, 0.1, 1.0);
	mirror->diffuse = Color(0.3, 0.3, 0.3, 1.0);
	mirror->specular = Color(0.6, 0.6, 0.7, 1.0);



	Material * diffuseBlue = new Material();
	diffuseBlue->type = DIFFUSE;
	diffuseBlue->kr = 1.0;
	diffuseBlue->kd = 0.8;
	diffuseBlue->kt = 0.0;
	diffuseBlue->ambient = Color(0.1, 0.1, 0.3, 1.0);
	diffuseBlue->diffuse = Color(0.6, 0.6, 0.8, 1.0);
	diffuseBlue->specular = Color(0.9, 0.9, 0.9, 1.0);


	Material * diffuseGreen = new Material();
	diffuseGreen->type = DIFFUSE;
	diffuseGreen->kr = 1.0;
	diffuseGreen->kd = 0.8;
	diffuseGreen->kt = 0.0;
	diffuseGreen->ambient = Color(0.5, 0.7, 0.5, 1.0);
	diffuseGreen->diffuse = Color(0.5, 0.6, 0.5, 1.0);
	diffuseGreen->specular = Color(1.0, 1.0, 1.0, 1.0);


	Material * diffusePurple = new Material();
	diffusePurple->type = DIFFUSE;
	diffusePurple->kr = 1.0;
	diffusePurple->kd = 0.3;
	diffusePurple->kt = 0.0;
	diffusePurple->ambient = Color(0.6, 0.3, 0.8, 1.0);
	diffusePurple->diffuse = Color(0.6, 0.6, 0.6, 1.0);
	diffusePurple->specular = Color(0.9, 0.9, 0.9, 1.0);


	Material * diffuseBlack = new Material();
	diffuseBlack->type = DIFFUSE;
	diffuseBlack->kr = 1.0;
	diffuseBlack->kd = 0.3;
	diffuseBlack->kt = 0.0;
	diffuseBlack->ambient = Color(0.2, 0.2, 0.2, 1.0);
	diffuseBlack->diffuse = Color(0.3, 0.3, 0.3, 1.0);
	diffuseBlack->specular = Color(0.6, 0.6, 0.6, 1.0);


	Material * translucent = new Material();
	translucent->type = REFRACTIVE;
	translucent->kr = 0.0;
	translucent->kd = 0.0;
	translucent->kt = 0.9;
	translucent->ambient = Color(0.0, 0.0, 0.0, 1.0);
	translucent->diffuse = Color(0.0, 0.0, 0.0, 1.0);
	translucent->specular = Color(0.0, 0.0, 0.0, 1.0);


	Material * translucentBlue = new Material();
	translucentBlue->type = REFRACTIVE;
	translucentBlue->kr = 0.1;
	translucentBlue->kd = 0.2;
	translucentBlue->kt = 0.6;
	translucentBlue->ambient = Color(0.1, 0.1, 0.3, 1.0);
	translucentBlue->diffuse = Color(0.0, 0.0, 0.0, 1.0);
	translucentBlue->specular = Color(0.3, 0.3, 0.3, 1.0);






    // Make points for square
    Point p1 = Point(0,0,0);
    Point p2 = Point(550,0,0);
    Point p3 = Point(0,0,560);
    Point p4 = Point(550,0,560);
    Point p5 = Point(560,550,0);
    Point p6 = Point(560,550,560);
    Point p7 = Point(0,550,560);
    Point p8 = Point(0,550,0);
    // make normals for triangles
    Point n1 = Point(0.0,-1.0,0.0);
    Point n2 = Point(0.0,1.0,0.0);
    Point n3 = Point(1.0,0.0,0.0);
    Point n4 = Point(-1.0,0.0,0.0);
    Point n5 = Point(0.0,0.0,-1.0);
	

	Object * s1 = new Sphere(Point(400.0, 130.0, 320.0), 120.0);
	s1->setMaterial(diffuseBlue);
    ret->addObject(s1);
    
	// Add in bottom square
    Object * t1 = new Triangle(p1,p3,p2,n2);
    Object * t2 = new Triangle(p4,p2,p3,n2);
    t1->setMaterial(diffuseBlue);
    t2->setMaterial(diffuseBlue);
    ret->addObject(t1);
    ret->addObject(t2);

    // Add in left square
    t1 = new Triangle(p3,p1,p8,n3);
    t2 = new Triangle(p3,p8,p7,n3);
    t1->setMaterial(diffusePurple);
    t2->setMaterial(diffusePurple);
	t1->id = 2;
	t2->id = 2;
	ret->addObject(t1);
    ret->addObject(t2);

    // Add in back square
    t1 = new Triangle(p4,p3,p7,n5);
    t2 = new Triangle(p4,p7,p6,n5);
    t1->setMaterial(perfectMirror);
    t2->setMaterial(perfectMirror);
    ret->addObject(t1);
    ret->addObject(t2);

    // Add in right square
    t1 = new Triangle(p4,p2,p5,n4);
    t2 = new Triangle(p4,p5,p6,n4);
    t1->setMaterial(diffuseBlack);
    t2->setMaterial(diffuseBlack);
	t1->id = 1;
	t2->id = 1;
	ret->addObject(t1);
    ret->addObject(t2);

	// Add light sources
	ret->addLight(Point(400.0, 2000.0, 320.0));
	ret->addLight(Point(400.0, 100.0, -200.0));

	// set Camera location
	ret->setCamera(new Point(278, 273, -500));


    // Now we will add in smaller box
    Point v1 = Point(100,165,65);
    Point v2 = Point(52,165,225);
    Point v3 = Point(210,165,272);
    Point v4 = Point(260,165,114);
    Point v5 = Point(260,0,114);
    Point v6 = Point(260,165,114);
    Point v7 = Point(210,165,272);
    Point v8 = Point(210,0,272);
    Point v9 = Point(100,0,65);
    Point v10 = Point(100,165,65);
    Point v11 = Point(52,0,225);
    
    Point no1 = Point(0.0,1.0,0.0);
    Point no2 = Point(0.0,0.0,-1.0);
    Point no3 = Point(-1.0,0.0,0.0);
    Point no4 = Point(1.0,0.0,0.0);
    Point no5 = Point(0.0,0.0,1.0);
    
    // top
    t1 = new Triangle(v2,v3,v1,no1);
    t2 = new Triangle(v4,v1,v3,no1);
	t1->setMaterial(translucentBlue);
	t2->setMaterial(translucentBlue);
	ret->addObject(t1);
    ret->addObject(t2);
    
	// right
    t1 = new Triangle(v5,v7,v4,no4);
    t2 = new Triangle(v5,v7,v8,no4);
    t1->setMaterial(translucentBlue);
    t2->setMaterial(translucentBlue);
    ret->addObject(t1);
    ret->addObject(t2);
    
    // front
    t1 = new Triangle(v9,v10,v4,no2);
    t2 = new Triangle(v9,v4,v5,no2);
    t1->setMaterial(translucentBlue);
    t2->setMaterial(translucentBlue);
    ret->addObject(t1);
    ret->addObject(t2);
    
    // left
    t1 = new Triangle(v11,v2,v10,no3);
    t2 = new Triangle(v11,v2,v9,no3);
    t1->setMaterial(translucentBlue);
    t2->setMaterial(translucentBlue);
    ret->addObject(t1);
    ret->addObject(t2);
        
   
    // back
    t1 = new Triangle(v8,v7,v2,no5);
    t2 = new Triangle(v8,v2,v11,no5);
    t1->setMaterial(translucentBlue);
    t2->setMaterial(translucentBlue);
    ret->addObject(t1);
    ret->addObject(t2);
    
    return ret;
}







// A test scene based on description handed out with the assignment.
// Please modify material properties as necessary to test your ray tracer.
Scene * Scene::initCustomScene(int W, int H, double FOV, Point * lookat, Point * position) {

	//// make new scene with up vector, direction vector and fov
	Scene * ret = new Scene(new Point(0.0, 1.0, 0), lookat, FOV, W, H);

	ret->addLight(Point(-1000.0, 1500.0, -5000.0));
	ret->addLight(Point(4000.0, 2000.0, -5000.0));
	ret->addLight(Point(0.0, 4000.0, 500.0));

	//	ret->setCamera(new Point(1500, 1000, -3000));
	//	ret->setCamera(new Point(1500, 1000, -3000));
	ret->setCamera(position);


	// Material Properties

	Material * perfectMirror = new Material();
	perfectMirror->type = REFLECTIVE;
	perfectMirror->kr = 0.9;
	perfectMirror->kd = 0.0;
	perfectMirror->kt = 0.0;
	perfectMirror->ambient = Color(0.0, 0.0, 0.0, 1.0);
	perfectMirror->diffuse = Color(0.0, 0.0, 0.0, 1.0);
	perfectMirror->specular = Color(1.0, 1.0, 1.0, 1.0);

	Material * diffuseBlue = new Material();
	diffuseBlue->type = DIFFUSE;
	diffuseBlue->kr = 0.9;
	diffuseBlue->kd = 0.6;
	diffuseBlue->kt = 0.0;
	diffuseBlue->ambient = Color(0.4, 0.4, 0.6, 1.0);
	diffuseBlue->diffuse = Color(0.8, 0.8, 0.8, 1.0);
	diffuseBlue->specular = Color(1.0, 1.0, 1.0, 1.0);
	
	Material * diffuseGreen = new Material();
	diffuseGreen->type = DIFFUSE;
	diffuseGreen->kr = 0.9;
	diffuseGreen->kd = 0.7;
	diffuseGreen->kt = 0.0;
	diffuseGreen->ambient = Color(0.4, 0.6, 0.4, 1.0);
	diffuseGreen->diffuse = Color(0.6, 0.6, 0.6, 1.0);
	diffuseGreen->specular = Color(1.0, 1.0, 1.0, 1.0);
	
	Material * diffusePurple = new Material();
	diffusePurple->type = DIFFUSE;
	diffusePurple->kr = 0.8;
	diffusePurple->kd = 0.3;
	diffusePurple->kt = 0.0;
	diffusePurple->ambient = Color(0.6, 0.3, 0.8, 1.0);
	diffusePurple->diffuse = Color(0.6, 0.6, 0.6, 1.0);
	diffusePurple->specular = Color(0.8, 0.8, 0.8, 1.0);
	
	Material * diffuseBlack = new Material();
	diffuseBlack->type = DIFFUSE;
	diffuseBlack->kr = 1.0;
	diffuseBlack->kd = 0.2;
	diffuseBlack->kt = 0.0;
	diffuseBlack->ambient = Color(0.1, 0.1, 0.1, 1.0);
	diffuseBlack->diffuse = Color(0.2, 0.2, 0.2, 1.0);
	diffuseBlack->specular = Color(0.6, 0.6, 0.6, 1.0);

	Material * translucent = new Material();
	translucent->type = REFRACTIVE;
	translucent->kr = 0.0;
	translucent->kd = 0.0;
	translucent->kt = 0.9;
	translucent->ambient = Color(0.0, 0.0, 0.0, 1.0);
	translucent->diffuse = Color(0.0, 0.0, 0.0, 1.0);
	translucent->specular = Color(0.0, 0.0, 0.0, 1.0);
	
	Material * translucentBlue = new Material();
	translucentBlue->type = REFRACTIVE;
	translucentBlue->kr = 0.1;
	translucentBlue->kd = 0.2;
	translucentBlue->kt = 0.9;
	translucentBlue->ambient = Color(0.1, 0.1, 0.3, 1.0);
	translucentBlue->diffuse = Color(0.0, 0.0, 0.0, 1.0);
	translucentBlue->specular = Color(0.3, 0.3, 0.3, 1.0);

	

	//// Geometry
	Object * t1;
	Object * t2;

	Point p1;
	Point p2;
	Point p3;
	Point p4;

	Point n1;
	Point n2;

	// Floor Pts
	p1 = Point(-1000, 0, 1000);
	p2 = Point(-1000, 0, -1000);
	p3 = Point(1000, 0, 1000);
	p4 = Point(1000, 0, -1000);

	n1 = Point(0.0, 1.0, 0.0);

	// Add in floor
	t1 = new Triangle(p3, p2, p1, n1);
	t2 = new Triangle(p2, p3, p4, n1);
	t1->setMaterial(diffuseBlack);
	t2->setMaterial(diffuseBlack);
	t1->id = 10;
	t2->id = 10;
	t1->name = "floor";
	t2->name = "floor";
	ret->addObject(t1);
	ret->addObject(t2);


	// Far mirror Pts
	p1 = Point(-1000, 200, +1200);		// Bot Left
	p2 = Point(-1000, 1200, +1000);		// Top Left
	p3 = Point(300, +200, +1650);		// Bot Right
	p4 = Point(300, +1200, +1450);		// Top Right


	// Mirror normal
	n1 = (p2 - p1).cross(p3 - p1);
	n1.normalize();

	n2 = Point(0.0, 0.0, -1.0);

	// Add Far Mirror
	t1 = new Triangle(p1, p2, p3, n1);
	t2 = new Triangle(p2, p3, p4, n1);
	t1->setMaterial(perfectMirror);
	t2->setMaterial(perfectMirror);
	ret->addObject(t1);
	ret->addObject(t2);


	// Far Purple Wall Pts
	p1 = Point(-2000, -2000, 2000);	// Bot Left
	p2 = Point(+2000, -2000, 2000);	// Bot Right
	p3 = Point(-2000, +2000, 2000);	// Top Left
	p4 = Point(+2000, +2000, 2000);	// Top Right

									// Add Far purple wall
	t1 = new Triangle(p3, p2, p1, n2);
	t2 = new Triangle(p2, p3, p4, n2);
	t1->setMaterial(diffusePurple);
	t2->setMaterial(diffusePurple);
	ret->addObject(t1);
	ret->addObject(t2);
	
	
	Object * s1;
	
	s1 = new Sphere(Point(1000.0, 1000.0, 1000.0), 400.0);
	s1->setMaterial(diffuseGreen);
	ret->addObject(s1);

	s1 = new Sphere(Point(-250.0, 250.0, -1000.0), 100.0);
	s1->setMaterial(diffuseBlue);
	ret->addObject(s1);
	
	s1 = new Sphere(Point(1250.0, 250.0, -1050.0), 300.0);
	s1->setMaterial(translucent);
	ret->addObject(s1);

	s1 = new Sphere(Point(-2000.0, 1500.0, 800.0), 800.0);
	s1->setMaterial(perfectMirror);
	ret->addObject(s1);

	return ret;
}
