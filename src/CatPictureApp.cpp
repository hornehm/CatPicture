/**
  *My solution to HW01
  *@author Heather Horne
  *@date 9/5/2012
  *
  *@This project satisfies goals of rectangle, circle, gradient, and a convolution based filter. 
  * I attempted to finish the triangle method, however, it is not complete. My gradient is also 
  * not very well incorporated into my picture. It works better without anything else.
  *
  *
  *I used a basic outline from Dr. Brinkman's code for the convolution, except I've added my
  * own conditions
  *
  *
*/
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CatPictureApp:public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();

  private:

	 Surface* mySurface;

	static const int appWidth = 800;
	static const int appHeight = 600;
	static const int textureSize = 1024;// number of pixels in the row

	uint8_t* convPattern;

	/**
	  *Fills a rectangle at a given point
	  *
	  *Fills the rectangle starting at origin (x, y) with a width 
	  * and height. It also fills the rectangle with a given color (color8u).
	  * The size of the rectangle is controlled by the height and width.
	  *
	  *This satisfies the rectangle requirement.
	*/
	void drawRectangle(uint8_t* pixels, int x, int y, int width, int height, Color8u color);

	/**
	  *Changes smoothly from one color to the next vertically
	  *
	  *Given the data array and color, it changes from one color 
	  * to another in a vertical fashion.
	  *
	  *This satisfies the gradient requirement.
	*/
	void gradient(uint8_t* pixels, Color8u c);

	/**
	  *Draws a circle at center (x, y) with radius r
	  *
	  *Draws an outline of a circle without filling it in with
	  * center (x, y) and radius r. The color can be changed as given.
	  * The thickness can also be determined.
	  *
	  *This satisfies the circle requirement
	*/
	void drawCircle(uint8_t* pixels, int x, int y, float r, int boldness, Color8u c);
	
	/**
	  *Abstracts the picture through a convolution filter
	  *
	  *Searches the whole screen while changing surrounding
	  * pixel values to the averages around. Although I did 
	  * not include a kernal.
	  *
	  *This satisfies the Convulation requirement.
	*/
	void convolution(uint8_t* pixels);

	/**
	  *Fills in a triangle with three given corner points
	  *
	  *This method is not complete. My logic is a bit messy here.
	  * However, it still manages to produce something.
	*/
	void drawTriangle(uint8_t* pixels, int x1, int y1, int x2, int y2, int x3, int y3);

};


void CatPictureApp::gradient(uint8_t* pixels, Color8u c){

	for(int i = 0; i <= appHeight; i++){
		
		for(int j = 0; j <=appWidth; j++){
			pixels[3*(j+i*textureSize)] += c.r;
			pixels[3*(j+i*textureSize)+1] += c.g;
			pixels[3*(j+i*textureSize)+2] += c.b;
		}
		
		}
	}
		
void CatPictureApp::drawTriangle(uint8_t* pixels, int x1, int y1, int x2, int y2, int x3, int y3){

	float d1, d2, d3;
	float xyD1, xyD2, xyD3;

	//Find the distances between each point
	d1 = sqrt((float)(((y2-y1)*(y2-y1)) + ((x2-x1)*(x2-x1))));
	d2 = sqrt((float)(((y3-y2)*(y3-y2)) + ((x3-x2)*(x3-x2))));
	d3 = sqrt((float)(((y3-y1)*(y3-y1)) + ((x3-x1)*(x3-x1))));

	int startx, endx, starty, endy;

	//Figure out which value is the lowest and highest for x and y
	if((x1< x2) && (x1<x3)){
		startx = x1;
	}
	else if((x2<x1) && (x2 < x3)){
		startx = x2;
	}
	else{
		startx = x3;
	}
	if((x1> x2) && (x1>x3)){
		endx = x1;
	}
	else if((x2>x1) && (x2 > x3)){
		endx = x2;
	}
	else{
		endx = x3;
	}

	if((y1< y2) && (y1<y3)){
		starty = y1;
	}
	else if((y2<y1) && (y2 < y3)){
		starty = y2;
	}
	else{
		starty = y3;
	}
	if((y1> y2) && (y1>y3)){
		endy = y1;
	}
	else if((y2>y1) && (y2 > y3)){
		endy = y2;
	}
	else{
		endy = y3;
	}

	//start with row y and the start column
	for(int y = starty; y<=endy; y++){
		for(int x = startx; x<=endx; x++){
			
			//Calculate distance between the (x,y) coordinates from the 
			// given points
			xyD1 = sqrt((float)(((y2-y)*(y2-y)) + ((x2-x)*(x2-x))));
			xyD2 = sqrt((float)(((y3-y)*(y3-y)) + ((x3-x)*(x3-x))));
			xyD3 = sqrt((float)(((y1-y)*(y1-y)) + ((x1-x)*(x1-x))));

			//Determine whether the point is in the triangle
			if((xyD1 < d1)&&(xyD2 <d2)&&(xyD3<d2)){
				pixels[3*(x+y*textureSize)] = 0;
				pixels[3*(x+y*textureSize)+1] = 255;
				pixels[3*(x+y*textureSize)+2] = 0;
			}
		}
	}


}


void CatPictureApp::convolution(uint8_t* pixels){
	static uint8_t work_buffer[3*textureSize*textureSize];
	memcpy(work_buffer, pixels, 3*textureSize*textureSize);
	
	int num_red, num_green, num_blue;
	num_red = num_green = num_blue = 0;
	int index;
	
	for(int y = 1; y < appHeight-1; y++){
		for(int x = 1; x < appWidth-1; x++){
				//looks at pixels two rows ahead to see if its not equal
				// if it isn't it changes the surrounding pixels into the average
				// and makes it look weird. In a way its sharpening my shapes.
				if((pixels[3*(x+y*textureSize)] != pixels[3*(x+ (y+2)*textureSize)])){
					for(int cy = y; cy<=y+2; cy++){
						for(int cx = x; cx <=x+2; cx++){
							index = 3*(cx+cy*textureSize);
							num_red += (work_buffer[index]);
							num_green += (work_buffer[index+1]);
							num_blue += (work_buffer[index+2]);
						}
						index = 3*(x+y*textureSize);
						pixels[index] = num_red/36;
						pixels[index+1]  = num_green/36;
						pixels[index+2] = num_blue/36;
					}
				}
				
		}	
	}
}


void CatPictureApp::drawCircle(uint8_t* pixels, int cx, int cy, float r, int boldness, Color8u c){

	float radius;
	
	for(int y = 0; y < appHeight-1; y++){
		for(int x = 0; x < appWidth-1; x++){
			//See if points are on circle
			radius = sqrt(((float)(x-cx)*(x-cx))+((float)(y-cy)*(y-cy)));
			//If the radius is within a certain range, change 9 surrounding pixels
			if((radius >= r-boldness)&&(radius <= r + boldness)){
				for(int i = y; i < y+2; i++){
					for(int j = x; j < x+2; j++){
						pixels[3*(x+y*textureSize)] = c.r;
						pixels[3*(x+y*textureSize) +1] = c.g;
						pixels[3*(x+y*textureSize) +2] = c.b;
					}
				}
		}
	}
}
}


void CatPictureApp::drawRectangle(uint8_t* pixels, int x, int y, int width, int height, Color8u color){
	
	for (int i = y; i <=y +height; i++){
		for(int j = x; j <=x +width; j++){
			//this creates a bit of a texture/or different surface on the rectangle
			if ((i%2==0)){
				pixels[3*(j + i*textureSize)] = 255;
				pixels[3*(j + i*textureSize)+1] = 255;
				pixels[3*(j + i*textureSize)+2] = 255;
			}
			else{
			pixels[3*(j + i*textureSize)] = color.r;
			pixels[3*(j + i*textureSize)+1] = color.g;
			pixels[3*(j + i*textureSize)+2] = color.b;
			}
		}
	}
	
	}


void CatPictureApp::setup()
{

	mySurface = new Surface(textureSize, textureSize, false);
	
}
	


void CatPictureApp::mouseDown( MouseEvent event )
{
}


void CatPictureApp::update()
{
	uint8_t* dataArray = (*mySurface).getData();
	//drawTriangle(dataArray, 0, 0, 100, 200, 200, 100);
	
	int posX = 0;
	int posY = 0;
	int posX1 = 0;
	int posY1 = 50;
	int posX2 = 0;
	int posY2 = 100;

	Color8u c = Color8u(255, 0, 0);
	Color8u c1 = Color8u(0, 0, 255);
	Color8u c2 = Color8u(0, 255, 0);

	
	for(int i = 0; i <=50; i++){
		drawRectangle(dataArray, posX, posY, 50, 50, c);
		drawRectangle(dataArray, posX1, posY1, 50, 50, c1);
		drawRectangle(dataArray, posX2, posY2, 50, 50, c2);
		posX+=60;
		posY+= 10;
		posX1+=60;
		posY1+= 10;
		posX2+=60;
		posY2+=10;
	
	}

	drawCircle(dataArray, 200, 200, 100, 5, c2);
	drawCircle(dataArray, 100, 100, 100, 3, c1);
	drawCircle(dataArray, 300, 100, 100, 10, c);
	drawCircle(dataArray, 500, 100, 100, 2, c2);
	drawCircle(dataArray, 500, 400, 200, 20, c1);
	drawCircle(dataArray, 150, 450, 150, 2, c);

	convolution(dataArray);

	gradient(dataArray, c1);
	
	int frameNum = 0;
	if(frameNum == 0){
		writeImage("hornehm.png", *mySurface);
		//We do this here, instead of setup, because we don't want to count the writeImage time in our estimate
			}
	frameNum++;
	}

void CatPictureApp::draw()
{
	
	gl::draw(*mySurface); 

}


CINDER_APP_BASIC( CatPictureApp, RendererGl )
