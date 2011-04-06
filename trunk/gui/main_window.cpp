///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "main_window.h"

///////////////////////////////////////////////////////////////////////////

main_window::main_window( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	m_menubar1 = new wxMenuBar( 0 );
	m_menubar1->SetForegroundColour( wxColour( 212, 208, 200 ) );
	m_menubar1->SetBackgroundColour( wxColour( 212, 208, 200 ) );

	this->SetMenuBar( m_menubar1 );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );

	m_bpButton1 = new wxBitmapButton( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	bSizer10->Add( m_bpButton1, 0, wxALL, 5 );

	m_bpButton2 = new wxBitmapButton( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	bSizer10->Add( m_bpButton2, 0, wxALL, 5 );

	m_bpButton3 = new wxBitmapButton( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	bSizer10->Add( m_bpButton3, 0, wxALL, 5 );

	bSizer8->Add( bSizer10, 0, wxEXPAND, 5 );

	m_found = new wxStaticText( this, wxID_ANY, wxT("Found: 0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_found->Wrap( -1 );
	bSizer8->Add( m_found, 0, wxALL, 5 );

	bSizer7->Add( bSizer8, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );

	m_process_selected = new wxStaticText( this, wxID_ANY, wxT("No process selected"), wxDefaultPosition, wxDefaultSize, 0 );
	m_process_selected->Wrap( -1 );
	bSizer9->Add( m_process_selected, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	m_gauge1 = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
	bSizer9->Add( m_gauge1, 0, wxALL|wxEXPAND, 5 );

	bSizer7->Add( bSizer9, 1, wxEXPAND, 5 );

	m_bitmap1 = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( m_bitmap1, 0, wxALL, 5 );

	bSizer2->Add( bSizer7, 1, wxEXPAND, 5 );

	bSizer1->Add( bSizer2, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );

	m_button1 = new wxButton( this, wxID_ANY, wxT("First Scan"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button1, 0, wxALL, 5 );

	m_button2 = new wxButton( this, wxID_ANY, wxT("Next Scan"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button2, 0, wxALL, 5 );


	bSizer3->Add( 0, 0, 1, wxEXPAND, 5 );

	m_button3 = new wxButton( this, wxID_ANY, wxT("Undo Scan"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button3, 0, wxALL, 5 );

	m_button4 = new wxButton( this, wxID_ANY, wxT("settings"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button4, 0, wxALL, 5 );

	bSizer1->Add( bSizer3, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );

	m_listCtrl1 = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_ICON|wxLC_REPORT );
	bSizer4->Add( m_listCtrl1, 1, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxHORIZONTAL );

	m_checkBox1 = new wxCheckBox( this, wxID_ANY, wxT("Hex"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_checkBox1, 0, wxALL, 5 );

	m_textCtrl1 = new wxTextCtrl( this, wxID_ANY, wxT("insert value here"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_textCtrl1, 1, wxALL, 5 );

	bSizer12->Add( bSizer13, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Scan type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer14->Add( m_staticText3, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxArrayString m_choice1Choices;
	m_choice1 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice1Choices, 0 );
	m_choice1->SetSelection( 0 );
	bSizer14->Add( m_choice1, 1, wxALL, 5 );

	m_checkBox2 = new wxCheckBox( this, wxID_ANY, wxT("Speedhack"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer14->Add( m_checkBox2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	bSizer12->Add( bSizer14, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText4 = new wxStaticText( this, wxID_ANY, wxT("Value type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	bSizer15->Add( m_staticText4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxArrayString m_choice2Choices;
	m_choice2 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice2Choices, 0 );
	m_choice2->SetSelection( 0 );
	bSizer15->Add( m_choice2, 1, wxALL, 5 );

	m_checkBox3 = new wxCheckBox( this, wxID_ANY, wxT("Unrandomize"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15->Add( m_checkBox3, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	bSizer12->Add( bSizer15, 0, wxEXPAND, 5 );

	bSizer4->Add( bSizer12, 0, wxEXPAND, 5 );

	bSizer1->Add( bSizer4, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );

	m_button5 = new wxButton( this, wxID_ANY, wxT("Memory view"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_button5, 0, wxALL, 5 );


	bSizer5->Add( 0, 0, 1, wxEXPAND, 5 );

	m_bpButton4 = new wxBitmapButton( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	bSizer5->Add( m_bpButton4, 0, wxALL, 5 );

	m_bpButton5 = new wxBitmapButton( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	bSizer5->Add( m_bpButton5, 0, wxALL, 5 );


	bSizer5->Add( 0, 0, 1, wxEXPAND, 5 );

	m_button6 = new wxButton( this, wxID_ANY, wxT("Add address manually"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_button6, 0, wxALL, 5 );

	bSizer1->Add( bSizer5, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );

	m_listCtrl2 = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_ICON|wxLC_REPORT );
	bSizer6->Add( m_listCtrl2, 1, wxALL|wxEXPAND, 5 );

	bSizer1->Add( bSizer6, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxHORIZONTAL );

	m_button7 = new wxButton( this, wxID_ANY, wxT("Advanced options"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( m_button7, 0, wxALL, 5 );


	bSizer16->Add( 0, 0, 1, wxEXPAND, 5 );

	m_button8 = new wxButton( this, wxID_ANY, wxT("Table extras"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( m_button8, 0, wxALL, 5 );

	bSizer1->Add( bSizer16, 0, wxEXPAND, 5 );

	this->SetSizer( bSizer1 );
	this->Layout();
}

main_window::~main_window()
{
}
