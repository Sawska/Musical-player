#include "AlbumSettingsDialog.h"

wxBEGIN_EVENT_TABLE(AlbumSettingsDialog, wxDialog)
    EVT_BUTTON(ID_DELETE, AlbumSettingsDialog::OnDelete)
    EVT_BUTTON(ID_REMOVE, AlbumSettingsDialog::OnRemove)
    EVT_BUTTON(ID_UPDATE, AlbumSettingsDialog::OnUpdate)
wxEND_EVENT_TABLE()

AlbumSettingsDialog::AlbumSettingsDialog(wxWindow* parent, const wxString& albumName)
    : wxDialog(parent, wxID_ANY, "Album Settings", wxDefaultPosition, wxSize(250, 150))
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    albumNameCtrl = new wxTextCtrl(this, wxID_ANY, albumName);
    mainSizer->Add(new wxStaticText(this, wxID_ANY, "Album Name:"), 0, wxALL | wxEXPAND, 5);
    mainSizer->Add(albumNameCtrl, 0, wxALL | wxEXPAND, 5);

    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton* deleteBtn = new wxButton(this, ID_DELETE, "Delete");
    wxButton* removeBtn = new wxButton(this, ID_REMOVE, "Remove");
    wxButton* updateBtn = new wxButton(this, ID_UPDATE, "Update");

    buttonSizer->Add(deleteBtn, 1, wxALL | wxEXPAND, 5);
    buttonSizer->Add(removeBtn, 1, wxALL | wxEXPAND, 5);
    buttonSizer->Add(updateBtn, 1, wxALL | wxEXPAND, 5);

    mainSizer->Add(buttonSizer, 1, wxALL | wxEXPAND, 5);

    SetSizerAndFit(mainSizer);
}



wxString AlbumSettingsDialog::GetNewAlbumName() const
{
    return albumNameCtrl->GetValue();
}

void AlbumSettingsDialog::OnDelete(wxCommandEvent& event)
{
    EndModal(ID_DELETE);
}

void AlbumSettingsDialog::OnRemove(wxCommandEvent& event)
{
    EndModal(ID_REMOVE);
}

void AlbumSettingsDialog::OnUpdate(wxCommandEvent& event)
{
    EndModal(ID_UPDATE);
}
