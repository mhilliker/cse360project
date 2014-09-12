#include "Graphics.h"

//default constructor initializes everything to NULL
Graphics::Graphics()
{
	factory=NULL;
	renderTarget=NULL;
	brush=NULL;
}

//default destructor
Graphics::~Graphics()
{
	if(factory) factory->Release();
	if(renderTarget) renderTarget->Release();
	if(brush) brush->Release();
}

bool Graphics::Init(HWND windowhandle)
{
	HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,&factory); //Creates a factory object that can be used to create Direct2D resources.
	if(res!=S_OK)return false; // If there is a problem creating factory, then we return false

	RECT rect;
	GetClientRect(windowhandle, &rect); //used to determine window size

	//sets factory to window
	res = factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(windowhandle,D2D1::SizeU(rect.right,rect.bottom)),
		&renderTarget);

	//initializes the brush to a white color by default
	renderTarget->CreateSolidColorBrush(D2D1::ColorF(0,0,0,0),&brush);
	
	if(res!=S_OK) return false; //checks for problem
	else return true;

}

//Clears the screen to whatever RGB color is specified
void Graphics::ClearScreen(float r, float g, float b)
{
	renderTarget->Clear(D2D1::ColorF(r,g,b));
}

//I implemented a draw circle method to draw circles to the screen.  Feel free to ignore or use at your leisure.
//This was mostly an example of how we can make methods to draw stuff for us.
void Graphics::DrawCircle(float x, float y, float radius, float r, float g, float b, float a)
{
	//A brush "paints" an area with its output. It is passed into the Draw commands of the renderTarget
	brush->SetColor(D2D1::ColorF(r,g,b,a));//sets color here
	renderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x,y),radius,radius),brush, 3.0f); //Creates an ellipse inside of the draw ellipse function.
}
