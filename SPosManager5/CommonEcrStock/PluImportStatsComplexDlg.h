#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPluImportStatsComplexDlg : public CDialog
{
public:
	CPluImportStatsComplexDlg ( CWnd* pParent );

	//{{AFX_DATA(CPluImportStatsComplexDlg)
	enum { IDD = IDD_PLU_IMPORTSTATS_COMPLEX };
	CStatic	m_staticIgnored;
	CStatic	m_staticSupplierAdded;
	CStatic	m_staticStockModified;
	CStatic	m_staticTotalLines;
	CStatic	m_staticStockAdded;
	CStatic	m_staticPluModified;
	CStatic	m_staticPluAdded;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPluImportStatsComplexDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPluImportStatsComplexDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int m_nTotalLines;
	int m_nIgnoredLines;
	int m_nPluAdded;
	int m_nPluModified;
	int m_nStockAdded;
	int m_nStockModified;
	int m_nSupplierAdded;
};	

/**********************************************************************/
