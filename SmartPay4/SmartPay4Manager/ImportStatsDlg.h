#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

struct CImportStatsDlgConfig
{
public:
	CImportStatsDlgConfig();

public:
	CString m_strCaption;
	CString m_strExceptionFilename;
	int m_nLinesRead;
	int m_nLinesInvalid;
	int m_nLinesIgnored;
	int m_nLinesUsed;
	int m_nRecordsAdded;
	int m_nRecordsDeleted;
	int m_nLinesReadPos;
	int m_nLinesInvalidPos;
	int m_nLinesIgnoredPos;
	int m_nLinesUsedPos;
	int m_nRecordsAddedPos;
	int m_nRecordsDeletedPos;
	int m_nLastLinePos;
};

//$$******************************************************************

class CImportStatsDlg : public CSSAutoShutdownDialog
{
public:
	CImportStatsDlg(CImportStatsDlgConfig& config, CWnd* pParent = NULL);

	enum { IDD = IDD_IMPORTSTATS };
	CStatic m_staticLinesRead;
	CStatic m_staticLinesInvalid;
	CStatic m_staticLinesIgnored;
	CStatic m_staticLinesUsed;
	CStatic m_staticRecordsAdded;
	CStatic m_staticRecordsDeleted;
	CEdit m_editLinesRead;
	CEdit m_editLinesInvalid;
	CEdit m_editLinesIgnored;
	CEdit m_editLinesUsed;
	CEdit m_editRecordsAdded;
	CEdit m_editRecordsDeleted;
	CButton m_buttonException;
	CButton m_buttonOK;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnButtonException();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	void ShowStat(int nRow, CStatic& label, CEdit& edit, int nVal);

private:
	CImportStatsDlgConfig& m_Config;
};

//$$******************************************************************
