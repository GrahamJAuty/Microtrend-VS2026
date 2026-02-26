#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRowGroup.h"
//$$******************************************************************

class CSetupGroupPeriodsDlg : public CSSDialog
{
public:
	CSetupGroupPeriodsDlg(CSQLRowGroup& RowGroup, bool bPurse2Allowed,CWnd* pParent = NULL);   
	virtual ~CSetupGroupPeriodsDlg();

	enum { IDD = IDD_SETUP_GROUPPERIOD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	bool ValidateMaxSpend(CEdit* pEdit, int nPeriod, double dMin, double dMax, CString strType);

private:
	CEdit* GetEditMaxSpend(int n);
	CEdit* GetEditOverSpend(int n);
	
public:
	CButton m_checkEnable[3][6];
	CEdit m_editPeriodText1;
	CEdit m_editPeriodText2;
	CEdit m_editPeriodText3;
	CEdit m_editPeriodText4;
	CEdit m_editPeriodText5;
	CEdit m_editPeriodText6;

private:
	CSQLRowGroup& m_RowGroup;
	bool m_bPurse2Allowed;
	CString m_strTitle;
};

//$$******************************************************************
