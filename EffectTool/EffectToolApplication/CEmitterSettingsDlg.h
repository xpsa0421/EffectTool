#pragma once
#include "CEmitterListViewDlg.h"


class CEmitterSettingsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEmitterSettingsDlg)
public:
	std::vector<W_STR> tex_file_paths;
public:
	CEmitterSettingsDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CEmitterSettingsDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EMITTER_SETTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBrowseTexfile();
	CStatic tex_thumbnail_;
	CListBox tex_paths_list_;
	CEdit editctrl_emitter_x;
	CEdit editctrl_emitter_y;
	CEdit editctrl_emitter_z;
	virtual BOOL OnInitDialog();
	CEdit editctrl_numparticles;
	CEdit editctrl_emitcycle;
	CEdit editctrl_offset_minX;
	CEdit editctrl_offset_minY;
	CEdit editctrl_offset_minZ;
	CEdit editctrl_offset_maxX;
	CEdit editctrl_offset_maxY;
	CEdit editctrl_offset_maxZ;
	CEdit editctrl_size_minX;
	CEdit editctrl_size_minY;
	CEdit editctrl_size_maxX;
	CEdit editctrl_size_maxY;
	CEdit editctrl_lifetime_min;
	CEdit editctrl_lifetime_max;
	CEdit editctrl_emittername;

	std::vector<CEdit*> edit_controls;

	//_-----------------------------------

	afx_msg void OnBnClickedGenEmitter();
	int emitter_init_num;
	float emitter_emit_cycle;
	float emitter_x;
	float emitter_y;
	float emitter_z;
	float pos_offset_minX;
	float pos_offset_minY;
	float pos_offset_minZ;
	float pos_offset_maxX;
	float pos_offset_maxY;
	float pos_offset_maxZ;
	float size_minX;
	float size_minY;
	float size_maxX;
	float size_maxY;
	float lifetime_min;
	float lifetime_max;
	CString emitter_name;
	CEdit editctrl_vel_min_x;
	CEdit editctrl_vel_min_y;
	CEdit editctrl_vel_min_z;
	CEdit editctrl_vel_max_x;
	CEdit editctrl_vel_max_y;
	CEdit editctrl_vel_max_z;
	float vel_min_x;
	float vel_min_y;
	float vel_min_z;
	float vel_max_x;
	float vel_max_y;
	float vel_max_z;
	bool use_random_color;
	afx_msg void OnBnClickedRandomcolor();
	afx_msg void OnBnClickedNocolor();
};
