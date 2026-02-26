#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CAdhocPreferredSupplierDlg : public CDialog
{
public:
	CAdhocPreferredSupplierDlg( int nStockIdx, int nStockSuppIdx, CWnd* pParent = NULL);   

	//{{AFX_DATA(CAdhocPreferredSupplierDlg)
	CStatic	m_staticDUItemSize2;
	CStatic	m_staticDUItemSize1;
	CStatic	m_staticDUItems2;
	CStatic	m_staticDUItems1;
	CStatic	m_staticCost2;
	CStatic	m_staticCost1;
	CStatic	m_staticWrapper2;
	CStatic	m_staticWrapper1;
	CStatic	m_staticUnit2;
	CStatic	m_staticUnit1;
	CStatic	m_staticCode2;
	CStatic	m_staticCode1;
	CStatic	m_staticName2;
	CStatic	m_staticName1;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAdhocPreferredSupplierDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CAdhocPreferredSupplierDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nStockIdx;
	int m_nStockSuppIdx;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/
