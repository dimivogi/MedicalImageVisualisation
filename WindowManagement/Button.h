#include	<string>
#include	"Control.h"
using namespace std;



#ifndef	BUTTON
#define	BUTTON


// Class containing a button controll
class	Button	: public Control
{
	private:
	
		// Button text
		wstring	text;



	public:

		Button();
		Button( HWND parent , int id );
		Button( HWND parent , int id , wstring text );
		~Button();


		// Set Button Text
		void	setText( wstring text );
		// Get Button Text
		wstring	getText();
};


#endif	/* BUTTON */