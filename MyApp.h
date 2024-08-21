#ifndef MYAPP_H
#define MYAPP_H

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
#include <stack>
#include <queue>
#include <SFML/Audio.hpp>
#include "AlbumSelectionDialog.h"
#include "SettingsDialog.h"
#include "AlbumSettingsDialog.h"
#include "SearchResultsDialog.h"
#include <thread>
#include "MyFrameIDs.h"


class MyApp : public wxApp
{
public:
    bool OnInit() override;
};

#endif // MYAPP_H
