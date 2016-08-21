/////////////////////////////////////////////////////////////////////////////
// Name:        wx/checktree/checktree.h
// Purpose:     wxCheckTree class
// Author:
// Modified by:
// Created:     2016/07/10
// Copyright:
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_CHECKTREE_CHECKTREE_H_
#define _WX_CHECKTREE_CHECKTREE_H_

#include <wx/treectrl.h>

#ifdef WXUSING_CHECKTREESOURCE
    #define WXDLLIMPEXP_CHECKTREE
#elif WXMAKINGDLL_CHECKTREE
    #define WXDLLIMPEXP_CHECKTREE WXEXPORT
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_CHECKTREE WXIMPORT
#else // not making nor using DLL
    #define WXDLLIMPEXP_CHECKTREE
#endif

wxDECLARE_EXPORTED_EVENT(WXDLLIMPEXP_CHECKTREE, wxEVT_CHECKTREE_CHOICE, wxTreeEvent);
const wxString wxCheckTreeNameStr = "wxCheckTree";

class WXDLLIMPEXP_CHECKTREE wxCheckTree : public wxTreeCtrl
{
    public:
        wxCheckTree(wxWindow *parent, wxWindowID id=wxID_ANY,
                    const wxPoint &pos=wxDefaultPosition,
                    const wxSize &size=wxDefaultSize,
                    long style=wxTR_DEFAULT_STYLE,
                    const wxValidator &validator=wxDefaultValidator,
                    const wxString &name=wxCheckTreeNameStr);
        wxCheckTree();
        bool Create(wxWindow *parent, wxWindowID id=wxID_ANY,
                    const wxPoint &pos=wxDefaultPosition,
                    const wxSize &size=wxDefaultSize,
                    long style=wxTR_DEFAULT_STYLE,
                    const wxValidator &validator=wxDefaultValidator,
                    const wxString &name=wxCheckTreeNameStr);
        virtual ~wxCheckTree();

        //interaction with the check boxes:
        void EnableCheckBox(const wxTreeItemId &item, bool enable = true );
        void DisableCheckBox(const wxTreeItemId &item);
        bool GetCheckBoxValue(const wxTreeItemId &item);
        void SetCheckBoxValue(const wxTreeItemId &item,bool value=true);
        void RemoveCheckbox(const wxTreeItemId &item);

    protected:
        //helpers
        void Init();
        void MakeLastItemUnhot();
        bool OnCheckOrLabel(int flags);

    private:
        //event handlers
        void On_Mouse_Leave_Tree( wxMouseEvent& event );
        void On_Left_Down( wxMouseEvent& event );
        void On_Left_Up( wxMouseEvent& event );
        void On_Mouse_Motion( wxMouseEvent& event );

        //privated data:
		enum
		{
		    wxCHECKTREE_STATE_NONE=-1,
		    wxCHECKTREE_STATE_UNCHECKED,
		    wxCHECKTREE_STATE_UNCHECKED_CURRENT,
		    wxCHECKTREE_STATE_UNCHECKED_PRESSED,
		    wxCHECKTREE_STATE_UNCHECKED_DISABLED,
            wxCHECKTREE_STATE_CHECKED,
		    wxCHECKTREE_STATE_CHECKED_CURRENT,
		    wxCHECKTREE_STATE_CHECKED_PRESSED,
		    wxCHECKTREE_STATE_CHECKED_DISABLED,
		    wxCHECKTREE_STATE_MAX
		};

        wxTreeItemId m_lastId;
};

#endif // _WX_CHECKTREE_CHECKTREE_H_
