/***************************************************************
 * Name:      checktreeApp.cpp
 * Purpose:   Code for Application Class
 * Author:     ()
 * Created:   2016-07-09
 * Copyright:  ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "checktreeApp.h"
#include "checktreeMain.h"

IMPLEMENT_APP(checktreeApp);

bool checktreeApp::OnInit()
{
    checktreeFrame* frame = new checktreeFrame(0L);
    frame->SetIcon(wxICON(aaaa)); // To Set App Icon
    frame->Show();
    
    return true;
}
