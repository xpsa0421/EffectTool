#pragma once


// CDetails2Dlg dialog

class CDetails2Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDetails2Dlg)

public:
	CDetails2Dlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDetails2Dlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	float spawn_rate_;
	afx_msg void OnEnKillfocusSpawnrate();
	virtual BOOL OnInitDialog();
	float size_min_x;
	float size_min_y;
	float size_max_x;
	float size_max_y;
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnEnKillfocusSizeMinX();
	afx_msg void OnEnKillfocusSizeMinY();
	afx_msg void OnEnKillfocusSizeMaxX();
	afx_msg void OnEnKillfocusSizeMaxY();
};
