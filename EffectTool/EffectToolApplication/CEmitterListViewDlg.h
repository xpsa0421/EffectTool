#pragma once


// CEmitterListViewDlg dialog

class CEmitterListViewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEmitterListViewDlg)

public:
	CEmitterListViewDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CEmitterListViewDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EMITTERLIST_VIEW};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
