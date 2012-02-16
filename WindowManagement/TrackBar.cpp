#include	"TrackBar.h"




TrackBar::TrackBar()	: Control() , value(0) , minValue(0) , maxValue(1)	, tickFrequency(1) {};
TrackBar::TrackBar( HWND prnt , int ID )	: Control(prnt,ID) , value(0) , minValue(0) , maxValue(1) , tickFrequency(1)	{};
TrackBar::TrackBar( HWND prnt , int ID , int minVal , int maxVal )	:  Control(prnt,ID) , value(0) , minValue(minVal) , maxValue(maxVal) , tickFrequency(1)	{};
TrackBar::TrackBar( HWND prnt , int ID , int minVal , int maxVal , int val )	:  Control(prnt,ID) , value(val) , minValue(minVal) , maxValue(maxVal) , tickFrequency(1)	{};
TrackBar::~TrackBar()	{};


void	TrackBar::setMinMaxValues( int min , int max )	
{
	this->minValue = min;
	this->maxValue = max;
	if ( this->handle ) SendMessageW(this->handle,TBM_SETRANGE,(WPARAM)true,(LPARAM)MAKELONG(this->minValue,this->maxValue)); 
};

void	TrackBar::setValue( int value )
{
	this->value = value;	
	if ( this->handle ) SendMessageW(this->handle,TBM_SETPOS,(WPARAM)true,(LPARAM)this->value); 
};


void	TrackBar::getMinMaxValues( int& min , int& max )	{ min = this->minValue; max = this->maxValue; };	// equivalent: min = (int)SendMessageW(handle, TBM_GETRANGEMIN, 0, 0); max = (int)SendMessage(handle, TBM_GETRANGEMAX, 0, 0);
int		TrackBar::getValue()								{ return this->value; };	// equivalent: (int)SendMessageW(handle, TBM_GETPOS, 0, 0);
void	TrackBar::updateValue()								{ if ( this->handle ) this->value = (int)SendMessageW(this->handle,TBM_GETPOS,0,0); };


void	TrackBar::setTic(int pos) 
{ 
	if ( pos <= this->minValue  ||  pos >= this->maxValue ) return;   // skip if it is the first or last tick
	if ( this->handle ) SendMessageW(this->handle, TBM_SETTIC, 0, (LPARAM)pos);
}

void	TrackBar::setTicFreq(int freq) 
{
	this->tickFrequency = freq;
	if ( this->handle ) SendMessageW(this->handle, TBM_SETTICFREQ, (WPARAM)this->tickFrequency, 0); 
};