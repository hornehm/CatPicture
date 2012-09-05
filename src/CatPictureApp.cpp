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

	void drawRectangle(uint8_t* pixels, int x, int y, int width, int height, Color8u color);

	void drawLine(uint8_t* pixels, int x1, int y1, int x2, int y2);

	void gradient(uint8_t* pixels, Color8u c);

	void drawCircle(uint8_t* pixels, int x, int y, int r);
	
	void convolution(uint8_t* pixels);

};


void CatPictureApp::gradient(uint8_t* pixels, Color8u c){

	for(int i = 0; i <= appHeight; i++){
		for(int j = 0; j <=appWidth; j++){
			pixels[3*(j+i*textureSize)] = c.r;
			pixels[3*(j+i*textureSize)+1] = c.g;
			pixels[3*(j+i*textureSize)+2] = c.b;
		}
		c.b -= 1;
		c.r += 1;
		c.g += 1;
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
			//if((pixels[3*(j+i*textureSize)] != (pixels[3*((j+1)+(i+1)*textureSize)])) 
				//&& ((pixels[3*((j-1)+i*textureSize)] != pixels[3*(j+i*textureSize)]))){
				
					for(int cy = y; cy<=y+2; cy++){
						for(int cx = x; cx <=y+2; cx++){
							index = 3*(cx+cy*textureSize);
							num_red += (work_buffer[index]);
							num_green += (work_buffer[index+1]);
							num_blue += (work_buffer[index+2]);
						}
					}
					
							index = 3*(x+y*textureSize);
							pixels[index] = work_buffer[index]/9;
							pixels[index+1]  = work_buffer[index+1]/9;
							pixels[index+2] = work_buffer[index+2]/9;
						
					

			
		}
	}
}

void CatPictureApp::drawCircle(uint8_t* pixels, int cx, int cy, int r){
	int x, y;
	double d;
	const double pi = 3.14159265;
	for(d=0; d<= 2*pi; d+= 0.01){
		x = (int)(cx+sin(d)*r);
		y = (int)(cy+sin(d+(pi/2))*r);
		pixels[3*(x + y*textureSize)] = 0;
		pixels[3*(x + y*textureSize)+1] = 255;
		pixels[3*(x + y*textureSize)+2] = 0;
	}
	
}


void CatPictureApp::drawRectangle(uint8_t* pixels, int x, int y, int width, int height, Color8u color){
	
	for (int i = y; i <=y +height; i++){
		for(int j = x; j <=x + width; j++){
			pixels[3*(j + i*textureSize)] = color.r;
			pixels[3*(j + i*textureSize)+1] = color.g;
			pixels[3*(j + i*textureSize)+2] = color.b;
		}
		
	}
	
	}



void CatPictureApp::drawLine(uint8_t* pixels, int x1, int y1, int x2, int y2){
	int tmp;
	if(x2 < x1){
		tmp = x2;
		x2 = x1;
		x1 = tmp;
		
	}
	if(y2 < y1){
		tmp = y2;
		y2 = y1;
		y1 = tmp;
	}

	double slope = ((double)(y2-y1)/(double)(x2-x1));
	
	int x, y=0;

	for(int y = y1; y <= y2; y++){
			x = (int)(x2 +(slope*(y-y1)));
			pixels[3*(x + y*textureSize)] = 0;
			pixels[3*(x + y*textureSize)+1] = 255;
			pixels[3*(x + y*textureSize)+2] = 0;
			
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
	
	convolution(dataArray);

	drawCircle(dataArray, 50, 50, 50);
	
	gradient(dataArray, c1);
}


void CatPictureApp::draw()
{
	
	gl::draw(*mySurface); 

}


CINDER_APP_BASIC( CatPictureApp, RendererGl )
