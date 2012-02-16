#include	"GenericWindow.h"
#include	<iostream>
using namespace	std;




bool CALLBACK	enumerateChildren(HWND handle, LPARAM lParam)
{
	if(lParam == WM_CLOSE)
		SendMessageW(handle, WM_CLOSE, 0, 0);  // close child windows


	return true;
};


GenericWindow::GenericWindow() :
	handle(NULL) , parent(NULL) , externalClassDependency(false) ,
	style(0) , extendedStyle(0) , width(CW_USEDEFAULT) , height(CW_USEDEFAULT) ,
	xPosition(CW_USEDEFAULT) , yPosition(CW_USEDEFAULT) , menu(NULL) , instance(NULL) , parameters(NULL)
{
	memset(this->windowName,'\0',MAX_LENGTH);
	wcsncpy_s(this->windowName,MAX_LENGTH,L"",MAX_LENGTH-1);
	memset(this->className,'\0',MAX_LENGTH);
	wcsncpy_s(this->className,MAX_LENGTH,L"",MAX_LENGTH-1);

	this->windowClass.cbSize = sizeof(WNDCLASSEX);
	this->windowClass.style = 0;	// class styles: CS_OWNDC, CS_PARENTDC, CS_CLASSDC, CS_GLOBALCLASS, ...
	this->windowClass.lpfnWndProc = NULL;	// pointer to window procedure
	this->windowClass.cbClsExtra = 0;
	this->windowClass.cbWndExtra = 0;
	this->windowClass.hInstance = instance;
	this->windowClass.hIcon = LoadIcon(0, IDI_APPLICATION);	// default icon
	this->windowClass.hCursor = LoadCursor(0, IDC_ARROW);	// default arrow cursor
	this->windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	this->windowClass.lpszMenuName = 0;
	this->windowClass.lpszClassName = this->className;
	this->windowClass.hIconSm = LoadIcon(0, IDI_APPLICATION);	// default small icon
};

GenericWindow::GenericWindow( HINSTANCE inst , const wchar_t* name , HWND parentWindow ) :
	handle(NULL) , parent(parentWindow) , externalClassDependency(false) ,
	style(0) , extendedStyle(0) , width(CW_USEDEFAULT) , height(CW_USEDEFAULT) ,
	xPosition(CW_USEDEFAULT) , yPosition(CW_USEDEFAULT) , menu(NULL) , instance(inst) , parameters(NULL)
{
	memset(this->windowName,'\0',MAX_LENGTH);
	wcsncpy_s(this->windowName,MAX_LENGTH,name,MAX_LENGTH-1);
	memset(this->className,'\0',MAX_LENGTH);
	wcsncpy_s(this->className,MAX_LENGTH,name,MAX_LENGTH-1);

	this->windowClass.cbSize = sizeof(WNDCLASSEX);
	this->windowClass.style = 0;	// class styles: CS_OWNDC, CS_PARENTDC, CS_CLASSDC, CS_GLOBALCLASS, ...
	this->windowClass.lpfnWndProc = NULL;	// pointer to window procedure
	this->windowClass.cbClsExtra = 0;
	this->windowClass.cbWndExtra = 0;
	this->windowClass.hInstance = instance;
	this->windowClass.hIcon = LoadIcon(0, IDI_APPLICATION);	// default icon
	this->windowClass.hCursor = LoadCursor(0, IDC_ARROW);	 // default arrow cursor
	this->windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	this->windowClass.lpszMenuName = 0;
	this->windowClass.lpszClassName = this->className;
	this->windowClass.hIconSm = LoadIcon(0, IDI_APPLICATION);	// default small icon
};

GenericWindow::~GenericWindow()
{
	if ( this->handle )
		this->destroy();
};


bool	GenericWindow::create()
{
	bool	returnValue = false;



	if ( !this->externalClassDependency )
	{
		if ( !RegisterClassExW(&(this->windowClass)) )
			return returnValue;
	}

	this->handle = CreateWindowExW(this->extendedStyle,this->className,this->windowName,this->style,
						this->xPosition,this->yPosition,this->width,this->height,
						this->parent,this->menu,this->instance,this->parameters);
	if ( this->handle )
		returnValue = true;
	


	wcout	<< L"Creating Window:\t" << this->windowName << " with handle: \t" << this->handle << endl;
	return returnValue;
};

void	GenericWindow::destroy()
{
	if ( this->handle )
	{	
		if ( !this->externalClassDependency )
			UnregisterClassW(this->className,this->instance);


		EnumChildWindows(this->handle,(WNDENUMPROC)enumerateChildren, (LPARAM)WM_CLOSE);
		wcout	<< L"Destroying Window:\t" << this->windowName << " with handle:\t" << this->handle << endl;
		DestroyWindow(this->handle);
		this->handle = NULL;
	}
};

void	GenericWindow::hide()														{ ShowWindow(this->handle,SW_HIDE); };
void	GenericWindow::show( int nCmdShow )
{
	ShowWindow(this->handle,nCmdShow);
	UpdateWindow(this->handle);
};


void			GenericWindow::setParent( const HWND parentWindow )					{ this->parent = parentWindow; };
void			GenericWindow::setName( const wchar_t* name )						{ memset(this->windowName,'\0',MAX_LENGTH); wcsncpy_s(this->windowName,MAX_LENGTH,name,MAX_LENGTH-1); };	
void			GenericWindow::setClassName( const wchar_t* name )					{ memset(this->className,'\0',MAX_LENGTH); wcsncpy_s(this->className,MAX_LENGTH,name,MAX_LENGTH-1); this->windowClass.lpszClassName = this->className;};
void			GenericWindow::setExternalWindowClass( const wchar_t* name )		{ this->externalClassDependency = true; this->setClassName(name); };
void			GenericWindow::setWindowClassStyle( UINT style )					{ this->windowClass.style = style; };
void			GenericWindow::setWindowClassProcedure( const WNDPROC procedure )	{ this->windowClass.lpfnWndProc = procedure; };
void			GenericWindow::setWindowClassExtraBytes( int bytes )				{ this->windowClass.cbClsExtra = bytes; };
void			GenericWindow::setWindowClassWindowExtraBytes( int bytes )			{ this->windowClass.cbWndExtra = bytes; };
void			GenericWindow::setWindowClassIcon( const HICON icon )				{ this->windowClass.hIcon = icon; };
void			GenericWindow::setWindowClassCursor( const HCURSOR cursor )			{ this->windowClass.hCursor = cursor; };
void			GenericWindow::setWindowClassBackground( const HBRUSH brush )		{ this->windowClass.hbrBackground = brush; };
void			GenericWindow::setWindowClassMenu( const LPCWSTR menu )				{ this->windowClass.lpszMenuName = menu; };
void			GenericWindow::setStyle( DWORD style )								{ this->style = style; };
void			GenericWindow::setExtendedStyle( DWORD style )						{ this->extendedStyle = style; };
void			GenericWindow::setMenu( const HMENU menu )							{ this->menu = menu; };
void			GenericWindow::setInstance( const HINSTANCE instance )				{ this->instance = instance; };
void			GenericWindow::setParameters( const LPVOID param )					{ this->parameters = param; };
void			GenericWindow::setDimensions( int width , int height )
{ 
	this->width = width;
	this->height = height; 
};

void			GenericWindow::setPosition( int x , int y )							
{ 
	this->xPosition = x;
	this->yPosition = y; 
};


const HWND		GenericWindow::getHandle() const									{ return this->handle; };
const HWND		GenericWindow::getParent() const									{ return this->parent; };
const wchar_t*	GenericWindow::getName() const										{ return this->windowName; };
const wchar_t*	GenericWindow::getClassName() const									{ return this->className; };
bool			GenericWindow::hasExternalClassDependency() const					{ return this->externalClassDependency; };
UINT			GenericWindow::getWindowClassStyle() const							{ return this->windowClass.style; };
const WNDPROC	GenericWindow::getWindowClassProcedure() const						{ return this->windowClass.lpfnWndProc; };
int				GenericWindow::getWindowClassExtraBytes() const						{ return this->windowClass.cbClsExtra; };
int				GenericWindow::getWindowClassWindowExtraBytes() const				{ return this->windowClass.cbWndExtra; };
const HICON		GenericWindow::getWindowClassIcon() const							{ return this->windowClass.hIcon; };
const HCURSOR	GenericWindow::getWindowClassCursor() const							{ return this->windowClass.hCursor; };
const HBRUSH	GenericWindow::getWindowClassBackground() const						{ return this->windowClass.hbrBackground; };
const wchar_t*	GenericWindow::getWindowClassMenu() const							{ return this->windowClass.lpszMenuName; };
DWORD			GenericWindow::getStyle() const										{ return this->style; };
DWORD			GenericWindow::getExtendedStyle() const								{ return this->extendedStyle; };
void			GenericWindow::getDimensions( int& width , int& height ) const		{ width = this->width; height = this->height; };
void			GenericWindow::getPosition( int& x , int& y ) const					{ x = this->xPosition; y = this->yPosition; };
const HMENU		GenericWindow::getMenu() const										{ return this->menu; };
const HINSTANCE	GenericWindow::getInstance() const									{ return this->instance; };
const LPVOID	GenericWindow::getParameters() const								{ return this->parameters; };