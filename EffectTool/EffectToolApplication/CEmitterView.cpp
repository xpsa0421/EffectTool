#include "pch.h"
#include "CEmitterView.h"
#include "resource.h"

BEGIN_MESSAGE_MAP(CEmitterView, CDockablePane)
	ON_WM_CREATE()
END_MESSAGE_MAP()


int CEmitterView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	emitter_list_dlg_.Create(IDD_EMITTERLIST_VIEW, this);
	emitter_list_dlg_.ShowWindow(SW_SHOW);

	return 0;
}
