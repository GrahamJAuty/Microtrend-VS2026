//$$******************************************************************
#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRowGroup.h"
//$$******************************************************************

class CGroupPropertyPage1 : public CSSPropertyPage
{
public:
	CGroupPropertyPage1(CSQLRowGroup& RowGroup);
	virtual ~CGroupPropertyPage1();
	
	enum { IDD = IDD_PROPPAGE_GROUP1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	/****/
	afx_msg void OnSelectRefresh();
	afx_msg void OnButtonRefresh();
	/****/
	afx_msg void OnSelectSpend();
	afx_msg void OnButtonSpend();
	DECLARE_MESSAGE_MAP()

private:
	bool ValidateMaxSpend(CEdit* pEdit, double dMin, double dMax, CString strType);

private:
	CEdit m_editName;
	CSSTabbedComboBox m_comboScheme;
	CButton m_checkPluBonus;
	CButton m_checkDeptBonus;
	CButton m_checkComplimentary;
	CButton m_checkStampOffer;
	/*****/
	CSSComboBox m_comboRefresh;
	CEdit m_editRefreshDays;
	CButton m_buttonRefresh;
	/*****/
	CSSComboBox m_comboSpend;
	CStatic m_staticOverSpend;
	CButton m_buttonSpend;
	/*****/
	double m_dRefreshValue;
	/*****/
	double m_dOverSpend;
	double m_dMaxSpendValue;

private:
	CEdit* GetEditOverSpend();
	CEdit* GetEditMaxSpend();
	CEdit* GetEditRefresh();

private:
	void DisplayRefreshDays();
	void SetDisplayMaxSpend();
	void SetAuditPeriodButton();
	
private:
	int m_nRefreshYearlyMonth;
	int m_nRefreshYearlyDay;
	int m_nRefreshWeeklyDay;
	bool m_bRefreshDay0;
	bool m_bRefreshDay1;
	bool m_bRefreshDay2;
	bool m_bRefreshDay3;
	bool m_bRefreshDay4;
	bool m_bRefreshDay5;
	bool m_bRefreshDay6;

private:
	CSQLRowGroup& m_RowGroup;
};

//$$******************************************************************
