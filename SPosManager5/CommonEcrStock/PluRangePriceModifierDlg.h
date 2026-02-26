#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DepartmentSelector.h"
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/

class CPluRangePriceModifierDlg : public CSSDialog
{
public:
	CPluRangePriceModifierDlg( CPluFilterArray& FilterArray, CWnd* pParent = NULL);   

	//{{AFX_DATA(CPluRangePriceModifierDlg)
	enum { IDD = IDD_PLU_RANGE_PRICE_MODIFIER };
	CEdit	m_editDeptNo;
	CTabCtrl m_TabModifiers;
	CSSComboBox	m_comboDeptName;
	CSSTabbedComboBox m_comboCategory;
	CSSComboBox	m_comboModifierEnable;
	CStatic m_staticModifier;
	int	m_nModifierType;
	int	m_nMethod;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CPluRangePriceModifierDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	static bool IsRelevant();

protected:
	//{{AFX_MSG(CPluRangePriceModifierDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboDeptname();
	afx_msg void OnKillFocusDeptNumber();
	afx_msg void OnCheckPrice();
	virtual void OnOK();
	afx_msg void OnKillFocusPluNo();
	afx_msg void OnKillFocusLevel();
	afx_msg void OnRadioCopy();
	afx_msg void OnRadioFixed();
	afx_msg void OnRadioPercent();
	afx_msg void OnRadioSetAt();
	afx_msg void OnButtonAll();
	afx_msg void OnButtonNone();
	afx_msg void OnStaticRange();
	afx_msg void OnToggleBasePlu();
	afx_msg void OnToggleModifier();
	afx_msg void OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SetPriceEnables();
	void ToggleModifier( int nMod );
	void TogglePrice( int nPrice );
	int UpdatePrices();

private:
	CEdit* GetEditPluFrom();
	CEdit* GetEditPluTo();
	CEdit* GetEditModifier( int nMod, int nPrice );
	CEdit* GetEditCopyLevel();

public:
	CString m_strPluFrom;
	CString m_strPluTo;
	
private:
	CDepartmentSelector m_DepartmentSelector;
	CPluFilterArray& m_FilterArray;

	int m_editPriceID[2][10];
	int m_checkPriceID[2][10];
	CButton m_checkPrice[2][10];
	
	int m_nCopyLevel;

private:
	int m_nActiveTab;
	bool m_bAllowModEnable[10];
	bool m_bAllowModTextEpos[10];
	bool m_bAllowModTextKey[10];
	bool m_bAllowPrice[10][10];
	double m_dPrice[10][10];
	BOOL m_bPrice[10][10];
};

/**********************************************************************/
#endif
/**********************************************************************/
