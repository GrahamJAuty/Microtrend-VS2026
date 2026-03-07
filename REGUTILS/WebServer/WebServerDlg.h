/**********************************************************************/
#pragma once
/**********************************************************************/
#include "resource.h"
#include "MyServer.h"
/**********************************************************************/

class CWebServerDlg : public CDialog
{
public:
	CWebServerDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_WEBSERVER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonServer();
	DECLARE_MESSAGE_MAP()

	HICON m_hIcon;

public:
	CMyServer m_server;
	bool m_bServerRunning;
	CButton m_buttonServer;
};

/**********************************************************************/
