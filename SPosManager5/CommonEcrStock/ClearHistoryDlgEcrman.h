#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DateSelectorSingle.h"
#include "..\CommonEcrStock\LocationSelector.h"
/**********************************************************************/

class CClearHistoryDlgEcrman : public CDialog
{
public:
	CClearHistoryDlgEcrman( CWnd* pParent = NULL);   

	//{{AFX_DATA(CClearHistoryDlgEcrman)
	enum { IDD = IDD_CLEAR_HISTORY_ECRMAN };
	CSSComboBox	m_comboDates;
	CDateTimeCtrl	m_DatePickerTo;
	CSSComboBoxEx	m_comboLocation;
	CSSTabbedComboBox	m_comboTerminal;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CClearHistoryDlgEcrman)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CClearHistoryDlgEcrman)
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
	CButton m_checkHistory[4];
	int m_checkHistoryID[4];
	int m_nHistoryType[4];

private:
	CDateSelectorSingle m_DateSelectorSingle;
	CLocationSelector m_LocationSelector;
};

/**********************************************************************/
