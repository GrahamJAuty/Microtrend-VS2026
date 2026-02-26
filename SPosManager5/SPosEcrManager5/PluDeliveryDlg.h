#pragma once
/**********************************************************************/
#ifndef STOCKMAN_SYSTEM
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DepartmentSelector.h"
#include "EcrmanSupplierSelector.h"
/**********************************************************************/

class CPluDeliveryDlg : public CSSDialog
{
public:
	CPluDeliveryDlg( int nPluIdx, CWnd* pParent = NULL );
	~CPluDeliveryDlg();

	//{{AFX_DATA(CPluDeliveryDlg)
	CEdit	m_editSupplier;
	CSSComboBox	m_comboSupplier;
	CEdit	m_editDeptNo;
	CSSComboBox	m_comboTaxBand;
	CSSComboBox	m_comboDeptName;
	CEdit	m_editRepText;
	CEdit	m_editEposText;
	CString	m_strEposText;
	CString	m_strRepText;
	int		m_nGPType;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPluDeliveryDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	void EditDepartments ( bool bFlag ) { m_bEditDepartments = bFlag; }
	
private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();
	void UpdatePrice1( int nTaxBand );
	void SetGPEnables();
	
private:
	void SetPrice1GPFixed ( double dTaxRate );
	void SetPrice1GPPercent ( double dTaxRate );

protected:
	//{{AFX_MSG(CPluDeliveryDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectDept();
	afx_msg void OnKillFocusDeptNo();
	afx_msg void OnCalculateProfit();
	afx_msg void OnRadioGpFixed();
	afx_msg void OnRadioGpPercent();
	afx_msg void OnRadioGpType();
	virtual void OnOK();
	afx_msg void OnKillFocusSupplier();
	afx_msg void OnSelectSupplier();
	afx_msg void OnButtonSupplier();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void FillTaxCombo();
	int GetTaxBand();

private:
	int m_nPluIdx;
	CDepartmentSelector m_DepartmentSelector;
	CPluCSVRecord m_PluRecord;

private:
	CEdit* GetEditCost();
	CEdit* GetEditGPFixed();
	CEdit* GetEditGPPercent();
	CEdit* GetEditBasePrice();
	CEdit* GetEditDelivery();
	CEdit* GetEditApparent();

private:
	double m_dBasePrice;
	
private:
	int m_nDeptNo;
	int m_nSuppNo;
	double m_dCost;
	double m_dGPFixed;
	double m_dGPPercent;
	double m_dApparent;
	double m_dDelivery;
	bool m_bEditDepartments;

private:
	CEcrmanSupplierSelector m_EcrmanSupplierSelector;
};

/**********************************************************************/
#endif 
/**********************************************************************/
#endif 
/**********************************************************************/
