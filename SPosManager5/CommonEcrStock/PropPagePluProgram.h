#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "PictCtrl.h"
#include "PluCSVArray.h"
/**********************************************************************/

class CPropPagePluProgram : public CSSPropertyPage
{
public:
	CPropPagePluProgram();
	~CPropPagePluProgram();

	//{{AFX_DATA(CPropPagePluProgram)
	enum { IDD = IDD_PROPPAGE_PLU_PROGRAM };
	
	CSSTabbedComboBox m_comboOPGroup;
	CButton	m_checkPrinter[PLU_ORDER_PRINTERS_NOW];
	/*****/
	CSSComboBox	m_comboPriceType;
	CSSComboBox	m_comboAgeType;
	CButton m_checkKPNotAlone;
	CButton m_checkKPTopItem;
	CButton m_checkAutoCoversQty;
	CButton m_checkEANOnReceipt;
	CButton m_checkSalesTracking;
	CButton m_checkSoldOut;
	CButton	m_checkDiscountable;
	CButton	m_checkStaffDiscount;
	CButton	m_checkFreeText;
	CButton	m_checkFreeTextDescription;
	CButton	m_checkAccountRequired;
	/*****/
	CTabCtrl m_TabImage;
	CPictureCtrl m_staticImage;
	CEdit m_editWidth;
	CEdit m_editHeight;
	CEdit m_editImageName;
	CButton m_buttonImage;
	CButton m_buttonDelete;
	CButton m_checkPrint;
	CButton m_checkModifier;
	/*****/
	int		m_nPriority;
	//}}AFX_DATA
	WORD m_IDPrinter[PLU_ORDER_PRINTERS_NOW];

	//{{AFX_VIRTUAL(CPropPagePluProgram)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	void SetReadOnlyFlag( bool b ){ m_bReadOnly = b; }

protected:
	//{{AFX_MSG(CPropPagePluProgram)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectPrinterGroup();
	afx_msg void OnSelectFreeText();
	afx_msg void OnButtonImage();
	afx_msg void OnButtonDelete();
	afx_msg void OnSelectTabImage(NMHDR *pNMHDR, LRESULT *pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetPluRecord ( CPluCSVRecord* pPluRecord ) { m_pPluRecord = pPluRecord; }
	void Refresh();
	bool UpdateRecord();

private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();
	void FillOPGroupCombo();
	void FillPriceTypeCombo();
	void FillAgeTypeCombo();
	void SavePriceType();
	void SaveAgeType();
	void SelectTabImage();

private:
	CEdit* GetEditPriority();

private:
	CPluCSVRecord* m_pPluRecord;
	bool m_bReadOnly;
	CString m_strInitialDir;
	int m_nTabImage;
};

/**********************************************************************/
#endif
/**********************************************************************/
