#include "pch.h"
#include "CDetailsSpawnPane.h"
#include "resource.h"
#include "CDetails2Dlg.h"

BEGIN_MESSAGE_MAP(CDetailsSpawnPane, CDockablePane)
	ON_WM_PAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()

int CDetailsSpawnPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	spawn_dlg = new CDetails2Dlg();
	spawn_dlg->Create(IDD_DIALOG2, this);
	spawn_dlg->ShowWindow(SW_SHOW);




	return 0;
}


void CDetailsSpawnPane::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete spawn_dlg;
	CDockablePane::PostNcDestroy();
}
