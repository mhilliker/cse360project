#include "Graphics.h"

using namespace std;

//default constructor initializes everything to NULL
Graphics::Graphics()
{
	factory = NULL;	renderTarget = NULL; brush = NULL;	writefactory = NULL; textformat = NULL; smalltextformat = NULL;
}

//default destructor
Graphics::~Graphics()
{
	if (factory) factory->Release();
	if (renderTarget) renderTarget->Release();
	if (brush) brush->Release();
}

bool Graphics::Init(HWND windowhandle)
{
	HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory); //Creates a factory object that can be used to create Direct2D resources.
	if (res != S_OK)return false; // If there is a problem creating factory, then we return false

	RECT rect;
	GetClientRect(windowhandle, &rect); //used to determine window size

	//sets factory to window
	res = factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(windowhandle, D2D1::SizeU(rect.right, rect.bottom)),
		&renderTarget);

	//sets write factory to window
	if (SUCCEEDED(res))
	res = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown **>(&writefactory)
		);

	//creates text format

	static const WCHAR msc_fontName[] = L"Verdana";
	static const FLOAT msc_fontSize = 50;

	if (SUCCEEDED(res))
	res = writefactory->CreateTextFormat(
		msc_fontName,
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		msc_fontSize,
		L"", //locale
		&textformat
		);

	//creates small text format
	static const WCHAR msc_fontName2[] = L"Verdana";
	static const FLOAT msc_fontSize2 = 16;

	if (SUCCEEDED(res))
	res = writefactory->CreateTextFormat(
		msc_fontName2,
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		msc_fontSize2,
		L"", //locale
		&smalltextformat
		);

	// Center the text horizontally and vertically.
	textformat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	textformat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	smalltextformat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	smalltextformat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	//initializes the brush to a white color by default
	renderTarget->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0, 0), &brush);

	if (res != S_OK) return false; //checks for problem
	else return true;

}

//Clears the screen to whatever RGB color is specified
void Graphics::ClearScreen(float r, float g, float b)
{
	renderTarget->Clear(D2D1::ColorF(r, g, b));
}

//I implemented a draw circle method to draw circles to the screen.  Feel free to ignore or use at your leisure.
//This was mostly an example of how we can make methods to draw stuff for us.
void Graphics::DrawCircle(float x, float y, float radius, float r, float g, float b, float a)
{
	//A brush "paints" an area with its output. It is passed into the Draw commands of the renderTarget
	brush->SetColor(D2D1::ColorF(r, g, b, a));//sets color here
	renderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), brush, 3.0f); //Creates an ellipse inside of the draw ellipse function.
}

void Graphics::DrawRect(float x, float y, float width, float height, float r, float g, float b, float a, bool fill)
{
	brush->SetColor(D2D1::ColorF(r, g, b, a));//sets color here
	D2D1_RECT_F rect = D2D1::RectF(x,y,width+x,height+y);
	if (!fill) renderTarget->DrawRectangle(rect, brush, 5, NULL); //Creates an ellipse inside of the draw ellipse function.
	else renderTarget->FillRectangle(rect, brush);
}

void Graphics::DrawUI()
{
	//draw background first
	float borderSize = 5;
	DrawRect(90.0f - borderSize, 100.0f - borderSize, 600.0f + 2 * borderSize, 600.0f + 2 * borderSize, 0.0f, 0.0f, 0.0f, 1.0f, true);	//draws black border to game board
	DrawRect(90.0f, 100.0f, 600.0f, 600.0f, 255.0f / 256, 196.0f / 256, 153.0f / 256, 1.0f, true);			//draws brown game board

	//draw grid next
	brush->SetColor(D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f));
	float width = 600, height = 600, x0 = 90, y0 = 100;
	float xspacing = width / 9, yspacing = height/9;
	int checkForBold = 0, strokeWidth = 2;

	for (int x = x0+xspacing; x < 8*xspacing+x0; x+=xspacing)
	{
		++checkForBold % 3 == 0 ? strokeWidth = 6 : strokeWidth = 2;
		renderTarget->DrawLine(	D2D1::Point2F(static_cast<FLOAT>(x), y0),D2D1::Point2F(static_cast<FLOAT>(x), y0+height),brush,	strokeWidth	);
	}
	checkForBold = 0;
	for (int y = y0 + yspacing; y < 8*yspacing+y0; y += yspacing)
	{
		++checkForBold % 3 == 0 ? strokeWidth = 6 : strokeWidth = 2;
		renderTarget->DrawLine(	D2D1::Point2F(x0, static_cast<FLOAT>(y)),D2D1::Point2F(x0 + width, static_cast<FLOAT>(y)),brush,strokeWidth);
	}

	//draw numbers in grid

	//use getBoard in real game
	int arr[9][9] = {{4,3,6,8,2,9,7,1,5},{5,1,7,3,4,6,9,8,2},{2,8,9,1,5,7,6,3,4},{3,6,4,5,9,2,8,7,1},{9,7,2,6,8,1,5,4,3},{1,5,8,4,7,3,2,9,6},{8,2,3,7,1,5,4,6,9},{7,9,1,2,6,4,3,5,8},{6,4,5,9,3,8,1,2,7}}; //where the numbers are
	string temp2;
	for(int x=0; x<9; x++)
	{
		for(int y=0; y<9; y++)
		{
			temp2 = static_cast<ostringstream*>( &(ostringstream() << arr[x][y]) )->str();
			DrawString(temp2,x0+x*xspacing+25,y0+y*yspacing+25,10,10 );
		}

		//draw buttons
		DrawRect(x0+width/6-10-2, y0+height+25-2,200+4,50+4, 0/256.0f, 0/256.0f,0 /256.0f,1.0f,true);
		DrawRect(x0+width/6-10, y0+height+25,200,50, 204/256.0f, 102/256.0f,0 /256.0f,1.0f,true);
		DrawSmallerString("Check Board",x0+width/6+5+10,y0+height+30,140,40);
		DrawRect(x0+width/6+200-2, y0+height+25-2,4+200,4+50,0 /256.0f, 0/256.0f,0 /256.0f,1.0f,true);
		DrawRect(x0+width/6+200, y0+height+25,200,50,204 /256.0f, 102/256.0f,0 /256.0f,1.0f,true);
		DrawSmallerString("Solve Board",x0+width/6+200+30,y0+height+30,140,40);
		DrawRect(740-2,760-2,50+4,30+4, 0/256.0f, 0/256.0f,0 /256.0f,1.0f,true);
		DrawRect(740,760,50,30, 204/256.0f, 102/256.0f,0 /256.0f,1.0f,true);
		DrawSmallerString("Help",740,750,50,50);
		DrawRect(5-2,760-2,60+4,30+4,0/256.0f, 0/256.0f,0 /256.0f,1.0f,true);
		DrawRect(5,760,60,30,204/256.0f, 102/256.0f,0 /256.0f,1.0f,true);
		DrawSmallerString("Scores",6,760,60,30);
	}
}

void Graphics::DrawString(string str, float x, float y, float width, float height)
{
	std::wstring widestr = std::wstring(str.begin(), str.end());
	const WCHAR* widecstr = widestr.c_str();
	brush->SetColor(D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f));
	renderTarget->DrawTextA(widecstr, str.length() , textformat, D2D1::RectF(x, y, x + width, y + height), brush);
}

void Graphics::DrawSmallerString(string str, float x, float y, float width, float height)
{
	std::wstring widestr = std::wstring(str.begin(), str.end());
	const WCHAR* widecstr = widestr.c_str();
	brush->SetColor(D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f));
	renderTarget->DrawTextA(widecstr, str.length() , smalltextformat, D2D1::RectF(x, y, x + width, y + height), brush);
}