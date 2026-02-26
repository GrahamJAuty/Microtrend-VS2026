/**********************************************************************/
#include "ImportDefines.h"
/**********************************************************************/
#include "DepartmentCSVArray.h"
/**********************************************************************/

CDepartmentCSVArray::CDepartmentCSVArray() : CSharedCSVArray(',', '"', FALSE, TRUE, TRUE)
{
	m_bCheckedSportsBookerDeptNo = FALSE;
	m_nSportsBookerDeptNo = 0;
}

/**********************************************************************/

int CDepartmentCSVArray::Open(const char* szFilename, int nMode)
{
	m_bCheckedSportsBookerDeptNo = FALSE;
	m_nSportsBookerDeptNo = 0;

	RemoveAt(0, GetSize());

	int nReply = CSharedCSVArray::Open(szFilename, nMode);

	if (nReply != DB_ERR_SHARE)
	{
		if (nMode == DB_READONLY)	// if file does not exist use default values
		{
			nReply = DB_ERR_NONE;
		}
	}

	SetDeptZero("PROMO");

	return nReply;
}

/**********************************************************************/

bool CDepartmentCSVArray::FindDeptByNumber(int nDeptNo, int& nIndex)
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	CDepartmentCSVRecord Record;

	while (nStart <= nEnd)
	{
		nIndex = (nStart + nEnd) / 2;

		GetAt(nIndex, Record);
		int nArrayDeptNo = Record.GetDeptNo();

		if (nArrayDeptNo < nDeptNo)
		{
			nStart = nIndex + 1;
		}
		else if (nArrayDeptNo > nDeptNo)
		{
			nEnd = nIndex - 1;
		}
		else
		{
			return TRUE;
		}
	}

	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

const char* CDepartmentCSVArray::GetReportTextByDeptNo(int nDeptNo)
{
	int nDeptIdx = 0;
	if (FindDeptByNumber(nDeptNo, nDeptIdx))
	{
		CDepartmentCSVRecord Department;
		GetAt(nDeptIdx, Department);
		m_strReportText = Department.GetReportText();
	}
	else
	{
		m_strReportText = "Unknown";
	}

	return m_strReportText;
}

/**********************************************************************/

bool CDepartmentCSVArray::GetReportEnableByDeptNo(int nDeptNo)
{
	int nDeptIdx = 0;
	if (FindDeptByNumber(nDeptNo, nDeptIdx))
	{
		CDepartmentCSVRecord Department;
		GetAt(nDeptIdx, Department);
		return Department.GetReportEnable();
	}

	return FALSE;
}

/**********************************************************************/

const char* CDepartmentCSVArray::GetDefaultTextByDeptNo(int nDeptNo)
{
	m_strDefaultText.Format("Department %d", nDeptNo);
	return m_strDefaultText;
}

/**********************************************************************/

void CDepartmentCSVArray::SetDeptZero(const char* sz)
{
	if (GetSize() != 0)
	{
		CDepartmentCSVRecord Dept;
		GetAt(0, Dept);

		if (Dept.GetDeptNo() == 0)
		{
			RemoveAt(0);
		}
	}

	CString strText = sz;

	CDepartmentCSVRecord Dept;
	Dept.SetDeptNo(0);
	Dept.SetEposGroup(0);
	Dept.SetEposText(strText);

	if (strText == "PROMO")
	{
		Dept.SetRepText(DealerFlags.GetDeptZeroText());
	}

	InsertAt(0, Dept);
}

/**********************************************************************/

void CDepartmentCSVArray::AddPMSDepartment(int nDeptNo, const char* szText)
{
	CDepartmentCSVRecord DeptRecord;
	DeptRecord.SetDeptNo(nDeptNo);
	DeptRecord.SetEposGroup(nDeptNo % 100);
	DeptRecord.SetEposText(szText);

	int nDeptIdx;
	if (FindDeptByNumber(nDeptNo, nDeptIdx) == FALSE)
	{
		InsertAt(nDeptIdx, DeptRecord);
	}
	else
	{
		SetAt(nDeptIdx, DeptRecord);
	}
}

/**********************************************************************/

int CDepartmentCSVArray::GetSportsBookerDeptNo()
{
	if (FALSE == m_bCheckedSportsBookerDeptNo)
	{
		m_nSportsBookerDeptNo = 0;
		for (int nDeptIdx = 0; nDeptIdx < GetSize(); nDeptIdx++)
		{
			CDepartmentCSVRecord DeptRecord;
			GetAt(nDeptIdx, DeptRecord);

			if (DeptRecord.GetSportsBookerFlag() == TRUE)
			{
				m_nSportsBookerDeptNo = DeptRecord.GetDeptNo();
				break;
			}
		}
	}

	m_bCheckedSportsBookerDeptNo = TRUE;
	return m_nSportsBookerDeptNo;
}

/**********************************************************************/

void CDepartmentCSVArray::FixSportsBookerDeptNo()
{
	if (TRUE == m_bCheckedSportsBookerDeptNo)
	{
		for (int nDeptIdx = 0; nDeptIdx < GetSize(); nDeptIdx++)
		{
			CDepartmentCSVRecord DeptRecord;
			GetAt(nDeptIdx, DeptRecord);
			DeptRecord.SetSportsBookerFlag(DeptRecord.GetDeptNo() == m_nSportsBookerDeptNo);
			SetAt(nDeptIdx, DeptRecord);
		}
	}
}

/**********************************************************************/

void CDepartmentCSVArray::SetSportsBookerDeptNo(int nDeptNo)
{
	if ((nDeptNo >= Dept::DeptNo.Min) && (nDeptNo <= Dept::DeptNo.Max))
	{
		m_bCheckedSportsBookerDeptNo = TRUE;
		m_nSportsBookerDeptNo = nDeptNo;
	}
}

/**********************************************************************/
