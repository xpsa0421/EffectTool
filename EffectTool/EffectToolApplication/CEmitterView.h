#pragma once
#include <afxdockablepane.h>
#include "CEmitterListViewDlg.h"

class CEmitterView :
    public CDockablePane
{
public:
    CEmitterListViewDlg emitter_list_dlg_;

    DECLARE_MESSAGE_MAP()
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

