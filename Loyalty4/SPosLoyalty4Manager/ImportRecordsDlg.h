#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************

class CImportRecordsDlg : public CDialog
{
public:
	CImportRecordsDlg( const char* szFilename, CWnd* pParent);   // standard constructor

// Dialog Data
	enum { IDD = IDD_IMPORTRECORDS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedButtonView();

	DECLARE_MESSAGE_MAP()

public:
	int m_nImportMethod;

private:
	CString m_strFilename;
	CWnd* m_pParent;
};
