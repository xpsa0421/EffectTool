// CShaderSetDlg.cpp : implementation file
//

#include "pch.h"
#include "EffectToolApplication.h"
#include "CShaderSetDlg.h"
#include "afxdialogex.h"
#include "resource.h"

// CShaderSetDlg dialog

IMPLEMENT_DYNAMIC(CShaderSetDlg, CDialogEx)

CShaderSetDlg::CShaderSetDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SHADER, pParent)
	, depth_write_enabled(FALSE)
	, depth_compared(FALSE)
	, alpha_tested(FALSE)
	, alpha_blended(FALSE)
	, wireframe_enabled(FALSE)
	, is_dual_blended(FALSE)
{

}

CShaderSetDlg::~CShaderSetDlg()
{
}

void CShaderSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_DB_WRITE, depth_enable_button);
	DDX_Check(pDX, IDC_CHECK_DB_WRITE, depth_write_enabled);
	DDX_Control(pDX, IDC_CHECK_DB_CMP, depth_compare_button);
	DDX_Check(pDX, IDC_CHECK_DB_CMP, depth_compared);
	DDX_Control(pDX, IDC_CHECK_ALPHA_TEST, alpha_test_button);
	DDX_Check(pDX, IDC_CHECK_ALPHA_TEST, alpha_tested);
	DDX_Control(pDX, IDC_CHECK_ALPHA_BLEND, alpha_blend_button);
	DDX_Check(pDX, IDC_CHECK_ALPHA_BLEND, alpha_blended);
	DDX_Control(pDX, IDC_CHECK_WIREFRAME, wireframe_button);
	DDX_Check(pDX, IDC_CHECK_WIREFRAME, wireframe_enabled);
	DDX_Check(pDX, IDC_DUALSOURCE, is_dual_blended);
	DDX_Control(pDX, IDC_DUALSOURCE, dualsource_btn);
}


BEGIN_MESSAGE_MAP(CShaderSetDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_DB_CMP, &CShaderSetDlg::OnBnClickedCheckDbCmp)
	ON_BN_CLICKED(IDC_CHECK_DB_WRITE, &CShaderSetDlg::OnBnClickedCheckDbWrite)
	ON_BN_CLICKED(IDC_CHECK_ALPHA_TEST, &CShaderSetDlg::OnBnClickedCheckAlphaTest)
	ON_BN_CLICKED(IDC_CHECK_ALPHA_BLEND, &CShaderSetDlg::OnBnClickedCheckAlphaBlend)
	ON_BN_CLICKED(IDC_CHECK_WIREFRAME, &CShaderSetDlg::OnBnClickedCheckWireframe)
	ON_BN_CLICKED(IDC_DUALSOURCE, &CShaderSetDlg::OnBnClickedDualsource)
END_MESSAGE_MAP()


// CShaderSetDlg message handlers


void CShaderSetDlg::OnBnClickedCheckDbCmp()
{
	UpdateData(TRUE);
	CEffectToolApplicationApp* app = (CEffectToolApplicationApp*)AfxGetApp();
	app->effect_tool_.depth_compared_ = depth_compared;
	return;
}


void CShaderSetDlg::OnBnClickedCheckDbWrite()
{
	UpdateData(TRUE);
	CEffectToolApplicationApp* app = (CEffectToolApplicationApp*)AfxGetApp();
	app->effect_tool_.depth_write_enabled_ = depth_write_enabled;
	return;
}


void CShaderSetDlg::OnBnClickedCheckAlphaTest()
{
	UpdateData(TRUE);
	CEffectToolApplicationApp* app = (CEffectToolApplicationApp*)AfxGetApp();
	app->effect_tool_.alpha_tested_ = alpha_tested;
	return;
}


void CShaderSetDlg::OnBnClickedCheckAlphaBlend()
{
	UpdateData(TRUE);
	CEffectToolApplicationApp* app = (CEffectToolApplicationApp*)AfxGetApp();
	app->effect_tool_.alpha_blended_ = alpha_blended;
	return;
}


void CShaderSetDlg::OnBnClickedCheckWireframe()
{
	UpdateData(TRUE);
	CEffectToolApplicationApp* app = (CEffectToolApplicationApp*)AfxGetApp();
	app->effect_tool_.wireframe_enabled_ = wireframe_enabled;
	return;
}


void CShaderSetDlg::OnBnClickedDualsource()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CEffectToolApplicationApp* app = (CEffectToolApplicationApp*)AfxGetApp();
	app->effect_tool_.dualsource_blended_ = is_dual_blended;
	return;
}


BOOL CShaderSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	depth_compare_button.SetCheck(TRUE);
	alpha_blend_button.SetCheck(TRUE);
	dualsource_btn.SetCheck(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
