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
}


BEGIN_MESSAGE_MAP(CEmitterListViewDlg, CDialogEx)
END_MESSAGE_MAP()


// CEmitterListViewDlg message handlers
