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
	static const int textureSize = 1024;

	void drawRectangle(uint8_t* pixels, int x, int y, int width, int height, Color8u color);

	void drawLine(uint8_t* pixels, int x1, int y1, int x2, int y2);

	void gradient(uint8_t* pixels, int r1, int g1, int b1, int r2, int g2, int b2);

	void drawCircle(uint8_t* pixels, int x, int y, int r);
	
	void splat(uint8_t* pixels, uint8_t* splatPixels);

};

void CatPictureApp::gradient(uint8_t* pixels, int r1, int g1, int b1, int r2, int g2, int b2){
	
	Color8u c1 = Color8u(r1, g1, b1);
	Color8u c2 = Color8u(r2, g2, b2);

	for(int i = 0; i <= appHeight; i++){
		for(int j = 0; j <= appWidth; j++){
		pixels[3*j] = c1.r;
		pixels[3*j+1] = c1.b;
		pixels[3*j+2] = c1.g;
		}
	}
		


}

void CatPictureApp::splat(uint8_t* pixels, uint8_t* splatPixels){

}

void CatPictureApp::drawCircle(uint8_t* pixels, int cx, int cy, int r){
	int x, y;
	double d;
	const double pi = 3.14159265;
	for(d=0; d<= 2*pi; d+= 0.01){
		x = cx+sin(d)*r;
		y = cy+sin(d+(pi/2))*r;
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
	
	int x, y;

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
	Color8u c = Color8u(255, 0,0);
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
	


	//drawRectangle(dataArray, 100, 100, 200, 100);
	//drawLine(dataArray, 300, 300, 300, 100);
	//gradient(dataArray, 0, 255, 0, 0, 0, 255);

	//drawCircle(dataArray, 50, 50, 50);
}


void CatPictureApp::draw()
{
	
	gl::draw(*mySurface); 

}


CINDER_APP_BASIC( CatPictureApp, RendererGl )
