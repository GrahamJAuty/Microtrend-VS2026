#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CProcessedOrderGenerateInfoDlg : public CDialog
{
public:
	CProcessedOrderGenerateInfoDlg( int nMatched, int nReordered, CWnd* pParent = NULL);   

	//{{AFX_DATA(CProcessedOrderGenerateInfoDlg)
	enum { IDD = IDD_ORDER_GENERATE_INFO };
	CStatic	m_staticReordered;
	CStatic	m_staticMatched;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CProcessedOrderGenerateInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CProcessedOrderGenerateInfoDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nMatched;
	int m_nReordered;
};

/**********************************************************************/
//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif 
/**********************************************************************/
