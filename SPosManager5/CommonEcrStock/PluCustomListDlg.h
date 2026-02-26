#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DateSelectorSingle.h"
#include "DepartmentSelector.h"
#include "KeyboardFilterSelector.h"
#include "KeyboardFilterPluArray.h"
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/
#include "ReportPluCustomList.h"
/**********************************************************************/

class CPluCustomListDlg : public CSSDialog
{
public:
	CPluCustomListDlg( CPluFilterArray& FilterArray, int nMode, int nRepNum, CWnd* pParent = NULL);   
	
	//{{AFX_DATA(CPluCustomListDlg)
	enum { IDD = IDD_PLU_CUSTOM_LIST };
	CStatic			m_staticKeyboard;
	CSSComboBoxEx	m_comboKeyboardLoc;
	CComboBox		m_comboKeyboardNum;
	CStatic			m_staticDepartment;
	CEdit			m_editDeptNo;
	CSSComboBox		m_comboDeptName;
	CStatic			m_staticDownload1;
	CStatic			m_staticDownload2;
	CStatic			m_staticDownload3;
	CSSComboBox		m_comboDates;
	CDateTimeCtrl	m_DatePickerFrom;
	CButton			m_checkModifiers;
	CButton			m_checkZeroPriced;
	CEdit			m_editName;
	CButton			m_checkSeparate;
	CButton			m_checkDeptSort;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPluCustomListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPluCustomListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnStaticRange();
	afx_msg void OnKillFocusPluNo();
	afx_msg void OnSelectKeyboardLoc();
	afx_msg void OnKillFocusEditDeptNumber();
	afx_msg void OnSelchangeComboDeptname();
	afx_msg void OnSelectDate();
	afx_msg void OnChangeDate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonUp();
	afx_msg void OnButtonDown();
	afx_msg void OnButtonResetOrder();
	afx_msg void OnButtonAll();
	afx_msg void OnButtonNone();
	afx_msg void OnButtonSave();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void CreateSavedSettingsLine();
	void CreateReportFieldsLine();
	void AppendFieldTicks( CCSV& csv );
	void AddPriceFields();
	void AddProgramFields();
	void AddCustomFields();
	void AddAvailableField( CustomPluField Field );
	void AddFieldToList( const char* szField, int nData, int nTagState );
		
private:
	CEdit* GetEditPluFrom();
	CEdit* GetEditPluTo();

public:
	int m_nDepartmentFilter;
	CString m_strPluFrom;
	CString m_strPluTo;

private:
	CSSListTaggedSelectCtrl m_listFields;
	CDateSelectorSingle m_DateSelectorSingle;
	CDepartmentSelector m_DepartmentSelector;
	CKeyboardFilterSelector m_KeyboardFilterSelector;
	CPluFilterArray& m_FilterArray;
	CKeyboardFilterPluArray m_KeyboardFilterPluArray;
	CString m_strSavedSettingsLine;
	CString m_strReportFieldsLine;
	
private:
	int m_nMode;
	int m_nRepNum;
	CString m_strWindowTitle;
	CString m_strFilenameSettings;

private:
	CArray<int,int> m_arrayDefaultFields;
	CReportConsolidationArray<CSortedStringByInt> m_arrayAvailableFields;
};

/**********************************************************************/
#endif
/**********************************************************************/

