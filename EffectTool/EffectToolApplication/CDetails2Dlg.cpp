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
	, size_min_x(25)
	, size_min_y(25)
	, size_max_x(25)
	, size_max_y(25)
{

}

CDetails2Dlg::~CDetails2Dlg()
{
}

void CDetails2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SPAWNRATE, spawn_rate_);
	DDX_Text(pDX, IDC_SIZE_MIN_X, size_min_x);
	DDX_Text(pDX, IDC_SIZE_MIN_Y, size_min_y);
	DDX_Text(pDX, IDC_SIZE_MAX_X, size_max_x);
	DDX_Text(pDX, IDC_SIZE_MAX_Y, size_max_y);
}


BEGIN_MESSAGE_MAP(CDetails2Dlg, CDialogEx)
	ON_EN_KILLFOCUS(IDC_SPAWNRATE, &CDetails2Dlg::OnEnKillfocusSpawnrate)
	ON_WM_KILLFOCUS()
	ON_EN_KILLFOCUS(IDC_SIZE_MIN_X, &CDetails2Dlg::OnEnKillfocusSizeMinX)
	ON_EN_KILLFOCUS(IDC_SIZE_MIN_Y, &CDetails2Dlg::OnEnKillfocusSizeMinY)
	ON_EN_KILLFOCUS(IDC_SIZE_MAX_X, &CDetails2Dlg::OnEnKillfocusSizeMaxX)
	ON_EN_KILLFOCUS(IDC_SIZE_MAX_Y, &CDetails2Dlg::OnEnKillfocusSizeMaxY)
END_MESSAGE_MAP()


// CDetails2Dlg message handlers


void CDetails2Dlg::OnEnKillfocusSpawnrate()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CEffectToolApplicationApp* app = (CEffectToolApplicationApp*)AfxGetApp();
	app->effect_tool_.SetSpawnRate(L"/default/", spawn_rate_);
}


BOOL CDetails2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	//SetDlgItemInt(IDC_SIZE_MIN_X, 25);
	//SetDlgItemInt(IDC_SIZE_MIN_Y, 25);
	//SetDlgItemInt(IDC_SIZE_MAX_X, 25);
	//SetDlgItemInt(IDC_SIZE_MAX_Y, 25);
	//UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDetails2Dlg::OnKillFocus(CWnd* pNewWnd)
{
	CDialogEx::OnKillFocus(pNewWnd);	
}


void CDetails2Dlg::OnEnKillfocusSizeMinX()
{
	//TODO errorcheck
	UpdateData(TRUE);
	CEffectToolApplicationApp * app = (CEffectToolApplicationApp*)AfxGetApp();
	app->effect_tool_.UpdateSizeOffset(L"/default/", { size_min_x, size_min_y }, {size_max_x, size_max_y});
}


void CDetails2Dlg::OnEnKillfocusSizeMinY()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CEffectToolApplicationApp* app = (CEffectToolApplicationApp*)AfxGetApp();
	app->effect_tool_.UpdateSizeOffset(L"/default/", { size_min_x, size_min_y }, { size_max_x, size_max_y });
}


void CDetails2Dlg::OnEnKillfocusSizeMaxX()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CEffectToolApplicationApp* app = (CEffectToolApplicationApp*)AfxGetApp();
	app->effect_tool_.UpdateSizeOffset(L"/default/", { size_min_x, size_min_y }, { size_max_x, size_max_y });
}


void CDetails2Dlg::OnEnKillfocusSizeMaxY()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CEffectToolApplicationApp* app = (CEffectToolApplicationApp*)AfxGetApp();
	app->effect_tool_.UpdateSizeOffset(L"/default/", { size_min_x, size_min_y }, { size_max_x, size_max_y });
}
