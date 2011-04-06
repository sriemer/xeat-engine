///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __main_window__
#define __main_window__

#include <wx/string.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/gauge.h>
#include <wx/statbmp.h>
#include <wx/listctrl.h>
#include <wx/checkbox.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class main_window
///////////////////////////////////////////////////////////////////////////////
class main_window : public wxFrame
{
	private:

	protected:
		wxMenuBar* m_menubar1;
		wxBitmapButton* m_bpButton1;
		wxBitmapButton* m_bpButton2;
		wxBitmapButton* m_bpButton3;
		wxStaticText* m_found;
		wxStaticText* m_process_selected;
		wxGauge* m_gauge1;
		wxStaticBitmap* m_bitmap1;
		wxButton* m_button1;
		wxButton* m_button2;

		wxButton* m_button3;
		wxButton* m_button4;
		wxListCtrl* m_listCtrl1;
		wxCheckBox* m_checkBox1;
		wxTextCtrl* m_textCtrl1;
		wxStaticText* m_staticText3;
		wxChoice* m_choice1;
		wxCheckBox* m_checkBox2;
		wxStaticText* m_staticText4;
		wxChoice* m_choice2;
		wxCheckBox* m_checkBox3;
		wxButton* m_button5;

		wxBitmapButton* m_bpButton4;
		wxBitmapButton* m_bpButton5;

		wxButton* m_button6;
		wxListCtrl* m_listCtrl2;
		wxButton* m_button7;

		wxButton* m_button8;

	public:

		main_window( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Xeat Engine V0.10"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 526,545 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~main_window();

};

#endif //__main_window__
