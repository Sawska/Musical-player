#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

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

#endif // SETTINGSDIALOG_H
