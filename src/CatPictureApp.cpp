#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

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
	float first, second, third;
	int time, initialTime, speed;
};

//First
void CatPictureApp::setup()
{
	first = second = third = 0.0f;
	time = initialTime = 500;
	speed = 10;
}

void CatPictureApp::mouseDown( MouseEvent event )
{
}

//Second -Logic in this method
void CatPictureApp::update()
{
	first = ((rand() % 11)/10.0f);
	second = ((rand() % 11)/10.0f);
	third = ((rand() % 11)/10.0f);

	time -= speed;

	if(time == 0){
		time = initialTime;
	}

	Sleep(time);
}

//Third
void CatPictureApp::draw()
{
	// clear out the window with black
	gl::clear( Color( first, second, third) ); 
}

CINDER_APP_BASIC( CatPictureApp, RendererGl )
