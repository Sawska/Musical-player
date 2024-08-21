#include "SearchResultsDialog.h"
#include "MyFrame.h"

wxBEGIN_EVENT_TABLE(SearchResultsDialog, wxDialog)
    EVT_LISTBOX(wxID_ANY, SearchResultsDialog::OnItemSelected)
wxEND_EVENT_TABLE()

SearchResultsDialog::SearchResultsDialog(wxWindow* parent, const std::vector<wxString>& results)
    : wxDialog(parent, wxID_ANY, "Search Results", wxDefaultPosition, wxSize(400, 300))
{
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    resultsListBox = new wxListBox(this, wxID_ANY);
    for (const auto& result : results)
    {
        resultsListBox->Append(result);
    }

    sizer->Add(resultsListBox, 1, wxEXPAND | wxALL, 10);
    SetSizerAndFit(sizer);
}

void SearchResultsDialog::OnItemSelected(wxCommandEvent& event)
{
    int selection = resultsListBox->GetSelection();
    if (selection != wxNOT_FOUND)
    {
        wxString selectedItem = resultsListBox->GetString(selection);
        
        MyFrame* frame = dynamic_cast<MyFrame*>(GetParent());
        if (frame)
        {
            frame->AddSongsToQueue(selectedItem);
        }
        
        EndModal(wxID_OK);
    }
}
