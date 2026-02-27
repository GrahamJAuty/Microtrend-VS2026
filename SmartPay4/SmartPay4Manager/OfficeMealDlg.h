#pragma once
/*********************************************************************/
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRowGroupFull.h"
/*********************************************************************/
#include "SmartPay4Manager.h"
/*********************************************************************/

class COfficeMealDlg : public CSSAutoShutdownDialog
{
public:
	COfficeMealDlg( CSQLRowAccountFull& RowAccount, CWnd* pParent );   // standard constructor
	virtual ~COfficeMealDlg();

// Dialog Data
	enum { IDD = IDD_OFFICEMEAL };
	CButton m_checkPurse1;
	CButton m_checkPurse2;
	CButton m_checkPurse3;
	CButton m_checkCash;
	double m_dMealValue;
	double m_dPurse1Balance;
	double m_dPurse2Balance;
	double m_dPurse3Balance;
	CString m_strUserName;
	CString m_strGroupName;
	CString m_strPurse1Date;
	CString m_strPurse2Date;
	CString m_strPurse3Date;
	CString m_strComment;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnTogglePurse1();
	afx_msg void OnTogglePurse2();
	afx_msg void OnTogglePurse3();
	afx_msg void OnToggleCash();
	afx_msg void OnKillFocusMealValue();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	virtual void CSSAutoShutdownPreInitDialog();
	virtual BOOL CSSAutoShutdownPostInitDialog();

public:
	bool UpdateAccount();
	void SaveAudit();

private:
	void TogglePayment(int nTickedNow);

private:
	CEdit* GetEditMealValue() { return GetEdit(IDC_EDIT_MEALVALUE); }
	CEdit* GetEditPurse2() { return GetEdit(IDC_EDIT_PURSE2); }
	CEdit* GetEditPurse3() { return GetEdit(IDC_EDIT_PURSE3); }

private:
	CSQLRowAccountFull& m_RowAccount;
	CSQLRowGroupFull m_RowGroup;
	CWnd* m_pParent;

	CString m_strTitle;
	CString m_strReportLabel;

	CSQLAuditRecord m_atcRecord;
	CSSDate m_dateTransaction;
	CSSTime m_timeTransaction;

	double m_dPurse1Starting;
	double m_dPurse2Starting;
	double m_dPurse3Starting;

	double m_dPurse1Deduction;
	double m_dPurse2Deduction;
	double m_dPurse3Deduction;
	double m_dCashDeduction;

public:
	double m_dUnapplied;
};

/*********************************************************************/
