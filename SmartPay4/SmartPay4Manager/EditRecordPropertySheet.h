#pragma once
//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SmartPay4Shared\SQLTable_ExternalAccount\SQLRowExternalAccount.h"
//$$******************************************************************
#include "..\SmartPay4Shared\GroupShiftValidator.h"
//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\LeaversHandler.h"
//$$******************************************************************
#include "EditRecordPropertyPageDetail.h"
#include "EditRecordPropertyPageBalance.h"
#include "EditRecordPropertyPageMaxSpend.h"
#include "EditRecordPropertyPagePurchaseControl.h"
#include "EditRecordPropertyPageExternal.h"
//$$******************************************************************

class CEditRecordPropertySheet : public CSSAutoShutdownPropertySheet
{
public:
	CEditRecordPropertySheet(CSQLRowAccountFull& RowAccount, CSQLRowExternalAccount& RowExternal, int nMode, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CEditRecordPropertySheet(CSQLRowAccountFull& RowAccount, CSQLRowExternalAccount& RowExternal, int nMode, CLeaversHandler* pLeaversData, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CEditRecordPropertySheet();

	void SaveChanges (CSQLRowAccountFull& Row)	{ m_PageAccount.SaveChanges ( Row ); }

public:
	bool AddMaxSpendTab();
	bool RemoveMaxSpendTab();
	bool AddPurchaseControlTab();
	bool RemovePurchaseControlTab();
	CString GetNewPINNumber() const { return m_EditAccountBuffer.m_strNewPINNumber; }

protected:
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();
	
public:
	void AddOptionalTabs();
	void UpdateAudit(bool bCancel) { m_PageBalance.UpdateAudit(bCancel); }
	static bool PromptBeforeCancel();

private:
	void InitialCheckOptionalTabs();
	
private:
	CEditRecordPropertyPageDetail m_PageAccount;
	CEditRecordPropertyPageBalance m_PageBalance;
	CEditRecordPropertyPageMaxSpend m_PageMaxSpend;
	CEditRecordPropertyPageExternal m_PageExternal;
	CEditRecordPropertyPagePurchaseControl m_PagePurchaseControl;
	
private:
	CSQLRowAccountFull& m_SQLRowAccount;
	CLeaversHandler* m_pLeaversData;
	CEditAccountBuffer m_EditAccountBuffer;
	CGroupShiftValidator m_GroupShiftValidator;

private:
	int m_nMode;
	bool m_bGotMaxSpendTab;
	bool m_bGotPurchaseControlTab;
	bool m_bGotExternalTab;
};

//$$******************************************************************
