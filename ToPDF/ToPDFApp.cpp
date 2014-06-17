/***************************************************************
 * Name:      ToPDFApp.cpp
 * Purpose:   Code for Application Class
 * Author:    kenjiuno ()
 * Created:   2014-06-12
 * Copyright: kenjiuno ()
 * License:
 **************************************************************/

#include "ToPDFApp.h"

//(*AppHeaders
#include "ToPDFMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(ToPDFApp);

bool ToPDFApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	ToPDFFrame* Frame = new ToPDFFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
