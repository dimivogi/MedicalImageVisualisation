#include	"DialogWindow.h"
#include	"WindowProcedure.h"
#include	<iostream>
using namespace	std;




DialogWindow::DialogWindow() : id(NULL) , GenericWindow()	{};
DialogWindow::DialogWindow(  WORD type, HINSTANCE hInst, const wchar_t* name ,  HWND hParent)	:
	id(type) , GenericWindow(hInst,name,hParent)	{};
DialogWindow::~DialogWindow()	{};

bool DialogWindow::create()
{
	bool	returnValue = false;


	this->handle = CreateDialogParamW(this->instance,MAKEINTRESOURCEW(this->id),this->parent,dialogProcedure,(LPARAM)this->parameters);
	if ( this->handle )
	{
		this->show();
		MoveWindow(this->handle,this->xPosition,this->yPosition,this->width,this->height,TRUE);
		returnValue = true;

		wcout	<< L"Creating Dialog Window:\t" << this->windowName << " with handle:\t" << this->handle << endl;
	}

	return returnValue;
};