/***************************************************************
 * Name:      checktreeMain.cpp
 * Purpose:   Code for Application Frame
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

#ifndef WX_PRECOMP
    #include <wx/msgdlg.h>
#endif

//#include <wx/renderer.h>
#include "checktreeMain.h"

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}


checktreeFrame::checktreeFrame(wxFrame *frame)
    : GUIFrame(frame)
{
#if wxUSE_STATUSBAR
    statusBar->SetStatusText(_("Hello Code::Blocks user!"), 0);
    statusBar->SetStatusText(wxbuildinfo(short_f), 1);
#endif

//	m_panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
//	wxBoxSizer* bSizer2;
//	bSizer2 = new wxBoxSizer( wxVERTICAL );
//
//	m_treeCtrl1 = new wxCheckTree( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
//	bSizer2->Add( m_treeCtrl1, 1, wxALL|wxEXPAND, 5 );
//
//	m_checkBox1 = new wxCheckBox( m_panel1, wxID_ANY, "item 1", wxDefaultPosition, wxDefaultSize, 0 );
//	bSizer2->Add( m_checkBox1, 0, wxALL, 5 );
//
//	wxCheckBox* m_checkBox2 = new wxCheckBox( m_panel1, wxID_ANY, "item 2", wxDefaultPosition, wxDefaultSize, 0 );
//	bSizer2->Add( m_checkBox2, 0, wxALL, 5 );
//
//	wxButton* but= new wxButton(m_panel1, wxID_ANY, "item 1");
//    bSizer2->Add( but, 0, wxALL, 5 );
//
//	m_textCtrl1 = new wxTextCtrl( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP|wxTE_MULTILINE );
//	bSizer2->Add( m_textCtrl1, 1, wxALL|wxEXPAND, 5 );
//
//
//	m_panel1->SetSizer( bSizer2 );
//	m_panel1->Layout();
//	//bSizer2->Fit( m_panel1 );


	wxTreeItemId root = m_treeCtrl1->AddRoot("root");
	for(int i=0;i<8;i++)
    {
        wxTreeItemId t = m_treeCtrl1->AppendItem(root,wxString::Format("item %d",i));
        m_treeCtrl1->SetItemState(t, 0);

        if(i==1)
        {
            m_secondItem=t;
        }
    }

	for(int i=0;i<3;i++)
    {
        wxTreeItemId t = m_treeCtrl1->AppendItem(m_secondItem,wxString::Format("item 1-%d",i));
        m_treeCtrl1->SetItemState(t, 0);
    }


//	wxTreeItemId t = m_treeCtrl1->AppendItem(root,"item 1");
//
//	m_treeCtrl1->AppendItem(root,"item 2");
//	m_treeCtrl1->AppendItem(root,"item 3");
//	m_treeCtrl1->AppendItem(root,"item 4");

	//but->Bind(wxEVT_COMMAND_BUTTON_CLICKED,&checktreeFrame::OnB1,this);
	m_treeCtrl1->Bind( wxEVT_LEFT_DOWN, &checktreeFrame::On_Left_Down, this );
	m_treeCtrl1->Bind( wxEVT_LEFT_UP, &checktreeFrame::On_Left_Up, this );
	m_treeCtrl1->Bind( wxEVT_LEFT_DCLICK, &checktreeFrame::On_Left_DClick, this );
	m_treeCtrl1->Bind( wxEVT_CHECKTREE_CHOICE, &checktreeFrame::OnCheckTreeChoice, this );



}

checktreeFrame::~checktreeFrame()
{
}

void checktreeFrame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void checktreeFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void checktreeFrame::OnAbout(wxCommandEvent &event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void checktreeFrame::On_Left_Down( wxMouseEvent& event )
{
    (*m_textCtrl1) << "On_Left_Down\n";
    event.Skip();
}

void checktreeFrame::On_Left_Up( wxMouseEvent& event )
{
    (*m_textCtrl1) << "On_Left_Up\n";
    event.Skip();
}

void checktreeFrame::On_Left_DClick( wxMouseEvent& event )
{
    (*m_textCtrl1) << "On_Left_DClick\n";
    event.Skip();
}

void checktreeFrame::OnB1(wxCommandEvent& event)
{
//    (*m_textCtrl1) << "onb2\n";
//
//    wxRect r;
//    m_treeCtrl1->GetBoundingRect( m_treeCtrl1->GetRootItem(),r);
//
//    (*m_textCtrl1) << r.GetHeight()<< "\n";
//
//    wxDelegateRendererNative rn = wxRendererNative::Get();
//    wxSize sz(rn.GetCheckBoxSize(this));
//
//    (*m_textCtrl1) << sz.GetHeight()<< "\n";

    m_treeCtrl1->EnableCheckBox(m_secondItem);

}

void checktreeFrame::OnB2(wxCommandEvent& event)
{
    m_treeCtrl1->DisableCheckBox(m_secondItem);
}

void checktreeFrame::OnB3(wxCommandEvent& event)
{
    (*m_textCtrl1) << (m_treeCtrl1->GetCheckBoxValue(m_secondItem)?"true\n":"false\n");
}

void checktreeFrame::OnB4(wxCommandEvent& event)
{
    m_treeCtrl1->SetCheckBoxValue(m_secondItem,true);
}

void checktreeFrame::OnB5(wxCommandEvent& event)
{
    m_treeCtrl1->SetCheckBoxValue(m_secondItem,false);
}

void checktreeFrame::OnB6(wxCommandEvent& event)
{
    m_treeCtrl1->RemoveCheckbox(m_secondItem);
}

void checktreeFrame::OnCheckTreeChoice(wxTreeEvent& event)
{
    wxString s = m_treeCtrl1->GetItemText(event.GetItem());
    bool b = event.GetExtraLong()?true:false;

    (*m_textCtrl1) << s << " set " << (b?"true\n":"false\n");
    //(*m_textCtrl1) << (m_treeCtrl1->GetCheckBoxValue(m_secondItem)?"true\n":"false\n");
}
