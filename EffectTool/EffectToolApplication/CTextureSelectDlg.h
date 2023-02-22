#pragma once


// CTextureSelectDlg dialog

class CTextureSelectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTextureSelectDlg)

public:
	std::vector<W_STR> tex_file_paths;
	CTextureSelectDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTextureSelectDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEXTURE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton single_uv_btn;
	afx_msg void OnBnClickedSingleUv();
	CButton browse_texfile_btn;
	afx_msg void OnBnClickedBrowseTexfile();
	CListBox tex_path_list;
	int num_rows;
	int num_cols;
	afx_msg void OnBnClickedButton1();
};
