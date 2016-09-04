// wxWidgets "Hello world" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#include "MyApp.h"
#include "MyFrame.h"
#include "wx/popupwin.h"
#include "wx/thread.h"

wxIMPLEMENT_APP(MyApp); //main()

bool MyApp::OnInit()
{
  MyFrame *frame = new MyFrame( "Hello World", wxPoint(500, 50), wxSize(1000, 800) );
  frame->Show( true );
  return true;
}


