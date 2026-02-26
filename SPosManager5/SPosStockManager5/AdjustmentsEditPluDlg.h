#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\PluFilterArray.h"
#include "StockFilterArray.h"
#include "StocktakeCSVArray.h"
/**********************************************************************/

class CAdjustmentsEditPluDlg : public CSSDialog
{
public:
	CAdjustmentsEditPluDlg( int nSpIdx, const char* szSpOrLoc, CString& strLastAdjustment, CAdjustmentsEditPluDlg** pParentToThis, int nStkTakeNo, CWnd* pParent = NULL);   
	CAdjustmentsEditPluDlg( int nSpIdx, const char* szSpOrLoc, CString& strDummy, int nStkTakeNo, CWnd* pParent = NULL);   

	//{{AFX_DATA(CAdjustmentsEditPluDlg)
	CSSComboBox m_comboReason;
	CButton	m_buttonCancel;
	CButton	m_buttonBrowsePlu;
	CButton	m_buttonBrowseStock;
	CButton	m_radioStockCode;
	CButton	m_radioPluNumber;
	CButton	m_buttonOK;
	CButton m_staticValue;
	CEdit	m_editStockCode;
	double	m_dValue;
	//}}AFX_DATA
	CStatic m_staticModifier1;
	CStatic m_staticModifier2;
	CSSTabbedComboBox m_comboLevel;
	CSSComboBox m_comboModifier;
	CEdit m_editMultiplier;

	//{{AFX_VIRTUAL(CAdjustmentsEditPluDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

public:
	CString m_strInputLine;
	CString m_strOutputLine;

protected:
	//{{AFX_MSG(CAdjustmentsEditPluDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocusStockCode();
	afx_msg void OnButtonBrowseStock();
	afx_msg void OnButtonSet();
	afx_msg void OnKillFocusPluNo();
	afx_msg void OnButtonBrowsePlu();
	afx_msg void OnKillFocusQty();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnRadioPluNumber();
	afx_msg void OnRadioStockCode();
	//}}AFX_MSG
	afx_msg void OnSelectLevel();
	afx_msg void OnSelectModifier();
	DECLARE_MESSAGE_MAP()

private:
	void DataInit( const char* szSpOrLoc );
	void UpdatePluNumber( int nPluIdx );
	void UpdateItemValue ( CPluCSVRecord& PluRecord, double dQty );
	void SetEditFocus();
	int HandleAdd();

private:
	bool LookUpPluNumber( __int64& nPluNo, CString& strStockCode, int& nPluIdx );
	bool LookUpStockCode( __int64& nPluNo, CString& strStockCode, int& nPluIdx );
	void UpdateModifierCombo();
	
private:
	void BrowsePluInternal();
	void BrowseStockInternal();

private:
	CEdit* GetEditQty();
	CEdit* GetEditPluNo();
	CEdit* GetEditValue();
	
private:
	bool m_bAdd;
	bool m_bModified;
	int m_nEntryMode;
	int m_nPluIdx;
	int m_nModifier;
	double m_dMultiplier;
	CString m_strSpOrLoc;
	
private:
	CPluFilterArray m_FilterArray;
	CStockFilterArray m_StockFilterArray;

private:
	CString m_strStockCode;
	CString m_strDescription;
	CString m_strPluNo;
	CString& m_strLastAdjustment;
	
private:
	CAdjustmentsEditPluDlg** m_pParentToThis;

private:
	double m_dQuantity;	

private:
	int m_nSpIdx;
	int m_nStkTakeNo;
};

/**********************************************************************/
#endif
/**********************************************************************/

