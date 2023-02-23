#pragma once


// CShaderSetDlg dialog

class CShaderSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CShaderSetDlg)

public:
	CShaderSetDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CShaderSetDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHADER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	CButton depth_enable_button;
	BOOL depth_write_enabled;
	CButton depth_compare_button;
	BOOL depth_compared;
	CButton alpha_test_button;
	BOOL alpha_tested;
	CButton alpha_blend_button;
	BOOL alpha_blended;
	afx_msg void OnBnClickedCheckDbCmp();
	afx_msg void OnBnClickedCheckDbWrite();
	afx_msg void OnBnClickedCheckAlphaTest();
	afx_msg void OnBnClickedCheckAlphaBlend();
	afx_msg void OnBnClickedCheckWireframe();
	CButton wireframe_button;
	BOOL wireframe_enabled;
	BOOL is_dual_blended;
	afx_msg void OnBnClickedDualsource();
	virtual BOOL OnInitDialog();
	CButton dualsource_btn;
};
