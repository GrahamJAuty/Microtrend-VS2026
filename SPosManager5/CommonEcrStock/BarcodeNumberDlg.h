#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "BarcodeCSVArray.h"
#include "BarcodeFilterArray.h"
//include "MyComboBox.h"
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/
#define BARCODE_PLUDB_ADD 0
#define BARCODE_PLUDB_EDIT 1
#define BARCODE_LIST_ADD 2 
#define BARCODE_LIST_EDIT 3
/**********************************************************************/

class CBarcodeNumberDlg : public CSSDialog
{
public:
	CBarcodeNumberDlg( int nMode, CBarcodeCSVRecord& BarcodeRecord, CWnd* pParent = NULL);   

	//{{AFX_DATA(CBarcodeNumberDlg)
	enum { IDD = IDD_BARCODE_NUMBER };
	CButton	m_buttonBrowseBarcode;
	CButton	m_checkPrintShelf;
	CButton	m_checkPrintProduct;
	CStatic m_staticModifier;
	CSSComboBox m_comboModifier;
	CEdit	m_editDescription;
	CButton	m_buttonBrowsePlu;
	CString	m_strBarcodeNo;
	CString	m_strPluNo;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CBarcodeNumberDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CBarcodeNumberDlg)
	afx_msg void OnButtonBrowseBarcode();
	afx_msg void OnButtonBrowsePlu();
	afx_msg void OnKillFocusBarcodeNo();
	afx_msg void OnKillFocusPluNo();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void UpdatePluDetails( CString& strPluNo );
	void UpdateModifierCombo( CString& strPluNo, bool bInitDialog );

private:
	CEdit* GetEditPluNo();
	CEdit* GetEditBarcodeNo();

private:
	CBarcodeCSVRecord& m_BarcodeRecord;
	CPluFilterArray m_PluFilterArray;
	CBarcodeFilterArray m_BarcodeFilterArray;
	int m_nMode;

private:
	__int64 m_nCurrentPluNo;
	__int64 m_nOriginalPluNo;
	int m_nOriginalModifier;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/
