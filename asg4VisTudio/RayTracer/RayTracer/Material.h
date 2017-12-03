#ifndef COLOR_H
#include "Color.h"
#endif
#ifndef GEOMETRY_H
#include "Geometry.h"
#endif

// An enumerated type to represent different types of materials.
enum Type{
    REFLECTIVE,
    DIFFUSE,
	REFRACTIVE
};

// A class that represents material properties.
// This can be extended to represent textured materials.
class Material {
    
public:
    Color diffuse, ambient, specular;
    double kr,kt,kd;
    Type type; 
    // this is for materials to decide!
    Material(){
        kr = 0.0;
        kt = 0.0;
        kd = 0.0;
    }
    virtual Color getAmbient(Point p){return ambient;}

	Color getDiffuse(Point p)
	{
		double u = p.x + 1000;
		double v = p.z + 1000;

		u /= 400.0;
		v /= 400.0;

		int intPartU = (int)u;
		int intPartV = (int)v;


		u -= intPartU;
		v -= intPartV;

		Color black(0.0, 0.0, 0.0, 0.0);
		Color white(0.0, 0.8, 0.8, 0.0);

		if (u < 0.5)
		{
			if (v < 0.5)
				return black;
			else
				return white;
		}
		else
		{
			if (v < 0.5)
				return white;
			else
				return black;
		}
	}


    Color getSpecular(Point p){return specular;}
    void setAmbient(Color c){ambient = c;}
    void setSpecular(Color c){specular = c;}
    void setDiffuse(Color c){diffuse = c;}
};
