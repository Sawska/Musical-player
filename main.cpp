
#include <wx/wx.h>


class MyApp : public wxApp
{
public:
    bool OnInit() override;
};

wxIMPLEMENT_APP(MyApp);

class MyFrame : public wxFrame
{
public:
    MyFrame();

private:
    void OnMusic(wxCommandEvent& event);
    void OnAlbum(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
};

enum
{
    ID_Options_AddMusic = 1,
    ID_Options_AddAlbum = 2,
    ID_Options_SearchMusic = 3,
    ID_Options_SearchAlbum = 4,
    ID_Options_RemoveMusic = 5,
    ID_Options_DeleteMusic = 6,
    ID_Options_RemoveAlbum = 7,
    ID_Options_DeleteAlbum = 8
};

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
}

MyFrame::MyFrame()
    : wxFrame(nullptr, wxID_ANY, "Music Player")
{
    wxMenu *menuMusic = new wxMenu;
    wxMenu *menuAlbum = new wxMenu;
    menuMusic->Append(ID_Options_AddMusic, "Add Music",
                      "Add music file manually");
    menuAlbum->Append(ID_Options_AddAlbum, "Add Album",
                      "Create album");
    menuMusic->Append(ID_Options_SearchMusic, "Search Music",
                      "Search for music");
    menuAlbum->Append(ID_Options_SearchAlbum, "Search Album",
                      "Search for album");
    menuMusic->Append(ID_Options_RemoveMusic, "Remove Music",
                      "Removes music from path");
    menuMusic->Append(ID_Options_DeleteMusic, "Delete Music",
                      "Deletes music file from your computer");
    menuAlbum->Append(ID_Options_RemoveAlbum, "Remove Album",
                      "Removes album");
    menuAlbum->Append(ID_Options_DeleteAlbum, "Delete Album",
                      "Deletes album with all music files from your computer");
    menuMusic->AppendSeparator();
    menuMusic->Append(wxID_EXIT);

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuMusic, "&Music");
    menuBar->Append(menuAlbum, "&Album");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);

    CreateStatusBar();

    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnMusic, this, ID_Options_AddMusic);
    Bind(wxEVT_MENU, &MyFrame::OnMusic, this, ID_Options_SearchMusic);
    Bind(wxEVT_MENU, &MyFrame::OnMusic, this, ID_Options_RemoveMusic);
    Bind(wxEVT_MENU, &MyFrame::OnMusic, this, ID_Options_DeleteMusic);
    Bind(wxEVT_MENU, &MyFrame::OnAlbum, this, ID_Options_AddAlbum);
    Bind(wxEVT_MENU, &MyFrame::OnAlbum, this, ID_Options_SearchAlbum);
    Bind(wxEVT_MENU, &MyFrame::OnAlbum, this, ID_Options_RemoveAlbum);
    Bind(wxEVT_MENU, &MyFrame::OnAlbum, this, ID_Options_DeleteAlbum);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is my music player written in C++",
                 "About Music Player", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnMusic(wxCommandEvent& event)
{
    wxLogMessage("Opened music menu");
}

void MyFrame::OnAlbum(wxCommandEvent& event)
{
    wxLogMessage("Opened album menu");
}


