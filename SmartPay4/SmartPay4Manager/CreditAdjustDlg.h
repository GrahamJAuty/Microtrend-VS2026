//$$******************************************************************
#pragma once
//$$******************************************************************
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRowGroupFull.h"
//$$******************************************************************
#include "SmartPay4Manager.h"
//$$******************************************************************

struct CCreditAdjustOutput
{
public:
	CCreditAdjustOutput();

public:
	bool GotCredit();
	
public:
	CString m_strCommentUp;
	CString m_strCommentDown;
	double m_dPurse1Credit;
	double m_dPurse3Credit;
};

//$$******************************************************************

class CCreditAdjustDlg : public CSSAutoShutdownDialog
{
public:
	CCreditAdjustDlg( CSQLRowAccountFull& RowAccount, CWnd* pParent );   // standard constructor
	virtual ~CCreditAdjustDlg();

// Dialog Data
	enum { IDD = IDD_CREDIT_ADJUST };
	CEdit m_editUserName;
	CEdit m_editGroupName;
	CEdit m_editPurse1Balance;
	CEdit m_editPurse3Balance;
	CEdit m_editPurse1Date;
	CEdit m_editPurse3Date;
	CEdit m_editCommentUp;
	CEdit m_editCommentDown;

private:
	double m_dPurse1Starting;
	double m_dPurse3Starting;
	bool m_bKnownGroup;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnKillFocusPurse1Credit();
	virtual void OnKillFocusPurse3Credit();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	virtual void CSSAutoShutdownPreInitDialog();
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	CEdit* GetEditPurse1Credit();
	CEdit* GetEditPurse3Credit();

public:
	bool UpdateAccount(CSQLRowAccountFull& RowAccount);

private:
	bool CheckAdjust(int nPurse, double dCurrentCredit, double dNewCredit);
	void DisplayBalance(CEdit* pEdit, double d);
	void AddCreditMessage(CString& strMsg, int nPurse, double dAmount);
	void SaveAudit();

private:
	CCreditAdjustOutput m_CreditAdjustOutput;

private:
	CSQLRowAccountFull& m_RowAccount;
	CSQLRowGroupFull m_RowGroup;
	CWnd* m_pParent;

	CString m_strTitle;
	CString m_strReportLabel;

	CSQLAuditRecord m_atcRecord;
	CSSDate m_dateTransaction;
	CSSTime m_timeTransaction;
};

//$$******************************************************************
