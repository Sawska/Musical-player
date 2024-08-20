#ifndef SEARCH_RESULTS_DIALOG_H
#define SEARCH_RESULTS_DIALOG_H

#include <wx/wx.h>
#include <wx/listbox.h>
#include <vector>
#include <string>

class SearchResultsDialog : public wxDialog
{
public:
    SearchResultsDialog(wxWindow* parent, const std::vector<wxString>& results);

private:
    void OnItemSelected(wxCommandEvent& event);

    wxListBox* resultsListBox;

    wxDECLARE_EVENT_TABLE();
};

#endif // SEARCH_RESULTS_DIALOG_H
