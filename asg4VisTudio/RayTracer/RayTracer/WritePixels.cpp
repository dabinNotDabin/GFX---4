#include <iostream>
#include <fstream>

#include <math.h>

#include "RayTracer.h"
#include <fstream>
using std::ofstream;
using std::ifstream;


int width = 512;
int height = 512;
double FOV = 50.0;
int depth = 12;
int sceneCode = 0;

Point * lookat = new Point(-0.5, -0.2, 1.0);
Point * position = new Point(2000, 1000, -4000);

void writeImage(const char * filePath, float ** pixels[4])
{
	// once we are done calculating, we will write to file.
	ofstream testimage;
	testimage.open(filePath, ios::binary | ios::out);
	testimage << "P6\n" << width << " " << height << "\n255\n";

	for (int j = height - 1; j >= 0; j--)
	{
		for (int i = 0; i< width; i++) {
			for (int k = 0; k < 3; k++) {
				// normalize color value to 0-255.
				// This assumes that the color values are in the
				// range [0,1].
				char c = int(pixels[i][j][k] * 255);
				testimage << c;
			}
		}
	}
	testimage.close();
}

int main(int argc, const char ** argv)
{
	string usage = "Usage:\n\n./rayTracer --default\n\n\tOR\n\n./rayTracer --yours\n\n\tOR\n\n";
	usage = usage +	"./rayTracer <Width> <Height> <FOV> <Trace Depth> <Scene Code>\n\n";
	usage = usage + "\n\n\tScene code 0 is for default\n\tScene code 1 is for my custom scene.\n\n\tOR\n\n";
	usage = usage + "./rayTracer <Width> <Height> <FOV> <Trace Depth> <Camera Lookat> <Camera Position>\n\n";
	usage = usage + "\tWhere camera lookat and position must be three doubles, for x, y, z look at/position vectors.\n";
	usage = usage + "\t\tDefault lookat   (-0.5, -0.2, 1.0)\n";
	usage = usage + "\t\tDefault position (2000.0, 1000.0, -4000.0)\n";
	usage = usage + "\t\tOnly works with custom scene.\n";
	usage = usage + "\t\tTry lookat (0.5, -0.2, 1.0) and position (-2000.0, 1000.0, -4000.0)\n\n";
	usage = usage + "\n\nAn attempt was made to handle different aspect ratios, results may vary.\n";
	usage = usage + "\n\nAlso note that the custom scene handed in was drawn at 1440 x 900.\n\n";

	RayTracer * rt;
	const char * filePath = "default.ppm";

	if (argc < 2)
	{
		cout << usage;
		return 0;
	}
	else
	{
		if (argc == 2)
		{
			if (string(argv[1]) == "--default")
			{
				filePath = "default.ppm";
			}
			else if (string(argv[1]) == "--yours")
			{
				width = 1440;
				height = 900;
				depth = 12;
				sceneCode = 1;
				FOV = 50.0;
				filePath = "yours.ppm";
			}
			else
			{
				cout << usage;
				return 0;
			}
		}
		else if (argc == 6)
		{
			width = atoi(argv[1]);
			height = atoi(argv[2]);
			FOV = (double)atoi(argv[3]);
			depth = atoi(argv[4]);
			sceneCode = atoi(argv[5]);
			filePath = "custom.ppm";
		}
		else if (argc == 11)
		{
			width = atoi(argv[1]);
			height = atoi(argv[2]);
			FOV = (double)atoi(argv[3]);
			depth = atoi(argv[4]);
			sceneCode = 1;
			lookat = new Point(stod(argv[5]), stod(argv[6]), stod(argv[7]));
			position = new Point(stod(argv[8]), stod(argv[9]), stod(argv[10]));
			filePath = "custom.ppm";
		}
		else
		{
			cout << usage;
			return 0;
		}
	}


	if (sceneCode == 0)
	    rt = new RayTracer(Scene::initTestScene(width, height, FOV, nullptr, nullptr), depth, 4);
	else
		rt = new RayTracer(Scene::initCustomScene(width, height, FOV, lookat, position), depth, 4);

	float *** pixels;

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


	writeImage(filePath, pixels);

	
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
			delete[] pixels[i][j];
		delete[] pixels[i];
	}
	delete[] pixels;

    return 0;   
}



