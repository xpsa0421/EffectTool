#pragma once
#include <afxdockablepane.h>
#include "CShaderSetDlg.h"

class CShaderSettingPane :
    public CDockablePane
{
public:
    CShaderSetDlg   shader_set_dlg_;
    DECLARE_MESSAGE_MAP()
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

