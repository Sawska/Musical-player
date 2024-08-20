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
#include <wx/log.h>

const wxWindowID wxID_RemoveFromAlbum = wxNewId();
const wxWindowID wxID_ADD_TO_ALBUM = wxNewId();
const wxWindowID wxID_UPDATE = wxNewId();

class SettingsDialog : public wxDialog  
{
public:

    SettingsDialog(wxWindow* parent, const wxString& name, const wxString& author);

    wxString GetName() const { return nameCtrl->GetValue(); }
    wxString GetAuthor() const { return authorCtrl->GetValue(); }

private:
    void OnDelete(wxCommandEvent& event);
    void OnRemove(wxCommandEvent& event);
    void OnUpdate(wxCommandEvent& event);
    void OnAddAlbum(wxCommandEvent& event);
    void OnRemoveFromAlbum(wxCommandEvent& event);

    wxTextCtrl* nameCtrl;   
    wxTextCtrl* authorCtrl; 
};

SettingsDialog::SettingsDialog(wxWindow* parent, const wxString& name, const wxString& author)
    : wxDialog(parent, wxID_ANY, "Settings", wxDefaultPosition, wxSize(300, 200))
{
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* nameLabel = new wxStaticText(this, wxID_ANY, "Name:");
    nameCtrl = new wxTextCtrl(this, wxID_ANY, name);

    wxStaticText* authorLabel = new wxStaticText(this, wxID_ANY, "Author:");
    authorCtrl = new wxTextCtrl(this, wxID_ANY, author);

    wxButton* deleteButton = new wxButton(this, wxID_DELETE, "Delete");
    wxButton* removeButton = new wxButton(this, wxID_REMOVE, "Remove");
    wxButton* addToAlbumButton = new wxButton(this, wxID_ADD_TO_ALBUM, "Add to Album");
    wxButton* updateButton = new wxButton(this, wxID_UPDATE, "Update");
    wxButton* removeFromAlbumButton = new wxButton(this, wxID_RemoveFromAlbum, "Remove from Album");

    sizer->Add(nameLabel, 0, wxALL, 10);
    sizer->Add(nameCtrl, 0, wxALL | wxEXPAND, 10);
    sizer->Add(authorLabel, 0, wxALL, 10);
    sizer->Add(authorCtrl, 0, wxALL | wxEXPAND, 10);
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
