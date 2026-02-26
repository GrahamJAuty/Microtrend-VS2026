#pragma once
//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
//$$******************************************************************
#include "CardPicker.h"
#include "DbExportHandler.h"
//$$******************************************************************

class CBatchUpdateHelpers
{
public:
	CBatchUpdateHelpers();

public:
	void SetIncludeValidFlag(bool b) { m_bIncludeValid = b; }
	void SetIncludeExpiredFlag(bool b) { m_bIncludeExpired = b; }
	void SetIncludeHotlistedFlag(bool b) { m_bIncludeHotlisted = b; }
	void SetIncludeInactiveFlag(bool b) { m_bIncludeInactive = b; }
	void SetCardRangeType(int n) { m_nCardRangeType = n; }
	void SetRangeFromCardNo(__int64 n) { m_nRangeFromCardNo = n; }
	void SetRangeToCardNo(__int64 n) { m_nRangeToCardNo = n; }
	void SetGroupSel(int n) { m_nGroupSel = n; }
	void SetSingleGroupNo(int n) { m_nSingleGroupNo = n; }
	void SetGenderSel(int n) { m_nGenderSel = n; }
	void SetLabel(CString str) { m_strLabel = str; }
	void SetComment(CString str) { m_strComment = str; }
	void SetAlertFilter(CString str) { m_strAlertFilter = str; }
	void SetBirthdayFilter(bool b) { m_bBirthdayFilter = b; }

public:
	void GetSQLFilters(CDbExportHandler& ExportHandler);
	bool NonSQLFilter( CSQLRowAccountFull& RowAccount);
	bool HaveRecords();
	void ResetPicker() { m_Picker.Reset(); }

public:
	CString GetComment() { return m_strComment; }

private:
	bool m_bIncludeValid;
	bool m_bIncludeExpired;
	bool m_bIncludeHotlisted;
	bool m_bIncludeInactive;
	int m_nCardRangeType;
	__int64 m_nRangeFromCardNo;
	__int64 m_nRangeToCardNo;
	int m_nGroupSel;
	int m_nSingleGroupNo;
	int m_nGenderSel;
	CString m_strLabel;
	CString m_strComment;
	CString m_strAlertFilter;
	bool m_bBirthdayFilter;

private:
	CCardPicker m_Picker;
};

//$$******************************************************************

