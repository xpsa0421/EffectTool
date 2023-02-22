#pragma once
#include <afxdockablepane.h>
class CDetails2Dlg;

class CDetailsSpawnPane :
    public CDockablePane
{
public:
	~CDetailsSpawnPane() {};
public:
	CDetails2Dlg* spawn_dlg;
	DECLARE_MESSAGE_MAP();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void PostNcDestroy();
};

