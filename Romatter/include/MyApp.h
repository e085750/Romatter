#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif

class wxArrayThread;
class MyApp: public wxApp
{
  public:
    virtual bool OnInit();
    wxCriticalSection m_critsect;
    wxArrayThread *m_threads;
    // semaphore used to wait for the threads to exit, see MyFrame::OnQuit()
    wxSemaphore m_semAllDone;
    // indicates that we're shutting down and all threads should exit
    bool m_shuttingDown;
};

