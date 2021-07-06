
// TwisterSpinner.h : main header file for the TwisterSpinner application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CTwisterSpinnerApp:
// See TwisterSpinner.cpp for the implementation of this class
//

class CTwisterSpinnerApp : public CWinAppEx
{
public:
	CTwisterSpinnerApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTwisterSpinnerApp theApp;
