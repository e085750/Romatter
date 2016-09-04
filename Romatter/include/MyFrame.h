#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
  #include "wx/timer.h"
#endif

class Twit;
class MyThread;
enum
{
  ID_Hello = 1,
  ID_read_timeline,
  ID_post,
  readTimeline_Timer
};

class MyFrame: public wxFrame
{
  public:
    MyFrame();
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    ~MyFrame();
    void read_timeline();

  public:
    Twit *twit;
    wxTextCtrl *tl;

  private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void read_timeline(wxCommandEvent& event);
    void post(wxCommandEvent& event);
    void OnTextURL(wxTextUrlEvent& event);
    int64_t last_id;
    wxTextCtrl *post_text;
  protected:
    // the timer for simulating gauge progress
    wxTimer *m_timer;
    // start progress timer
    void StartTimer();
    void read_timeline(wxTimerEvent& event);
    wxDECLARE_EVENT_TABLE();
};

