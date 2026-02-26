#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "StockCodeSearch.h"
#include "StockFilterArray.h"
/**********************************************************************/

class CStockCodeDlg : public CDialog
{
public:
	CStockCodeDlg( const char* szTitle, CStockCodeSearch& StockCodeSearch, CWnd* pParent = NULL);   

	//{{AFX_DATA(CStockCodeDlg)
	enum { IDD = IDD_STOCK_CODE };
	CButton	m_checkSearchFromStart;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockCodeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CStockCodeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CStockCodeSearch& m_StockCodeSearch;
	CString m_strStockCodeEntered;
	CString m_strStockCodeToUse;
	BOOL	m_bSearchFromStart;
	
private:
	CString m_strTitle;
};

/**********************************************************************/
#endif
/**********************************************************************/
