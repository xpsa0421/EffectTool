#include "pch.h"
#include "CShaderSettingPane.h"
#include "resource.h"

BEGIN_MESSAGE_MAP(CShaderSettingPane, CDockablePane)
	ON_WM_CREATE()
END_MESSAGE_MAP()


int CShaderSettingPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	shader_set_dlg_.Create(IDD_SHADER, this);
	shader_set_dlg_.ShowWindow(SW_SHOW);
	// TODO:  Add your specialized creation code here

	return 0;
}
