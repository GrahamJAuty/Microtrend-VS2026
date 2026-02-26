#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CStockDeleteSingleDlg : public CDialog
{
public:
	CStockDeleteSingleDlg( CStockCSVRecord& StockRecord, CStockLevelsCSVRecord& StockLevels, CWnd* pParent = NULL); 

	//{{AFX_DATA(CStockDeleteSingleDlg)
	enum { IDD = IDD_STOCK_DELETE_SINGLE };
	CStatic	m_staticOnOrder;
	CStatic	m_staticDescription;
	CStatic	m_staticStockCode;
	CStatic	m_staticCategory;
	CStatic	m_staticApparent;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockDeleteSingleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CStockDeleteSingleDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CStockCSVRecord& m_StockRecord;
	CStockLevelsCSVRecord& m_StockLevels;
};


//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/
