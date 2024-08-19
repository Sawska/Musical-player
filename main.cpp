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
#include <stack>
#include <thread>
#include <wx/wx.h>
#include <wx/log.h>

enum class FrameState {
    MAIN_MENU,
    SONG_LIST,
    ALBUM_LIST,
    ALBUM_SONGS
};


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
    std::stack<wxSizer*> previousStates;
    PlayerManager pm;
    std::stack<Music*> historyStack;
    bool isPlaying = false;
    sf::Sound currentSound;
sf::SoundBuffer currentBuffer;
std::queue<Music*> playingQueue;


public:
    void OnMusic(wxCommandEvent& event);
    void OnAlbum(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnButtonClick(wxCommandEvent& event);
    void DisplaySongs();
    void DisplayAlbumSongs(const Album& album);
    void DisplayAlbum();
    void OnPlayButton(wxCommandEvent& event);
    void OnPlayAlbumButton(wxCommandEvent& event);
    void OnSettingsButton(wxCommandEvent& event);
    void OnSettingsButtonAlbum(wxCommandEvent& event);
    void OnLikeButton(wxCommandEvent& event);
    void OnLikeAlbumButton(wxCommandEvent& event);
    void OnAddToQueueButton(wxCommandEvent& event);
    void OnDislikedSongs(wxCommandEvent& event);
    void OnLikedSongs(wxCommandEvent& event);
    void OnLikedAlbums(wxCommandEvent& event);
    void OnDislikedAlbums(wxCommandEvent& event);
    void OnAddMusic(wxCommandEvent& event);
    void OnAddAlbum(wxCommandEvent& event);
    void OnDislikeButtonSongs(wxCommandEvent &event);
    void OnDislikeButton(wxCommandEvent& event);
    void PlayNextSong();
    void UpdatePlayButtonUI(wxBitmapButton* button, bool isPlaying);
    void OnSkipPrevious(wxCommandEvent &event);
    void OnVolumeChange(wxCommandEvent &event);
    void OnSkipNext(wxCommandEvent &event);
    void OnShuffle(wxCommandEvent &event);
    
    
    
    FrameState currentState;
    FrameState previousState;
    
    void OnBackButton(wxCommandEvent& event);


    wxPanel* sidebarPanel;
    wxPanel* mainPanel;
    wxBitmapButton* btnMusic;
    wxBitmapButton* btnSearchMusic;
    wxBitmapButton* btnAlbum;
    wxBitmapButton* likedSongsButton;
    wxBitmapButton* likedAlbumsButton;
    wxBitmapButton* DislikedAlbumsButton;
    wxBitmapButton* dislikedSongsButton;
    wxBitmapButton* ShuflButton;
    wxBitmapButton* GoFurtherButton;
    wxBitmapButton* PlayButton;
    wxBitmapButton* GoDownButton;
    wxBitmapButton* RepeatButton;
    wxBitmapButton* AddMusicButton;
    wxBitmapButton* AddAlbumButton;
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
    ID_Options_DeleteAlbum = 8,
    ID_PlayNextSong = 9
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

    sidebarPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, -1), wxBORDER_SIMPLE);
    mainPanel = new wxPanel(this, wxID_ANY);

    int buttonWidth = 50;
    int buttonHeight = 50;

    wxBitmap MusicBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/music.png", buttonWidth, buttonHeight);
    wxBitmap SearchBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/search.png", buttonWidth, buttonHeight);
    wxBitmap AlbumBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/album.png", buttonWidth, buttonHeight);
    wxBitmap LikedSongsBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/like.png", buttonWidth, buttonHeight);
    wxBitmap DislikedSongsBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/dislike.png", buttonWidth, buttonHeight);
    wxBitmap LikedAlbumsBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/AlbumsLiked.png", buttonWidth, buttonHeight);
    wxBitmap DisLikedAlbumsBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/AlbumsDisLiked.png", buttonWidth, buttonHeight);
    wxBitmap AddMusicBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/AddMusic.png", buttonWidth, buttonHeight);
    wxBitmap AddAlbumBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/AddAlbum.png", buttonWidth, buttonHeight);

    btnMusic = new wxBitmapButton(sidebarPanel, wxID_ANY, MusicBitmap);
btnSearchMusic = new wxBitmapButton(sidebarPanel, wxID_ANY, SearchBitmap);
btnAlbum = new wxBitmapButton(sidebarPanel, wxID_ANY, AlbumBitmap);
likedSongsButton = new wxBitmapButton(sidebarPanel, wxID_ANY, LikedSongsBitmap);
dislikedSongsButton = new wxBitmapButton(sidebarPanel, wxID_ANY, DislikedSongsBitmap);
likedAlbumsButton = new wxBitmapButton(sidebarPanel, wxID_ANY, LikedAlbumsBitmap);
DislikedAlbumsButton = new wxBitmapButton(sidebarPanel, wxID_ANY, DisLikedAlbumsBitmap);
AddMusicButton = new wxBitmapButton(sidebarPanel, wxID_ANY, AddMusicBitmap);
AddAlbumButton = new wxBitmapButton(sidebarPanel, wxID_ANY, AddAlbumBitmap);

wxStaticText* lblMusic = new wxStaticText(sidebarPanel, wxID_ANY, "Music");
wxStaticText* lblSearchMusic = new wxStaticText(sidebarPanel, wxID_ANY, "Search");
wxStaticText* lblAlbum = new wxStaticText(sidebarPanel, wxID_ANY, "Album");


wxBoxSizer* searchSizer = new wxBoxSizer(wxHORIZONTAL);


wxTextCtrl* searchInput = new wxTextCtrl(sidebarPanel, wxID_ANY);


searchSizer->Add(btnSearchMusic, 0, wxALL, 5);
searchSizer->Add(searchInput, 1, wxALL | wxEXPAND, 5);


wxBoxSizer* sidebarSizer = new wxBoxSizer(wxVERTICAL);
sidebarSizer->Add(btnMusic, 0, wxALL, 5);
sidebarSizer->Add(lblMusic, 0, wxALIGN_CENTER | wxBOTTOM, 5);
sidebarSizer->Add(searchSizer, 0, wxEXPAND | wxALL, 5); 
sidebarSizer->Add(lblSearchMusic, 0, wxALIGN_CENTER | wxBOTTOM, 5);
sidebarSizer->Add(btnAlbum, 0, wxALL, 5);
sidebarSizer->Add(lblAlbum, 0, wxALIGN_CENTER | wxBOTTOM, 5);



    sidebarSizer->Add(likedSongsButton, 0, wxALL, 5);
    sidebarSizer->Add(dislikedSongsButton, 0, wxALL, 5);
    sidebarSizer->Add(likedAlbumsButton, 0, wxALL, 5);
    sidebarSizer->Add(DislikedAlbumsButton, 0, wxALL, 5);

    sidebarSizer->Add(AddMusicButton, 0, wxALL, 5);
    sidebarSizer->Add(AddAlbumButton, 0, wxALL, 5);

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

    
    likedSongsButton->Bind(wxEVT_BUTTON, &MyFrame::OnLikedSongs, this);
    dislikedSongsButton->Bind(wxEVT_BUTTON, &MyFrame::OnDislikedSongs, this);
    likedAlbumsButton->Bind(wxEVT_BUTTON, &MyFrame::OnLikedAlbums, this);
    DislikedAlbumsButton->Bind(wxEVT_BUTTON, &MyFrame::OnDislikedAlbums, this);
    AddMusicButton->Bind(wxEVT_BUTTON, &MyFrame::OnAddMusic, this);
    AddMusicButton->Bind(wxEVT_BUTTON, &MyFrame::OnAddAlbum, this);
}

#include <wx/wx.h>
#include <wx/textctrl.h>

void MyFrame::OnAddAlbum(wxCommandEvent& event) {
    wxDialog* addAlbumDialog = new wxDialog(this, wxID_ANY, "Create New Album", wxDefaultPosition, wxSize(300, 150));


    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);


    wxStaticText* albumNameLabel = new wxStaticText(addAlbumDialog, wxID_ANY, "Album Name:");
    wxTextCtrl* albumNameInput = new wxTextCtrl(addAlbumDialog, wxID_ANY);


    wxButton* submitButton = new wxButton(addAlbumDialog, wxID_OK, "Submit");


    submitButton->Bind(wxEVT_BUTTON, [addAlbumDialog, albumNameInput, this](wxCommandEvent& event) {
        wxString albumName = albumNameInput->GetValue();


        if (!albumName.IsEmpty()) {
            Album newAlbum;
            newAlbum.name = albumName.ToStdString();
            

            pm.add_album(newAlbum);
        }

        addAlbumDialog->EndModal(wxID_OK);
    });


    vbox->Add(albumNameLabel, 0, wxALL, 10);
    vbox->Add(albumNameInput, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    vbox->Add(submitButton, 0, wxALIGN_CENTER | wxALL, 10);


    addAlbumDialog->SetSizer(vbox);
    addAlbumDialog->ShowModal();


    addAlbumDialog->Destroy();
}



void MyFrame::OnAddMusic(wxCommandEvent& event) {

    wxDialog* addMusicDialog = new wxDialog(this, wxID_ANY, "Add New Music", wxDefaultPosition, wxSize(400, 250));


    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);


    wxStaticText* nameLabel = new wxStaticText(addMusicDialog, wxID_ANY, "Name:");
    wxTextCtrl* nameInput = new wxTextCtrl(addMusicDialog, wxID_ANY);


    wxStaticText* authorLabel = new wxStaticText(addMusicDialog, wxID_ANY, "Author:");
    wxTextCtrl* authorInput = new wxTextCtrl(addMusicDialog, wxID_ANY);


    wxStaticText* pathLabel = new wxStaticText(addMusicDialog, wxID_ANY, "Path:");
    wxTextCtrl* pathInput = new wxTextCtrl(addMusicDialog, wxID_ANY);


    wxButton* submitButton = new wxButton(addMusicDialog, wxID_OK, "Submit");


    submitButton->Bind(wxEVT_BUTTON, [addMusicDialog, nameInput, authorInput, pathInput, this](wxCommandEvent& event) {
        wxString name = nameInput->GetValue();
        wxString author = authorInput->GetValue();
        wxString path = pathInput->GetValue();


        if (!name.IsEmpty() && !author.IsEmpty() && !path.IsEmpty()) {
            Music newMusic;
            newMusic.name = name.ToStdString();
            newMusic.author = author.ToStdString();
            newMusic.path_to_file = path.ToStdString();
            

            pm.add_music(newMusic);
        }

        addMusicDialog->EndModal(wxID_OK);
    });


    vbox->Add(nameLabel, 0, wxALL, 10);
    vbox->Add(nameInput, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    vbox->Add(authorLabel, 0, wxALL, 10);
    vbox->Add(authorInput, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    vbox->Add(pathLabel, 0, wxALL, 10);
    vbox->Add(pathInput, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    vbox->Add(submitButton, 0, wxALIGN_CENTER | wxALL, 10);


    addMusicDialog->SetSizer(vbox);
    addMusicDialog->ShowModal();


    addMusicDialog->Destroy();
}


void MyFrame::OnDislikedAlbums(wxCommandEvent& event) {
    previousState = currentState;
    currentState = FrameState::ALBUM_LIST;
    mainPanel->DestroyChildren();
    pm.load_albums();

    wxBoxSizer* albumListSizer = new wxBoxSizer(wxVERTICAL);
    int buttonWidth = 25;
    int buttonHeight = 25;
    wxBitmap playBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/play.png", buttonWidth, buttonHeight);
    wxBitmap settingsBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/settings.png", buttonWidth, buttonHeight);
    wxBitmap plusBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/plus.png", buttonWidth, buttonHeight);
    wxBitmap dislikedBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/dislike.png", buttonWidth, buttonHeight);
    wxButton* backButton = new wxButton(mainPanel, wxID_ANY, "Back");
    backButton->Bind(wxEVT_BUTTON, &MyFrame::OnBackButton, this);

    for (Album& album : pm.load_filtered_albums(FilterType::DISLIKED)) {
        wxBoxSizer* albumSizer = new wxBoxSizer(wxHORIZONTAL);

        wxStaticText* albumName = new wxStaticText(mainPanel, wxID_ANY, wxString::Format("Name: %s", album.name.c_str()));
        pm.add_real_music_to_album(album);
        album.compute_listen_time();
        wxStaticText* playingTime = new wxStaticText(mainPanel, wxID_ANY, wxString::Format("Playing time: %s", album.time_to_listen));

        wxBitmapButton* playButton = new wxBitmapButton(mainPanel, wxID_ANY, playBitmap);
        wxBitmapButton* settingsButton = new wxBitmapButton(mainPanel, wxID_ANY, settingsBitmap);
        wxBitmapButton* addToQueueButton = new wxBitmapButton(mainPanel, wxID_ANY, plusBitmap);
        wxBitmapButton* dislikeButton = new wxBitmapButton(mainPanel, wxID_ANY, dislikedBitmap);

        wxBitmap likeBitmap;
        if (album.status_of_liked == 1) {
            likeBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/liked.png", buttonWidth, buttonHeight);
        } else if (album.status_of_liked == -1) {
            likeBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/disliked.png", buttonWidth, buttonHeight);
        } else {
            likeBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/reset.png", buttonWidth, buttonHeight);
        }
        wxBitmapButton* likeButton = new wxBitmapButton(mainPanel, wxID_ANY, likeBitmap);


        addToQueueButton->SetClientData(new Album(album));
        playButton->SetClientData(new Album(album));
        settingsButton->SetClientData(new Album(album));
        dislikeButton->SetClientData(new Album(album));
        likeButton->SetClientData(new Album(album));


        playButton->Bind(wxEVT_BUTTON, &MyFrame::OnPlayButton, this);
        settingsButton->Bind(wxEVT_BUTTON, &MyFrame::OnSettingsButton, this);
        likeButton->Bind(wxEVT_BUTTON, &MyFrame::OnLikeButton, this);
        addToQueueButton->Bind(wxEVT_BUTTON, &MyFrame::OnAddToQueueButton, this);
        dislikeButton->Bind(wxEVT_BUTTON, &MyFrame::OnDislikeButton, this);


        albumSizer->Add(playButton, 0, wxALL, 5);
        albumSizer->Add(albumName, 1, wxALL | wxEXPAND, 5);
        albumSizer->Add(playingTime, 1, wxALL | wxEXPAND, 5);
        albumSizer->Add(likeButton, 0, wxALL, 5);
        albumSizer->Add(addToQueueButton, 0, wxALL, 5);
        albumSizer->Add(settingsButton, 0, wxALL, 5);
        albumSizer->Add(dislikeButton, 0, wxALL, 5);

        albumListSizer->Add(albumSizer, 0, wxEXPAND | wxALL, 5);
    }

    mainPanel->SetSizer(albumListSizer);
    mainPanel->Layout();
    mainPanel->Refresh();
}


void MyFrame::OnLikedAlbums(wxCommandEvent& event) {
    previousState = currentState;
    currentState = FrameState::ALBUM_LIST;
    mainPanel->DestroyChildren();
    pm.load_albums();

    wxBoxSizer* albumListSizer = new wxBoxSizer(wxVERTICAL);
    int buttonWidth = 25;
    int buttonHeight = 25;
    wxBitmap playBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/play.png", buttonWidth, buttonHeight);
    wxBitmap settingsBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/settings.png", buttonWidth, buttonHeight);
    wxBitmap plusBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/plus.png", buttonWidth, buttonHeight);
    wxBitmap dislikeBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/dislike.png", buttonWidth, buttonHeight);
    wxBitmap likeBitmap;
    wxButton* backButton = new wxButton(mainPanel, wxID_ANY, "Back");
    backButton->Bind(wxEVT_BUTTON, &MyFrame::OnBackButton, this);

    for (Album& album : pm.load_filtered_albums(FilterType::LIKED)) {
        wxBoxSizer* albumSizer = new wxBoxSizer(wxHORIZONTAL);

        wxStaticText* albumName = new wxStaticText(mainPanel, wxID_ANY, wxString::Format("Name: %s", album.name.c_str()));
        pm.add_real_music_to_album(album);
        album.compute_listen_time();
        wxStaticText* playingTime = new wxStaticText(mainPanel, wxID_ANY, wxString::Format("Playing time: %d", album.time_to_listen));

        wxBitmapButton* playButton = new wxBitmapButton(mainPanel, wxID_ANY, playBitmap);
        wxBitmapButton* settingsButton = new wxBitmapButton(mainPanel, wxID_ANY, settingsBitmap);
        wxBitmapButton* addToQueueButton = new wxBitmapButton(mainPanel, wxID_ANY, plusBitmap);

        if (album.status_of_liked == 1) {
            likeBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/liked.png", buttonWidth, buttonHeight);
        } else if (album.status_of_liked == -1) {
            likeBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/disliked.png", buttonWidth, buttonHeight);
        } else {
            likeBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/reset.png", buttonWidth, buttonHeight);
        }
        wxBitmapButton* likeButton = new wxBitmapButton(mainPanel, wxID_ANY, likeBitmap);
        wxBitmapButton* dislikeButton = new wxBitmapButton(mainPanel, wxID_ANY, dislikeBitmap);

        
        playButton->SetClientData(new Album(album));
        settingsButton->SetClientData(new Album(album));
        addToQueueButton->SetClientData(new Album(album));
        likeButton->SetClientData(new Album(album));
        dislikeButton->SetClientData(new Album(album));

        
        playButton->Bind(wxEVT_BUTTON, &MyFrame::OnPlayButton, this);
        settingsButton->Bind(wxEVT_BUTTON, &MyFrame::OnSettingsButton, this);
        likeButton->Bind(wxEVT_BUTTON, &MyFrame::OnLikeButton, this);
        addToQueueButton->Bind(wxEVT_BUTTON, &MyFrame::OnAddToQueueButton, this);
        dislikeButton->Bind(wxEVT_BUTTON, &MyFrame::OnDislikeButton, this);


        albumSizer->Add(playButton, 0, wxALL, 5);
        albumSizer->Add(albumName, 1, wxALL | wxEXPAND, 5);
        albumSizer->Add(playingTime, 1, wxALL | wxEXPAND, 5);
        albumSizer->Add(likeButton, 0, wxALL, 5);
        albumSizer->Add(dislikeButton, 0, wxALL, 5);
        albumSizer->Add(addToQueueButton, 0, wxALL, 5);
        albumSizer->Add(settingsButton, 0, wxALL, 5);

        albumListSizer->Add(albumSizer, 0, wxEXPAND | wxALL, 5);
    }

    mainPanel->SetSizer(albumListSizer);
    mainPanel->Layout();
    mainPanel->Refresh();
}

void MyFrame::OnDislikeButton(wxCommandEvent& event) {
    wxBitmapButton* button = dynamic_cast<wxBitmapButton*>(event.GetEventObject());
    Album* album = static_cast<Album*>(button->GetClientData());

    album->album_disliked();
    pm.update_album(album);
}



void MyFrame::OnDislikedSongs(wxCommandEvent& event) {
    previousState = currentState;
    currentState = FrameState::SONG_LIST;

    mainPanel->DestroyChildren();
    pm.load_songs();

    wxBoxSizer* songListSizer = new wxBoxSizer(wxVERTICAL);
    int buttonWidth = 25;
    int buttonHeight = 25;

    wxBitmap playBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/play.png", buttonWidth, buttonHeight);
    wxBitmap settingsBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/settings.png", buttonWidth, buttonHeight);
    wxBitmap dislikeBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/dislike.png", buttonWidth, buttonHeight);
    wxBitmap likeBitmap;

    for ( Music& music : pm.load_filtered_song(FilterType::DISLIKED)) {
        wxBoxSizer* songSizer = new wxBoxSizer(wxHORIZONTAL);

        wxStaticText* songName = new wxStaticText(mainPanel, wxID_ANY, wxString::Format("Name: %s", music.name.c_str()));
        wxStaticText* authorName = new wxStaticText(mainPanel, wxID_ANY, wxString::Format("Author: %s", music.author.c_str()));
        wxStaticText* playingTime = new wxStaticText(mainPanel, wxID_ANY, wxString::Format("Playing time: %s", music.time_to_listen));

        wxBitmapButton* playButton = new wxBitmapButton(mainPanel, wxID_ANY, playBitmap);
        wxBitmapButton* settingsButton = new wxBitmapButton(mainPanel, wxID_ANY, settingsBitmap);

        if (music.status_of_liked == 1) {
            likeBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/liked.png", buttonWidth, buttonHeight);
        } else if (music.status_of_liked == -1) {
            likeBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/disliked.png", buttonWidth, buttonHeight);
        } else {
            likeBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/reset.png", buttonWidth, buttonHeight);
        }
        wxBitmapButton* likeButton = new wxBitmapButton(mainPanel, wxID_ANY, likeBitmap);
        wxBitmapButton* dislikeButton = new wxBitmapButton(mainPanel, wxID_ANY, dislikeBitmap);

        
        playButton->SetClientData(new Music(music));
        settingsButton->SetClientData(new Music(music));
        likeButton->SetClientData(new Music(music));
        dislikeButton->SetClientData(new Music(music));

        
        playButton->Bind(wxEVT_BUTTON, &MyFrame::OnPlayButton, this);
        settingsButton->Bind(wxEVT_BUTTON, &MyFrame::OnSettingsButton, this);
        likeButton->Bind(wxEVT_BUTTON, &MyFrame::OnLikeButton, this);
        dislikeButton->Bind(wxEVT_BUTTON, &MyFrame::OnDislikeButtonSongs, this);

        
        songSizer->Add(playButton, 0, wxALL, 5);
        songSizer->Add(songName, 1, wxALL | wxEXPAND, 5);
        songSizer->Add(authorName, 1, wxALL | wxEXPAND, 5);
        songSizer->Add(playingTime, 1, wxALL | wxEXPAND, 5);
        songSizer->Add(likeButton, 0, wxALL, 5);
        songSizer->Add(dislikeButton, 0, wxALL, 5);
        songSizer->Add(settingsButton, 0, wxALL, 5);

        songListSizer->Add(songSizer, 0, wxEXPAND | wxALL, 5);
    }

    mainPanel->SetSizer(songListSizer);
    mainPanel->Layout();
    mainPanel->Refresh();
}




void MyFrame::OnLikedSongs(wxCommandEvent& event) {
    previousState = currentState;
    currentState = FrameState::SONG_LIST;

    mainPanel->DestroyChildren();
    pm.load_songs();

    wxBoxSizer* songListSizer = new wxBoxSizer(wxVERTICAL);
    int buttonWidth = 25;
    int buttonHeight = 25;

    wxBitmap playBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/play.png", buttonWidth, buttonHeight);
    wxBitmap settingsBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/settings.png", buttonWidth, buttonHeight);
    wxBitmap likeBitmap;
    wxBitmap dislikeBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/dislike.png", buttonWidth, buttonHeight);

    for (const Music& music : pm.load_filtered_song(FilterType::LIKED)) {
        wxBoxSizer* songSizer = new wxBoxSizer(wxHORIZONTAL);

        wxStaticText* songName = new wxStaticText(mainPanel, wxID_ANY, wxString::Format("Name: %s", music.name.c_str()));
        wxStaticText* authorName = new wxStaticText(mainPanel, wxID_ANY, wxString::Format("Author: %s", music.author.c_str()));
        wxStaticText* playingTime = new wxStaticText(mainPanel, wxID_ANY, wxString::Format("Playing time: %s", music.time_to_listen));

        wxBitmapButton* playButton = new wxBitmapButton(mainPanel, wxID_ANY, playBitmap);
        wxBitmapButton* settingsButton = new wxBitmapButton(mainPanel, wxID_ANY, settingsBitmap);

        if (music.status_of_liked == 1) {
            likeBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/liked.png", buttonWidth, buttonHeight);
        } else if (music.status_of_liked == -1) {
            likeBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/disliked.png", buttonWidth, buttonHeight);
        } else {
            likeBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/reset.png", buttonWidth, buttonHeight);
        }
        wxBitmapButton* likeButton = new wxBitmapButton(mainPanel, wxID_ANY, likeBitmap);
        wxBitmapButton* dislikeButton = new wxBitmapButton(mainPanel, wxID_ANY, dislikeBitmap);


        playButton->SetClientData(new Music(music));
        settingsButton->SetClientData(new Music(music));
        likeButton->SetClientData(new Music(music));
        dislikeButton->SetClientData(new Music(music));


        playButton->Bind(wxEVT_BUTTON, &MyFrame::OnPlayButton, this);
        settingsButton->Bind(wxEVT_BUTTON, &MyFrame::OnSettingsButton, this);
        likeButton->Bind(wxEVT_BUTTON, &MyFrame::OnLikeButton, this);
        dislikeButton->Bind(wxEVT_BUTTON, &MyFrame::OnDislikeButtonSongs, this);


        songSizer->Add(playButton, 0, wxALL, 5);
        songSizer->Add(songName, 1, wxALL | wxEXPAND, 5);
        songSizer->Add(authorName, 1, wxALL | wxEXPAND, 5);
        songSizer->Add(playingTime, 1, wxALL | wxEXPAND, 5);
        songSizer->Add(likeButton, 0, wxALL, 5);
        songSizer->Add(dislikeButton, 0, wxALL, 5);
        songSizer->Add(settingsButton, 0, wxALL, 5);

        songListSizer->Add(songSizer, 0, wxEXPAND | wxALL, 5);
    }

    mainPanel->SetSizer(songListSizer);
    mainPanel->Layout();
    mainPanel->Refresh();
}

void MyFrame::OnDislikeButtonSongs(wxCommandEvent &event) {
        wxBitmapButton* button = dynamic_cast<wxBitmapButton*>(event.GetEventObject());
    Music* music = static_cast<Music*>(button->GetClientData());
    music->song_disliked();
    pm.update_song(music);
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

void MyFrame::DisplaySongs()
{
    previousState = currentState;
    currentState = FrameState::SONG_LIST;

    mainPanel->DestroyChildren();
    pm.load_songs();

    wxBoxSizer* songListSizer = new wxBoxSizer(wxVERTICAL);
    int buttonWidth = 25;
    int buttonHeight = 25;

    wxBitmap playBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/play.png", buttonWidth, buttonHeight);
    wxBitmap settingsBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/settings.png", buttonWidth, buttonHeight);
    wxBitmap likeBitmap;

    for (const Music& music : pm.music) {
        wxBoxSizer* songSizer = new wxBoxSizer(wxHORIZONTAL);

        wxStaticText* songName = new wxStaticText(mainPanel, wxID_ANY, wxString::Format("Name: %s", music.name.c_str()));
        wxStaticText* authorName = new wxStaticText(mainPanel, wxID_ANY, wxString::Format("Author: %s", music.author.c_str()));
        wxStaticText* playingTime = new wxStaticText(mainPanel, wxID_ANY, wxString::Format("Playing time: %s", music.time_to_listen));

        wxBitmapButton* playButton = new wxBitmapButton(mainPanel, wxID_ANY, playBitmap);
        wxBitmapButton* settingsButton = new wxBitmapButton(mainPanel, wxID_ANY, settingsBitmap);

        if (music.status_of_liked == 1) {
            likeBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/liked.png", buttonWidth, buttonHeight);
        } else if (music.status_of_liked == -1) {
            likeBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/disliked.png", buttonWidth, buttonHeight);
        } else {
            likeBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/reset.png", buttonWidth, buttonHeight);
        }
        wxBitmapButton* likeButton = new wxBitmapButton(mainPanel, wxID_ANY, likeBitmap);


        playButton->Bind(wxEVT_BUTTON, &MyFrame::OnPlayButton, this);
        settingsButton->Bind(wxEVT_BUTTON, &MyFrame::OnSettingsButton, this);
        likeButton->Bind(wxEVT_BUTTON, &MyFrame::OnLikeButton, this);

        songSizer->Add(playButton, 0, wxALL, 5);
        songSizer->Add(songName, 1, wxALL | wxEXPAND, 5);
        songSizer->Add(authorName, 1, wxALL | wxEXPAND, 5);
        songSizer->Add(playingTime, 1, wxALL | wxEXPAND, 5);
        songSizer->Add(likeButton, 0, wxALL, 5);
        songSizer->Add(settingsButton, 0, wxALL, 5);

    songListSizer->Add(songSizer, 0, wxEXPAND | wxALL, 5);
    }

    mainPanel->SetSizer(songListSizer);
    mainPanel->Layout();
    mainPanel->Refresh();
}


void MyFrame::DisplayAlbumSongs(const Album& album)
{
    previousState = currentState;
    currentState = FrameState::ALBUM_SONGS;
    mainPanel->DestroyChildren();
    pm.load_songs();

    wxBoxSizer* songListSizer = new wxBoxSizer(wxVERTICAL);
    int buttonWidth = 25;
    int buttonHeight = 25;

    wxBitmap playBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/play.png", buttonWidth, buttonHeight);
    wxBitmap settingsBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/settings.png", buttonWidth, buttonHeight);
    wxBitmap likeBitmap;

    wxButton* backButton = new wxButton(mainPanel, wxID_ANY, "Back");
    backButton->Bind(wxEVT_BUTTON, &MyFrame::OnBackButton, this);

    for (const Music& music : album.album_songs) {
        wxBoxSizer* songSizer = new wxBoxSizer(wxHORIZONTAL);

        wxStaticText* songName = new wxStaticText(mainPanel, wxID_ANY, wxString::Format("Name: %s", music.name.c_str()));
        wxStaticText* authorName = new wxStaticText(mainPanel, wxID_ANY, wxString::Format("Author: %s", music.author.c_str()));
        wxStaticText* playingTime = new wxStaticText(mainPanel, wxID_ANY, wxString::Format("Playing time: %s", music.time_to_listen));

        wxBitmapButton* playButton = new wxBitmapButton(mainPanel, wxID_ANY, playBitmap);
        wxBitmapButton* settingsButton = new wxBitmapButton(mainPanel, wxID_ANY, settingsBitmap);

        if (music.status_of_liked == 1) {
            likeBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/liked.png", buttonWidth, buttonHeight);
        } else if (music.status_of_liked == -1) {
            likeBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/disliked.png", buttonWidth, buttonHeight);
        } else {
            likeBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/reset.png", buttonWidth, buttonHeight);
        }
        wxBitmapButton* likeButton = new wxBitmapButton(mainPanel, wxID_ANY, likeBitmap);


        playButton->Bind(wxEVT_BUTTON, &MyFrame::OnPlayButton, this);
        settingsButton->Bind(wxEVT_BUTTON, &MyFrame::OnSettingsButton, this);
        likeButton->Bind(wxEVT_BUTTON, &MyFrame::OnLikeButton, this);

        songSizer->Add(playButton, 0, wxALL, 5);
        songSizer->Add(songName, 1, wxALL | wxEXPAND, 5);
        songSizer->Add(authorName, 1, wxALL | wxEXPAND, 5);
        songSizer->Add(playingTime, 1, wxALL | wxEXPAND, 5);
        songSizer->Add(likeButton, 0, wxALL, 5);
        songSizer->Add(settingsButton, 0, wxALL, 5);

        songListSizer->Add(songSizer, 0, wxEXPAND | wxALL, 5);
    }

    mainPanel->SetSizer(songListSizer);
    mainPanel->Layout();
    mainPanel->Refresh();
}

void MyFrame::DisplayAlbum()
{
    previousState = currentState;
    currentState = FrameState::ALBUM_LIST;
    mainPanel->DestroyChildren();
    pm.load_albums();

    wxBoxSizer* albumListSizer = new wxBoxSizer(wxVERTICAL);
    int buttonWidth = 25;
    int buttonHeight = 25;
    wxBitmap playBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/play.png", buttonWidth, buttonHeight);
    wxBitmap settingsBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/settings.png", buttonWidth, buttonHeight);
    wxBitmap plusBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/plus.png", buttonWidth, buttonHeight);
    wxBitmap likeBitmap;
    wxButton* backButton = new wxButton(mainPanel, wxID_ANY, "Back");
    backButton->Bind(wxEVT_BUTTON, &MyFrame::OnBackButton, this);

    for (Album& album : pm.albums) {
        wxBoxSizer* albumSizer = new wxBoxSizer(wxHORIZONTAL);

        wxStaticText* albumName = new wxStaticText(mainPanel, wxID_ANY, wxString::Format("Name: %s", album.name.c_str()));
        pm.add_real_music_to_album(album);
        album.compute_listen_time();
        wxStaticText* playingTime = new wxStaticText(mainPanel, wxID_ANY, wxString::Format("Playing time: %s", album.time_to_listen));

        wxBitmapButton* playButton = new wxBitmapButton(mainPanel, wxID_ANY, playBitmap);
        wxBitmapButton* settingsButton = new wxBitmapButton(mainPanel, wxID_ANY, settingsBitmap);
        wxBitmapButton* addToQueueButton = new wxBitmapButton(mainPanel, wxID_ANY, plusBitmap);

        if (album.status_of_liked == 1) {
            likeBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/liked.png", buttonWidth, buttonHeight);
        } else if (album.status_of_liked == -1) {
            likeBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/disliked.png", buttonWidth, buttonHeight);
        } else {
            likeBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/reset.png", buttonWidth, buttonHeight);
        }
        wxBitmapButton* likeButton = new wxBitmapButton(mainPanel, wxID_ANY, likeBitmap);

        addToQueueButton->SetClientData(new Album(album));
        playButton->Bind(wxEVT_BUTTON, &MyFrame::OnPlayButton, this);
        settingsButton->Bind(wxEVT_BUTTON, &MyFrame::OnSettingsButton, this);
        likeButton->Bind(wxEVT_BUTTON, &MyFrame::OnLikeButton, this);
        addToQueueButton->Bind(wxEVT_BUTTON, &MyFrame::OnAddToQueueButton, this);

        albumSizer->Add(playButton, 0, wxALL, 5);
        albumSizer->Add(albumName, 1, wxALL | wxEXPAND, 5);
        albumSizer->Add(playingTime, 1, wxALL | wxEXPAND, 5);
        albumSizer->Add(likeButton, 0, wxALL, 5);
        albumSizer->Add(addToQueueButton, 0, wxALL, 5);
        albumSizer->Add(settingsButton, 0, wxALL, 5);

        albumListSizer->Add(albumSizer, 0, wxEXPAND | wxALL, 5);
    }

    mainPanel->SetSizer(albumListSizer);
    mainPanel->Layout();
    mainPanel->Refresh();
}




void MyFrame::OnPlayButton(wxCommandEvent &event) {
    if (!isPlaying) {
        PlayNextSong();
    } else {
        currentSound.pause();
        UpdatePlayButtonUI(PlayButton,false);
        isPlaying = false;
    }
}

void MyFrame::PlayNextSong() {
    if (!playingQueue.empty()) {
        Music* music = playingQueue.front();

        if (isPlaying) {
            historyStack.push(music);
        }

        playingQueue.pop();

        if (!currentBuffer.loadFromFile(music->path_to_file)) {
            std::cerr << "Error loading music file" << std::endl;
            return;
        }

        currentSound.setBuffer(currentBuffer);
        currentSound.setVolume(volumeSlider->GetValue());
        currentSound.play();

        MusicName->SetLabel(music->name);
        AuthorName->SetLabel(music->author);

        UpdatePlayButtonUI(PlayButton, true); 
        isPlaying = true;

        std::thread([this]() {
            while (currentSound.getStatus() == sf::Sound::Playing) {
                sf::sleep(sf::milliseconds(100));
            }
            wxQueueEvent(this, new wxCommandEvent(wxEVT_NULL, ID_PlayNextSong));
        }).detach();
    } else {
        UpdatePlayButtonUI(PlayButton, false);
        isPlaying = false;
    }
}



void MyFrame::OnSkipPrevious(wxCommandEvent &event) {
    if (!historyStack.empty()) {
        Music* previousMusic = historyStack.top();
        historyStack.pop();

        if (!currentBuffer.loadFromFile(previousMusic->path_to_file)) {
            std::cerr << "Error loading music file" << std::endl;
            return;
        }

        currentSound.setBuffer(currentBuffer);
        currentSound.setVolume(volumeSlider->GetValue());
        currentSound.play();

        MusicName->SetLabel(previousMusic->name);
        AuthorName->SetLabel(previousMusic->author);

        UpdatePlayButtonUI(PlayButton,true);
        isPlaying = true;

        std::thread([this]() {
            while (currentSound.getStatus() == sf::Sound::Playing) {
                sf::sleep(sf::milliseconds(100));
            }
            wxQueueEvent(this, new wxCommandEvent(wxEVT_NULL, ID_PlayNextSong));
        }).detach();
    }
}

void MyFrame::OnVolumeChange(wxCommandEvent &event) {
    currentSound.setVolume(volumeSlider->GetValue());
}

void MyFrame::OnSkipNext(wxCommandEvent &event) {
    if (isPlaying) {
        currentSound.stop();
        PlayNextSong();
    }
}


void MyFrame::OnShuffle(wxCommandEvent &event) {
        pm.playing_queue.shuffle();
}





Bind(wxEVT_COMMAND_SLIDER_UPDATED, &MyFrame::OnVolumeChange, this, volumeSlider->GetId());
Bind(wxEVT_BUTTON, &MyFrame::OnSkipNext, this, GoFurtherButton->GetId());
Bind(wxEVT_BUTTON, &MyFrame::OnSkipPrevious, this, GoDownButton->GetId());
Bind(wxEVT_BUTTON, &MyFrame::OnShuffle, this, ShuflButton->GetId());



void MyFrame::OnSettingsButton(wxCommandEvent &event)
{
    wxBitmapButton* button = dynamic_cast<wxBitmapButton*>(event.GetEventObject());
    Music* music = static_cast<Music*>(button->GetClientData());

    SettingsDialog* dialog = new SettingsDialog(this, music->name, music->author);
    
    if (dialog->ShowModal() == wxID_DELETE)
    {
        pm.delete_music(music->path_to_file);
    }
    else if (dialog->ShowModal() == wxID_REMOVE)
    {
        pm.remove_music_from_db(music->name);
    }
    else if (dialog->ShowModal() == wxID_UPDATE)
    {
        
        wxString newName = dialog->GetName();
        wxString newAuthor = dialog->GetAuthor();

        
        music->name = std::string(newName.mb_str());
        music->author = std::string(newAuthor.mb_str());

        pm.update_music(music);
    }
    else if (dialog->ShowModal() == wxID_ADD_TO_ALBUM)
    {
        std::vector<Album> albums = pm.get_all_albums();
        AlbumSelectionDialog albumDialog(this, albums, music);
        if (albumDialog.ShowModal() == wxID_OK)
    {
        OnBackButton();
    }
    }
     else if(dialog->ShowModal() == wxID_RemoveFromAlbum)
    {
        std::vector<Album> albums = pm.get_all_albums();
        AlbumSelectionDialog albumDialog(this, albums, music);
        if (albumDialog.ShowModal() == wxID_OK)
    }

    dialog->Destroy();
}

void MyFrame::OnSettingsButtonAlbum(wxCommandEvent &event)
{
    wxBitmapButton* button = dynamic_cast<wxBitmapButton*>(event.GetEventObject());
    Album* album = static_cast<Album*>(button->GetClientData());

    AlbumSettingsDialog* dialog = new AlbumSettingsDialog(this, album->name);

    if (dialog->ShowModal() == ID_DELETE)
    {
        pm.delete_album(album->name);  
    }
    else if (dialog->ShowModal() == ID_REMOVE)
    {
        pm.remove_album_from_db(album->name);
    }
    else if (dialog->ShowModal() == ID_UPDATE)
    {
        wxString newName = dialog->GetNewAlbumName();
        album->name = std::string(newName.mb_str());
        pm.update_album(album);  
    }

    dialog->Destroy();
}


void MyFrame::OnLikeButton(wxCommandEvent &event)
{
    wxBitmapButton* button = dynamic_cast<wxBitmapButton*>(event.GetEventObject());
    Music* music = static_cast<Music*>(button->GetClientData());

    if (music->status_of_liked == Music::LIKED)
    {

        music->status_of_liked = Music::NONE; 
    }
    else
    {

        music->status_of_liked = Music::LIKED;
    }


    pm.update_music(music);


    UpdateLikeButtonUI(button, music->status_of_liked);
}

void MyFrame::OnLikeAlbumButton(wxCommandEvent &event)
{
    
    wxBitmapButton* button = dynamic_cast<wxBitmapButton*>(event.GetEventObject());
    Album* album = static_cast<Album*>(button->GetClientData());

    
    if (album->status_of_liked == Album::LIKED)
    {
        album->status_of_liked = Album::NONE; 
    }
    else
    {
        album->status_of_liked = Album::LIKED;
    }

    
    pm.update_album(album);

    
    UpdateAlbumButtonUI(button, album->status_of_liked);
}

void MyFrame::UpdateAlbumButtonUI(wxBitmapButton* button, Album::Status status)
{
    
    if (status == Album::LIKED)
    {
        button->SetBitmap(wxBitmap("like.png", wxBITMAP_TYPE_PNG));  
    }
    else
    {
        button->SetBitmap(wxBitmap("reset.png", wxBITMAP_TYPE_PNG)); 
    }
}


void MyFrame::UpdateLikeButtonUI(wxBitmapButton* button, Music::Status status)
{
    if (status == Music::LIKED)
    {
        button->SetBitmap(wxBitmap("like.png", wxBITMAP_TYPE_PNG)); 
    }
    else
    {
        button->SetBitmap(wxBitmap("reset.png", wxBITMAP_TYPE_PNG)); 
    }
}


void MyFrame::OnAddToQueueButton(wxCommandEvent &event)
{

    wxBitmapButton* button = dynamic_cast<wxBitmapButton*>(event.GetEventObject());

    if (button != nullptr) {

        Album* album = static_cast<Album*>(button->GetClientData());

        if (album != nullptr) {
            pm.playing_queue.merge_queues(album.order_of_songs);
        }
    }
}


wxBitmap MyFrame::LoadBitmap(const wxString& filename, int width, int height)
{
    wxImage image;
    image.LoadFile(filename, wxBITMAP_TYPE_PNG);
    image.Rescale(width, height, wxIMAGE_QUALITY_HIGH);
    return wxBitmap(image);
}

void MyFrame::OnBackButton(wxCommandEvent& event) {
    switch (previousState) {
        case FrameState::MAIN_MENU:
            MyFrame();
            break;
        case FrameState::SONG_LIST:
            DisplaySongs();
            break;
        case FrameState::ALBUM_LIST:
            DisplayAlbum();
            break;
        case FrameState::ALBUM_SONGS:

            break;
    }
}




const wxWindowID wxID_RemoveFromAlbum = wxNewId();

class SettingsDialog::SettingsDialog(wxWindow* parent, wxWindowID id)
    : wxDialog(parent, id, "Settings", wxDefaultPosition, wxSize(300, 200))
{
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxButton* deleteButton = new wxButton(this, wxID_DELETE, "Delete");
    wxButton* removeButton = new wxButton(this, wxID_REMOVE, "Remove");
    wxButton* addToAlbumButton = new wxButton(this, wxID_ADD_TO_ALBUM, "Add to Album");
    wxButton* updateButton = new wxButton(this, wxID_UPDATE, "Update");
    wxButton* removeFromAlbumButton = new wxButton(this, wxID_RemoveFromAlbum, "Remove from Album");

    
    sizer->Add(deleteButton, 0, wxALL, 10);
    sizer->Add(removeButton, 0, wxALL, 10);
    sizer->Add(addToAlbumButton, 0, wxALL, 10);
    sizer->Add(updateButton, 0, wxALL, 10);
    sizer->Add(removeFromAlbumButton, 0, wxALL, 10);

    
    deleteButton->Bind(wxEVT_BUTTON, &SettingsDialog::OnDelete, this);
    removeButton->Bind(wxEVT_BUTTON, &SettingsDialog::OnRemove, this);
    addToAlbumButton->Bind(wxEVT_BUTTON, &SettingsDialog::OnAddAlbum, this);
    updateButton->Bind(wxEVT_BUTTON, &SettingsDialog::OnUpdate, this);
    removeFromAlbumButton->Bind(wxEVT_BUTTON, &SettingsDialog::OnRemoveFromAlbum, this);

    SetSizer(sizer);
}

void SettingsDialog::OnDelete(wxCommandEvent& event)
{
    wxLogMessage("Delete button clicked");
    EndModal(wxID_DELETE);
}

void SettingsDialog::OnRemove(wxCommandEvent& event)
{
    wxLogMessage("Remove button clicked");
    EndModal(wxID_REMOVE);
}

void SettingsDialog::OnUpdate(wxCommandEvent& event)
{
    wxLogMessage("Update button clicked");
    EndModal(wxID_UPDATE);
}

void SettingsDialog::OnAddAlbum(wxCommandEvent& event)
{
    wxLogMessage("Add to Album button clicked");
    EndModal(wxID_ADD_TO_ALBUM);
}


void SettingsDialog::OnRemoveFromAlbum(wxCommandEvent& event)
{
    wxLogMessage("Remove from Album button clicked");
    EndModal(wxID_RemoveFromAlbum);
}


void MyFrame::UpdatePlayButtonUI(wxBitmapButton* button, bool isPlaying)
{
    wxBitmap newBitmap;
    if (isPlaying)
    {
        newBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/pause.png", buttonWidth, buttonHeight);
    }
    else
    {
        newBitmap = LoadBitmap("/Users/alexanderkorzh/Desktop/coding/musical_player/images/play.png", buttonWidth, buttonHeight);
    }
    button->SetBitmap(newBitmap);
}
