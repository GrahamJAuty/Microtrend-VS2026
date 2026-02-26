#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DepartmentSelector.h"
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/

class CPluRangePriceDlg : public CSSDialog
{
public:
	CPluRangePriceDlg( CPluFilterArray& FilterArray, CWnd* pParent = NULL);   

	//{{AFX_DATA(CPluRangePriceDlg)
	enum { IDD = IDD_PLU_RANGE_PRICE };
	CEdit	m_editDeptNo;
	CSSComboBox	m_comboDeptName;
	CSSTabbedComboBox m_comboCategory;
	int		m_nMethod;
	//}}AFX_DATA
	CButton m_checkPrice[10];
	CButton m_checkPoints;
	BOOL m_bPrice[10];
	BOOL m_bPoints;

	int m_checkPriceID[10];
	int m_editPriceID[10];

	//{{AFX_VIRTUAL(CPluRangePriceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	static bool IsRelevant();

protected:
	//{{AFX_MSG(CPluRangePriceDlg)
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
	afx_msg void OnStaticRange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SetPriceEnables();
	int UpdatePrices();

	CEdit* GetEditPluFrom();
	CEdit* GetEditPluTo();
	CEdit* GetEditCopyLevel();
	CEdit* GetEditPrice( int n );
	CEdit* GetEditPoints();

public:
	CString m_strPluFrom;
	CString m_strPluTo;
	
private:
	CDepartmentSelector m_DepartmentSelector;
	CPluFilterArray& m_FilterArray;

private:
	bool m_bAllowPrice[10];
	double m_dPrice[10];
	bool m_bAllowPoints;
	int m_nPoints;

private:
	int m_nCopyLevel;
};

/**********************************************************************/
#endif
/**********************************************************************/
