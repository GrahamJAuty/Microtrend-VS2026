#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DateSelectorSingle.h"
#include "..\CommonEcrStock\LocationSelector.h"
/**********************************************************************/

class CStockDeleteUnusedDlg : public CDialog
{
public:
	CStockDeleteUnusedDlg( CWnd* pParent = NULL );   

	//{{AFX_DATA(CStockDeleteUnusedDlg)
	enum { IDD = IDD_STOCK_DELETE_UNUSED };
	CSSComboBoxEx	m_comboDatabase;
	CSSComboBox	m_comboDates;
	CDateTimeCtrl	m_DatePickerTo;
	BOOL	m_bKeepApparent;
	BOOL	m_bKeepOnOrder;
	BOOL	m_bKeepSales;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockDeleteUnusedDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	afx_msg void OnChangeDate(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_VIRTUAL

private:
	int DeleteStockInternal( int nDbIdx, COleDateTime& date );

public:
	CString m_strDate;

protected:
	//{{AFX_MSG(CStockDeleteUnusedDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelectDate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CDateSelectorSingle m_DateSelectorSingle;
	CLocationSelector m_LocationSelector;
};

/**********************************************************************/
#endif	
/**********************************************************************/

