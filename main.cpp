#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/bmpbuttn.h>
#include <wx/image.h>
#include <wx/bitmap.h>
#include "PlayerManager.h"
#include "Music.h"
#include "Album.h"


class MyApp : public wxApp
{
public:
    bool OnInit() override;
    PlayerManager pm;

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
    void OnButtonClick(wxCommandEvent& event);
    void MyFrame::DisplaySongs();

    wxPanel* sidebarPanel;
    wxPanel* mainPanel;
    wxBitmapButton* btnMusic;
    wxBitmapButton* btnSearchMusic;
    wxBitmapButton* btnAlbum;
    wxBitmapButton* ShuflButton;
    wxBitmapButton* GoFurtherButton;
    wxBitmapButton* PlayButton;
    wxBitmapButton* GoDownButton;
    wxBitmapButton* RepeatButton;
    wxStaticText* MusicName;
    wxStaticText* AuthorName;
    wxSlider* volumeSlider;

    wxBitmap LoadBitmap(const wxString& filename, int width, int height);
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
    wxInitAllImageHandlers(); 
    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
}

MyFrame::MyFrame()
    : wxFrame(nullptr, wxID_ANY, "Music Player")
{
    wxMenu *menuMusic = new wxMenu;
    wxMenu *menuAlbum = new wxMenu;
    menuMusic->Append(ID_Options_AddMusic, "Add Music", "Add music file manually");
    menuAlbum->Append(ID_Options_AddAlbum, "Add Album", "Create album");
    menuMusic->Append(ID_Options_SearchMusic, "Search Music", "Search for music");
    menuAlbum->Append(ID_Options_SearchAlbum, "Search Album", "Search for album");
    menuMusic->Append(ID_Options_RemoveMusic, "Remove Music", "Removes music from path");
    menuMusic->Append(ID_Options_DeleteMusic, "Delete Music", "Deletes music file from your computer");
    menuAlbum->Append(ID_Options_RemoveAlbum, "Remove Album", "Removes album");
    menuAlbum->Append(ID_Options_DeleteAlbum, "Delete Album", "Deletes album with all music files from your computer");
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

    // Panel and control initialization
    sidebarPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, -1), wxBORDER_SIMPLE);
    mainPanel = new wxPanel(this, wxID_ANY);

    // Button dimensions
    int buttonWidth = 50;
    int buttonHeight = 50;

    wxBitmap MusicBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/music.png", buttonWidth, buttonHeight);
    wxBitmap SearchBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/search.png", buttonWidth, buttonHeight);
    wxBitmap AlbumBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/album.png", buttonWidth, buttonHeight);

    btnMusic = new wxBitmapButton(sidebarPanel, wxID_ANY, MusicBitmap);
    btnSearchMusic = new wxBitmapButton(sidebarPanel, wxID_ANY, SearchBitmap);
    btnAlbum = new wxBitmapButton(sidebarPanel, wxID_ANY, AlbumBitmap);

    // Create labels for sidebar buttons
    wxStaticText* lblMusic = new wxStaticText(sidebarPanel, wxID_ANY, "Music");
    wxStaticText* lblSearchMusic = new wxStaticText(sidebarPanel, wxID_ANY, "Search");
    wxStaticText* lblAlbum = new wxStaticText(sidebarPanel, wxID_ANY, "Album");

    wxBoxSizer* sidebarSizer = new wxBoxSizer(wxVERTICAL);
    sidebarSizer->Add(btnMusic, 0, wxALL, 5);
    sidebarSizer->Add(lblMusic, 0, wxALIGN_CENTER | wxBOTTOM, 5);
    sidebarSizer->Add(btnSearchMusic, 0, wxALL, 5);
    sidebarSizer->Add(lblSearchMusic, 0, wxALIGN_CENTER | wxBOTTOM, 5);
    sidebarSizer->Add(btnAlbum, 0, wxALL, 5);
    sidebarSizer->Add(lblAlbum, 0, wxALIGN_CENTER | wxBOTTOM, 5);
    sidebarPanel->SetSizer(sidebarSizer);

    wxBitmap shuflBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/shuffle.png", buttonWidth, buttonHeight);
    wxBitmap goFurtherBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/gofurther.png", buttonWidth, buttonHeight);
    wxBitmap playBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/play.png", buttonWidth, buttonHeight);
    wxBitmap goDownBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/godown.png", buttonWidth, buttonHeight);
    wxBitmap repeatBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/repeat.png", buttonWidth, buttonHeight);

    ShuflButton = new wxBitmapButton(mainPanel, wxID_ANY, shuflBitmap);
    GoFurtherButton = new wxBitmapButton(mainPanel, wxID_ANY, goFurtherBitmap);
    PlayButton = new wxBitmapButton(mainPanel, wxID_ANY, playBitmap);
    GoDownButton = new wxBitmapButton(mainPanel, wxID_ANY, goDownBitmap);
    RepeatButton = new wxBitmapButton(mainPanel, wxID_ANY, repeatBitmap);

    volumeSlider = new wxSlider(mainPanel, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxSize(200, -1), wxSL_HORIZONTAL);
    MusicName = new wxStaticText(mainPanel, wxID_ANY, "MusicName");
    AuthorName = new wxStaticText(mainPanel, wxID_ANY, "AuthorName");

    wxBoxSizer* textSizer = new wxBoxSizer(wxVERTICAL);
    textSizer->Add(MusicName, 0, wxALL, 5);
    textSizer->Add(AuthorName, 0, wxALL, 5);

    wxBoxSizer* bottomSizer = new wxBoxSizer(wxHORIZONTAL);
    bottomSizer->Add(ShuflButton, 0, wxALL, 5);
    bottomSizer->Add(GoDownButton, 0, wxALL, 5);
    bottomSizer->Add(PlayButton, 0, wxALL, 5);
    bottomSizer->Add(GoFurtherButton, 0, wxALL, 5);
    bottomSizer->Add(RepeatButton, 0, wxALL, 5);
    bottomSizer->Add(textSizer, 0, wxALL, 5); 
    bottomSizer->Add(volumeSlider, 1, wxALL | wxEXPAND, 5);

    wxBoxSizer* mainContentSizer = new wxBoxSizer(wxVERTICAL);
    mainContentSizer->Add(bottomSizer, 0, wxEXPAND);

    mainPanel->SetSizer(mainContentSizer);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(sidebarPanel, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(mainPanel, 1, wxEXPAND | wxALL, 5);

    SetSizerAndFit(mainSizer);
    Layout();

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

    Bind(wxEVT_BUTTON, &MyFrame::OnButtonClick, this, wxID_ANY);
}

void MyFrame::OnMusic(wxCommandEvent& event)
{
    wxMessageBox("Music menu item clicked", "Info", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnAlbum(wxCommandEvent& event)
{
    wxMessageBox("Album menu item clicked", "Info", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Music Player application.",
                 "About Music Player", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnButtonClick(wxCommandEvent& event)
{
    wxMessageBox("Button clicked", "Info", wxOK | wxICON_INFORMATION);
}

wxBitmap MyFrame::LoadBitmap(const wxString& filename, int width, int height)
{
    wxImage image;
    image.LoadFile(filename, wxBITMAP_TYPE_PNG);
    image.Rescale(width, height, wxIMAGE_QUALITY_HIGH);
    return wxBitmap(image);
}
