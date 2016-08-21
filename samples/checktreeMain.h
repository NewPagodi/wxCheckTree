/***************************************************************
 * Name:      checktreeMain.h
 * Purpose:   Defines Application Frame
 * Author:     ()
 * Created:   2016-07-09
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef CHECKTREEMAIN_H
#define CHECKTREEMAIN_H


//#include "checktree.h"
#include "GUIFrame.h"

class checktreeFrame: public GUIFrame
{
    public:
        checktreeFrame(wxFrame *frame);
        ~checktreeFrame();
    private:
        virtual void OnClose(wxCloseEvent& event);
        virtual void OnQuit(wxCommandEvent& event);
        virtual void OnAbout(wxCommandEvent& event);

        void On_Left_Down( wxMouseEvent& event );
        void On_Left_Up( wxMouseEvent& event );
        void On_Left_DClick( wxMouseEvent& event );
        void OnCheckTreeChoice(wxTreeEvent& event);

        void OnB1(wxCommandEvent& event);
        void OnB2(wxCommandEvent& event);
        void OnB3(wxCommandEvent& event);
        void OnB4(wxCommandEvent& event);
        void OnB5(wxCommandEvent& event);
        void OnB6(wxCommandEvent& event);

        wxTreeItemId m_secondItem;
};

#endif // CHECKTREEMAIN_H
