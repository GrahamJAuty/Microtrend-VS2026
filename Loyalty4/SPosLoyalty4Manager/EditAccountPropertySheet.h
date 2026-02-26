#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SPosLoyalty4Shared\SQLTable_ExternalAccount\SQLRowExternalAccount.h"
//$$******************************************************************
#include "EditAccountBuffer.h"
#include "EditAccountPropertyPage1.h"
#include "EditAccountPropertyPage2.h"
#include "EditAccountPropertyPage4.h"
#include "EditAccountPropertyPage5.h"
#include "PhotoIDFilenameStack.h"
//$$******************************************************************

class CEditAccountPropertySheet : public CSSPropertySheet
{
public:
	CEditAccountPropertySheet(CSQLRowAccountFull& RowAccount, CSQLRowExternalAccount& RowExternal, CPhotoIDFilenameStack& PhotoIDFilenameStack, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CEditAccountPropertySheet();
	void UpdateAudit() { m_PageBalance.UpdateAudit(); }

public:
	void AddStampOfferTab();
	void RemoveStampOfferTab();
	static bool PromptBeforeCancel();

private:
	void InitialCheckStampOfferTab();
	
public:
	CEditAccountPropertyPage1 m_PageDetail;
	CEditAccountPropertyPage2 m_PageBalance;
	CEditAccountPropertyPage4 m_PageExternal;
	CEditAccountPropertyPage5 m_PageOfferStamp;

public:
	void UpdateTitle(CSQLRowAccountFull& RowAccount);

protected:
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	CSQLRowAccountFull& m_RowAccount;
	CPhotoIDFilenameStack& m_PhotoIDFilenameStack;
	CEditAccountBuffer m_EditAccountBuffer;

private:
	bool m_bGotStampOfferTab;
	bool m_bGotExternalTab;
};

//$$******************************************************************
