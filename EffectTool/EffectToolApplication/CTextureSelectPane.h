#pragma once
#include <afxdockablepane.h>

class CTextureSelectDlg;

class CTextureSelectPane :
    public CDockablePane
{
public:
    CTextureSelectDlg* texture_dlg;
public:
    DECLARE_MESSAGE_MAP()
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    virtual void PostNcDestroy();
};

