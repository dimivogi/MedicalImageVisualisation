#include	"Control.h"



#ifndef	TRACK_BAR
#define	TRACK_BAR


// Class containing a trackbar control
class	TrackBar	: public Control
{
	private:

		// Current Value
		int		value;
		// Maximum Value
		int		maxValue;
		// Minimum Value
		int		minValue;
		// Tick frequency between [min value, max value]
		int		tickFrequency;



	public:

		TrackBar();
		TrackBar( HWND prnt , int ID );
		TrackBar( HWND prnt , int ID , int minVal , int maxVal );
		TrackBar( HWND prnt , int ID , int minVal , int maxVal , int val );
		~TrackBar();


		// Set minimum and maximum values
		void	setMinMaxValues( int min , int max );
		// Set current value
		void	setValue( int value );
		// Set tic at the specified position
		void	setTic(int pos);
		// Set tick frequency
		void	setTicFreq(int freq);


		// Get minimum and maximum values
		void	getMinMaxValues( int& min , int& max );
		// Get current value
		int		getValue();
		// Update current value
		void	updateValue();
};


#endif	/* TRACK_BAR */