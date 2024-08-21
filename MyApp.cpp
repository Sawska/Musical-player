#include "MyApp.h"
#include "MyFrame.h"
#include <wx/image.h>

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    wxInitAllImageHandlers(); 
    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
}
