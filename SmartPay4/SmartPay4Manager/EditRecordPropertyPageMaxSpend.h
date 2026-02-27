#pragma once
//$$******************************************************************
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRowGroupFull.h"
//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\LeaversHandler.h"
//$$******************************************************************

class CEditRecordPropertyPageMaxSpend : public CSSAutoShutdownPropertyPage
{
public:
	CEditRecordPropertyPageMaxSpend(CSQLRowAccountFull& Row );
	virtual ~CEditRecordPropertyPageMaxSpend();
	void SetPointer ( int nMode, CLeaversHandler* pLeaversData = NULL );
	
// Dialog Data
	enum { IDD = IDD_PROPPAGE_EDITRECORD_MAXSPEND };
	CButton m_checkOwnMaxSpend;
	CStatic m_staticGroupMaxSpend;
	CButton m_buttonCopyGroup;
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
	afx_msg void OnToggleMaxSpend();
	afx_msg void OnButtonCopyGroup();
	virtual BOOL OnQueryCancel();
	DECLARE_MESSAGE_MAP()

private:
	void SetPeriodDisplay();
	void SetEnables();
	void ToggleMaxSpend( bool bForceUpdate );

private:
	bool IsPurseAllowed( int nPurse, int nPeriod );
	double GetMaxSpend( int nPeriod );
	double GetOverspend( int nPeriod );
	double GetMaxSpendP3( int nPeriod );
	double GetOverspendP3( int nPeriod );

private:
	CEdit* GetEditMaxSpend( int n );
	CEdit* GetEditMaxSpendP3( int n );
	CEdit* GetEditOverspend( int n );
	CEdit* GetEditOverspendP3( int n );

private:
	bool ValidateMaxSpend(CEdit* pEdit, int nPeriod, double dMin, double dMax, CString strType);

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
	CSQLRowAccountFull& m_SQLRowAccount;
	CSQLRowGroupFull m_SQLRowGroup;
	bool m_bUseRecordSettings;
	bool m_bReadOnly;
};

//$$******************************************************************
