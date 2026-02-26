/**********************************************************************/
#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPropPageHiddenOptionsEcrman : public CSSPropertyPage
{
public:
	CPropPageHiddenOptionsEcrman();
	~CPropPageHiddenOptionsEcrman();

	//{{AFX_DATA(CPropPageHiddenOptionsEcrman)
	enum { IDD = IDD_PROPPAGE_HIDDEN_ECRMAN };
	CSSComboBox	m_comboLabelFlag2;
	CSSComboBox	m_comboReportFlag9;
	CButton	m_checkCashRSPFlag1;
	CButton	m_checkCashRSPFlag2;
	CButton	m_checkCashRSPFlag3;
	CButton	m_checkCashRSPFlag4;
	CButton	m_checkCashRSPFlag6;
	CButton	m_checkCashRSPFlag7;
	CButton	m_checkCashRSPFlag8;
	CButton	m_checkPluFlag6;
	CButton	m_checkPluFlag8;
	CButton	m_checkLabelFlag1;
	CEdit	m_editLocCombo;
	CButton	m_checkReportFlag1;
	CButton	m_checkReportFlag8;
	CButton	m_checkReportFlag17;
	CButton	m_checkReportFlag18;
	CButton	m_checkReportFlag19;
	CButton	m_checkReportFlag20;
	CButton	m_checkGraphFlag1;
	CButton m_checkPluFlag10;
	CButton m_checkPluFlag12;
	CEdit m_editPluFlag10;
	int		m_nPluFlagMode;
	CButton m_buttonBespoke;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPageHiddenOptionsEcrman)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropPageHiddenOptionsEcrman)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocusLocCombo();
	afx_msg void OnCheckReportFlag1();
	afx_msg void OnCheckPluflag10();
	afx_msg void OnButtonBespoke();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void UpdateControls();
	void ValidateLocComboString();
};


/**********************************************************************/
