/***************************************************************
 * Name:      mathomatic_guiApp.cpp
 * Purpose:   Code for Application Class
 * Author:    lilEzek (lilezek@gmail.com)
 * Created:   2011-02-07
 * Copyright: lilEzek (www.lilezek.game-hunters.com)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "xeatengine_gui.h"
#include "main_window.h"

bool MAIN(main_window * win);

bool xeatengine_guiApp::OnInit()
{
    main_window * win = new main_window(0L);
    return MAIN(win);
}

IMPLEMENT_APP(xeatengine_guiApp);
