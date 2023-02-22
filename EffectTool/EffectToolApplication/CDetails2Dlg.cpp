// CDetails2Dlg.cpp : implementation file
//

#include "pch.h"
#include "EffectToolApplication.h"
#include "CDetails2Dlg.h"
#include "afxdialogex.h"
#include "resource.h"


// CDetails2Dlg dialog

IMPLEMENT_DYNAMIC(CDetails2Dlg, CDialogEx)

CDetails2Dlg::CDetails2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, spawn_rate_(20)
{

}

CDetails2Dlg::~CDetails2Dlg()
{
}

void CDetails2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SPAWNRATE, spawn_rate_);
}


BEGIN_MESSAGE_MAP(CDetails2Dlg, CDialogEx)
	ON_EN_KILLFOCUS(IDC_SPAWNRATE, &CDetails2Dlg::OnEnKillfocusSpawnrate)
END_MESSAGE_MAP()


// CDetails2Dlg message handlers


void CDetails2Dlg::OnEnKillfocusSpawnrate()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CEffectToolApplicationApp* app = (CEffectToolApplicationApp*)AfxGetApp();
	app->effect_tool_.SetSpawnRate(L"/default/", spawn_rate_);
}
