#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CImportStatsDlg : public CSSDialog
{
public:
	CImportStatsDlg(CWnd* pParent = NULL);   
	
	enum { IDD = IDD_IMPORTSTATS };
	CButton m_buttonException;
	CEdit m_editLinesRead;
	CEdit m_editLinesInvalid;
	CEdit m_editLinesIgnored;
	CEdit m_editLinesUsed;
	CEdit m_editRecordsAdded;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonException();
	DECLARE_MESSAGE_MAP()

public:
	int m_nLinesRead;
	int m_nLinesInvalid;
	int m_nLinesIgnored;
	int m_nLinesUsed;
	int m_nRecordsAdded;
	CString m_strExceptionFilename;
};

//$$******************************************************************
