//**********************************************************************
#include "SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "GroupShiftValidator.h"
//**********************************************************************

CGroupShiftValidator::CGroupShiftValidator( const COleDateTime& dateNow, const CSQLRowAccountFull& RowAccount)
{
	m_dateNow = COleDateTime(dateNow.GetYear(), dateNow.GetMonth(), dateNow.GetDay(), 0, 0, 0);

	m_infoOriginal.m_nGroupShiftStatus = RowAccount.GetGroupShiftStatus();
	m_infoOriginal.m_strGroupShiftStartDate = RowAccount.GetGroupShiftStartDate();
	m_infoOriginal.m_strGroupShiftEndDate = RowAccount.GetGroupShiftEndDate();
	m_infoOriginal.m_nGroupShiftFutureGroup = RowAccount.GetGroupShiftFutureGroup();
	m_infoOriginal.m_nGroupNow = RowAccount.GetGroupNo();

	m_infoEdited = ValidateForCurrentDate(m_infoOriginal);
}

//**********************************************************************

COleDateTime CGroupShiftValidator::GetEditedStartOleDate()
{
	COleDateTime result;
	CSQLFieldValidatorBase::StringToOleDate(m_infoEdited.m_strGroupShiftStartDate, result);
	return result;
}

//**********************************************************************

COleDateTime CGroupShiftValidator::GetEditedEndOleDate()
{
	COleDateTime result;
	CSQLFieldValidatorBase::StringToOleDate(m_infoEdited.m_strGroupShiftEndDate, result);
	return result;
}

//**********************************************************************

void CGroupShiftValidator::SetEditedStartOleDate(COleDateTime& date)
{
	m_infoEdited.m_strGroupShiftStartDate = CSQLFieldValidatorBase::OleDateToString(date);
}

//**********************************************************************

void CGroupShiftValidator::SetEditedEndOleDate(COleDateTime& date)
{
	m_infoEdited.m_strGroupShiftEndDate = CSQLFieldValidatorBase::OleDateToString(date);
}

//**********************************************************************

void CGroupShiftValidator::CreateEditedGroupShift(int nFutureGroup, int nDuration, int& nPendingStatus)
{
	m_infoEdited.m_nGroupShiftStatus = 2;
	m_infoEdited.m_strGroupShiftStartDate = CSQLFieldValidatorBase::OleDateToString(m_dateNow);
	m_infoEdited.m_strGroupShiftEndDate = CSQLFieldValidatorBase::OleDateToString(m_dateNow + COleDateTimeSpan(nDuration,0,0,0));
	m_infoEdited.m_nGroupShiftFutureGroup = nFutureGroup;
	m_infoEdited.m_nGroupNow = m_infoOriginal.m_nGroupNow;
	ValidateEditedDates(nPendingStatus);
}

//**********************************************************************

CGroupShiftInfo CGroupShiftValidator::ValidateForCurrentDate(CGroupShiftInfo& source)
{
	CGroupShiftInfo result;

	if ((source.m_nGroupShiftStatus < 1) || (source.m_nGroupShiftStatus > 2))
	{
		result.Reset(source.m_nGroupNow);
		return result;
	}

	if ((source.m_nGroupShiftFutureGroup < 1) || (source.m_nGroupShiftFutureGroup > Account::GroupShiftFutureGroup.Max))
	{
		result.Reset(source.m_nGroupNow);
		return result;
	}

	COleDateTime dateStart;
	if (CSQLFieldValidatorBase::StringToOleDate(source.m_strGroupShiftStartDate, dateStart) == FALSE)
	{
		result.Reset(source.m_nGroupNow);
		return result;
	}

	COleDateTime dateEnd;
	if (CSQLFieldValidatorBase::StringToOleDate(source.m_strGroupShiftEndDate, dateEnd) == FALSE)
	{
		result.Reset(source.m_nGroupNow);
		return result;
	}

	if ((dateStart < GetMinStartOleDate()) || (dateStart > GetMaxStartOleDate()))
	{
		result.Reset(source.m_nGroupNow);
		return result;
	}

	if ((dateEnd < GetMinEndOleDate()) || (dateEnd > GetMaxEndOleDate()))
	{
		result.Reset(source.m_nGroupNow);
		return result;
	}

	COleDateTimeSpan span = dateEnd - dateStart;

	if (span.GetDays() < 1)
	{
		dateEnd = dateStart + COleDateTimeSpan(1, 0, 0, 0);
	}

	if (span.GetDays() > GetMaxGroupShiftDuration())
	{
		dateEnd = dateStart + COleDateTimeSpan(GetMaxGroupShiftDuration(), 0, 0, 0);
	}

	if (1 == source.m_nGroupShiftStatus)
	{
		//CHANGE GROUP SHIFT FROM CURRENT TO FUTURE
		if (m_dateNow < dateStart)
		{
			result.m_nGroupShiftStatus = 2;
			result.m_strGroupShiftStartDate = CSQLFieldValidatorBase::OleDateToString(dateStart);
			result.m_strGroupShiftEndDate = CSQLFieldValidatorBase::OleDateToString(dateEnd);
			result.m_nGroupShiftFutureGroup = source.m_nGroupNow;
			result.m_nGroupNow = source.m_nGroupShiftFutureGroup;
		}
		//CHANGE GROUP SHIFT FROM CURRENT TO NONE
		else if (m_dateNow >= dateEnd)
		{
			result.m_nGroupShiftStatus = 0;
			result.m_strGroupShiftStartDate = "";
			result.m_strGroupShiftEndDate = "";
			result.m_nGroupShiftFutureGroup = 0;
			result.m_nGroupNow = source.m_nGroupShiftFutureGroup;
		}
		//GROUP SHIFT STILL GOING ON, BUT DATES MAY HAVE CHANGED
		else
		{
			result.m_nGroupShiftStatus = 1;
			result.m_strGroupShiftStartDate = CSQLFieldValidatorBase::OleDateToString(dateStart);
			result.m_strGroupShiftEndDate = CSQLFieldValidatorBase::OleDateToString(dateEnd);
			result.m_nGroupShiftFutureGroup = source.m_nGroupShiftFutureGroup;
			result.m_nGroupNow = source.m_nGroupNow;
		}
	}
	else
	{
		//CHANGE FROM FUTURE TO NONE
		if (m_dateNow >= dateEnd)
		{
			result.m_nGroupShiftStatus = 0;
			result.m_strGroupShiftStartDate = "";
			result.m_strGroupShiftEndDate = "";
			result.m_nGroupShiftFutureGroup = 0;
			result.m_nGroupNow = source.m_nGroupNow;
		}
		//CHANGE FROM FUTURE TO CURRENT
		else if (m_dateNow >= dateStart)
		{
			result.m_nGroupShiftStatus = 1;
			result.m_strGroupShiftStartDate = CSQLFieldValidatorBase::OleDateToString(dateStart);
			result.m_strGroupShiftEndDate = CSQLFieldValidatorBase::OleDateToString(dateEnd);
			result.m_nGroupShiftFutureGroup = source.m_nGroupNow;
			result.m_nGroupNow = source.m_nGroupShiftFutureGroup;
		}
		//STILL IN FUTURE, BUT DATES MAY HAVE CHANGED
		else
		{
			result.m_nGroupShiftStatus = 2;
			result.m_strGroupShiftStartDate = CSQLFieldValidatorBase::OleDateToString(dateStart);
			result.m_strGroupShiftEndDate = CSQLFieldValidatorBase::OleDateToString(dateEnd);
			result.m_nGroupShiftFutureGroup = source.m_nGroupShiftFutureGroup;
			result.m_nGroupNow = source.m_nGroupNow;
		}
	}

	return result;
}

//**********************************************************************

void CGroupShiftValidator::ValidateEditedDates(int& nPendingStatus)
{
	CGroupShiftInfo validated = DateValidate(m_infoEdited, nPendingStatus);
	m_infoEdited = validated;
}

//**********************************************************************

void CGroupShiftValidator::ValidateEditedGroupShift()
{
	CGroupShiftInfo validated = ValidateForCurrentDate(m_infoEdited);
	m_infoEdited = validated;
}

//**********************************************************************

void CGroupShiftValidator::ResetEditedGroupShift()
{
	if (m_infoOriginal.m_nGroupShiftStatus == 1)
	{
		m_infoEdited.Reset(m_infoOriginal.m_nGroupShiftFutureGroup);
	}
	else
	{
		m_infoEdited.Reset(m_infoOriginal.m_nGroupNow);
	}
}

//**********************************************************************

CGroupShiftInfo CGroupShiftValidator::DateValidate(CGroupShiftInfo& source, int& nPendingStatus)
{
	CGroupShiftInfo result;

	COleDateTime dateStart;
	if (CSQLFieldValidatorBase::StringToOleDate(source.m_strGroupShiftStartDate, dateStart) == FALSE)
	{
		dateStart = m_dateNow;
	}

	COleDateTime dateEnd;
	if (CSQLFieldValidatorBase::StringToOleDate(source.m_strGroupShiftEndDate, dateEnd) == FALSE)
	{
		dateEnd = dateStart + COleDateTimeSpan(1, 0, 0, 0);
	}

	if ((dateStart < GetMinStartOleDate()) || (dateStart > GetMaxStartOleDate()))
	{
		dateStart = m_dateNow;
	}

	if ((dateEnd < GetMinEndOleDate()) || (dateEnd > GetMaxEndOleDate()))
	{
		dateEnd = dateStart + COleDateTimeSpan(1, 0, 0, 0);

	}

	COleDateTimeSpan span = dateEnd - dateStart;

	if (span.GetDays() < 1)
	{
		dateEnd = dateStart + COleDateTimeSpan(1, 0, 0, 0);
	}

	if (span.GetDays() > GetMaxGroupShiftDuration())
	{
		dateEnd = dateStart + COleDateTimeSpan(GetMaxGroupShiftDuration(), 0, 0, 0);
	}

	result = source;
	result.m_strGroupShiftStartDate = CSQLFieldValidatorBase::OleDateToString(dateStart);
	result.m_strGroupShiftEndDate = CSQLFieldValidatorBase::OleDateToString(dateEnd);

	if (dateStart > m_dateNow)
	{
		nPendingStatus = 2;
	}
	else if (dateEnd <= m_dateNow)
	{
		nPendingStatus = 0;
	}
	else
	{
		nPendingStatus = 1;
	}

	return result;
}

//**********************************************************************

bool CGroupShiftValidator::UpdateAccount(CSQLRowAccountFull& RowAccount)
{
	bool bResult = FALSE;

	if (m_infoOriginal.m_nGroupShiftStatus != m_infoEdited.m_nGroupShiftStatus)
	{
		RowAccount.SetGroupShiftStatus(m_infoEdited.m_nGroupShiftStatus);
		bResult = TRUE;
	}

	if (m_infoOriginal.m_strGroupShiftStartDate != m_infoEdited.m_strGroupShiftStartDate)
	{
		RowAccount.SetGroupShiftStartDate(m_infoEdited.m_strGroupShiftStartDate);
		bResult = TRUE;
	}

	if (m_infoOriginal.m_strGroupShiftEndDate != m_infoEdited.m_strGroupShiftEndDate)
	{
		RowAccount.SetGroupShiftEndDate(m_infoEdited.m_strGroupShiftEndDate);
		bResult = TRUE;
	}

	if (m_infoOriginal.m_nGroupShiftFutureGroup != m_infoEdited.m_nGroupShiftFutureGroup)
	{
		RowAccount.SetGroupShiftFutureGroup(m_infoEdited.m_nGroupShiftFutureGroup);
		bResult = TRUE;
	}

	if (m_infoOriginal.m_nGroupNow != m_infoEdited.m_nGroupNow)
	{
		RowAccount.SetGroupNo(m_infoEdited.m_nGroupNow);
		bResult = TRUE;
	}

	return bResult;
}

//**********************************************************************

CString CGroupShiftValidator::GetChangeLogString(CSQLRowAccountFull& RowAccount)
{
	CCSV csvLog;
	csvLog.Add(RowAccount.GetGroupNo());
	csvLog.Add(m_infoOriginal.m_nGroupShiftStatus);
	csvLog.Add(m_infoOriginal.m_strGroupShiftStartDate);
	csvLog.Add(m_infoOriginal.m_strGroupShiftEndDate);
	csvLog.Add(m_infoOriginal.m_nGroupShiftFutureGroup);
	csvLog.Add(m_infoEdited.m_nGroupShiftStatus);
	csvLog.Add(m_infoEdited.m_strGroupShiftStartDate);
	csvLog.Add(m_infoEdited.m_strGroupShiftEndDate);
	csvLog.Add(m_infoEdited.m_nGroupShiftFutureGroup);
	return csvLog.GetLine();
}

//**********************************************************************

void CGroupShiftValidator::InsertIntoArray(CArray<int, int>& array, int nNum)
{
	bool bFound = FALSE;

	for (int n = 0; n < array.GetSize(); n++)
	{
		int nCheck = array.GetAt(n);

		if (nCheck == nNum)
		{
			bFound = TRUE;
			break;
		}

		if (nCheck > nNum)
		{
			array.InsertAt(n, nNum);
			bFound = TRUE;
			break;
		}
	}

	if (FALSE == bFound)
	{
		array.Add(nNum);
	}
}

//**********************************************************************
