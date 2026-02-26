#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
//*******************************************************************

class CPhotoIDDlg : public CDialog
{
public:
	CPhotoIDDlg(CSQLRowAccountFull& RowAccount, const char* szImageFilename, CWnd* pParent = NULL);   // standard constructor
	virtual ~CPhotoIDDlg();

// Dialog Data
	enum { IDD = IDD_PHOTOID };
	CStatic	m_staticPicture;
	CString m_strCardNo;
	CString m_strCardName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()

private:
	CImage m_image;
	CString m_strTitle;
	CString m_strImageFilename;
};

//*******************************************************************
