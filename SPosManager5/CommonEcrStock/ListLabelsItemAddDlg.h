#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "BarcodeFilterArray.h"
#include "LabelListCSVArray.h"
//include "MySSDialog.h"
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/
#define BARCODE_ITEM_BARCODE 1
#define BARCODE_ITEM_PLU 2
/**********************************************************************/

class CListLabelsItemAddDlg : public CSSDialog
{
public:
	CListLabelsItemAddDlg( bool bProductLabel, CLabelListCSVRecord& LabelListRecord, CBarcodeFilterArray& BarcodeFilterArray, CPluFilterArray& PluFilterArray, CWnd* pParent = NULL);   

	//{{AFX_DATA(CListLabelsItemAddDlg)
	enum { IDD = IDD_LIST_LABELS_ITEM_ADD };
	CButton m_radioBarcode;
	CButton	m_buttonBrowseBarcode;
	CButton m_radioPlu;
	CButton	m_buttonBrowsePlu;
	CStatic m_staticDescription;
	CEdit m_editDescription;
	CStatic m_staticModifier;
	CEdit m_editModifier;
	CStatic m_staticQty;
	CButton m_buttonOK;
	CButton m_buttonCancel;
	CString	m_strDescription;
	CString	m_strModifier;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CListLabelsItemAddDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CListLabelsItemAddDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnKillFocusBarcode();
	afx_msg void OnKillFocusPlu();
	afx_msg void OnButtonBrowseBarcode();
	afx_msg void OnButtonBrowsePlu();
	afx_msg void OnRadioBarcode();
	afx_msg void OnRadioPlu();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void UpdateEditBoxes( int nMode );
	void SwitchMode( int nMode );

private:
	CEdit* GetEditBarcodeNo();
	CEdit* GetEditPluNo();
	CEdit* GetEditQty();
	
private:
	CBarcodeFilterArray& m_BarcodeFilterArray;
	CPluFilterArray& m_PluFilterArray;
		
private:
	CLabelListCSVRecord& m_LabelListRecord;
	__int64 m_nBarcodeNo;
	__int64 m_nPluNo;
	int m_nModifier;
	int m_nBarcodeQty;
	
private:
	CString m_strModifierNotAllowed;
	bool m_bProductLabel;
	int m_nNewItemMode;
};

/**********************************************************************/

