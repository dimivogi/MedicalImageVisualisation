
#ifndef	REDIRECT_IO_TO_CONSOLE
#define	REDIRECT_IO_TO_CONSOLE


#define	MAX_CONSOLE_LINES	500

// Redirect all input and output streams to the console
// used in debug mode since window by default do not allocate a console for windows applications
void	RedirectIOToConsole();


#endif	/* REDIRECT_IO_TO_CONSOLE */