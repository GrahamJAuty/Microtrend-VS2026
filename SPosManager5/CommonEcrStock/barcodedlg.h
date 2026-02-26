#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DepartmentSelector.h"
#include "..\CommonEcrStock\LocationSelector.h"
//include "MyComboBoxEx.h"
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/

class CBarcodeDlg : public CSSDialog
{
public:
	CBarcodeDlg( bool bShelf, CWnd* pParent = NULL);  

public:
	void SetPluNoFrom( __int64 nPluNo ){ m_nPluNoFrom = nPluNo; }
	__int64 GetPluNoFrom(){ return m_nPluNoFrom; }
	void SetPluNoTo( __int64 nPluNo ){ m_nPluNoTo = nPluNo; }
	__int64 GetPluNoTo(){ return m_nPluNoTo; }
	
	void SetBarcodeNoFrom( __int64 nBarcodeNo ){ m_nBarcodeNoFrom = nBarcodeNo; }
	__int64 GetBarcodeNoFrom(){ return m_nBarcodeNoFrom; }
	void SetBarcodeNoTo( __int64 nBarcodeNo ){ m_nBarcodeNoTo = nBarcodeNo; }
	__int64 GetBarcodeNoTo(){ return m_nBarcodeNoTo; }

	//{{AFX_DATA(CBarcodeDlg)
	enum { IDD = IDD_PLU_BARCODE };
	CSSComboBoxEx	m_comboLocation;
	CButton	m_radioType2;
	CButton	m_radioType;
	CStatic	m_staticType;
	CEdit	m_editDeptNo;
	CSSComboBox	m_comboDeptName;
	int		m_nLabel;
	int		m_nItemLabels;
	int		m_nType;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CBarcodeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CBarcodeDlg)
	afx_msg void OnSelchangeComboDeptname();
	afx_msg void OnKillFocusDeptNumber();
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocusPluNo();
	afx_msg void OnStaticRange();
	afx_msg void OnStaticRange2();
	afx_msg void OnKillFocusBarcodeNo();
	afx_msg void OnRadioType();
	afx_msg void OnRadioType2();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CLocationSelectorEntity m_LocSelEntity;
	int m_nDepartmentFilter;
	
private:
	CEdit* GetEditPluFrom();
	CEdit* GetEditPluTo();
	CEdit* GetEditBarcodeFrom();
	CEdit* GetEditBarcodeTo();
	CEdit* GetEditItemLabel();

private:
	CDepartmentSelector m_DepartmentSelector;
	CLocationSelector m_LocationSelector;
	bool m_bShelf;

private:
	__int64 m_nPluNoFrom;
	__int64 m_nPluNoTo;
	__int64 m_nBarcodeNoFrom;
	__int64 m_nBarcodeNoTo;

	CString m_strPluNoFrom;
	CString m_strPluNoTo;
	CString m_strBarcodeNoFrom;
	CString m_strBarcodeNoTo;
};

/**********************************************************************/
