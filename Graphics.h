#pragma once

#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include <string>
#include <stdio.h>
#include <sstream>
#include <string>

using namespace std;

class Graphics
{
private:
	ID2D1Factory* factory;//creates resources
	ID2D1HwndRenderTarget* renderTarget;//place in memory where graphics are being drawn
	ID2D1SolidColorBrush* brush; //prepares colors
	IDWriteFactory* writefactory;//prepares text drawing
	IDWriteTextFormat* textformat;//prepared text format
	IDWriteTextFormat* smalltextformat;//prepared text format

	
public:
	Graphics();	//default constructor
	~Graphics();//default destructor

	//initializes the window
	bool Init(HWND windowHandle);

	//used to begin and end drawing to screen
	void BeginDraw(){ renderTarget->BeginDraw(); }
	void EndDraw(){ renderTarget->EndDraw(); }

	//tools to use to draw on screen
	void ClearScreen(float r, float g, float b); //clears the screen to the designated color
	void DrawCircle(float x, float y, float radius, float r, float g, float b, float a); //draws a circle
	void DrawRect(float x, float y, float width, float height, float r, float g, float b, float a, bool fill);
	void DrawUI();
	void DrawString(string text, float x, float y, float width, float height);
	void DrawSmallerString(string text, float x, float y, float width, float height);

};