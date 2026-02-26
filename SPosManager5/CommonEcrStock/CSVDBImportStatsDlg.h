#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CCSVDBImportStatsDlg : public CDialog
{
public:
	CCSVDBImportStatsDlg( int nType, CWnd* pParent );

	//{{AFX_DATA(CCSVDBImportStatsDlg)
	enum { IDD = IDD_CSVDB_IMPORTSTATS };
	CStatic	m_staticIgnored;
	CStatic	m_staticRecordsAdded;
	CStatic	m_staticRecordsModified;
	CStatic	m_staticTotalLines;
	CStatic	m_staticText1;
	CStatic	m_staticText2;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CCSVDBImportStatsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CCSVDBImportStatsDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int m_nType;
	int m_nTotalLines;
	int m_nIgnoredLines;
	int m_nRecordsAdded;
	int m_nRecordsModified;
};	

/**********************************************************************/
