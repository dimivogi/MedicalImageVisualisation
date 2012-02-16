#include	"Control.h"




Control::Control() : handle(NULL) , parent(NULL) , id(NULL)	{};
Control::Control( HWND prnt , int ID ) : handle(GetDlgItem(prnt, ID)) , parent(prnt) , id(ID)	{};
Control::~Control()	{};


void	Control::enable()									{ if ( this->handle )	EnableWindow(this->handle,true); } ;
void	Control::disable()									{ if ( this->handle )	EnableWindow(this->handle,false); };
void	Control::show()										{ if ( this->handle )	ShowWindow(this->handle,SW_SHOW); };
void	Control::hide()										{ if ( this->handle )	ShowWindow(this->handle,SW_HIDE); };
void	Control::focus()									{ if ( this->handle )	SetFocus(this->handle); };
bool	Control::isVisible()	
{ 
	if ( this->handle )
		return ( IsWindowVisible(this->handle) != FALSE );
	else
		return false; 
};


void	Control::setParent( HWND parent )	
{ 
	this->parent = parent;
	if ( this->id ) this->handle = GetDlgItem(this->parent,this->id); 
};
void	Control::setID( int id )	
{ 
	this->id = id; 
	if ( this->parent ) this->handle = GetDlgItem(this->parent,this->id); 
};


const HWND	Control::getHandle()							{ return this->handle; };
const HWND	Control::getParent()							{ return this->parent; };
int		Control::getID()									{ return this->id; };