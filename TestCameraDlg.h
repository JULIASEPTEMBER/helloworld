
// TestCameraDlg.h : PROJECT_NAME 应用程序的主头文件
//

#if !defined(AFX_PREVIEW_H__91C2FA3D_BCBC_4DBD_927D_F438529B8733__INCLUDED_)
#define AFX_PREVIEW_H__91C2FA3D_BCBC_4DBD_927D_F438529B8733__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CTestCameraDlgApp:
// 有关此类的实现，请参阅 TestCameraDlg.cpp
//

class CTestCameraDlgApp : public CWinApp
{
public:
	CTestCameraDlgApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CTestCameraDlgApp theApp;
#endif // !defined(AFX_PREVIEW_H__91C2FA3D_BCBC_4DBD_927D_F438529B8733__INCLUDED_)
