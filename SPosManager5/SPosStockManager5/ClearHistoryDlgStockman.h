#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DateSelectorSingle.h"
#include "..\CommonEcrStock\LocationSelector.h"
/**********************************************************************/

class CClearHistoryDlgStockman : public CDialog
{
public:
	CClearHistoryDlgStockman( CWnd* pParent = NULL);   

	//{{AFX_DATA(CClearHistoryDlgStockman)
	enum { IDD = IDD_CLEAR_HISTORY_STOCKMAN };
	CSSComboBoxEx	m_comboStockpoint;
	CSSComboBox	m_comboDates;
	CDateTimeCtrl	m_DatePickerTo;
	CSSComboBoxEx	m_comboLocation;
	CSSTabbedComboBox	m_comboTerminal;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CClearHistoryDlgStockman)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CClearHistoryDlgStockman)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelectLocation();
	afx_msg void OnSelectDate();
	afx_msg void OnChangeDate(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	static bool IsAvailable();

private:
	static bool IsEposAvailable();
	static bool IsStockAvailable();
	
private:
	CButton m_checkHistory[5];
	int m_checkHistoryID[5];
	int m_nHistoryType[5];

private:
	CDateSelectorSingle m_DateSelectorSingle;
	CLocationSelector m_LocationSelector;
	CLocationSelector m_StockpointSelector;
};

/**********************************************************************/
#endif
/**********************************************************************/
