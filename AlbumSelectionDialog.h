#include <wx/wx.h>
#include <wx/listbox.h>
#include <vector>
#include "Album.h"
#include "Music.h"

class AlbumSelectionDialog : public wxDialog
{
public:
    AlbumSelectionDialog(wxWindow* parent, const std::vector<Album>& albums, Music* music);

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
