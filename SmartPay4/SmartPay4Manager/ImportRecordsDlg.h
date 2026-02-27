#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CImportRecordsDlg : public CSSAutoShutdownDialog
{
public:
	CImportRecordsDlg( const char* szCaption, const char* szFilename, CWnd* pParent = NULL);   // standard constructor
	virtual ~CImportRecordsDlg();

	enum { IDD = IDD_IMPORTRECORDS };
	CString m_strImportFilename;
	int m_nImportMethod;
	BOOL m_bDeleteFile;

private:
	CButton m_buttonView;
	CButton m_buttonOK;
	CButton m_checkDeleteFile;
	CButton m_radioImportAll;
	CButton m_radioImportNew;
	CButton m_radioImportExisting;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	afx_msg void OnEnKillfocusEditImportfilename();
	afx_msg void OnBnClickedButtonView();
	afx_msg void OnBnClickedButtonBrowse();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

public:
	bool m_bEnableSelective;
	CString m_strFileType;
	int m_bShowDeleteFile;

private:
	void SetDisplay();

private:
	CString m_strCaption;
	CWnd* m_pParent;
};

//$$******************************************************************
