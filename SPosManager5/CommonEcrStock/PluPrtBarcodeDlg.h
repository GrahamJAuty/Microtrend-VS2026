#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DepartmentSelector.h"
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/

class CPluPrtBarcodeDlg : public CSSDialog
{
public:
	CPluPrtBarcodeDlg( CPluFilterArray& FilterArray, CWnd* pParent = NULL);   
	
	//{{AFX_DATA(CPluPrtBarcodeDlg)
	enum { IDD = IDD_PLU_PRT_BARCODE };
	CEdit	m_editDeptNo;
	CButton	m_checkDeptSort;
	CSSComboBox	m_comboDeptName;
	BOOL	m_bSortByDept;
	BOOL	m_bSeparate;
	BOOL	m_bDirect;
	BOOL	m_bZeroPriced;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPluPrtBarcodeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPluPrtBarcodeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSave();
	afx_msg void OnSelchangeComboDeptname();
	afx_msg void OnKillFocusEditDeptNumber();
	afx_msg void OnKillFocusPluNo();
	virtual void OnOK();
	afx_msg void OnButtonDefault();
	afx_msg void OnStaticRange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	const char* GetCSVLine();

private:
	void SetDefaults();

private:
	CEdit* GetEditPluFrom();
	CEdit* GetEditPluTo();

public:
	int m_nDepartmentFilter;
	CString m_strPluFrom;
	CString m_strPluTo;

private:
	CDepartmentSelector m_DepartmentSelector;
	CPluFilterArray& m_FilterArray;
	CString m_strFieldSelect;
};

/**********************************************************************/

class CCSVPluPrtBarcodeKey : public CCSV
{
public:
	CCSVPluPrtBarcodeKey ( const char* szCSVLine ) : CCSV(szCSVLine){}
	bool PrintInhibited() { return GetBool(0); }
	bool PrintZeroPriced() { return GetBool(1); }
	bool PrintSeparate() { return GetBool(2); }
	bool PrintDirect(){ return GetBool(3); }
};

/**********************************************************************/

