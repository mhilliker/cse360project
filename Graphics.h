#pragma once

#include <Windows.h>
#include <d2d1.h>

class Graphics
{
	ID2D1Factory* factory;//creates resources
	ID2D1HwndRenderTarget* renderTarget;//place in memory where graphics are being drawn
	ID2D1SolidColorBrush* brush; //

public:
	Graphics();	//default constructor
	~Graphics();//default destructor

	//initializes the window
	bool Init(HWND windowHandle);

	//used to begin and end drawing to screen
	void BeginDraw(){renderTarget->BeginDraw();} 
	void EndDraw(){renderTarget->EndDraw();}

	//methods I made as examples of how to make the coding more efficient
	void ClearScreen(float r, float g, float b); //clears the screen to the designated color
	void DrawCircle(float x, float y, float radius, float r, float g, float b, float a); //draws a circle

};