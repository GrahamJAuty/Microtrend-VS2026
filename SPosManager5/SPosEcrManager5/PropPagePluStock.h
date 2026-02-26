#pragma once
/**********************************************************************/
#ifndef STOCKMAN_SYSTEM
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "PluCSVArray.h"
#include "EcrmanSupplierSelector.h"
/**********************************************************************/

class CPropPagePluStock : public CSSPropertyPage
{
public:
	CPropPagePluStock();
	~CPropPagePluStock();
	//{{AFX_DATA(CPropPagePluStock)
	enum { IDD = IDD_PROPPAGE_PLU_STOCK };
	CButton	m_buttonSupplier;
	CButton	m_buttonClear;
	CButton	m_buttonAdd;
	CEdit	m_editSuppRef;
	CEdit	m_editSupplier;
	CSSComboBox	m_comboSupplier;
	double	m_dApparent;
	double	m_dMinimum;
	CString	m_strSupplierRef;
	BOOL	m_bNonStockItem;
	CButton m_checkNonStockItem;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPagePluStock)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPagePluStock)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonClear();
	afx_msg void OnKillFocusSupplier();
	afx_msg void OnSelectSupplier();
	afx_msg void OnButtonSupplier();
	afx_msg void OnToggleNonStockItem();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetReadOnlyFlag( bool b ){ m_bReadOnly = b; }

public:
	void SetPluRecord ( CPluCSVRecord* pPluRecord ) { m_pPluRecord = pPluRecord; }
	void SetPluAuditMemory ( CStringArray* pMemory ) { m_pPluAuditMemory = pMemory; }
	void Refresh();
	bool UpdateRecord();

private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();
	void UpdateEnables();

private:
	CEdit* GetEditMinimum();

private:
	CPluCSVRecord* m_pPluRecord;
	CStringArray* m_pPluAuditMemory;

private:
	CEcrmanSupplierSelector m_EcrmanSupplierSelector;
	int m_nSuppNo;
	bool m_bReadOnly;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
