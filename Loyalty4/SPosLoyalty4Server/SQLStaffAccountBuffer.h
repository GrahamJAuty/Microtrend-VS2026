#pragma once
//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_StaffGiftException\SQLRowStaffGiftException.h"
//**********************************************************************
#include "EposLoyaltyFile.h"
//**********************************************************************

struct CSQLStaffAccountBufferItem
{
public:
	CSQLStaffAccountBufferItem()
	{
		m_nStaffGiftMode = STAFFGIFTMODE_NONE;
	}

public:
	void ValidateStaffGift();
	bool ApplyStaffGift(CDatabase* pDatabase);
	bool PrepareAuditStaffGift(CAuditRecord_base& atc, int nAuditType);

private:
	bool ValidateStaffGiftInternal();

public:
	CSQLRowAccountFull m_RowAccountStaff;
	CSQLRowStaffGiftException m_RowStaffGiftException;
	int m_nStaffGiftMode;
};

//**********************************************************************

class CSQLStaffAccountBuffer
{
public:
	CSQLStaffAccountBuffer();

public:
	void Reset();
	
public:
	int GetStaffGiftCount() { return m_arrayItems.GetSize(); }
	void BufferStaffGift(CSQLRowStaffGiftException& RowException);
	void ValidateStaffGifts();
	bool ApplyStaffGift( int n, CDatabase* pDatabase);
	bool PrepareAuditStaffGift(int n, CAuditRecord_base& atc, int nAuditType);
	
public:
	CArray<CSQLStaffAccountBufferItem, CSQLStaffAccountBufferItem> m_arrayItems;
};

//**********************************************************************
extern CSQLStaffAccountBuffer SQLStaffAccountBuffer;
//**********************************************************************
