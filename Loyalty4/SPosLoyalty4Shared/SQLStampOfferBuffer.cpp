//**********************************************************************
#include "AuditReportTexts.h"
#include "DateRangeHelpers.h"
#include "SQLDefines.h"
#include "SQLNamespaces.h"
//**********************************************************************
#include "SQLTable_CCNo\SQLRepositoryCCNo.h"
#include "SQLTable_OfferStamp\SQLRepositoryOfferStamp.h"
#include "SQLTable_OfferStamp\SQLRecordSetOfferStamp.h"
#include "SQLTable_OfferStamp\SQLRowOfferStamp.h"
//**********************************************************************
#include "SQLStampOfferBuffer.h"
//**********************************************************************

int CSQLStampOfferPendingInfo::Compare(CSQLStampOfferPendingInfo& source, int nHint)
{
	if (m_nOfferID != source.m_nOfferID)
	{
		return (m_nOfferID > source.m_nOfferID ? 1 : -1);
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

void CSQLStampOfferPendingInfo::Add(CSQLStampOfferPendingInfo& source)
{
	m_nPluQty += source.m_nPluQty;
	m_nLineStamps += source.m_nLineStamps;
}

//**********************************************************************

void CSQLStampOfferAuditLine::ConvertFromString(CString strLine)
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
		case APPNO_SO_REDEEM:
			m_nUserID = csvLine.GetInt64(n++);
			m_strUserName = csvLine.GetString(n++);
			m_nGroupNo = csvLine.GetInt(n++);
			
			if (nDetailVer >= 2)
			{
				m_nDbNo = csvLine.GetInt(n++);
				m_nFolderSet = csvLine.GetInt(n++);
			}
			
			m_nTNo = csvLine.GetInt(n++);
			m_nCCNo = csvLine.GetInt(n++);
			m_nServerNo = csvLine.GetInt(n++);
			m_nOfferID = csvLine.GetInt(n++);
			m_nEdition = csvLine.GetInt(n++);
			m_strOfferName = csvLine.GetString(n++);
			m_strAwardDate = csvLine.GetString(n++);
			m_strExpiryDate = csvLine.GetString(n++);
			m_nPluNo = csvLine.GetInt64(n++);
			m_strPluDescription = csvLine.GetString(n++);
			m_nPluQty = csvLine.GetInt(n++);
			m_nLineStamps = csvLine.GetInt(n++);
			m_nNewStampCount = csvLine.GetInt(n++);
			break;

		case APPNO_SO_REDEEM2:
			m_strAwardDate = csvLine.GetString(n++);
			m_strExpiryDate = csvLine.GetString(n++);
			m_nLineStamps = csvLine.GetInt(n++);
			m_nNewStampCount = csvLine.GetInt(n++);
			break;

		case APPNO_SO_MDEL:
		case APPNO_SO_MDEL_EXPIRE:
		case APPNO_SO_MDEL_EDITION:
		case APPNO_SO_MFIX_EXPIRE:
		case APPNO_SO_MFIX_EDITION:
		case APPNO_SO_MEDIT:
		case APPNO_SO_MADD:
		case APPNO_SO_ADEL:
		case APPNO_SO_XFER:
		case APPNO_SO_HOTL:
			m_nUserID = csvLine.GetInt64(n++);
			m_strUserName = csvLine.GetString(n++);
			m_nGroupNo = csvLine.GetInt(n++);
			m_nCCNo = csvLine.GetInt(n++);
			m_nOfferID = csvLine.GetInt(n++);
			m_nEdition = csvLine.GetInt(n++);
			m_strOfferName = csvLine.GetString(n++);
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

CSQLStampOfferBuffer::CSQLStampOfferBuffer()
{
	Reset();
}

//**********************************************************************

void CSQLStampOfferBuffer::Reset()
{
	m_nUserID = 0;
	m_strUsername = "";
	m_nGroupNo = 0;
	m_nServerNo = 0;
	m_nDbNo = 0;
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

void CSQLStampOfferBuffer::SetAccount(CSQLRowAccountFull& RowAccount)
{
	m_nUserID = RowAccount.GetUserID();
	m_strUsername = RowAccount.GetFullname();
	m_nGroupNo = RowAccount.GetGroupNo();
}

//**********************************************************************

void CSQLStampOfferBuffer::BufferAward(CSQLStampOfferPendingInfo& line)
{
	m_arrayAwards.Consolidate(line);
}

//**********************************************************************

void CSQLStampOfferBuffer::BufferRedeem(CSQLStampOfferPendingInfo& line)
{
	m_arrayRedeem.Consolidate(line);
}

//**********************************************************************

void CSQLStampOfferBuffer::BufferPCAction(int nAppNo, CSQLRowAccountFull& RowAccount, CSQLRowOfferStamp& RowStamps, CString strOfferName, int nNewStamps, int nStampCount)
{
	CSQLStampOfferAuditLine line;
	line.m_nSourceType = AUDIT_PC;
	line.m_nAppNo = nAppNo;
	line.m_nCCNo = 0;
	line.m_nUserID = RowAccount.GetUserID();
	line.m_strUserName = RowAccount.GetFullname();
	line.m_nGroupNo = RowAccount.GetGroupNo();
	line.m_nOfferID = RowStamps.GetStampOfferID();
	line.m_strOfferName = strOfferName;
	line.m_strAwardDate = RowStamps.GetAwardDate();
	line.m_strExpiryDate = RowStamps.GetExpireDate();
	line.m_nLineStamps = nNewStamps;
	line.m_nNewStampCount = nStampCount;
	BufferAuditLine(line);
}

//**********************************************************************

bool CSQLStampOfferBuffer::ApplyOfferStampAward(int n, CDatabase* pDatabase)
{
	bool bResult = TRUE;

	if ((n >= 0) && (n < m_arrayAwards.GetSize()) && (m_nUserID > 0))
	{
		if (m_strSortDate = "")
		{
			ValidateAwardDate();
		}

		CSQLStampOfferPendingInfo info;
		m_arrayAwards.GetAt(n, info);

		if (info.m_nLineStamps > 0)
		{
			CSQLStampOfferAuditLine Line;
			Line.m_nLineStamps = info.m_nLineStamps;
			Line.m_nOfferID = info.m_nOfferID;
			Line.m_nEdition = info.m_nEdition;
			Line.m_strOfferName = info.m_strOfferName;
			Line.m_nPluNo = info.m_nPluNo;
			Line.m_strPluDescription = info.m_strPluDescription;
			Line.m_nPluQty = info.m_nPluQty;

			CSQLRowOfferStamp Row;
			Row.SetUserID(m_nUserID);
			Row.SetStampOfferID(Line.m_nOfferID);
			Row.SetEdition(Line.m_nEdition);
			Row.SetAwardDate(m_strSortDate);

			CString strExpireDate = CSQLRowOfferStamp::GetNeverExpireDate();

			if (info.m_nExpiry >= 1)
			{
				COleDateTime oleExpireDate = m_oleAwardDate;

				CDateRangeHelpers DateRangeHelpers;
				DateRangeHelpers.AddDaysToTime(oleExpireDate, info.m_nExpiry);

				strExpireDate.Format("%4.4d%2.2d%2.2d",
					oleExpireDate.GetYear(),
					oleExpireDate.GetMonth(),
					oleExpireDate.GetDay());
			}

			Row.SetExpireDate(strExpireDate);

			CSQLRepositoryOfferStamp repoStamp;
			CSQLResultInfo info = repoStamp.SelectRow(Row, pDatabase);

			if (info.GetSQLError() == SQLCRUD_ERR_NONE)
			{
				Row.SetStampCount(Row.GetStampCount() + Line.m_nLineStamps);

				if (repoStamp.UpdateRow(Row, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
				{
					bResult = FALSE;
				}
			}
			else if (info.GetSQLError() == SQLCRUD_ERR_NOMATCH)
			{
				Row.SetStampCount(Line.m_nLineStamps);

				if (repoStamp.InsertRow(Row, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
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
				Line.m_nDbNo = m_nDbNo;
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

bool CSQLStampOfferBuffer::ApplyOfferStampRedeem(int n, int& nLastOfferID, __int64& nLastPluNo,  CDatabase* pDatabase)
{
	bool bResult = TRUE;

	if ((n >= 0) && (n < m_arrayRedeem.GetSize()) && (m_nUserID > 0))
	{
		CSQLStampOfferPendingInfo info;
		m_arrayRedeem.GetAt(n,info);

		if ( info.m_nLineStamps < 0 )
		{
			CSQLStampOfferAuditLine Line;
			Line.m_nOfferID = info.m_nOfferID;
			Line.m_nEdition = info.m_nEdition;
			Line.m_strOfferName = info.m_strOfferName;
			Line.m_nPluNo = info.m_nPluNo;
			Line.m_strPluDescription = info.m_strPluDescription;
			Line.m_nPluQty = info.m_nPluQty;

			CSQLRecordSetOfferStamp RecordSet(pDatabase, 
				RSParams_OfferStamp_NormalByUserIdOfferIdEdition{
					SimpleFormatInt64Value(m_nUserID),
					Line.m_nOfferID,
					Line.m_nEdition
				});
				
			RecordSet.m_strSort = "";
			RecordSet.m_strSort += OfferStamp::ExpireDate.Label;
			RecordSet.m_strSort += ",";
			RecordSet.m_strSort += OfferStamp::AwardDate.Label;

			if (RecordSet.Open(CRecordset::forwardOnly, NULL, CRecordset::readOnly) == FALSE)
			{
				return FALSE;
			}

			CArray<CSQLRowOfferStamp, CSQLRowOfferStamp> arrayRows;
			int nStampsToRedeem = info.m_nLineStamps * (-1);

			while ((RecordSet.IsEOF() == FALSE) && (nStampsToRedeem > 0))
			{
				CSQLRowOfferStamp Row;
				RecordSet.SaveToOfferStampRow(Row);

				int nStampCount = Row.GetStampCount();
				int nRedeemedThisLine = 0;

				if (nStampCount >= 1)
				{
					if (nStampsToRedeem > nStampCount)
					{
						Row.SetStampCount(0);
						nRedeemedThisLine = nStampCount;
						nStampsToRedeem -= nStampCount;
						arrayRows.Add(Row);
					}
					else
					{
						Row.SetStampCount(nStampCount - nStampsToRedeem);
						nRedeemedThisLine = nStampsToRedeem;
						nStampsToRedeem = 0;
						arrayRows.Add(Row);
					}

					Line.m_nSourceType = AUDIT_POS;

					if ( (nLastOfferID == Line.m_nOfferID) && (nLastPluNo == Line.m_nPluNo) )
					{
						Line.m_nAppNo = APPNO_SO_REDEEM2;
					}
					else
					{
						Line.m_nAppNo = APPNO_SO_REDEEM;
					}

					nLastOfferID = Line.m_nOfferID;
					nLastPluNo = Line.m_nPluNo;

					Line.m_nUserID = m_nUserID;
					Line.m_strUserName = m_strUsername;
					Line.m_nGroupNo = m_nGroupNo;
					Line.m_nDbNo = m_nDbNo;
					Line.m_nFolderSet = m_nFolderSet;
					Line.m_nTNo = m_nTillNo;
					Line.m_nServerNo = m_nServerNo;
					Line.m_nLineStamps = nRedeemedThisLine * (-1);
					Line.m_nNewStampCount = Row.GetStampCount();
					Line.m_strAwardDate = Row.GetAwardDate();
					Line.m_strExpiryDate = Row.GetExpireDate();
					BufferAuditLine(Line);
				}

				RecordSet.MoveNext();
			}

			RecordSet.Close();

			for (int n = 0; n < arrayRows.GetSize(); n++)
			{
				CSQLRowOfferStamp Row = arrayRows.GetAt(n);

				CSQLRepositoryOfferStamp repoStamp;

				if (Row.GetStampCount() == 0)
				{
					if (repoStamp.DeleteRow(Row, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
					{
						return FALSE;
					}
				}
				else
				{
					if (repoStamp.UpdateRow(Row, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
					{
						return FALSE;
					}
				}
			}
		}
	}

	return bResult;
}

//**********************************************************************

void CSQLStampOfferBuffer::ValidateAwardDate()
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

void CSQLStampOfferBuffer::WritePCAuditLines(CDatabase* pDatabase)
{
	if (m_arrayAuditLines.GetSize() > 0)
	{
		CSQLRepositoryCCNo repoCC;
		WriteAuditLines(repoCC.GetNextCCNoForPC(pDatabase));
	}
}

//**********************************************************************

void CSQLStampOfferBuffer::WriteAuditLines(int nCCNo)
{
	CString strFolder = Filenames.GetStampOfferLogFolder();
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
			CSQLStampOfferAuditLine line = m_arrayAuditLines.GetAt(n);

			CCSV csv;
			csv.Add(1);	//SEARCH VERSION
			csv.Add(strDateNow);
			csv.Add(strTimeNow);
			csv.Add(line.m_nSourceType);
			csv.Add(line.m_nAppNo);
			
			switch (line.m_nAppNo)
			{
			case APPNO_SO_AWARD:
			case APPNO_SO_REDEEM:
				csv.Add(2);	//MAIN DATA VERSION
				csv.Add(line.m_nUserID);
				csv.Add(line.m_strUserName);
				csv.Add(line.m_nGroupNo);
				csv.Add(line.m_nDbNo);
				csv.Add(line.m_nFolderSet);
				csv.Add(line.m_nTNo);
				csv.Add(nCCNo);
				csv.Add(line.m_nServerNo);
				csv.Add(line.m_nOfferID);
				csv.Add(line.m_nEdition);
				csv.Add(line.m_strOfferName);
				csv.Add(line.m_strAwardDate);
				csv.Add(line.m_strExpiryDate);
				csv.Add(line.m_nPluNo);
				csv.Add(line.m_strPluDescription);
				csv.Add(line.m_nPluQty);
				csv.Add(line.m_nLineStamps);
				csv.Add(line.m_nNewStampCount);
				break;

			case APPNO_SO_REDEEM2:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
				csv.Add(1);	//MAIN DATA VERSION
				csv.Add(line.m_strAwardDate);
				csv.Add(line.m_strExpiryDate);
				csv.Add(line.m_nLineStamps);
				csv.Add(line.m_nNewStampCount);
				break;

			case APPNO_SO_MDEL:
			case APPNO_SO_MDEL_EXPIRE:
			case APPNO_SO_MDEL_EDITION:
			case APPNO_SO_MFIX_EXPIRE:
			case APPNO_SO_MFIX_EDITION:
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
				csv.Add(line.m_nOfferID);
				csv.Add(line.m_nEdition);
				csv.Add(line.m_strOfferName);
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
