#include "AlbumSelectionDialog.h"
#include "PlayerManager.h"

wxBEGIN_EVENT_TABLE(AlbumSelectionDialog, wxDialog)
    EVT_BUTTON(ID_AddToAlbum, AlbumSelectionDialog::OnAddToAlbum)
    EVT_BUTTON(ID_RemoveFromAlbum, AlbumSelectionDialog::OnRemoveFromAlbum)
wxEND_EVENT_TABLE()

AlbumSelectionDialog::AlbumSelectionDialog(wxWindow* parent, const std::vector<Album>& albums, Music* music)
    : wxDialog(parent, wxID_ANY, "Select Album", wxDefaultPosition, wxSize(300, 200)),
      albums(albums), music(music)
{
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    albumListBox = new wxListBox(this, wxID_ANY);
    for (const auto& album : albums)
    {
        albumListBox->Append(album.name);
    }

    addButton = new wxButton(this, ID_AddToAlbum, "Add to Album");
    removeButton = new wxButton(this, ID_RemoveFromAlbum, "Remove from Album");

    sizer->Add(albumListBox, 1, wxEXPAND | wxALL, 10);
    sizer->Add(addButton, 0, wxALL, 10);
    sizer->Add(removeButton, 0, wxALL, 10);

    SetSizer(sizer);
}



wxString AlbumSelectionDialog::GetSelectedAlbum() const
{
    return albumListBox->GetStringSelection();
}

void AlbumSelectionDialog::OnAddToAlbum(wxCommandEvent& event)
{
    wxString selectedAlbumName = GetSelectedAlbum();
    if (!selectedAlbumName.IsEmpty())
    {
        for (auto& album : albums)
        {
            if (album.name == selectedAlbumName)
            {
                PlayerManager pm;
                pm.add_music_to_album(album, *music);
                break;
            }
        }
        EndModal(wxID_OK);
    }
}

void AlbumSelectionDialog::OnRemoveFromAlbum(wxCommandEvent& event)
{
    wxString selectedAlbumName = GetSelectedAlbum();
    if (!selectedAlbumName.IsEmpty())
    {
        for (auto& album : albums)
        {
            if (album.name == selectedAlbumName)
            {
                PlayerManager pm;
                pm.remove_music_from_album(album, *music);
                break;
            }
        }
        EndModal(wxID_OK);
    }
}
