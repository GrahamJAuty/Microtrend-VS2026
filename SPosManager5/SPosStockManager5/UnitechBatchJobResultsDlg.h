#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

struct CUnitechBatchJobResults
{
public:
	CUnitechBatchJobResults();

public:
	int m_nDelivery;
	int m_nWrapper;
	int m_nReturn;
	int m_nSale;
	int m_nAdjust;
	int m_nClosing;
};

/**********************************************************************/

class CUnitechBatchJobResultsDlg : public CDialog
{
public:
	CUnitechBatchJobResultsDlg( CWnd* pParent = NULL );   

	//{{AFX_DATA(CUnitechBatchJobResultsDlg)
	CButton	m_staticDelivery;
	CButton	m_staticWrapper;
	CButton	m_staticAdjust;
	CButton	m_staticClosing;
	CButton	m_staticReturn;
	CButton	m_staticSale;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CUnitechBatchJobResultsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CUnitechBatchJobResultsDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void ShowTotal( CButton& staticCtrl, int nCount );

public:
	CUnitechBatchJobResults m_Results;
};

/**********************************************************************/
#endif
/**********************************************************************/
