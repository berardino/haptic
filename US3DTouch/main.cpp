#include "US3DTouch.h"

int main()
{
	US3DTouch *us=new US3DTouch;

	us->m_CWnd->show();

	return Fl::run();
}
