#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DepartmentSelector.h"
#include "KeyboardFilterSelector.h"
#include "KeyboardFilterPluArray.h"
/**********************************************************************/
#include "ReportPluCustomList.h"
/**********************************************************************/

class CPriceSetCustomListDlg : public CSSDialog
{
public:
	CPriceSetCustomListDlg( int nDbIdx, CWnd* pParent = NULL); 
	~CPriceSetCustomListDlg();
	
	//{{AFX_DATA(CPriceSetCustomListDlg)
	enum { IDD = IDD_PRICESET_CUSTOM_LIST };
	CStatic			m_staticKeyboard;
	CSSComboBoxEx	m_comboKeyboardLoc;
	CComboBox		m_comboKeyboardNum;
	CStatic			m_staticDepartment;
	CEdit			m_editDeptNo;
	CSSComboBox		m_comboDeptName;
	CButton			m_checkModifiers;
	CButton			m_checkZeroPriced;
	CButton			m_checkSeparate;
	CButton			m_checkDeptSort;
	CButton			m_radioPriceSet1;
	CButton			m_radioPriceSet2;
	CButton			m_radioPriceSet3;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPriceSetCustomListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPriceSetCustomListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnStaticRange();
	afx_msg void OnKillFocusPluNo();
	afx_msg void OnSelectKeyboardLoc();
	afx_msg void OnKillFocusEditDeptNumber();
	afx_msg void OnSelchangeComboDeptname();
	afx_msg void OnButtonFieldUp();
	afx_msg void OnButtonFieldDown();
	afx_msg void OnButtonFieldResetOrder();
	afx_msg void OnButtonFieldAll();
	afx_msg void OnButtonFieldNone();
	afx_msg void OnButtonPSetUp();
	afx_msg void OnButtonPSetDown();
	afx_msg void OnButtonPSetResetOrder();
	afx_msg void OnButtonPSetAll();
	afx_msg void OnButtonPSetNone();
	afx_msg void OnButtonSave();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void CreateReportFieldsLine();
	void AppendFieldTicks( CCSV& csv );
	void CreateAvailableFieldList();
	void AddLocations();
	void AddAvailableField( CustomPluField Field, int nTagStateType = TAGSTATETYPE_2STATEBLUE );
	void AddFieldToList( const char* szField, int nData, int nTagState, int nTagStateType );
		
private:
	CEdit* GetEditPluFrom();
	CEdit* GetEditPluTo();

private:
	void SetPluRangeAll();

public:
	int m_nDepartmentFilter;
	CString m_strPluFrom;
	CString m_strPluTo;

private:
	CSSListTaggedSelectCtrl m_listFields;
	CSSListTaggedSelectCtrl m_listLocations;
	CDepartmentSelector m_DepartmentSelector;
	CKeyboardFilterSelector m_KeyboardFilterSelector;
	CKeyboardFilterPluArray m_KeyboardFilterPluArray;
	CString m_strSavedSettingsLine;
	CString m_strReportFieldsLine;
	
private:
	int m_nDbIdx;
	int m_nPriceSet;
	CString m_strWindowTitle;
	CString m_strFilenameSettings;

private:
	CArray<int,int> m_arrayDefaultFields;
	CReportConsolidationArray<CSortedIntAndStringByInt> m_arrayAvailableFields;
};

/**********************************************************************/
#endif
/**********************************************************************/

