#pragma once
//$$******************************************************************
#include <atlimage.h>
//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//$$******************************************************************
#include "SmartPay4Manager.h"
//$$******************************************************************

class CPhotoIDDlg : public CSSAutoShutdownDialog
{
public:
	CPhotoIDDlg(CSQLRowAccountFull& SQLRowAccount, const char* szImageFilename, CString strSurname, CString strForename, CWnd* pParent = NULL);   // standard constructor
	virtual ~CPhotoIDDlg();

	enum { IDD = IDD_PHOTOID };
	CStatic	m_staticPicture;
	CString m_strCardNo;
	CString m_strCardName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	CImage m_image;
	CString m_strTitle;
	CString m_strImageFilename;
};

//$$******************************************************************
