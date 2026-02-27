//**********************************************************************
#include "SQLAuditRecord_base.h"
#include "DateRangeHelpers.h"
#include "SQLDefines.h"
//**********************************************************************
#include "SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLTable_CCNo\SQLRepositoryCCNo.h"
#include "SQLTable_PurchaseControlStamp\SQLRepositoryPurchaseControlStamp.h"
#include "SQLTable_PurchaseControlStamp\SQLRecordSetPurchaseControlStamp.h"
#include "SQLTable_PurchaseControlStamp\SQLRowPurchaseControlStamp.h"
//**********************************************************************
#include "SQLPurchaseControlBuffer.h"
//**********************************************************************

int CSQLPurchaseControlPendingInfo::Compare(CSQLPurchaseControlPendingInfo& source, int nHint)
{
	if (m_nControlID != source.m_nControlID)
	{
		return (m_nControlID > source.m_nControlID ? 1 : -1);
	}
	
	if (m_nPluNo != source.m_nPluNo)
	{
		return (m_nPluNo > source.m_nPluNo ? 1 : -1);
	}
	else
	{
		return 0;
	}
}

//**********************************************************************

void CSQLPurchaseControlPendingInfo::Add(CSQLPurchaseControlPendingInfo& source)
{
	m_nPluQty += source.m_nPluQty;
	m_nLineStamps += source.m_nLineStamps;
}

//**********************************************************************

void CSQLPurchaseControlAuditLine::ConvertFromString(CString strLine)
{
	Reset();
	CCSV csvLine(strLine);

	int n = 1;
	switch (csvLine.GetInt(0))
	{
	case 1:
		m_strEventDate = csvLine.GetString(n++);
		m_strEventTime = csvLine.GetString(n++);
		m_nSourceType = csvLine.GetInt(n++);
		m_nAppNo = csvLine.GetInt(n++);
		break;

	default:
		return;
	}

	int nDetailVer = csvLine.GetInt(n++);

	switch (nDetailVer)
	{
	case 1:
	case 2:
		switch (m_nAppNo)
		{
		case APPNO_SO_AWARD:
			m_nUserID = csvLine.GetInt64(n++);
			m_strUserName = csvLine.GetString(n++);
			m_nGroupNo = csvLine.GetInt(n++);

			if (nDetailVer >= 2)
			{
				m_nFolderSet = csvLine.GetInt(n++);
			}

			m_nTNo = csvLine.GetInt(n++);
			m_nCCNoFull = csvLine.GetInt(n++);
			m_nServerNo = csvLine.GetInt(n++);
			m_nControlID = csvLine.GetInt(n++);
			m_strControlName = csvLine.GetString(n++);
			m_strAwardDate = csvLine.GetString(n++);
			m_strExpiryDate = csvLine.GetString(n++);
			m_nPluNo = csvLine.GetInt64(n++);
			m_strPluDescription = csvLine.GetString(n++);
			m_nPluQty = csvLine.GetInt(n++);
			m_nLineStamps = csvLine.GetInt(n++);
			m_nNewStampCount = csvLine.GetInt(n++);
			break;

		case APPNO_SO_MDEL:
		case APPNO_SO_MDEL_EXPIRE:
		case APPNO_SO_MFIX_EXPIRE:
		case APPNO_SO_XDEL_EXPIRE:
		case APPNO_SO_MEDIT:
		case APPNO_SO_MADD:
		case APPNO_SO_ADEL:
		case APPNO_SO_XFER:
		case APPNO_SO_HOTL:
			m_nUserID = csvLine.GetInt64(n++);
			m_strUserName = csvLine.GetString(n++);
			m_nGroupNo = csvLine.GetInt(n++);
			m_nCCNoFull = csvLine.GetInt(n++);
			m_nControlID = csvLine.GetInt(n++);
			m_strControlName = csvLine.GetString(n++);
			m_strAwardDate = csvLine.GetString(n++);
			m_strExpiryDate = csvLine.GetString(n++);
			m_nLineStamps = csvLine.GetInt(n++);
			m_nNewStampCount = csvLine.GetInt(n++);
		}
		break;
	}
}

//**********************************************************************
//**********************************************************************
//**********************************************************************

CSQLPurchaseControlBuffer::CSQLPurchaseControlBuffer()
{
	Reset();
}

//**********************************************************************

void CSQLPurchaseControlBuffer::Reset()
{
	m_nUserID = 0;
	m_strUsername = "";
	m_nGroupNo = 0;
	m_nServerNo = 0;
	m_nFolderSet = 0;
	m_nTillNo = 0;
	m_strAwardDate = "";
	m_strSortDate = "";
	m_oleAwardDate = COleDateTime::GetCurrentTime();
	m_arrayAwards.RemoveAll();
	m_arrayRedeem.RemoveAll();
	m_arrayAuditLines.RemoveAll();
}

//**********************************************************************

void CSQLPurchaseControlBuffer::SetAccount(CSQLRowAccountFull& RowAccount)
{
	m_nUserID = RowAccount.GetUserID();
	m_strUsername = RowAccount.GetUsername();
	m_nGroupNo = RowAccount.GetGroupNo();
}

//**********************************************************************

void CSQLPurchaseControlBuffer::BufferAward(CSQLPurchaseControlPendingInfo& line)
{
	m_arrayAwards.Consolidate(line);
}

//**********************************************************************

void CSQLPurchaseControlBuffer::BufferRedeem(CSQLPurchaseControlPendingInfo& line)
{
	m_arrayRedeem.Consolidate(line);
}

//**********************************************************************

void CSQLPurchaseControlBuffer::BufferPCAction(int nAppNo, CSQLRowAccountFull& RowAccount, CSQLRowPurchaseControlStamp& RowStamps, CString strRuleName, int nNewStamps, int nStampCount)
{
	CSQLPurchaseControlAuditLine line;
	line.m_nSourceType = AUDIT_PC;
	line.m_nAppNo = nAppNo;
	line.m_nCCNoFull = 0;
	line.m_nUserID = RowAccount.GetUserID();
	line.m_strUserName = RowAccount.GetUsername();
	line.m_nGroupNo = RowAccount.GetGroupNo();
	line.m_nControlID = RowStamps.GetRuleID();
	line.m_strControlName = strRuleName;
	line.m_strAwardDate = RowStamps.GetAwardDate();
	line.m_strExpiryDate = RowStamps.GetExpireDate();
	line.m_nLineStamps = nNewStamps;
	line.m_nNewStampCount = nStampCount;
	BufferAuditLine(line);
}

//**********************************************************************

bool CSQLPurchaseControlBuffer::ApplyPurchaseControlStampAward(int n, CDatabase* pDatabase)
{
	bool bResult = TRUE;

	if ((n >= 0) && (n < m_arrayAwards.GetSize()) && (m_nUserID > 0))
	{
		if (m_strSortDate = "")
		{
			ValidateAwardDate();
		}

		CSQLPurchaseControlPendingInfo info;
		m_arrayAwards.GetAt(n, info);

		if (info.m_nLineStamps > 0)
		{
			CSQLPurchaseControlAuditLine Line;
			Line.m_nLineStamps = info.m_nLineStamps;
			Line.m_nControlID = info.m_nControlID;
			Line.m_strControlName = info.m_strControlName;
			Line.m_nPluNo = info.m_nPluNo;
			Line.m_strPluDescription = info.m_strPluDescription;
			Line.m_nPluQty = info.m_nPluQty;

			CSQLRowPurchaseControlStamp Row;
			Row.SetUserID(m_nUserID);
			Row.SetRuleID(Line.m_nControlID);
			Row.SetAwardDate(m_strSortDate);

			CString strExpireDate = CSQLRowPurchaseControlStamp::GetNeverExpireDate();

			if (info.m_nExpiryDays >= 1)
			{
				COleDateTime oleExpireDate = m_oleAwardDate;

				if (info.m_nExpiryDays > 1)
				{
					CDateRangeHelpers DateRangeHelpers;
					DateRangeHelpers.AddDaysToTime(oleExpireDate, info.m_nExpiryDays - 1);
				}

				strExpireDate.Format("%4.4d%2.2d%2.2d",
					oleExpireDate.GetYear(),
					oleExpireDate.GetMonth(),
					oleExpireDate.GetDay());
			}

			Row.SetExpireDate(strExpireDate);

			CSQLRepositoryPurchaseControlStamp RepoStamp;
			CSQLResultInfo info = RepoStamp.SelectRow(Row, pDatabase);

			if (info.GetSQLError() == SQLCRUD_ERR_NONE)
			{
				Row.SetStampCount(Row.GetStampCount() + Line.m_nLineStamps);

				if (RepoStamp.UpdateRow(Row, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
				{
					bResult = FALSE;
				}
			}
			else if (info.GetSQLError() == SQLCRUD_ERR_NOMATCH)
			{
				Row.SetStampCount(Line.m_nLineStamps);

				if (RepoStamp.InsertRow(Row, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
				{
					bResult = FALSE;
				}
			}
			else
			{
				bResult = FALSE;
			}

			if (TRUE == bResult)
			{
				Line.m_nSourceType = AUDIT_POS;
				Line.m_nAppNo = APPNO_SO_AWARD;
				Line.m_nUserID = m_nUserID;
				Line.m_strUserName = m_strUsername;
				Line.m_nGroupNo = m_nGroupNo;
				Line.m_nFolderSet = m_nFolderSet;
				Line.m_nTNo = m_nTillNo;
				Line.m_nServerNo = m_nServerNo;
				Line.m_strAwardDate = Row.GetAwardDate();
				Line.m_strExpiryDate = Row.GetExpireDate();
				Line.m_nNewStampCount = Row.GetStampCount();
				BufferAuditLine(Line);
			}
		}
	}

	return bResult;
}

//**********************************************************************

bool CSQLPurchaseControlBuffer::ApplyPurchaseControlStampRedeem(int n, int& nLastRuleID, __int64& nLastPluNo,  CDatabase* pDatabase)
{
	return TRUE;
}

//**********************************************************************

void CSQLPurchaseControlBuffer::ValidateAwardDate()
{
	m_oleAwardDate = COleDateTime::GetCurrentTime();
	
	if (m_strAwardDate.GetLength() == 8)
	{
		CString strDate = "";
		strDate += "20";
		strDate += m_strAwardDate.Right(2);
		strDate += m_strAwardDate.Mid(3, 2);
		strDate += m_strAwardDate.Left(2);

		if (::TestNumeric(strDate) == TRUE)
		{
			COleDateTime dateTest = COleDateTime(
				atoi(strDate.Left(4)),
				atoi(strDate.Mid(4, 2)),
				atoi(strDate.Right(2)),
				0, 0, 0);

			if (dateTest.m_status == COleDateTime::valid)
			{
				m_oleAwardDate = dateTest;
			}
		}
	}

	m_strSortDate.Format("%4.4d%2.2d%2.2d",
		m_oleAwardDate.GetYear(),
		m_oleAwardDate.GetMonth(),
		m_oleAwardDate.GetDay());
}

//**********************************************************************

void CSQLPurchaseControlBuffer::WritePCAuditLines(CDatabase* pDatabase)
{
	if (m_arrayAuditLines.GetSize() > 0)
	{
		CSQLRepositoryCCNo repoCC;
		WriteAuditLines(repoCC.GetNextCCNoForPC(pDatabase));
	}
}

//**********************************************************************

void CSQLPurchaseControlBuffer::WriteAuditLines(int nCCNo)
{
	CString strFolder = Filenames.GetPurchaseControlLogFolder();
	CreateSubdirectory(strFolder);

	CString strDateNow = "";
	CString strTimeNow = "";
	{
		COleDateTime dateNow = COleDateTime::GetCurrentTime();
		strDateNow.Format("%4.4d%2.2d%2.2d",
			dateNow.GetYear(),
			dateNow.GetMonth(),
			dateNow.GetDay());

		strTimeNow.Format("%2.2d%2.2d%2.2d",
			dateNow.GetHour(),
			dateNow.GetMinute(),
			dateNow.GetSecond());
	}

	CString strFilename = "";
	strFilename += strFolder;
	strFilename += "\\SO_";
	strFilename += strDateNow;
	strFilename += ".dat";

	CSSFile fileLog;
	if (fileLog.Open(strFilename, "ab") == TRUE)
	{
		for (int n = 0; n < m_arrayAuditLines.GetSize(); n++)
		{
			CSQLPurchaseControlAuditLine line = m_arrayAuditLines.GetAt(n);

			CCSV csv;
			csv.Add(1);	//SEARCH VERSION
			csv.Add(strDateNow);
			csv.Add(strTimeNow);
			csv.Add(line.m_nSourceType);
			csv.Add(line.m_nAppNo);
			
			switch (line.m_nAppNo)
			{
			case APPNO_SO_AWARD:
				csv.Add(2);	//MAIN DATA VERSION
				csv.Add(line.m_nUserID);
				csv.Add(line.m_strUserName);
				csv.Add(line.m_nGroupNo);
				csv.Add(line.m_nFolderSet);
				csv.Add(line.m_nTNo);
				csv.Add(nCCNo);
				csv.Add(line.m_nServerNo);
				csv.Add(line.m_nControlID);
				csv.Add(line.m_strControlName);
				csv.Add(line.m_strAwardDate);
				csv.Add(line.m_strExpiryDate);
				csv.Add(line.m_nPluNo);
				csv.Add(line.m_strPluDescription);
				csv.Add(line.m_nPluQty);
				csv.Add(line.m_nLineStamps);
				csv.Add(line.m_nNewStampCount);
				break;

			case APPNO_SO_MDEL:
			case APPNO_SO_MDEL_EXPIRE:
			case APPNO_SO_MFIX_EXPIRE:
			case APPNO_SO_XDEL_EXPIRE:
			case APPNO_SO_MEDIT:
			case APPNO_SO_MADD:
			case APPNO_SO_ADEL:
			case APPNO_SO_XFER:
			case APPNO_SO_HOTL:
				csv.Add(1);	//MAIN DATA VERSION
				csv.Add(line.m_nUserID);
				csv.Add(line.m_strUserName);
				csv.Add(line.m_nGroupNo);
				csv.Add(nCCNo);
				csv.Add(line.m_nControlID);
				csv.Add(line.m_strControlName);
				csv.Add(line.m_strAwardDate);
				csv.Add(line.m_strExpiryDate);
				csv.Add(line.m_nLineStamps);
				csv.Add(line.m_nNewStampCount);
				break;
			}

			fileLog.WriteLine(csv.GetLine());
		}
	}
}

//**********************************************************************
