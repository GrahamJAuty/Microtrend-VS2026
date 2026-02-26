#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPeriodEndWarningDlg : public CDialog
{
public:
	CPeriodEndWarningDlg( int nStockItems, int nZeroClosing, int nStocktakeType, CWnd* pParent = NULL);   

	//{{AFX_DATA(CPeriodEndWarningDlg)
	enum { IDD = IDD_PERIODEND_WARNING };
	CStatic	m_staticMessage4;
	CStatic	m_staticMessage3;
	CStatic	m_staticInfo1;
	CStatic	m_staticMessage2;
	CStatic	m_staticMessage1;
	CStatic	m_staticZeroClosing;
	CStatic	m_staticStockItems;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPeriodEndWarningDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPeriodEndWarningDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nStockItems;
	int m_nZeroClosing;
	int m_nStocktakeType;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/


