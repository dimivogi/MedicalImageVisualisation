#include	"TextBox.h"




TextBox::TextBox()	: Control() , text(L"") , length(wstring(L"").length())	, maxLength(0)	{};
TextBox::TextBox( HWND parent , int id )	: Control(parent,id) , text(L"") , length(wstring(L"").length()) , maxLength(0)	{};
TextBox::TextBox( HWND parent , int id , wstring txt )	: Control(parent,id) , text(txt) , length(txt.length()) , maxLength(txt.length())	{};
TextBox::TextBox( HWND parent , int id , wstring txt , int max )	:
	Control(parent,id) , text(txt) , length(txt.length()) , maxLength(max)
{
	if ( txt.length() > maxLength )
	{
		this->text = txt.substr(0,maxLength);
		this->length = this->text.length();
	}
};
TextBox::~TextBox()	{};


void	TextBox::setText( wstring txt )	
{	
	if ( this->maxLength >= txt.length() )
		this->text = txt;
	else
		this->text = txt.substr(0,maxLength);
	this->length = this->text.length();

	if ( this->handle )	SendMessageW(this->handle,WM_SETTEXT,0,(LPARAM)this->text.c_str()); 
};
wstring	TextBox::getText()					{ return this->text; };
void	TextBox::updateText()
{
	wchar_t*	buff;

	if ( this->handle )
	{
		buff = new wchar_t[this->maxLength+1];
		SendMessageW(this->handle,WM_GETTEXT,(WPARAM)this->maxLength+1,(LPARAM)buff);
		this->text = buff;
		this->length = this->text.length();
		delete[] buff;
	}
}

void	TextBox::updateText( int length )
{
	wchar_t*	buff;

	if ( this->handle )
	{
		if ( this->maxLength >= length )
			this->length = length;
		else
			this->length = this->maxLength;

		buff = new wchar_t[this->length+1];
		SendMessageW(this->handle,WM_GETTEXT,(WPARAM)this->length+1,(LPARAM)buff);
		this->text = buff;
		this->length = this->text.length();
		delete[] buff;
	}
}


void	TextBox::setLength( int length )	{ this->length = length; };
int		TextBox::getLength()				{ return this->length; };

void	TextBox::setMaxLength( int max )
{
	this->maxLength = max;
	this->setText(this->text);

	if ( this->handle )	SendMessageW(this->handle,EM_SETLIMITTEXT,(WPARAM)this->maxLength,0);
}
int		TextBox::getMaxLength()				{ return this->maxLength; };


void	TextBox::selectText()				{ if ( this->handle )	SendMessageW(this->handle,EM_SETSEL,0,-1); };
void	TextBox::unselectText()				{ if ( this->handle )	SendMessageW(this->handle,EM_SETSEL,-1,0); };
