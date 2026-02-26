#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ListDataDlg.h"
/**********************************************************************/

class CDefEditTimePlanDlg : public CSSDialog
{
public:
	CDefEditTimePlanDlg( bool bTemplateMode, int nIndex, int nMinIndex, int nMaxIndex, bool bAllowSpin, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent = NULL);  

	//{{AFX_DATA(CDefEditTimePlanDlg)
	enum { IDD = IDD_DEF_EDIT_TIMEPLAN };
	
	CEdit m_editDescription;
	CButton m_checkEnable; 
	CSSComboBox	m_comboType;
	CStatic m_staticAction;
	CStatic m_staticSelection;
	CSSTabbedComboBox m_comboActionDetail;
	CButton m_buttonEdit;
	CStatic m_staticStartDate;
	CDateTimeCtrl m_DatePickerFrom;
	CStatic m_staticEndDate;
	CDateTimeCtrl m_DatePickerTo;
	CStatic m_staticActive;
	CStatic m_staticTo;
	CStatic m_staticEndPeriod;
	CDateTimeCtrl m_TimePickerStart;
	CDateTimeCtrl m_TimePickerEnd;
	CButton m_checkSunday;
	CButton m_checkMonday;
	CButton m_checkTuesday;
	CButton m_checkWednesday;
	CButton m_checkThursday;
	CButton m_checkFriday;
	CButton m_checkSaturday;
	CSpinButtonCtrl	m_SpinCtrl;
	CString	m_strDescription;
	int m_nPeriod;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDefEditTimePlanDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	int GetIndex() { return m_nIndex; }
	void SetListDataDlg( CListDataDlg* pDlg ) { m_pListDataDlg = pDlg; }

private:
	CEdit* GetEditPeriod();
	CEdit* GetEditSelection();

private:
	void GetData();
	void SaveData();
	void SpinPrevious();
	void SpinNext();

	void FillSelectionCombo();
	void ForceSelection( int nSelection );
	void EditPromo();
	void EditMixMatch();
	void SetWeekdayFlags( bool b );

protected:
	//{{AFX_MSG(CDefEditTimePlanDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDefaultButton();
	virtual void OnOK();
	afx_msg void OnSelectType();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonAll();
	afx_msg void OnButtonNone();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
private:
	CListDataDlg* m_pListDataDlg = NULL;
	CListDataDlgChangeFlag& m_WarnCancelAfterChange;
	int m_nLastType = 0;
	int m_nIndex = 0;
	int m_nMinIndex = 0;
	int m_nMaxIndex = 0;
	bool m_bAllowSpin = FALSE;

private:
	CString m_strInitialName = "";
	int m_nInitialType = 0;
	int m_nInitialSelection = 0;

private:
	bool m_bTemplateMode = FALSE;
	CTimePlanEntriesCSVArray* m_pTimePlanEntryArray = NULL;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/
