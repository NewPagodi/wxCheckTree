/////////////////////////////////////////////////////////////////////////////
// Name:        src/checktree.cpp
// Purpose:     wxCheckTree
// Author:
// Modified by:
// Created:     2016/07/10
// Copyright:
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

//The goal is to emulate checkboxes with the state icons in the tree control.
//The logic is more complicated that it might seem should be needed, but 90% of
//the complication arises from trying to match what happens when you press and
//hold the mouse button over a checkbox and then move to another checkbox
//or control.  Keeping track of what should be highlighted and in what
//state turns out to be a bit of a pain.  I think I've got everything emulated
//though.


// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/dcmemory.h>
    #include <wx/window.h>
    #include <wx/imaglist.h>
#endif

#include <wx/renderer.h>
#include <wx/checktree/checktree.h>

wxDEFINE_EVENT(wxEVT_CHECKTREE_CHOICE, wxTreeEvent);

wxCheckTree::wxCheckTree(wxWindow *parent, wxWindowID id, const wxPoint &pos,
                        const wxSize &size, long style,
                        const wxValidator &validator, const wxString &name)
{
    Create(parent, id, pos, size, style, validator, name);
}

wxCheckTree::wxCheckTree()
{
}

bool wxCheckTree::Create(wxWindow *parent, wxWindowID id, const wxPoint &pos,
                        const wxSize &size, long style,
                        const wxValidator &validator, const wxString &name)
{
    if(!wxTreeCtrl::Create(parent, id, pos, size, style, validator, name))
    {
        return false;
    }

    Init();
    return true;
}

void wxCheckTree::Init()
{
    //We need to know how tall a typical item is, so we can add padding to the
    //checkbox bitmaps to keep them centered.  I'll also add 1 pixel of padding
    //between the edge of the icon and the start of the label text.
    AddRoot("");
    wxRect br;
    GetBoundingRect(GetRootItem(),br);
    DeleteAllItems();

    wxDelegateRendererNative rn = wxRendererNative::Get();
    wxSize sz(rn.GetCheckBoxSize(this));
    wxRect rect(sz);
    int h=sz.GetHeight();
    int w=sz.GetWidth();
    int padding=(br.GetHeight()-h)/2;
    if(padding>0)
    {
        h+=padding;
    }
    w++;

    wxBitmap CheckBoxPlain(w,h);
    wxBitmap CheckBoxPlainHot(w,h);
    wxBitmap CheckBoxPlainPressed(w,h);
    wxBitmap CheckBoxPlainDisabled(w,h);
    wxBitmap CheckBoxChecked(w,h);
    wxBitmap CheckBoxCheckedHot(w,h);
    wxBitmap CheckBoxCheckedPressed(w,h);
    wxBitmap CheckBoxCheckedDisabled(w,h);

    wxMemoryDC mem;
    //we need a mask colour.  This is a gold colour picked at random that
    //hopefully doesn't appear in any of the bitmaps.
    wxColour maskcolor(232,226,93);
    mem.SetBackground(maskcolor);

    mem.SelectObject(CheckBoxPlain);
    mem.Clear();
    rn.DrawCheckBox(this, mem, rect, wxCONTROL_NONE );

    mem.SelectObject(CheckBoxPlainHot);
    mem.Clear();
    rn.DrawCheckBox(this, mem, rect, wxCONTROL_CURRENT );

    mem.SelectObject(CheckBoxPlainPressed);
    mem.Clear();
    rn.DrawCheckBox(this, mem, rect, wxCONTROL_PRESSED );

    mem.SelectObject(CheckBoxPlainDisabled);
    mem.Clear();
    rn.DrawCheckBox(this, mem, rect, wxCONTROL_DISABLED );


    mem.SelectObject(CheckBoxChecked);
    mem.Clear();
    rn.DrawCheckBox(this, mem, rect, wxCONTROL_CHECKED );

    mem.SelectObject(CheckBoxCheckedHot);
    mem.Clear();
    rn.DrawCheckBox(this, mem, rect, wxCONTROL_CHECKED|wxCONTROL_CURRENT );

    mem.SelectObject(CheckBoxCheckedPressed);
    mem.Clear();
    rn.DrawCheckBox(this, mem, rect, wxCONTROL_CHECKED|wxCONTROL_PRESSED );

    mem.SelectObject(CheckBoxCheckedDisabled);
    mem.Clear();
    rn.DrawCheckBox(this, mem, rect, wxCONTROL_CHECKED|wxCONTROL_DISABLED );

    mem.SelectObject(wxNullBitmap);

    wxImageList *states = new wxImageList(w, h, true);

    states->Add(CheckBoxPlain,maskcolor);
    states->Add(CheckBoxPlainHot,maskcolor);
    states->Add(CheckBoxPlainPressed,maskcolor);
    states->Add(CheckBoxPlainDisabled,maskcolor);
    states->Add(CheckBoxChecked,maskcolor);
    states->Add(CheckBoxCheckedHot,maskcolor);
    states->Add(CheckBoxCheckedPressed,maskcolor);
    states->Add(CheckBoxCheckedDisabled,maskcolor);

    AssignStateImageList(states);

	Bind( wxEVT_LEFT_DOWN, &wxCheckTree::On_Left_Down, this );
	Bind( wxEVT_LEFT_UP, &wxCheckTree::On_Left_Up, this );
	Bind( wxEVT_LEFT_DCLICK, &wxCheckTree::On_Left_Down, this );
	Bind( wxEVT_LEAVE_WINDOW, &wxCheckTree::On_Mouse_Leave_Tree, this );
	Bind( wxEVT_MOTION, &wxCheckTree::On_Mouse_Motion, this );
}

wxCheckTree::~wxCheckTree()
{
}

void wxCheckTree::EnableCheckBox(const wxTreeItemId &item, bool enable )
{
    if(item.IsOk())
    {
        int state = GetItemState(item);
        bool unchecked = state<wxCHECKTREE_STATE_CHECKED;

        if(enable)
        {
            SetItemState(item,unchecked?wxCHECKTREE_STATE_UNCHECKED:wxCHECKTREE_STATE_CHECKED);
        }
        else
        {
            SetItemState(item,unchecked?wxCHECKTREE_STATE_UNCHECKED_DISABLED:wxCHECKTREE_STATE_CHECKED_DISABLED);
        }
    }
}

void wxCheckTree::DisableCheckBox(const wxTreeItemId &item)
{
    if( item.IsOk() )
    {
        int state = GetItemState(item);

        SetItemState(item,state<wxCHECKTREE_STATE_CHECKED?wxCHECKTREE_STATE_UNCHECKED_DISABLED:wxCHECKTREE_STATE_CHECKED_DISABLED);
    }
}

bool wxCheckTree::GetCheckBoxValue(const wxTreeItemId &item)
{
    if( item.IsOk() )
    {
        int state = GetItemState(item);
        return wxCHECKTREE_STATE_CHECKED<=state && state<wxCHECKTREE_STATE_MAX;
    }
    else
    {
        return false;
    }
}

void wxCheckTree::SetCheckBoxValue(const wxTreeItemId &item,bool value)
{
    if(item.IsOk())
    {
        int state=GetItemState(item);

        if( state==wxCHECKTREE_STATE_UNCHECKED_DISABLED || state==wxCHECKTREE_STATE_CHECKED_DISABLED )
        {
            SetItemState(item,value?wxCHECKTREE_STATE_CHECKED_DISABLED:wxCHECKTREE_STATE_UNCHECKED_DISABLED);
        }
        else
        {
            SetItemState(item,value?wxCHECKTREE_STATE_CHECKED:wxCHECKTREE_STATE_UNCHECKED);
        }
    }
}

void wxCheckTree::RemoveCheckbox(const wxTreeItemId &item)
{
    if( item.IsOk() )
    {
        SetItemState(item,wxCHECKTREE_STATE_NONE);
    }
}

void wxCheckTree::MakeLastItemUnhot()
{
    if( !m_lastId.IsOk() )
    {
        return;
    }

    int state=GetItemState(m_lastId);

    if( state==wxCHECKTREE_STATE_UNCHECKED_CURRENT )
    {
        SetItemState(m_lastId,wxCHECKTREE_STATE_UNCHECKED);
    }
    else if( state==wxCHECKTREE_STATE_CHECKED_CURRENT )
    {
        SetItemState(m_lastId,wxCHECKTREE_STATE_CHECKED);
    }
}

bool wxCheckTree::OnCheckOrLabel(int flags)
{
    return flags&(wxTREE_HITTEST_ONITEMSTATEICON|wxTREE_HITTEST_ONITEMLABEL)?true:false;
}

void wxCheckTree::On_Mouse_Leave_Tree( wxMouseEvent& event )
{
    //if the items run to the edge of the tree, it's possible to leave
    //the tree without getting the motion event that would tell us to
    //set the item back to its plane state.

    if( event.LeftIsDown() )
    {
        if( m_lastId.IsOk() && !HasCapture() )
        {
            MakeLastItemUnhot();
            m_lastId=wxTreeItemId();
        }
    }
    else
    {
        MakeLastItemUnhot();
        m_lastId=wxTreeItemId();
    }

    event.Skip();
}

void wxCheckTree::On_Left_Up( wxMouseEvent& event )
{
    int flags;
    wxTreeItemId id = HitTest( event.GetPosition(), flags );

    if( HasCapture() && id==m_lastId )
    {
        //if we have capture, then we started with a good item, so if the old
        //and new ids match and we're on the check or label of the item,
        //toggle the item and throw a wxEVT_CHECKTREE_CHOICE event.

        if( OnCheckOrLabel(flags) )
        {
            int state=GetItemState(id);
            bool extralong(0);

            if( state==wxCHECKTREE_STATE_UNCHECKED_PRESSED )
            {
                SetItemState(id,wxCHECKTREE_STATE_CHECKED_CURRENT);
                extralong=1;
            }
            else if( state==wxCHECKTREE_STATE_CHECKED_PRESSED )
            {
                SetItemState(id,wxCHECKTREE_STATE_UNCHECKED_CURRENT);
            }

            wxTreeEvent event(wxEVT_CHECKTREE_CHOICE,this,id);
            event.SetExtraLong(extralong);
            ProcessWindowEvent(event);
        }
    }
    else
    {
        //if the ids dont match, then we're not going to toggle the item. set
        //the old item plain if it exists and the new item hot if it exists
        MakeLastItemUnhot();

        if( id.IsOk() && OnCheckOrLabel(flags) )
        {
            int state=GetItemState(id);

            if( state==wxCHECKTREE_STATE_UNCHECKED )
            {
                SetItemState(id,wxCHECKTREE_STATE_UNCHECKED_CURRENT);
            }
            else if( state==wxCHECKTREE_STATE_CHECKED )
            {
                SetItemState(id,wxCHECKTREE_STATE_CHECKED_CURRENT);
            }
        }
    }

    m_lastId=id;
    if(HasCapture())
    {
        ReleaseMouse();
    }

    event.Skip();
}

void wxCheckTree::On_Left_Down( wxMouseEvent& event )
{
    //for some reason, if we skip this, the left up event isn't sent.  However,
    //if we're on the toggle button and we don't skip it, the tree won't expand/
    //collapse.  So I'll skip it in only that circumstance.

    //since we're not skipping this event, the item under the cursor is not
    //set focused.  We could get around that by manually calling SelectItem(m_lastId);
    //but I like not changing the focus - it makes things look more like
    //ordinary check boxes when they're not highlighted in blue.

    //I'm not sure what else we lose by not skipping this, but it doesn't SEEM
    //to be anything important.

    int flags;
    wxTreeItemId m_lastId = HitTest( event.GetPosition(), flags );

    if(flags&wxTREE_HITTEST_ONITEMBUTTON)
    {
        m_lastId=wxTreeItemId();
        event.Skip();
    }
    else if( !m_lastId.IsOk() || !OnCheckOrLabel(flags) )
    {
        //If we're not over an item's label/check, set m_lastId invalid
        m_lastId=wxTreeItemId();
    }
    else
    {
        //otherwise, capture the mouse so we'll receive the left up event even
        //if the mouse leaves the tree window, and toggle the check box to the
        //pressed state.

        CaptureMouse();
        int state = GetItemState(m_lastId);

        if( state==wxCHECKTREE_STATE_UNCHECKED || state==wxCHECKTREE_STATE_UNCHECKED_CURRENT )
        {
            SetItemState(m_lastId,wxCHECKTREE_STATE_UNCHECKED_PRESSED);
        }
        else if( state==wxCHECKTREE_STATE_CHECKED || state==wxCHECKTREE_STATE_CHECKED_CURRENT )
        {
            SetItemState(m_lastId,wxCHECKTREE_STATE_CHECKED_PRESSED);
        }
    }

    SetFocus();
}

void wxCheckTree::On_Mouse_Motion( wxMouseEvent& event )
{
    int flags;
    wxTreeItemId id = HitTest( event.GetPosition(), flags );

    if( event.LeftIsDown() && HasCapture() )
    {
        //if the tree has captured the mouse, that means there was
        //an actual item selected in the left down event handler.  We
        //need to toggle it between the pressed and hot state depending
        //on where the mouse is

        int state2=GetItemState(m_lastId);

        if( !OnCheckOrLabel(flags) || id!=m_lastId )
        {
            if( state2==wxCHECKTREE_STATE_UNCHECKED_PRESSED )
            {
                SetItemState(m_lastId,wxCHECKTREE_STATE_UNCHECKED_CURRENT);
            }
            else if(state2==wxCHECKTREE_STATE_CHECKED_PRESSED)
            {
                SetItemState(m_lastId,wxCHECKTREE_STATE_CHECKED_CURRENT);
            }
        }
        else
        {
            if( state2==wxCHECKTREE_STATE_UNCHECKED_CURRENT )
            {
                SetItemState(m_lastId,wxCHECKTREE_STATE_UNCHECKED_PRESSED);
            }
            else if( state2==wxCHECKTREE_STATE_CHECKED_CURRENT )
            {
                SetItemState(m_lastId,wxCHECKTREE_STATE_CHECKED_PRESSED);
            }
        }
    }
    else
    {
        //otherwise, we're moving the mouse on the tree with or without the
        //left button down.  As the mouse moves from one item to another, the
        //items under the cursor should be drawn hot and the old item set plain.

        if( !id.IsOk() || !OnCheckOrLabel(flags) )
        {
            MakeLastItemUnhot();
        }
        else
        {
            if(id!=m_lastId)
            {
                MakeLastItemUnhot();
            }

            int state=GetItemState(id);

            if( state==wxCHECKTREE_STATE_UNCHECKED )
            {
                SetItemState(id,wxCHECKTREE_STATE_UNCHECKED_CURRENT);
            }
            else if( state==wxCHECKTREE_STATE_CHECKED )
            {
                SetItemState(id,wxCHECKTREE_STATE_CHECKED_CURRENT);
            }
        }

        m_lastId=id;
    }

    event.Skip();
}
