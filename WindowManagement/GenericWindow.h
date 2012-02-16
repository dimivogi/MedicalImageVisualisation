#ifndef	WIN32_LEAN_AND_MEAN
	#define	WIN32_LEAN_AND_MEAN
#endif
#include	<Windows.h>



#ifndef	GENERIC_WINDOW
#define	GENERIC_WINDOW


#define	MAX_LENGTH	256

// Class handling a window
class	GenericWindow
{
	protected:
		
		// Window handle
		HWND			handle;
		// Window parent's handle
		HWND			parent;
		// Window name, max name size MAX_LENGTH
		wchar_t			windowName[MAX_LENGTH];
		// Window class name, max name size MAX_LENGTH
		wchar_t			className[MAX_LENGTH];
		// Whether the window uses a class defined by another window
		bool			externalClassDependency;
		// Window Class structure
		WNDCLASSEXW		windowClass;
		// Window's Style
		DWORD			style;
		// Window's Extened Style
		DWORD			extendedStyle;
		// Window's Width
		int				width;
		// Window's Height
		int				height;
		// Window's horizontal Position
		int				xPosition;
		// Window's Vertical Position
		int				yPosition;
		// Window's Menu
		HMENU			menu;
		// Instance in which the window belongs
		HINSTANCE		instance;
		// Window's Parameters
		LPVOID			parameters;



	public:

		GenericWindow();
		GenericWindow( HINSTANCE inst , const wchar_t* name , HWND parentWindow );
		~GenericWindow();


		// Create the window
		virtual bool	create();
		// Destroy the window
		void			destroy();
		// Show the window
		void			show( int nCmdShow = SW_SHOWDEFAULT );
		// Hide the window
		void			hide();


		// Mutator Functions
		void			setParent( const HWND parentWindow );
		void			setName( const wchar_t* name );
		void			setClassName( const wchar_t* name );
		void			setExternalWindowClass( const wchar_t* name );
		void			setWindowClassStyle( UINT style );
		void			setWindowClassProcedure( const WNDPROC procedure );
		void			setWindowClassExtraBytes( int bytes );
		void			setWindowClassWindowExtraBytes( int bytes );
		void			setWindowClassIcon( const HICON icon );
		void			setWindowClassCursor( const HCURSOR cursor );
		void			setWindowClassBackground( const HBRUSH brush );
		void			setWindowClassMenu( const LPCWSTR menu );
		void			setStyle( DWORD style );
		void			setExtendedStyle( DWORD style );
		virtual void	setDimensions( int width , int height );
		void			setPosition( int x , int y );
		void			setMenu( const HMENU menu );
		void			setInstance( const HINSTANCE instance );
		void			setParameters( const LPVOID param );


		// Functions returning information stored in the class
		const HWND		getHandle() const;
		const HWND		getParent() const;
		const wchar_t*	getName() const;
		const wchar_t*	getClassName() const;
		bool			hasExternalClassDependency() const;
		UINT			getWindowClassStyle() const;
		const WNDPROC	getWindowClassProcedure() const;
		int				getWindowClassExtraBytes() const;
		int				getWindowClassWindowExtraBytes() const;
		const HICON		getWindowClassIcon() const;
		const HCURSOR	getWindowClassCursor() const;
		const HBRUSH	getWindowClassBackground() const;
		const wchar_t*	getWindowClassMenu() const;
		DWORD			getStyle() const;
		DWORD			getExtendedStyle() const;
		void			getDimensions( int& width , int& height ) const;
		void			getPosition( int& x , int& y ) const;
		const HMENU		getMenu() const;
		const HINSTANCE	getInstance() const;
		const LPVOID	getParameters() const;
};


#endif	/* GENERIC_WINDOW */