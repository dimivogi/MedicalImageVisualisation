#include	"GenericController.h"



GenericController::GenericController() :
	handle(0) , controlSubject(NULL)	{};

GenericController::GenericController( GenericWindow* subject ) :
	handle(0) , controlSubject(subject)	{};

GenericController::~GenericController()
{
	PostQuitMessage(0);
};


inline void				GenericController::setControlSubject( GenericWindow* subject )					{ this->controlSubject = subject; };
inline void				GenericController::setHandle( const HWND handle )								{ this->handle = handle; };
inline int				GenericController::sendMessage( UINT message , WPARAM wParam , LPARAM lParam )	{ return PostMessageW(this->handle,message,wParam,lParam); };

inline int				GenericController::create()														{ return 0; }
inline int				GenericController::destroy()													{ PostQuitMessage(0); return 0; }
inline int				GenericController::enable(bool flag)											{ return 0; }
inline int				GenericController::close()														{ this->controlSubject->destroy(); return 0; }
inline int				GenericController::activate( WPARAM wParam , LPARAM lParam )					{ return DefWindowProcW(this->controlSubject->getHandle(),WM_ACTIVATE,wParam,lParam); };

inline int				GenericController::notify(int id, LPARAM lParam)								{ return 0; }
inline int				GenericController::contextMenu(HWND handle, int x, int y)						{ return 0; }
inline int				GenericController::size(int w, int h, WPARAM type)								{ return 0; }
inline int				GenericController::timer(WPARAM id, LPARAM lParam)								{ return 0; }
inline int				GenericController::command(LPARAM msg , WPARAM wParam , LPARAM lParam )			{ return 0; }
inline int				GenericController::systemCommand( WPARAM wParam , LPARAM lParam )				{ return DefWindowProcW(this->controlSubject->getHandle(), WM_SYSCOMMAND, wParam, lParam); };

inline int				GenericController::keyUp(int key, LPARAM lParam)								{ SendMessageW(HWND_BROADCAST,WM_KEYUP,(WPARAM)key,lParam); return 0; }
inline int				GenericController::keyDown(int key, LPARAM lParam)								{ return 0; }
inline int				GenericController::leftButtonUp(WPARAM wParam, int x, int y)					{ return 0; }
inline int				GenericController::leftButtonDown(WPARAM wParam, int x, int y)					{ return 0; }
inline int				GenericController::middleButtonUp(WPARAM wParam, int x, int y)					{ return 0; }
inline int				GenericController::middleButtonDown(WPARAM wParam, int x, int y)				{ return 0; }
inline int				GenericController::rightButtonUp(WPARAM wParam, int x, int y)					{ return 0; }
inline int				GenericController::rightButtonDown(WPARAM wParam, int x, int y)					{ return 0; }
inline int				GenericController::horizontalScroll(WPARAM wParam, LPARAM lParam)				{ return 0; }
inline int				GenericController::verticalScroll(WPARAM wParam, LPARAM lParam)					{ return 0; }
inline int				GenericController::mouseMove(WPARAM keyState, int x, int y)						{ return 0; }
inline int				GenericController::mouseWheelMove( int state , int delta , int x , int y )		{ return 0; }

inline int				GenericController::paint()														{ return DefWindowProcW(this->controlSubject->getHandle(),WM_PAINT,0,0); }
inline int				GenericController::eraseBkgnd(HDC hdc)											{ return DefWindowProcW(this->controlSubject->getHandle(),WM_ERASEBKGND,(WPARAM)hdc,0); }

inline void				GenericController::setMovement( bool move )										{}

const HWND				GenericController::getHandle() const											{ return this->handle; };
const GenericWindow*	GenericController::getControlSubject() const									{ return this->controlSubject; };
