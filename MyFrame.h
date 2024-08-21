#ifndef MYFRAME_H
#define MYFRAME_H

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
#include <queue>
#include <SFML/Audio.hpp>
#include "AlbumSelectionDialog.h"
#include "SettingsDialog.h"
#include "AlbumSettingsDialog.h"
#include "SearchResultsDialog.h"
#include <thread>
#include "MyFrameIDs.h"

enum class FrameState {
    MAIN_MENU,
    SONG_LIST,
    ALBUM_LIST,
    ALBUM_SONGS
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

class MyFrame : public wxFrame
{
public:
    MyFrame();
    void AddSongsToQueue(wxString selectedItem);

private:
    std::stack<wxSizer*> previousStates;
    PlayerManager pm;
    std::stack<Music*> historyStack;
    bool isPlaying = false;
    sf::Sound currentSound;
    sf::SoundBuffer currentBuffer;
    std::queue<Music*> playingQueue;

    FrameState currentState;
    FrameState previousState;

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
    wxTextCtrl* searchInput;

    void OnMusic(wxCommandEvent& event);
    void OnAlbum(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnButtonClick(wxCommandEvent& event);
    void DisplaySongs();
    void DisplayAlbumSongs(const Album& album);
    void DisplayAlbum();
    void OnPlayButton(wxCommandEvent& event);
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
    void UpdateLikeButtonUI(wxBitmapButton* button, int status);
    void UpdateAlbumButtonUI(wxBitmapButton* button, int status);
    void OnSearch(wxCommandEvent &event);
    void OnBackButton(wxCommandEvent& event);

    wxBitmap LoadBitmap(const wxString& filename, int width, int height);
};

#endif // MYFRAME_H
