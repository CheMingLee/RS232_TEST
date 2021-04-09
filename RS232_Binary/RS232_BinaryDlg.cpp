﻿
// RS232_BinaryDlg.cpp: 實作檔案
//

#include "pch.h"
#include "framework.h"
#include "RS232_Binary.h"
#include "RS232_BinaryDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int BLAZER_CmdTrans(BYTE * buf, int iRW, BYTE cmd, BYTE data_H, BYTE data_L)
{
	int i;
	int iPtr = 0;
	BYTE ckeck_xor = 0x00;

	buf[iPtr++] = 0x1a;
	if (iRW == 0) //Read
	{
		buf[iPtr++] = 0x03;
	}
	else if (iRW == 1) //Write
	{
		buf[iPtr++] = 0x06;
	}
	buf[iPtr++] = 0xaa;
	buf[iPtr++] = cmd;
	buf[iPtr++] = data_H;
	buf[iPtr++] = data_L;
	for (i = 0; i < 6; i++)
		ckeck_xor ^= buf[i];
	buf[iPtr++] = ckeck_xor;
	buf[iPtr++] = 0xef;
	buf[iPtr++] = 0xf0;

	return iPtr;
}

// 對 App About 使用 CAboutDlg 對話方塊

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

// 程式碼實作
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRS232BinaryDlg 對話方塊



CRS232BinaryDlg::CRS232BinaryDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RS232_BINARY_DIALOG, pParent)
	, m_iComPort(1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRS232BinaryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_COMPORT, m_iComPort);
}

BEGIN_MESSAGE_MAP(CRS232BinaryDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_W_ONLINE, &CRS232BinaryDlg::OnBnClickedButtonWOnline)
	ON_BN_CLICKED(IDC_BUTTON_W_OFFLINE, &CRS232BinaryDlg::OnBnClickedButtonWOffline)
	ON_BN_CLICKED(IDC_BUTTON_SET_COM, &CRS232BinaryDlg::OnBnClickedButtonSetCom)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CRS232BinaryDlg 訊息處理常式

BOOL CRS232BinaryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 將 [關於...] 功能表加入系統功能表。

	// IDM_ABOUTBOX 必須在系統命令範圍之中。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO: 在此加入額外的初始設定
	m_hCom = INVALID_HANDLE_VALUE;
	UpdateData(TRUE);
	Open(m_iComPort);

	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void CRS232BinaryDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CRS232BinaryDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 繪製的裝置內容

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CRS232BinaryDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CRS232BinaryDlg::Open(int iComPort)
{
	char szBuff[20];
	sprintf_s(szBuff, "COM%d", iComPort);

	m_hCom = CreateFile(szBuff, GENERIC_READ | GENERIC_WRITE,
		0,    // exclusive access 
		NULL, // no security attributes 
		OPEN_EXISTING,
		0,
		NULL);

	if (m_hCom == INVALID_HANDLE_VALUE)
		return FALSE;

	BOOL		bReturn;
	COMMCONFIG	ComConfig;
	DWORD		dwSize;
	dwSize = sizeof(COMMCONFIG);

	bReturn = GetCommConfig(m_hCom, &ComConfig, &dwSize);

	DCB	dcb;
	bReturn = GetCommState(m_hCom, &dcb);

	COMMPROP	CommProp;
	bReturn = GetCommProperties(m_hCom, &CommProp);

	SetupComm(m_hCom, 1024, 1024);

	PurgeComm(m_hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

	COMMTIMEOUTS CommTimeOuts;
	GetCommTimeouts(m_hCom, &CommTimeOuts);
	CommTimeOuts.ReadIntervalTimeout = MAXDWORD;
	CommTimeOuts.ReadTotalTimeoutMultiplier = MAXDWORD;
	CommTimeOuts.ReadTotalTimeoutConstant = 80;//1000; //500000;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = 80;//1000;//5000;
	SetCommTimeouts(m_hCom, &CommTimeOuts);
	dcb.DCBlength = sizeof(DCB);
	GetCommState(m_hCom, &dcb);

	dcb.ByteSize = (BYTE)8;
	dcb.Parity = (BYTE)0;
	dcb.fParity = FALSE;
	dcb.StopBits = (BYTE)0;

	dcb.BaudRate = CBR_19200;
	dcb.fBinary = TRUE;
	dcb.fOutxCtsFlow = 0;
	dcb.fOutxDsrFlow = 0;
	dcb.fDtrControl = 1;
	dcb.fRtsControl = 1;
	dcb.fDsrSensitivity = FALSE;
	dcb.fTXContinueOnXoff = FALSE;//TRUE;
	dcb.fOutX = 0;
	dcb.fInX = 0;
	dcb.XonLim = 50;
	dcb.XoffLim = 50;
	dcb.XonChar = 0;
	dcb.XoffChar = 0;

	m_ovRW.hEvent = NULL;
	m_ovRW.Offset = 0;
	m_ovRW.OffsetHigh = 0;

	if (SetCommState(m_hCom, &dcb) == 0)
	{
		Close();
		return FALSE;
	}
	PurgeComm(m_hCom, PURGE_RXCLEAR);

	return TRUE;
}

BOOL CRS232BinaryDlg::Close()
{
	if (m_hCom != INVALID_HANDLE_VALUE)
	{
		EscapeCommFunction(m_hCom, CLRDTR);
		EscapeCommFunction(m_hCom, CLRRTS);

		PurgeComm(m_hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

		CloseHandle(m_hCom);
	}

	m_hCom = INVALID_HANDLE_VALUE;

	return TRUE;
}

BOOL CRS232BinaryDlg::Write(LPCSTR lpStr, DWORD iSize)
{
	DWORD iWrite;

	if (m_hCom == INVALID_HANDLE_VALUE)
		return FALSE;

	WriteFile(m_hCom, lpStr, iSize, &iWrite, &m_ovRW);
	Sleep(200);
	return TRUE;
}

long CRS232BinaryDlg::Read(LPSTR lpStr, int iSize)
{
	DWORD iRead;
	BOOL bBool = FALSE;

	if (m_hCom == INVALID_HANDLE_VALUE)
		return 0;

	ReadFile(m_hCom, lpStr, iSize, &iRead, &m_ovRW);

	return iRead;
}


void CRS232BinaryDlg::OnBnClickedButtonWOnline()
{
	int ret, i;
	int iSize;
	BYTE buf[256];
	BYTE rData[256];
	CString str, strShow;

	memset(rData, 0, 256);

	iSize = BLAZER_CmdTrans(buf, 1, 0x01, 0x00, 0x01);
	Write((LPCSTR)buf, iSize);

	ret = Read((LPSTR)rData, 256);
	if (ret > 0)
	{
		strShow = "";
		for (i = 0; i < ret; i++)
		{
			str.Format(_T("%02X"), rData[i]);
			strShow += str;
		}
		GetDlgItem(IDC_STATIC_R)->SetWindowText(strShow);
	}
}


void CRS232BinaryDlg::OnBnClickedButtonWOffline()
{
	int ret, i;
	int iSize;
	BYTE buf[256];
	BYTE rData[256];
	CString str, strShow;

	memset(rData, 0, 256);
	
	iSize = BLAZER_CmdTrans(buf, 1, 0x01, 0x00, 0x00);
	Write((LPCSTR)buf, iSize);

	ret = Read((LPSTR)rData, 256);
	if (ret > 0)
	{
		strShow = "";
		for (i = 0; i < ret; i++)
		{
			str.Format(_T("%02X"), rData[i]);
			strShow += str;
		}
		GetDlgItem(IDC_STATIC_R)->SetWindowText(strShow);
	}
}


void CRS232BinaryDlg::OnBnClickedButtonSetCom()
{
	Close();
	UpdateData(TRUE);
	Open(m_iComPort);
}


void CRS232BinaryDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	Close();
}
