
#ifndef	WINDOW_PROCEDURE
#define	WINDOW_PROCEDURE


// Window Procedure for message handling
LRESULT CALLBACK	WndProc( HWND hWnd , UINT message , WPARAM wParam , LPARAM lParam );
// Dialog procedure for message handling
BOOL CALLBACK		dialogProcedure( HWND hwnd , UINT msg , WPARAM wParam , LPARAM lParam );


#endif	/* WINDOW_PROCEDURE */