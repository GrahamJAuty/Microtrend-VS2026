#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPluImportStatsDlgStockman : public CDialog
{
public:
	CPluImportStatsDlgStockman ( bool bStockMode, CWnd* pParent );

	//{{AFX_DATA(CPluImportStatsDlgStockman)
	enum { IDD = IDD_PLU_IMPORTSTATS_STOCKMAN };
	CStatic	m_staticIgnored;
	CStatic	m_staticLabel3;
	CStatic	m_staticLabel2;
	CStatic	m_staticLabel1;
	CStatic	m_staticTotalLines;
	CStatic	m_staticStockAdded;
	CStatic	m_staticPluModified;
	CStatic	m_staticPluAdded;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPluImportStatsDlgStockman)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPluImportStatsDlgStockman)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	bool m_bStockMode;
	int m_nTotalLines;
	int m_nIgnoredLines;
	int m_nPluAdded;
	int m_nPluModified;
	int m_nStockAdded;
};	

/**********************************************************************/
#endif				//__PLUIMPORTSTATS_DLG__
/**********************************************************************/
