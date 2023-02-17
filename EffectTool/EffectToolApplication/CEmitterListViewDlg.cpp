// CEmitterListViewDlg.cpp : implementation file
//

#include "pch.h"
#include "EffectToolApplication.h"
#include "CEmitterListViewDlg.h"
#include "afxdialogex.h"


// CEmitterListViewDlg dialog

IMPLEMENT_DYNAMIC(CEmitterListViewDlg, CDialogEx)

CEmitterListViewDlg::CEmitterListViewDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EMITTERLIST_VIEW, pParent)
{

}

CEmitterListViewDlg::~CEmitterListViewDlg()
{
}

void CEmitterListViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EMITTER_LIST, emitter_list);
}


BEGIN_MESSAGE_MAP(CEmitterListViewDlg, CDialogEx)
	ON_WM_CREATE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CEmitterListViewDlg message handlers


int CEmitterListViewDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  Add your specialized creation code here

	return 0;
}


void CEmitterListViewDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	delete emitter_list;
	CDialogEx::OnClose();
}
