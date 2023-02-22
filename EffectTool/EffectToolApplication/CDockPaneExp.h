#pragma once
#include <afxdockablepane.h>
class CEmitterSettingsDlg;


class CDockPaneExp : public CDockablePane
{
public:
	~CDockPaneExp() {};
public:
	CEmitterSettingsDlg* required_dlg_;
public:
	DECLARE_MESSAGE_MAP();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void PostNcDestroy();
};

