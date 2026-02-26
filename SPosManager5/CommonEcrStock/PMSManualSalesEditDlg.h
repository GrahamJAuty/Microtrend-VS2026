#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\PluFilterArray.h"
#include "PMSInvoiceLineInfo.h"
/**********************************************************************/

class CPMSManualSalesEditDlg : public CSSDialog
{
public:
	CPMSManualSalesEditDlg( CPMSManualSalesEditDlg** pParentToThis, int nLineType, CWnd* pParent = NULL);   
	CPMSManualSalesEditDlg( CWnd* pParent = NULL);   

	//{{AFX_DATA(CPMSManualSalesEditDlg)
	enum { IDD = IDD_PMS_MANUAL_SALES_EDIT };
	CButton	m_buttonCancel;
	CButton	m_buttonBrowsePlu;
	CButton	m_buttonOK;
	CButton m_staticValue;
	double	m_dValue;
	double	m_dDiscount;
	double	m_dNet;
	//}}AFX_DATA
	CSSTabbedComboBox m_comboLevel;
	CSSComboBox m_comboModifier;
	CEdit m_editMultiplier;
	CStatic m_staticModifier1;
	CStatic m_staticModifier2;
	CStatic m_staticSales;
	CStatic m_staticDiscount;
	CStatic m_staticNet;

	//{{AFX_VIRTUAL(CPMSManualSalesEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

public:
	CString m_strInputLine;
	CString m_strOutputLine;

protected:
	//{{AFX_MSG(CPMSManualSalesEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocusPluNo();
	afx_msg void OnButtonBrowsePlu();
	afx_msg void OnKillFocusQty();
	afx_msg void OnKillFocusValue();
	afx_msg void OnKillFocusDiscount();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	afx_msg void OnSelectLevel();
	afx_msg void OnSelectModifier();
	DECLARE_MESSAGE_MAP()

private:
	void DataInit();
	void UpdatePluNumber( int nPluIdx );
	void UpdateItemValue ( CPluCSVRecord& PluRecord, double dQty );
	void SetEditFocus();
	int HandleAdd();

private:
	bool LookUpPluNumber( __int64 nPluNo, int& nPluIdx );
	void UpdateModifierCombo();
	
private:
	void BrowsePluInternal();
	
private:
	CEdit* GetEditQty();
	CEdit* GetEditPluNo();
	CEdit* GetEditValue();
	CEdit* GetEditDiscount();
	CEdit* GetEditNet();

public:
	void SetSalesInfo( CPMSInvoiceLineInfo& sale ){ m_SaleInfo = sale; }
	void GetSalesInfo( CPMSInvoiceLineInfo& sale ){ sale = m_SaleInfo; }
	
private:
	bool m_bAdd;
	bool m_bModified;
	int m_nPluIdx;
	int m_nModifier;
	double m_dMultiplier;
	
private:
	CPluFilterArray m_FilterArray;
	
private:
	CString m_strDescription;
	CString m_strPluNo;
	
private:
	CPMSManualSalesEditDlg** m_pParentToThis;

private:
	double m_dQuantity;	

private:
	CPMSInvoiceLineInfo m_SaleInfo;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
