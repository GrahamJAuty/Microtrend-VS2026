/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPropPageHiddenOptionsStockman : public CSSPropertyPage
{
public:
	CPropPageHiddenOptionsStockman();
	~CPropPageHiddenOptionsStockman();

	//{{AFX_DATA(CPropPageHiddenOptionsStockman)
	enum { IDD = IDD_PROPPAGE_HIDDEN_STOCKMAN };
	CSSComboBox	m_comboPluFlag9;
	CButton	m_checkPluFlag9;
	CButton m_checkPluFlag11;
	CButton	m_checkStockFlag1;
	CButton	m_checkStockFlag2;
	CButton	m_checkStockFlag6;
	CButton	m_checkStockFlag7;
	CButton	m_checkStockFlag8;
	CButton	m_checkStockFlag10;
	CButton	m_checkStockFlag11;
	CButton	m_checkStockFlag12;
	CButton	m_checkStockFlag13;
	CButton	m_checkAuditFlag1;
	CButton	m_checkAuditFlag2;
	CButton	m_checkOrderFlag1;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPageHiddenOptionsStockman)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPageHiddenOptionsGlobals)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCheckPluFlag9();
	DECLARE_MESSAGE_MAP()

private:
	void UpdateControls();
	
private:
	CSSComboBox m_comboStockFlag4;
	CSSComboBox m_comboStockFlag5;
};

/**********************************************************************/
#endif
/**********************************************************************/
