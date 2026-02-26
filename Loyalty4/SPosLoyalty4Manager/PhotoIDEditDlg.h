#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRecordSetAccountFull.h"
//*******************************************************************
#include "PhotoIDFilenameStack.h"
//*******************************************************************

class CPhotoIDEditDlg : public CDialog
{
public:
	CPhotoIDEditDlg(CSQLRowAccountFull& RowAccount, CPhotoIDFilenameStack* pPhotoIDFilenameStack, CWnd* pParent = NULL);   // standard constructor
	virtual ~CPhotoIDEditDlg();

// Dialog Data
	enum { IDD = IDD_PHOTOIDEDIT };
	CStatic	m_staticPicture;
	CString m_strCardNo;
	CString m_strCardName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnChange();
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()

private:
	void UpdatePhotoID( int nLevel );

private:
//	HBITMAP m_hBitmap;
	CImage m_image;
	CString m_strInitialDir;
	CString m_strTitle;
	CPhotoIDFilenameStack* m_pPhotoIDFilenameStack;
};

//*******************************************************************
