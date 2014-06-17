/***************************************************************
 * Name:      ToPDFMain.h
 * Purpose:   Defines Application Frame
 * Author:    kenjiuno ()
 * Created:   2014-06-12
 * Copyright: kenjiuno ()
 * License:
 **************************************************************/

#ifndef TOPDFMAIN_H
#define TOPDFMAIN_H

//(*Headers(ToPDFFrame)
#include <wx/menu.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
//*)

class ToPDFFrame: public wxFrame
{
    public:

        ToPDFFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~ToPDFFrame();

    private:

        //(*Handlers(ToPDFFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        //*)

        //(*Identifiers(ToPDFFrame)
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(ToPDFFrame)
        wxStatusBar* StatusBar1;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // TOPDFMAIN_H
