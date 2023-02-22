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
};
