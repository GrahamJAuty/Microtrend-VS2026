#pragma once
//$$******************************************************************
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRowGroupFull.h"
//$$******************************************************************

class CGroupPropertyPage7 : public CSSAutoShutdownPropertyPage
{
public:
	CGroupPropertyPage7(CSQLRowGroupFull& RowGroup);
	virtual ~CGroupPropertyPage7();
	
// Dialog Data
	enum { IDD = IDD_PROPPAGE_GROUP7 };
	CSSComboBox m_comboMaxSpendType;
	CSSComboBox m_comboMaxSpendOverride;
	CStatic m_staticPeriod;
	CStatic m_staticStart;
	CStatic m_staticPurse1;
	CStatic m_staticMaxSpend;
	CStatic m_staticOverdraft;
	CStatic m_staticPurse2;
	CStatic m_staticPurse3;
	CStatic m_staticMaxSpendP3;
	CStatic m_staticOverdraftP3;	;
	CEdit m_editPeriodText[7];
	CStatic m_staticPeriodStart[7];
	CButton m_checkPeriodAllow[7][4];

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	virtual void OnOK();
	afx_msg void OnSelectMaxSpendType();

private:
	bool ValidateMaxSpend(CEdit* pEdit, int nPeriod, double dMin, double dMax, CString strType);

	DECLARE_MESSAGE_MAP()

private:
	void SetPeriodDisplay();

private:
	CEdit* GetEditMaxSpend( int n );
	CEdit* GetEditMaxSpendP3( int n );
	CEdit* GetEditOverspend( int n );
	CEdit* GetEditOverspendP3( int n );

private:
	int m_nControlIDPeriodText[7];
	int m_nControlIDPeriodStart[7];
	int m_nControlIDPeriodAllow[7][4];
	int m_nControlIDMaxSpend[7];
	int m_nControlIDMaxSpendP3[7];
	int m_nControlIDOverspend[7];
	int m_nControlIDOverspendP3[7];

private:
	double m_dMaxSpend[7];
	double m_dMaxSpendP3[7];
	double m_dOverspend[7];
	double m_dOverspendP3[7];

private:
	CSQLRowGroupFull& m_RowGroup;
};

//$$******************************************************************
