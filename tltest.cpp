// wxWidgets "Hello world" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#include "wx/popupwin.h"
#include <ostream>
#include <fstream>
#include "Twit.h"

enum
{
  ID_Hello = 1,
  Minimal_StartSimplePopup,
  Minimal_PopupButton
};

class MyApp: public wxApp
{
  public:
    virtual bool OnInit();
};

// a text ctrl which allows to call different wxTextCtrl functions
// interactively by pressing function keys in it
class MyTextCtrl : public wxTextCtrl
{
  public:
    MyTextCtrl(wxWindow *parent, wxWindowID id, const wxString &value,
        const wxPoint &pos, const wxSize &size, int style = 0)
      : wxTextCtrl(parent, id, value, pos, size, style)
    {
      m_hasCapture = false;
    }

    //void OnKeyDown(wxKeyEvent& event);
    //void OnKeyUp(wxKeyEvent& event);
    //void OnChar(wxKeyEvent& event);

    //void OnText(wxCommandEvent& event);
    //void OnTextEnter(wxCommandEvent& event);
    //void OnTextURL(wxTextUrlEvent& event);
    //void OnTextMaxLen(wxCommandEvent& event);

    //void OnTextCut(wxClipboardTextEvent & event);
    //void OnTextCopy(wxClipboardTextEvent & event);
    //void OnTextPaste(wxClipboardTextEvent & event);

    //void OnMouseEvent(wxMouseEvent& event);
    //void OnContextMenu(wxContextMenuEvent& event);

    //void OnSetFocus(wxFocusEvent& event);
    //void OnKillFocus(wxFocusEvent& event);

    static bool ms_logKey;
    static bool ms_logChar;
    static bool ms_logMouse;
    static bool ms_logText;
    static bool ms_logFocus;
    static bool ms_logClip;

  private:
    //static inline wxChar GetChar(bool on, wxChar c) { return on ? c : wxT('-'); }

    //void LogKeyEvent(const wxChar *name, wxKeyEvent& event) const;
    //void LogClipEvent(const wxChar *what, wxClipboardTextEvent& event);

    bool m_hasCapture;

    wxDECLARE_EVENT_TABLE();
};

class MyPanel: public wxPanel
{
  public:
    MyPanel(wxFrame *frame, int x, int y, int w, int h);
    virtual ~MyPanel()
    {
#if wxUSE_LOG
      delete wxLog::SetActiveTarget(m_logOld);
#endif // wxUSE_LOG
    }

#if wxUSE_CLIPBOARD
    void DoPasteFromClipboard();
    void DoCopyToClipboard();
#endif // wxUSE_CLIPBOARD

    void DoRemoveText();
    void DoReplaceText();
    void DoSelectText();
    void DoMoveToEndOfText();
    void DoMoveToEndOfEntry();
    void DoGetWindowCoordinates();

    // return true if currently text control has any selection
    bool HasSelection() const
    {
      long from, to;
      GetFocusedText()->GetSelection(&from, &to);
      return from != to;
    }

    MyTextCtrl    *m_enter;
    MyTextCtrl    *m_enter2;
    MyTextCtrl    *m_tab;

    MyTextCtrl    *m_textrich;

#if wxUSE_LOG
    wxTextCtrl    *m_log;
    wxLog         *m_logOld;
#endif // wxUSE_LOG

  private:
    // get the currently focused text control or return the default one
    // (m_multitext) is no text ctrl has focus -- in any case, returns
    // something non NULL
    wxTextCtrl *GetFocusedText() const;
    wxScrolledWindow *m_panel;

  protected:
    wxSizer *m_sizerText;
};

//----------------------------------------------------------------------------
// SimpleTransientPopup
//----------------------------------------------------------------------------
class SimpleTransientPopup: public wxPopupTransientWindow
{
  public:
    SimpleTransientPopup( wxWindow *parent, bool scrolled );
    virtual ~SimpleTransientPopup();

    // wxPopupTransientWindow virtual methods are all overridden to log them
    virtual void Popup(wxWindow *focus = NULL) wxOVERRIDE;
    //virtual void OnDismiss() wxOVERRIDE;
    //virtual bool ProcessLeftDown(wxMouseEvent& event) wxOVERRIDE;
    virtual bool Show( bool show = true ) wxOVERRIDE;

  private:
    wxScrolledWindow *m_panel;
    wxButton *m_button;
    //wxSpinCtrl *m_spinCtrl;
    wxStaticText *m_mouseText;

  private:
    void OnMouse( wxMouseEvent &event );
    //void OnSize( wxSizeEvent &event );
    //void OnSetFocus( wxFocusEvent &event );
    //void OnKillFocus( wxFocusEvent &event );
    //void OnSpinCtrl( wxSpinEvent& event );
    void OnButton( wxCommandEvent& event );

  private:
    wxDECLARE_ABSTRACT_CLASS(SimpleTransientPopup);
    wxDECLARE_EVENT_TABLE();
};

//----------------------------------------------------------------------------
// SimpleTransientPopup
//----------------------------------------------------------------------------
wxIMPLEMENT_CLASS(SimpleTransientPopup,wxPopupTransientWindow);

wxBEGIN_EVENT_TABLE(SimpleTransientPopup,wxPopupTransientWindow)
    EVT_MOUSE_EVENTS( SimpleTransientPopup::OnMouse )
    //EVT_SIZE( SimpleTransientPopup::OnSize )
    //EVT_SET_FOCUS( SimpleTransientPopup::OnSetFocus )
    //EVT_KILL_FOCUS( SimpleTransientPopup::OnKillFocus )
    EVT_BUTTON( Minimal_PopupButton, SimpleTransientPopup::OnButton )
    //EVT_SPINCTRL( Minimal_PopupSpinctrl, SimpleTransientPopup::OnSpinCtrl )
wxEND_EVENT_TABLE()

SimpleTransientPopup::SimpleTransientPopup( wxWindow *parent, bool scrolled )
                     :wxPopupTransientWindow( parent )
{
    m_panel = new wxScrolledWindow( this, wxID_ANY );
    m_panel->SetBackgroundColour( *wxLIGHT_GREY );

    // Keep this code to verify if mouse events work, they're required if
    // you're making a control like a combobox where the items are highlighted
    // under the cursor, the m_panel is set focus in the Popup() function
    m_panel->Connect(wxEVT_MOTION,
                     wxMouseEventHandler(SimpleTransientPopup::OnMouse),
                     NULL, this);

    wxStaticText *text = new wxStaticText( m_panel, wxID_ANY,
                          wxT("wxPopupTransientWindow is a\n")
                          wxT("wxPopupWindow which disappears\n")
                          wxT("automatically when the user\n")
                          wxT("clicks the mouse outside it or if it\n")
                          wxT("(or its first child) loses focus in \n")
                          wxT("any other way.") );

    m_button = new wxButton(m_panel, Minimal_PopupButton, wxT("Press Me"));
    //m_spinCtrl = new wxSpinCtrl(m_panel, Minimal_PopupSpinctrl, wxT("Hello"));
    m_mouseText = new wxStaticText(m_panel, wxID_ANY,
                                   wxT("<- Test Mouse ->"));

    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
    topSizer->Add( text, 0, wxALL, 5 );
    topSizer->Add( m_button, 0, wxALL, 5 );
    //topSizer->Add( m_spinCtrl, 0, wxALL, 5 );
    topSizer->Add( m_mouseText, 0, wxCENTRE|wxALL, 5 );

    if ( scrolled )
    {
        // Add a big window to ensure that scrollbars are shown when we set the
        // panel size to a lesser size below.
        topSizer->Add(new wxPanel(m_panel, wxID_ANY, wxDefaultPosition,
                                  wxSize(600, 900)));
    }

    m_panel->SetSizer( topSizer );
    if ( scrolled )
    {
        // Set the fixed size to ensure that the scrollbars are shown.
        m_panel->SetSize(300, 300);

        // And also actually enable them.
        m_panel->SetScrollRate(10, 10);
    }
    else
    {
        // Use the fitting size for the panel if we don't need scrollbars.
        topSizer->Fit(m_panel);
    }

    SetClientSize(m_panel->GetSize());
}

SimpleTransientPopup::~SimpleTransientPopup()
{
}

void SimpleTransientPopup::Popup(wxWindow* WXUNUSED(focus))
{
    wxLogMessage( "%p SimpleTransientPopup::Popup", this );
    wxPopupTransientWindow::Popup();
}

void SimpleTransientPopup::OnMouse(wxMouseEvent &event)
{
    wxRect rect(m_mouseText->GetRect());
    rect.SetX(-100000);
    rect.SetWidth(1000000);
    wxColour colour(*wxLIGHT_GREY);

    if (rect.Contains(event.GetPosition()))
    {
        colour = wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT);
        wxLogMessage( "%p SimpleTransientPopup::OnMouse pos(%d, %d)",
                      event.GetEventObject(), event.GetX(), event.GetY());
    }

    if (colour != m_mouseText->GetBackgroundColour())
    {
        m_mouseText->SetBackgroundColour(colour);
        m_mouseText->Refresh();
    }
    event.Skip();
}

bool SimpleTransientPopup::Show( bool show )
{
    wxLogMessage( "%p SimpleTransientPopup::Show %d", this, int(show));
    return wxPopupTransientWindow::Show(show);
}

class MyFrame: public wxFrame
{
  public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

  public:
    Twit twit;

  private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnStartSimplePopup(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();
    MyPanel *m_panel;
    SimpleTransientPopup *m_simplePopup;
};


//----------------------------------------------------------------------
// MyPanel
//----------------------------------------------------------------------

MyPanel::MyPanel( wxFrame *frame, int x, int y, int w, int h )
       : wxPanel( frame, wxID_ANY, wxPoint(x, y), wxSize(w, h) )
{
#if wxUSE_LOG
    m_log = new wxTextCtrl( this, wxID_ANY, wxT("This is the log window.\n"),
                            wxPoint(5,260), wxSize(630,100),
                            wxTE_MULTILINE | wxTE_READONLY);

    m_logOld = wxLog::SetActiveTarget( new wxLogTextCtrl( m_log ) );
#endif // wxUSE_LOG

    // multi line text controls

    m_enter = new MyTextCtrl( this, 100, wxT(""),
      wxPoint(180,170), wxSize(200,70), wxTE_MULTILINE | wxTE_PROCESS_ENTER );
    m_enter->SetClientData((void *)wxT("enter"));


    m_enter2 = new MyTextCtrl( this, 100, wxT(""),
      wxPoint(400,50), wxSize(600,1200), wxTE_MULTILINE | wxTE_PROCESS_ENTER );
    m_enter2->SetClientData((void *)wxT("tl"));

    // lay out the controls
    wxBoxSizer *column2 = new wxBoxSizer(wxVERTICAL);
    column2->Add( m_enter, 1, wxALL | wxEXPAND, 10 );

}

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
  EVT_MENU(ID_Hello,   MyFrame::OnHello)
  EVT_MENU(wxID_EXIT,  MyFrame::OnExit)
  EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
  EVT_BUTTON(Minimal_StartSimplePopup, MyFrame::OnStartSimplePopup)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(MyTextCtrl, wxTextCtrl)
  //EVT_KEY_DOWN(MyTextCtrl::OnKeyDown)
  //EVT_KEY_UP(MyTextCtrl::OnKeyUp)
  //EVT_CHAR(MyTextCtrl::OnChar)

  //EVT_TEXT(wxID_ANY, MyTextCtrl::OnText)
  //EVT_TEXT_ENTER(wxID_ANY, MyTextCtrl::OnTextEnter)
  //EVT_TEXT_URL(wxID_ANY, MyTextCtrl::OnTextURL)
  //EVT_TEXT_MAXLEN(wxID_ANY, MyTextCtrl::OnTextMaxLen)
  //EVT_TEXT_CUT(wxID_ANY,   MyTextCtrl::OnTextCut)
  //EVT_TEXT_COPY(wxID_ANY,  MyTextCtrl::OnTextCopy)
  //EVT_TEXT_PASTE(wxID_ANY, MyTextCtrl::OnTextPaste)

  //EVT_MOUSE_EVENTS(MyTextCtrl::OnMouseEvent)
  //EVT_CONTEXT_MENU(MyTextCtrl::OnContextMenu)

  //EVT_SET_FOCUS(MyTextCtrl::OnSetFocus)
  //EVT_KILL_FOCUS(MyTextCtrl::OnKillFocus)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MyApp); //main()

bool MyApp::OnInit()
{
  MyFrame *frame = new MyFrame( "Hello World", wxPoint(500, 50), wxSize(680, 450) );
  frame->Show( true );
  return true;
}

void SimpleTransientPopup::OnButton(wxCommandEvent& event)
{
    wxLogMessage( "%p SimpleTransientPopup::OnButton ID %d", this, event.GetId());

    wxButton *button = wxDynamicCast(event.GetEventObject(), wxButton);
    if (button->GetLabel() == wxT("Press Me"))
        button->SetLabel(wxT("Pressed"));
    else
        button->SetLabel(wxT("Press Me"));

    event.Skip();
}


MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
  : wxFrame(NULL, wxID_ANY, title, pos, size)
{
  m_simplePopup = NULL;
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

  m_panel = new MyPanel( this, 10, 10, 300, 100 );
  wxButton *button1 = new wxButton( m_panel, Minimal_StartSimplePopup, wxT("Show simple popup"), wxPoint(20,20) );

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

void MyFrame::OnStartSimplePopup(wxCommandEvent& event)
{
    wxLogMessage( wxT("================================================") );
    wxString box_text = this->m_panel->m_enter->GetLineText(1L);
    wxLogMessage( box_text.c_str() );

    //this->twit.read_timeline();
    this->m_panel->m_enter2->SetInsertionPoint(0);
    
    //this->m_panel->m_enter2->WriteText(wxT("hoge\n"));
    this->m_panel->m_enter2->WriteText(box_text);

    delete m_simplePopup;
    m_simplePopup = new SimpleTransientPopup( this, false );
    wxWindow *btn = (wxWindow*) event.GetEventObject();
    wxPoint pos = btn->ClientToScreen( wxPoint(0,0) );
    wxSize sz = btn->GetSize();
    m_simplePopup->Position( pos, sz );
    wxLogMessage( "%p Simple Popup Shown pos(%d, %d) size(%d, %d)", m_simplePopup, pos.x, pos.y, sz.x, sz.y );
    //m_simplePopup->Popup();
}
