
// EffectToolApplication.h : main header file for the EffectToolApplication application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "EffectTool.h"


// CEffectToolApplicationApp:
// See EffectToolApplication.cpp for the implementation of this class
//

class CEffectToolApplicationApp : public CWinAppEx
{
public:
	CEffectToolApplicationApp() noexcept;
	EffectTool	effect_tool_;

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CEffectToolApplicationApp theApp;
