#include "pch.h"
#include "CDockPaneExp.h"
#include "resource.h"
BEGIN_MESSAGE_MAP(CDockPaneExp, CDockablePane)
	ON_WM_PAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()

int CDockPaneExp::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	emitter_settings_dlg_.Create(IDD_EMITTER_SETTER, this);
	emitter_settings_dlg_.ShowWindow(SW_SHOW);

	return 0;
}
