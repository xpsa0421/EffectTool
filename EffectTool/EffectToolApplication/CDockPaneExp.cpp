#include "pch.h"
#include "CDockPaneExp.h"
#include "resource.h"
#include "CEmitterSettingsDlg.h"
BEGIN_MESSAGE_MAP(CDockPaneExp, CDockablePane)
	ON_WM_PAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()

int CDockPaneExp::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	required_dlg_ = new CEmitterSettingsDlg();
	required_dlg_->Create(IDD_EMITTER_SETTER, this);
	required_dlg_->ShowWindow(SW_SHOW);

	


	return 0;
}


void CDockPaneExp::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete required_dlg_;
	CDockablePane::PostNcDestroy();
}
