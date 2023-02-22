// CTextureSelectDlg.cpp : implementation file
//

#include "pch.h"
#include "EffectToolApplication.h"
#include "CTextureSelectDlg.h"
#include "afxdialogex.h"


// CTextureSelectDlg dialog

IMPLEMENT_DYNAMIC(CTextureSelectDlg, CDialogEx)

CTextureSelectDlg::CTextureSelectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TEXTURE, pParent)
	, num_rows(0)
	, num_cols(0)
{

}

CTextureSelectDlg::~CTextureSelectDlg()
{
}

void CTextureSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SINGLE_UV, single_uv_btn);
	DDX_Control(pDX, IDC_BROWSE_TEXFILE, browse_texfile_btn);
	DDX_Control(pDX, IDC_TEX_LIST, tex_path_list);
	DDX_Text(pDX, IDC_UV_ROWS, num_rows);
	DDX_Text(pDX, IDC_UV_COLS, num_cols);
}


BEGIN_MESSAGE_MAP(CTextureSelectDlg, CDialogEx)
	ON_BN_CLICKED(IDC_SINGLE_UV, &CTextureSelectDlg::OnBnClickedSingleUv)
	ON_BN_CLICKED(IDC_BROWSE_TEXFILE, &CTextureSelectDlg::OnBnClickedBrowseTexfile)
	ON_BN_CLICKED(IDC_BUTTON1, &CTextureSelectDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CTextureSelectDlg message handlers


void CTextureSelectDlg::OnBnClickedSingleUv()
{
	// TODO: Add your control notification handler code here
	browse_texfile_btn.EnableWindow();
	GetDlgItem(IDC_UV_ROWS)->EnableWindow();
	GetDlgItem(IDC_UV_COLS)->EnableWindow();
}


void CTextureSelectDlg::OnBnClickedBrowseTexfile()
{
	// TODO: Add your control notification handler code here

	static TCHAR BASED_CODE szFilter[] = _T("Image files(*.BMP, *.GIF, *.JPG, *.DDS, *TGA, *PNG)|*.BMP;*.GIF;*.JPG;*.bmp;*.jpg;*.gif;*.dds;*.DDS;*.tga;*.TGA;*.png;*.PNG||");

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
			tex_path_list.ResetContent();
		}
		for (POSITION pos = dlg.GetStartPosition(); pos != NULL;)
		{
			CString tex_path = dlg.GetNextPathName(pos);
			tex_file_paths.push_back(LPCWSTR(tex_path));
			CString tex_nameonly = tex_path.Right(tex_path.GetLength() - tex_path.ReverseFind(_T('\\')) - 1);
			tex_path_list.AddString(tex_nameonly);
		}

		// load thumbnail image
		CImage img;
		HRESULT hr = img.Load(tex_file_paths[0].c_str());
		CRect ImgRect;
		GetDlgItem(IDC_TEX_THUMBNAIL)->GetWindowRect(&ImgRect);
		ScreenToClient(ImgRect);
		CDC* dc;
		dc = GetDlgItem(IDC_TEX_THUMBNAIL)->GetDC();
		dc->SetStretchBltMode(COLORONCOLOR);

		if (hr == S_OK)
		{
			img.StretchBlt(dc->m_hDC, 0, 0, ImgRect.Width(), ImgRect.Height(), SRCCOPY);
		}
	}
	return;
}


void CTextureSelectDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CEffectToolApplicationApp* app = (CEffectToolApplicationApp*)AfxGetApp();
	app->effect_tool_.SetEmitterTexture(L"/default/", tex_file_paths[0], num_rows, num_cols);
}
