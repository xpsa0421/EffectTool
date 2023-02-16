// CEmitterSettingsDlg.cpp : implementation file
//

#include "pch.h"
#include "EffectToolApplication.h"
#include "CEmitterSettingsDlg.h"
#include "afxdialogex.h"


// CEmitterSettingsDlg dialog

IMPLEMENT_DYNAMIC(CEmitterSettingsDlg, CDialogEx)

CEmitterSettingsDlg::CEmitterSettingsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EMITTER_SETTER, pParent)
	, emitter_init_num(0)
	, emitter_emit_cycle(0)
	, emitter_x(0)
	, emitter_y(0)
	, emitter_z(0)
	, pos_offset_minX(0)
	, pos_offset_minY(0)
	, pos_offset_minZ(0)
	, pos_offset_maxX(0)
	, pos_offset_maxY(0)
	, pos_offset_maxZ(0)
	, size_minX(0)
	, size_minY(0)
	, size_maxX(0)
	, size_maxY(0)
	, lifetime_min(0)
	, lifetime_max(0)
	, emitter_name(_T(""))
{

}

CEmitterSettingsDlg::~CEmitterSettingsDlg()
{
}

void CEmitterSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEX_THUMBNAIL, tex_thumbnail_);
	DDX_Control(pDX, IDC_TEX_LIST, tex_paths_list_);
	DDX_Control(pDX, IDC_EMITTER_X, editctrl_emitter_x);
	DDX_Control(pDX, IDC_EMITTER_Y, editctrl_emitter_y);
	DDX_Control(pDX, IDC_EMITTER_Z, editctrl_emitter_z);
	DDX_Control(pDX, IDC_EMITTER_INIT_NUM, editctrl_numparticles);
	DDX_Control(pDX, IDC_EMITTER_EMIT_CYCLE, editctrl_emitcycle);
	DDX_Control(pDX, IDC_POS_MIN_X, editctrl_offset_minX);
	DDX_Control(pDX, IDC_POS_MIN_Y, editctrl_offset_minY);
	DDX_Control(pDX, IDC_POS_MIN_Z, editctrl_offset_minZ);
	DDX_Control(pDX, IDC_POS_MAX_X, editctrl_offset_maxX);
	DDX_Control(pDX, IDC_POS_MAX_Y, editctrl_offset_maxY);
	DDX_Control(pDX, IDC_POS_MAX_Z, editctrl_offset_maxZ);
	DDX_Control(pDX, IDC_SIZE_MIN_X, editctrl_size_minX);
	DDX_Control(pDX, IDC_SIZE_MIN_Y, editctrl_size_minY);
	DDX_Control(pDX, IDC_SIZE_MAX_X, editctrl_size_maxX);
	DDX_Control(pDX, IDC_SIZE_MAX_Y, editctrl_size_maxY);
	DDX_Control(pDX, IDC_LIFETIME_MIN, editctrl_lifetime_min);
	DDX_Control(pDX, IDC_LIFETIME_MAX, editctrl_lifetime_max);
	DDX_Control(pDX, IDC_EMITTER_NAME, editctrl_emittername);
	DDX_Text(pDX, IDC_EMITTER_INIT_NUM, emitter_init_num);
	DDX_Text(pDX, IDC_EMITTER_EMIT_CYCLE, emitter_emit_cycle);
	DDX_Text(pDX, IDC_EMITTER_X, emitter_x);
	DDX_Text(pDX, IDC_EMITTER_Y, emitter_y);
	DDX_Text(pDX, IDC_EMITTER_Z, emitter_z);
	DDX_Text(pDX, IDC_POS_MIN_X, pos_offset_minX);
	DDX_Text(pDX, IDC_POS_MIN_Y, pos_offset_minY);
	DDX_Text(pDX, IDC_POS_MIN_Z, pos_offset_minZ);
	DDX_Text(pDX, IDC_POS_MAX_X, pos_offset_maxX);
	DDX_Text(pDX, IDC_POS_MAX_Y, pos_offset_maxY);
	DDX_Text(pDX, IDC_POS_MAX_Z, pos_offset_maxZ);
	DDX_Text(pDX, IDC_SIZE_MIN_X, size_minX);
	DDX_Text(pDX, IDC_SIZE_MIN_Y, size_minY);
	DDX_Text(pDX, IDC_SIZE_MAX_X, size_maxX);
	DDX_Text(pDX, IDC_SIZE_MAX_Y, size_maxY);
	DDX_Text(pDX, IDC_LIFETIME_MIN, lifetime_min);
	DDX_Text(pDX, IDC_LIFETIME_MAX, lifetime_max);
	DDX_Text(pDX, IDC_EMITTER_NAME, emitter_name);
}


BEGIN_MESSAGE_MAP(CEmitterSettingsDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BROWSE_TEXFILE, &CEmitterSettingsDlg::OnBnClickedBrowseTexfile)
	ON_EN_CHANGE(IDC_EMITTER_Z, &CEmitterSettingsDlg::OnEnChangeEmitterZ)
	ON_BN_CLICKED(BUTTON_GEN_EMITTER, &CEmitterSettingsDlg::OnBnClickedGenEmitter)
END_MESSAGE_MAP()


// CEmitterSettingsDlg message handlers


void CEmitterSettingsDlg::OnBnClickedBrowseTexfile()
{
	// TODO: Add your control notification handler code here

	static TCHAR BASED_CODE szFilter[] = _T("이미지 파일(*.BMP, *.GIF, *.JPG, *.DDS, *TGA, *PNG)|*.BMP;*.GIF;*.JPG;*.bmp;*.jpg;*.gif;*.dds;*.DDS;*.tga;*.TGA;*.png;*.PNG||");

	CFileDialog dlg(TRUE, NULL, _T("image"), OFN_ALLOWMULTISELECT, szFilter);

	CString strFileList;
	const int c_cMaxFiles = 36;
	const int c_cbBuffSize = (c_cMaxFiles * (MAX_PATH + 1)) + 1;
	dlg.GetOFN().lpstrFile = strFileList.GetBuffer(c_cbBuffSize);
	dlg.GetOFN().nMaxFile = c_cbBuffSize;

	if (IDOK == dlg.DoModal())
	{
		if (!tex_file_paths.empty())
		{
			tex_file_paths.clear();
			tex_paths_list_.ResetContent();
		}
		for (POSITION pos = dlg.GetStartPosition(); pos != NULL;)
		{
			CString tex_path = dlg.GetNextPathName(pos);
			tex_file_paths.push_back(LPCWSTR(tex_path));
			CString tex_nameonly = tex_path.Right(tex_path.GetLength() - tex_path.ReverseFind(_T('\\')) - 1);
			tex_paths_list_.AddString(tex_nameonly);
		}
	}
	return;
}



void CEmitterSettingsDlg::OnEnChangeEmitterZ()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


BOOL CEmitterSettingsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	editctrl_emitter_x.SetWindowText(L"0");
	editctrl_emitter_y.SetWindowText(L"0");
	editctrl_emitter_z.SetWindowText(L"0");
	editctrl_numparticles.SetWindowText(L"10");
	editctrl_emitcycle.SetWindowText(L"3");
	editctrl_offset_minX.SetWindowText(L"-1");
	editctrl_offset_minY.SetWindowText(L"-1");
	editctrl_offset_minZ.SetWindowText(L"-1");
	editctrl_offset_maxX.SetWindowText(L"1");
	editctrl_offset_maxY.SetWindowText(L"1");
	editctrl_offset_maxZ.SetWindowText(L"1");
	editctrl_size_minX.SetWindowText(L"0.5");
	editctrl_size_minY.SetWindowText(L"0.5");
	editctrl_size_maxX.SetWindowText(L"1");
	editctrl_size_maxY.SetWindowText(L"1");
	editctrl_lifetime_min.SetWindowText(L"2");
	editctrl_lifetime_max.SetWindowText(L"5");
	editctrl_emittername.SetWindowText(L"");



	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CEmitterSettingsDlg::OnBnClickedGenEmitter()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CEffectToolApplicationApp* app = (CEffectToolApplicationApp*)AfxGetApp();
	if (app->effect_tool_.NameExists(LPCWSTR(emitter_name)))
	{
		AfxMessageBox(_T("Name already exists!"), MB_ICONSTOP);
		return;
	}
	
	app->effect_tool_.GenEmitterFromMultipleTex(tex_file_paths,
		emitter_init_num, emitter_emit_cycle,
		{ emitter_x, emitter_y, emitter_z },
		{ pos_offset_minX, pos_offset_minY, pos_offset_minZ },
		{ pos_offset_maxX, pos_offset_maxY, pos_offset_maxZ },
		{ size_minX, size_minY }, { size_maxX, size_maxY },
		{ lifetime_min, lifetime_max },
		LPCWSTR(emitter_name));
	
	return;
}
