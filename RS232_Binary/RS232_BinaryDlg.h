
// RS232_BinaryDlg.h: 標頭檔
//

#pragma once

int BLAZER_CmdTrans(BYTE * buf, int iRW, BYTE cmd, BYTE data_H, BYTE data_L);

// CRS232BinaryDlg 對話方塊
class CRS232BinaryDlg : public CDialogEx
{
// 建構
public:
	CRS232BinaryDlg(CWnd* pParent = nullptr);	// 標準建構函式

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RS232_BINARY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援


// 程式碼實作
protected:
	HICON m_hIcon;

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	HANDLE m_hCom;
	OVERLAPPED m_ovRW;
public:
	BOOL Open(int iComPort);
	BOOL Close();
	BOOL Write(LPCSTR lpStr, DWORD iSize);
	long Read(LPSTR lpStr, int iSize);
	int m_iComPort;
	afx_msg void OnBnClickedButtonWOnline();
	afx_msg void OnBnClickedButtonWOffline();
	afx_msg void OnBnClickedButtonSetCom();
	afx_msg void OnDestroy();
};
