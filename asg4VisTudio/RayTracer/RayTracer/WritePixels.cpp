#include <iostream>
#include <fstream>

#include <math.h>

#include "RayTracer.h"
#include <fstream>
using std::ofstream;
using std::ifstream;

// store width and height of the render
int width = 256;
int height = 256;


int main(int argc, const char ** argv)
{
	RayTracer * rt;

	// Test scene with max depth of 4 and sampling of 1
    rt = new RayTracer(Scene::initTestScene(width),12,4);
	float *** pixels;// [256][256][4];

	pixels = new float **[width];
	for (int i = 0; i < width; i++)
	{
		pixels[i] = new float *[height];
		for (int j = 0; j < height; j++)
			pixels[i][j] = new float[4];
	}

    for(int ctr = 0; ctr < height*width; ctr++){
    	int i = ctr % width;
    	int j = ctr / width;
    	Color rad = rt->calculate(i,j);
    	pixels[i][j][0] = rad.r; //Red
    	pixels[i][j][1] = rad.g; //Green
    	pixels[i][j][2] = rad.b; //Blue
    	pixels[i][j][3] = 1.0; //Alpha
    }
    // once we are done calculating, we will write to file.
    ofstream testimage;
	testimage.open("TestScene.ppm",ios::binary | ios::out);


	testimage << "P6\n" << width << " " << height << "\n255\n";

	for(int j = height-1; j >=0 ; j-- )
	{
	    for(int i = 0; i< width; i++){
	        for(int k = 0; k < 3; k++){
	             // normalize color value to 0-255.
	        	 // This assumes that the color values are in the
	        	 // range [0,1].
	        	 char c = int(pixels[i][j][k]*255);
	             testimage << c;
			}
		}
	}
	testimage.close();

	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
			delete[] pixels[i][j];
		delete[] pixels[i];
	}
	delete[] pixels;

    return 0;   
}
