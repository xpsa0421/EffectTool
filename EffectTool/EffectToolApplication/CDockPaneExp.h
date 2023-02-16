#pragma once
#include <afxdockablepane.h>
#include "CEmitterSettingsDlg.h"

class CDockPaneExp : public CDockablePane
{
public:
	~CDockPaneExp() {};
public:
	CEmitterSettingsDlg emitter_settings_dlg_;
public:
	DECLARE_MESSAGE_MAP();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

