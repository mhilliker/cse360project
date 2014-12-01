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
	if (!fill) renderTarget->DrawRectangle(rect, brush, 4, NULL); //Creates an ellipse inside of the draw ellipse function.
	else renderTarget->FillRectangle(rect, brush);
}

void Graphics::DrawUI(int arr[9][9])
{
	//draw background first
	float borderSize = 5;
	DrawRect(90.0f - borderSize, 100.0f - borderSize, 600.0f -7+ 2 * borderSize, 600.0f -7+ 2 * borderSize, 0.0f, 0.0f, 0.0f, 1.0f, true);	//draws black border to game board
	DrawRect(90.0f, 100.0f, 600.0f-7, 600.0f-7, 255.0f / 256, 196.0f / 256, 153.0f / 256, 1.0f, true);			//draws brown game board


	//draw grid next
	brush->SetColor(D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f));
	float width = 600-2, height = 600-2, x0 = 90, y0 = 100;
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

	string temp2;
	for(int x=0; x<9; x++)
	{
		for(int y=0; y<9; y++)
		{
			if(arr[x][y]>0 && arr[x][y]<10)
				temp2 = static_cast<ostringstream*>( &(ostringstream() << arr[x][y]) )->str();
			else
				temp2 = " ";
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

		//draw guess and select boards
		int x_distance_from_board = 25, x0_select = x0+width+x_distance_from_board, x0_guess=5;
		//draw boarder first
		DrawRect(x0_guess-borderSize/2.0,y0-borderSize/2.0,60+2*borderSize/2.0,height+2*borderSize/2.0,0.0f,0.0f,0.0f,1.0f,true);
		DrawRect(x0_select-borderSize/2.0,y0-borderSize/2.0,60+2*borderSize/2.0,height+2*borderSize/2.0,0.0f,0.0f,0.0f,1.0f,true);
		//draw foreground next
		DrawRect(x0_guess,y0,60,height, 255.0f / 256, 196.0f / 256, 153.0f / 256, 1.0f,true);
		DrawRect(x0_select,y0,60,height, 255.0f / 256, 196.0f / 256, 153.0f / 256, 1.0f,true);
		//draw labels over guess and select boards
		DrawSmallerString("Maybe",x0_guess-borderSize/2.0,y0-40,70,50);
		DrawSmallerString("Maybe Not",x0_select-borderSize/2.0-2,y0-50,70,50);
		//draw dividers and numbers
		for(int i=1; i<9; i++){
				DrawRect(x0_select,y0+height/9*i,60,borderSize/2.0f,0.0f,0.0f,0.0f,1.0f,true);
				DrawRect(x0_guess,y0+height/9*i,60,borderSize/2.0f,0.0f,0.0f,0.0f,1.0f,true);
				DrawString(to_string(i),x0_select,y0+height/9*(i-1)+5,60,60);
				DrawString(to_string(i),x0_guess,y0+height/9*(i-1)+5,60,60);
		}
		DrawString(to_string(9),x0_select,y0+height/9*(9-1)+5,60,60);
		DrawString(to_string(9),x0_guess,y0+height/9*(9-1)+5,60,60);
		
		
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

void Graphics::DrawSelection(int x_block, int y_block, string color)
{
	int xPos=90+600/9*x_block+2, yPos=100+600/9*y_block+2;
	int length=600/9-4;
	if(color == "green"){
		DrawRect(xPos,yPos,length,length,0.0f,1.0f,0.0f,1.0f,false);
	}else if(color == "red"){
		DrawRect(xPos,yPos,length,length,1.0f,0.0f,0.0f,1.0f,false);
	}else{
		DrawRect(xPos,yPos,length,length,0.0f,0.0f,1.0f,1.0f,false);
	}
}

bool Graphics::DrawGuessSelected(bool* trues, bool* falses){
	bool something_was_drawn=false;
		for(int i=0; i<9; i++){ //draw selected guesses
			if(trues[i])
				DrawRect(6,100+600/9.0*i,60-2,600/9,0.0f,1.0f,0.0f,1.0f,false);
			if(falses[i])
				DrawRect(714,100+600/9.0*i,60-2,600/9,1.0f,0.0f,0.0f,1.0f,false);
		}
	return something_was_drawn;
	}
