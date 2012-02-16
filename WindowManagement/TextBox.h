#include	"Control.h"
#include	<string>
using namespace	std;



#ifndef	TEXT_BOX
#define	TEXT_BOX


// Class containing a text box control class
class	TextBox	: public Control
{
	private:

		// Contained Text
		wstring	text;
		// Contained Text's length
		int		length;
		// Contained Text's max length
		int		maxLength;

	public:

		TextBox();
		TextBox( HWND parent , int id );
		TextBox( HWND parent , int id , wstring text );
		TextBox( HWND parent , int id , wstring text , int maxLength );
		~TextBox();


		// Contained Text Functions
		void	setText( wstring txt );
		wstring	getText();
		void	updateText();
		void	updateText( int length );

		// Contained Text's Length functions
		void	setLength( int length );
		int		getLength();

		// Contained Text's Max Length functions
		void	setMaxLength( int maxLength );
		int		getMaxLength();

		// Contained Text selection functions
		void	selectText();
		void	unselectText();
};


#endif	/* TEXT_BOX */