#include "MyFrame.h"
#include <fstream>
#include <sstream>
#include "picojson.h"
#include "Twit.h"
#include "MyApp.h"

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
  //EVT_CLOSE(MyFrame::OnClose)
  EVT_MENU(ID_Hello,   MyFrame::OnHello)
  EVT_MENU(wxID_EXIT,  MyFrame::OnExit)
  EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
  EVT_TEXT_ENTER(ID_post, MyFrame::post)
  EVT_TIMER(readTimeline_Timer, MyFrame::read_timeline)
  EVT_TEXT_URL(wxID_ANY, MyFrame::OnTextURL)
wxEND_EVENT_TABLE()

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
  : wxFrame(NULL, wxID_ANY, title, pos, size)
{
  wxMenu *menuFile = new wxMenu;
  menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
      "Help string shown in status bar for this menu item");
  menuFile->AppendSeparator();
  menuFile->Append(wxID_EXIT);
  wxMenu *menuHelp = new wxMenu;
  menuHelp->Append(wxID_ABOUT);
  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append( menuFile, "&File" );
  menuBar->Append( menuHelp, "&Help" );
  SetMenuBar( menuBar );
  CreateStatusBar();
  SetStatusText( "Welcome to wxWidgets!" );
  last_id = 1;
  twit = new Twit();

    wxPanel* p = new wxPanel(this, wxID_ANY);
    wxBoxSizer *topsizer = new wxBoxSizer( wxVERTICAL );

    // 2) top: create wxTextCtrl with minimum size (100x60)
    post_text = new wxTextCtrl( p, ID_post, wxT(""), wxDefaultPosition, wxSize(50,30), wxTE_PROCESS_ENTER);
    topsizer->Add(post_text,
        wxSizerFlags().Expand().Border(wxLEFT|wxRIGHT, 5));
        
    // 4) create two centred wxButtons
    //wxBoxSizer *button_box = new wxBoxSizer( wxHORIZONTAL );
    //button_box->Add(
    //    new wxButton(p, ID_read_timeline, wxT("read timeline")),
    //    wxSizerFlags().Border(wxALL, 7));
    //button_box->Add(
    //    new wxButton(p, ID_post, wxT("post") ),
    //    wxSizerFlags().Border(wxALL, 7));

    //topsizer->Add(button_box, wxSizerFlags().Center());

    // 2) top: create wxTextCtrl with minimum size (100x60)
    tl = new wxTextCtrl( p, wxID_ANY, wxT(" tl"), wxDefaultPosition, wxSize(100,60), wxTE_READONLY | wxTE_MULTILINE | wxTE_AUTO_URL);
    topsizer->Add(tl, wxSizerFlags(1).Expand().Border(wxALL, 5));

    p->SetSizer( topsizer );

    // don't allow frame to get smaller than what the sizers tell it and also set
    // the initial size as calculated by the sizers
    topsizer->SetSizeHints( this );
    read_timeline();
    StartTimer();
}

MyFrame::~MyFrame()
{
  delete twit;
}

void MyFrame::OnTextURL(wxTextUrlEvent& event)
{
  const wxMouseEvent& ev = event.GetMouseEvent();
  // filter out mouse moves, too many of them
  if ( ev.Moving() ) return;
  long start = event.GetURLStart(),
       end = event.GetURLEnd();
  printf("clicked url \n");
  if (ev.LeftUp()) {
    printf("hidari\n");
    wxLaunchDefaultBrowser(tl->GetValue().Mid(start, end - start));
  } else {
    printf("other event\n"); 
  }
}

void MyFrame::StartTimer()
{
    static const int INTERVAL = 60000;

    //wxLogMessage(wxT("Launched progress timer (interval = %d ms)"), INTERVAL);

    m_timer = new wxTimer(this,
        readTimeline_Timer);
    m_timer->Start(INTERVAL);

}

void MyFrame::OnExit(wxCommandEvent& event)
{
  Close( true );
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
  wxMessageBox( "This is a wxWidgets' Hello world sample",
      "About Hello World", wxOK | wxICON_INFORMATION );
}

void MyFrame::OnHello(wxCommandEvent& event)
{
  wxLogMessage("Hello world from wxWidgets!");
}

void MyFrame::read_timeline()
{
    tl->SetInsertionPoint(1);
    std::string resp = "";
    twit->read_timeline(resp, last_id);
    picojson::value v;
    try { 
      std::string err = picojson::parse(v, resp);
      if (! err.empty()) {
        std::cerr << "json parse error: " << err << std::endl;
        return;
        //exit(1);
      }

      if (v.is<picojson::object>()) {
        picojson::object po = v.get<picojson::object>();
        picojson::array array = po["error"].get<picojson::array>();
        for (picojson::array::iterator it = array.begin(); it != array.end(); it++) {
          picojson::object& o = it->get<picojson::object>();
          std::string message = o["message"].get<std::string>();
          int code = (int)o["message"].get<bool>();

          std::cout << message << " code: " << code << std::endl;

        }
        return;
      }
      //std::stringstream ss;
      std::string user;
      std::string tweet;
      picojson::array array = v.get<picojson::array>();
      for (picojson::array::iterator it = array.begin(); it != array.end(); it++) {
        picojson::object& o = it->get<picojson::object>();
        picojson::object& user_o = o["user"].get<picojson::object>();
        user = user_o["name"].get<std::string>();

        tweet = o["text"].get<std::string>();

        if((int64_t)o["id"].get<double>() > last_id) {
          last_id = (int64_t)o["id"].get<double>();
        } else if((int64_t)o["id"].get<double>() == last_id) {
          last_id = (int64_t)o["id"].get<double>() + 1;
        }

        std::string str = user;
        str += ":\t";
        str += tweet;
        str += "\n";
        tl->WriteText(wxString::FromUTF8(str.c_str()));
      }
    } catch (std::exception e) {
      std::cout << "exception: " << e.what() << " " << resp << std::endl;
    } catch (...) {
      std::cout << "get timelien error" << " " << resp << std::endl;
    }
    wxString text = tl->GetValue();
    std::string str(text.utf8_str());
    if (5000 < str.length()) {
      printf("delete. 5000, %d\n", (int)str.length());
      tl->Remove(5000, str.length()); 
    } else {
      printf("no delete. length %d\n", (int)str.length());
    }
}

void MyFrame::read_timeline(wxCommandEvent& event)
{
  read_timeline();
}

void MyFrame::post(wxCommandEvent& event)
{
    wxString box_text = post_text->GetValue();
    printf("%s\n",(const char *)box_text.utf8_str());

    twit->post((const char *)box_text.utf8_str());
    post_text->Clear();
}


void MyFrame::read_timeline(wxTimerEvent& WXUNUSED(event))
{
    read_timeline();
}

