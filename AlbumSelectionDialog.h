#ifndef ALBUM_SELECTION_DIALOG_H
#define ALBUM_SELECTION_DIALOG_H

#include <wx/wx.h>
#include <wx/listbox.h>
#include <vector>
#include "Album.h"
#include "Music.h"

enum
{
    ID_AddToAlbum = wxID_HIGHEST + 1,
    ID_RemoveFromAlbum
};

class AlbumSelectionDialog : public wxDialog
{
public:
    AlbumSelectionDialog(wxWindow* parent, const std::vector<Album>& albums, Music* music);
    virtual ~AlbumSelectionDialog();

    wxString GetSelectedAlbum() const;
    
private:
    void OnAddToAlbum(wxCommandEvent& event);
    void OnRemoveFromAlbum(wxCommandEvent& event);

    std::vector<Album> albums;
    Music* music;
    wxListBox* albumListBox;
    wxButton* addButton;
    wxButton* removeButton;

    wxDECLARE_EVENT_TABLE();
};

#endif // ALBUM_SELECTION_DIALOG_H
