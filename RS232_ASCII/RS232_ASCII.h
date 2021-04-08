
// RS232_ASCII.h: PROJECT_NAME 應用程式的主要標頭檔
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// 主要符號


// CRS232ASCIIApp:
// 查看 RS232_ASCII.cpp 以了解此類別的實作
//

class CRS232ASCIIApp : public CWinApp
{
public:
	CRS232ASCIIApp();

// 覆寫
public:
	virtual BOOL InitInstance();

// 程式碼實作

	DECLARE_MESSAGE_MAP()
};

extern CRS232ASCIIApp theApp;
