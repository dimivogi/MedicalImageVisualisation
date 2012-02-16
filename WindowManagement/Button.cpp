#include	"Button.h"




Button::Button() : Control() , text(L"")	{};
Button::Button( HWND prnt , int ID ) : Control(prnt,ID) , text(L"")	{};
Button::Button( HWND prnt , int ID , wstring txt ) : Control(prnt,ID) , text(txt)	{};
Button::~Button()	{};


void	Button::setText( wstring txt )	{ this->text = txt; if ( this->handle )	SendMessageW(this->handle, WM_SETTEXT, 0, (LPARAM)text.c_str()); };
wstring	Button::getText()				{ return this->text; };