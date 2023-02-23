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
	, vel_min_x(0)
	, vel_min_y(0)
	, vel_min_z(0)
	, vel_max_x(0)
	, vel_max_y(0)
	, vel_max_z(0)
	, use_random_color(FALSE)
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
	DDX_Control(pDX, IDC_VEL_MIN_X, editctrl_vel_min_x);
	DDX_Control(pDX, IDC_VEL_MIN_Y, editctrl_vel_min_y);
	DDX_Control(pDX, IDC_VEL_MIN_Z, editctrl_vel_min_z);
	DDX_Control(pDX, IDC_VEL_MAX_X, editctrl_vel_max_x);
	DDX_Control(pDX, IDC_VEL_MAX_Y, editctrl_vel_max_y);
	DDX_Control(pDX, IDC_VEL_MAX_Z, editctrl_vel_max_z);
	DDX_Text(pDX, IDC_VEL_MIN_X, vel_min_x);
	DDX_Text(pDX, IDC_VEL_MIN_Y, vel_min_y);
	DDX_Text(pDX, IDC_VEL_MIN_Z, vel_min_z);
	DDX_Text(pDX, IDC_VEL_MAX_X, vel_max_x);
	DDX_Text(pDX, IDC_VEL_MAX_Y, vel_max_y);
	DDX_Text(pDX, IDC_VEL_MAX_Z, vel_max_z);
}


BEGIN_MESSAGE_MAP(CEmitterSettingsDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BROWSE_TEXFILE, &CEmitterSettingsDlg::OnBnClickedBrowseTexfile)
	ON_BN_CLICKED(BUTTON_GEN_EMITTER, &CEmitterSettingsDlg::OnBnClickedGenEmitter)
	ON_BN_CLICKED(IDC_RANDOMCOLOR, &CEmitterSettingsDlg::OnBnClickedRandomcolor)
	ON_BN_CLICKED(IDC_NOCOLOR, &CEmitterSettingsDlg::OnBnClickedNocolor)
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

		// load thumbnail image
		CImage img;
		HRESULT hr = img.Load(tex_file_paths[0].c_str());
		CRect ImgRect;
		GetDlgItem(IDC_TEX_THUMBNAIL)->GetWindowRect(&ImgRect);
		ScreenToClient(ImgRect);
		CDC* dc;
		dc = tex_thumbnail_.GetDC();
		dc->SetStretchBltMode(COLORONCOLOR);

		if (hr == S_OK)
		{
			img.StretchBlt(dc->m_hDC, 0, 0, ImgRect.Width(), ImgRect.Height(), SRCCOPY);
		}
	}
	return;
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
	editctrl_vel_min_x.SetWindowText(L"0");
	editctrl_vel_min_y.SetWindowText(L"0");
	editctrl_vel_min_z.SetWindowText(L"0");
	editctrl_vel_max_x.SetWindowText(L"0");
	editctrl_vel_max_y.SetWindowText(L"0");
	editctrl_vel_max_z.SetWindowText(L"0");

	edit_controls.push_back(&editctrl_emitter_x);
	edit_controls.push_back(&editctrl_emitter_y);
	edit_controls.push_back(&editctrl_emitter_z);
	edit_controls.push_back(&editctrl_numparticles);
	edit_controls.push_back(&editctrl_emitcycle);
	edit_controls.push_back(&editctrl_offset_minX);
	edit_controls.push_back(&editctrl_offset_minY);
	edit_controls.push_back(&editctrl_offset_minZ);
	edit_controls.push_back(&editctrl_offset_maxX);
	edit_controls.push_back(&editctrl_offset_maxY);
	edit_controls.push_back(&editctrl_offset_maxZ);
	edit_controls.push_back(&editctrl_size_minX);
	edit_controls.push_back(&editctrl_size_minY);
	edit_controls.push_back(&editctrl_size_maxX);
	edit_controls.push_back(&editctrl_size_maxY);
	edit_controls.push_back(&editctrl_lifetime_min);
	edit_controls.push_back(&editctrl_lifetime_max);
	edit_controls.push_back(&editctrl_emittername);
	edit_controls.push_back(&editctrl_vel_min_x);
	edit_controls.push_back(&editctrl_vel_min_y);
	edit_controls.push_back(&editctrl_vel_min_z);
	edit_controls.push_back(&editctrl_vel_max_x);
	edit_controls.push_back(&editctrl_vel_max_y);
	edit_controls.push_back(&editctrl_vel_max_z);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CEmitterSettingsDlg::OnBnClickedGenEmitter()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	//check all data entered
	if (tex_file_paths.empty())
	{
		AfxMessageBox(_T("Please fill out all information"), MB_OK);
		return;
	}
	
	CString checkstr;
	for (int i = 0; i < edit_controls.size(); i++)
	{
		edit_controls[i]->GetWindowText(checkstr);
		if (checkstr.IsEmpty())
		{
			AfxMessageBox(_T("Please fill out all information"), MB_OK);
			return;
		}
	}

	CEffectToolApplicationApp* app = (CEffectToolApplicationApp*)AfxGetApp();
	if (app->effect_tool_.NameExists(LPCWSTR(emitter_name)))
	{
		AfxMessageBox(_T("Name already exists!"), MB_ICONSTOP);
		return;
	}
	/*
	app->effect_tool_.GenEmitterFromMultipleTex(
		spawn_rate_,
		{ emitter_x, emitter_y, emitter_z },
		{ pos_offset_minX, pos_offset_minY, pos_offset_minZ },
		{ pos_offset_maxX, pos_offset_maxY, pos_offset_maxZ },
		{ size_minX, size_minY }, { size_maxX, size_maxY },
		{ lifetime_min, lifetime_max }, 
		{ vel_min_x, vel_min_y, vel_min_z },
		{ vel_max_x, vel_max_y, vel_max_z },
		use_random_color,
		LPCWSTR(emitter_name),
		tex_file_paths);*/

	return;
}



void CEmitterSettingsDlg::OnBnClickedRandomcolor()
{
	use_random_color = true;
}


void CEmitterSettingsDlg::OnBnClickedNocolor()
{
	use_random_color = false;
}
