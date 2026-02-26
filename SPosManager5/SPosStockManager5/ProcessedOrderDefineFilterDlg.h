#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ProcessedOrderFilterArray.h"
/**********************************************************************/

class CProcessedOrderDefineFilterDlg : public CDialog
{
public:
	CProcessedOrderDefineFilterDlg( CProcessedOrderFilterArray& FilterArray, CWnd* pParent = NULL); 
	
	//{{AFX_DATA(CProcessedOrderDefineFilterDlg)
	enum { IDD = IDD_ORDER_DEFINE_FILTER };
	BOOL	m_bMatchCase;
	CString	m_strSearchText;
	BOOL	m_bMatchDescription;
	BOOL	m_bMatchStockCode;
	BOOL	m_bMatchSuppRef;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CProcessedOrderDefineFilterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	
	CProcessedOrderFilterArray& m_OrderFilterArray;

protected:
	//{{AFX_MSG(CProcessedOrderDefineFilterDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nSearchType;
};

/**********************************************************************/
#endif
/**********************************************************************/
