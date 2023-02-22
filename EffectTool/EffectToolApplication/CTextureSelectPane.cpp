#include "pch.h"
#include "CTextureSelectPane.h"
#include "resource.h"
#include "CTextureSelectDlg.h"
BEGIN_MESSAGE_MAP(CTextureSelectPane, CDockablePane)
	ON_WM_CREATE()
END_MESSAGE_MAP()


int CTextureSelectPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	texture_dlg = new CTextureSelectDlg();
	texture_dlg->Create(IDD_TEXTURE, this);
	texture_dlg->ShowWindow(SW_SHOW);
	// TODO:  Add your specialized creation code here

	return 0;
}


void CTextureSelectPane::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete texture_dlg;
	CDockablePane::PostNcDestroy();
}
