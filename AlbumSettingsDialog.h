#ifndef ALBUMSETTINGSDIALOG_H
#define ALBUMSETTINGSDIALOG_H

#include <wx/wx.h>

class AlbumSettingsDialog : public wxDialog
{
public:
    AlbumSettingsDialog(wxWindow* parent, const wxString& albumName);
    
    wxString GetNewAlbumName() const;

private:
    wxTextCtrl* albumNameCtrl;

    void OnDelete(wxCommandEvent& event);
    void OnRemove(wxCommandEvent& event);
    void OnUpdate(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};

enum
{
    ID_DELETE = wxID_HIGHEST + 1,
    ID_REMOVE,
    ID_UPDATE
};

#endif // ALBUMSETTINGSDIALOG_H
