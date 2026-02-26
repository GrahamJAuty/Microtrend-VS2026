#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DateSelectorSingle.h"
#include "..\CommonEcrStock\LocationSelector.h"
/**********************************************************************/

class CPluDeleteUnusedDlg : public CDialog
{
public:
	CPluDeleteUnusedDlg( CWnd* pParent = NULL );   

	//{{AFX_DATA(CPluDeleteUnusedDlg)
	enum { IDD = IDD_PLU_DELETE_UNUSED };
	CSSComboBoxEx	m_comboDatabase;
	CSSComboBox	m_comboDates;
	CDateTimeCtrl	m_DatePickerTo;
	int m_nAction;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPluDeleteUnusedDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	afx_msg void OnChangeDate(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_VIRTUAL

private:
	int DeletePluInternal( int nDbIdx, COleDateTime& date );

public:
	CString m_strDate;

protected:
	//{{AFX_MSG(CPluDeleteUnusedDlg)
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
