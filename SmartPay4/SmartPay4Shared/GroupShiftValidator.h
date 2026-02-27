//**********************************************************************
#pragma once
//**********************************************************************
#include "SQLTable_Account/SQLRowAccountFull.h"
//**********************************************************************

struct CGroupShiftInfo
{
public:
	CGroupShiftInfo()
	{
		Reset();
	}

public:
	void Reset(int nGroupNow = 0 )
	{
		m_nGroupShiftStatus = 0;
		m_strGroupShiftStartDate = "";
		m_strGroupShiftEndDate = "";
		m_nGroupShiftFutureGroup = 0;
		m_nGroupNow = nGroupNow;
	}

public:
	int m_nGroupShiftStatus;
	CString m_strGroupShiftStartDate;
	CString m_strGroupShiftEndDate;
	int m_nGroupShiftFutureGroup;
	int m_nGroupNow;
};

//**********************************************************************

class CGroupShiftValidator
{
public:
	CGroupShiftValidator(const COleDateTime& dateNow, const CSQLRowAccountFull& RowAccount);

public:
	bool UpdateAccount(CSQLRowAccountFull& RowAccount);
	void CopyEditedToOriginal() { m_infoOriginal = m_infoEdited; }
	void ValidateEditedDates( int& nPendingStatus);
	void ValidateEditedGroupShift();
	void ResetEditedGroupShift();
	void CreateEditedGroupShift(int nFutureGroup, int nDuration, int& nPendingStatus);
	void SetEditedStartOleDate(COleDateTime& date);
	void SetEditedEndOleDate(COleDateTime& date);
	COleDateTime GetEditedStartOleDate();
	COleDateTime GetEditedEndOleDate();
	CString GetChangeLogString(CSQLRowAccountFull& RowAccount);

public:
	int GetOriginalGroupShiftStatus() { return m_infoOriginal.m_nGroupShiftStatus; }
	int GetEditedGroupShiftStatus() { return m_infoEdited.m_nGroupShiftStatus; }
	int GetEditedGroupNow() { return m_infoEdited.m_nGroupNow; }
	int GetEditedGroupShiftFutureGroup() { return m_infoEdited.m_nGroupShiftFutureGroup; }
	void SetEditedGroupNow(int n) { m_infoEdited.m_nGroupNow = n; }
	void SetEditedGroupShiftFutureGroup(int n) { m_infoEdited.m_nGroupShiftFutureGroup = n; }

public:
	static COleDateTime GetMinStartOleDate() { return COleDateTime(2023, 1, 1, 0, 0, 0); }
	static COleDateTime GetMinEndOleDate() { return COleDateTime(2023, 1, 2, 0, 0, 0); }
	static COleDateTime GetMaxStartOleDate() { return COleDateTime(2199, 12, 30, 0, 0, 0); }
	static COleDateTime GetMaxEndOleDate() { return COleDateTime(2199, 12, 31, 0, 0, 0); }
	static int GetMaxGroupShiftDuration() { return 999; }
	static void InsertIntoArray(CArray<int, int>& array, int nNum);

private:
	CGroupShiftInfo ValidateForCurrentDate(CGroupShiftInfo& source);
	CGroupShiftInfo DateValidate(CGroupShiftInfo& source, int& nPendingStatus);

private:
	CGroupShiftInfo m_infoOriginal;
	CGroupShiftInfo m_infoEdited;
	COleDateTime m_dateNow;
};

//**********************************************************************
