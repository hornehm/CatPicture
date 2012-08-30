#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CatPictureApp : public AppBasic {
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

	void drawRectangle(uint8_t* pixels, int x, int y, int width, int height);

	void drawLine(uint8_t* pixels, int x1, int y1, int x2, int y2);

	

};

void CatPictureApp::drawRectangle(uint8_t* pixels, int x, int y, int width, int height){
}

void CatPictureApp::drawLine(uint8_t* pixels, int x1, int y1, int x2, int y2){
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
}


void CatPictureApp::draw()
{
	
	gl::draw(*mySurface); 
}

CINDER_APP_BASIC( CatPictureApp, RendererGl )
