//*******************************************************************
#include "PaymentAuditRecord.h"
//*******************************************************************
#define nVERSIONNO		1
//*******************************************************************
#define szEOD_MARKER	"$$EOD"
//*******************************************************************
// used by SmartPayServer & Audit reporting

CPaymentAuditRecord::CPaymentAuditRecord()
{
	m_dateFiled.SetCurrentDate();		// audit filed date
	m_timeFiled.SetCurrentTime();		// audit filed time
	m_strUserID = "";					// User ID ( or '$$EOD' as eod marker )
	m_dateLastUsed.SetDate("");			// date last used @ ecr
	m_timeLastUsed.SetTime("");			// time last used @ ecr
	m_strOperatorID = "";
	m_nFolderSet = 0;
	m_nTerminalNo = 0;
	m_strTransactionNo = "";
	m_nPaymentType = 0;
	m_dPaymentValue1 = 0;				// Purse1 spend / All other spends ( depend on Type )
	m_dPaymentValue2 = 0;				// Type 20 only - Purse2 spend
	m_dPaymentValue3 = 0;				// Type 20 only - Purse3 spend
	m_bExternalMode = FALSE;
}

//**********************************************************************

bool CPaymentAuditRecord::SaveEODEntry()
{
	m_strUserID = szEOD_MARKER;
	return SaveEntry();
}

//**********************************************************************
// must call extract before calling this routine

bool CPaymentAuditRecord::IsEODEntry()
{
	return ( m_strUserID == szEOD_MARKER ) ? TRUE : FALSE;
}

//**********************************************************************
// save newpayment audit line to audit file

bool CPaymentAuditRecord::SaveEntry()
{
	return SaveEntry ( Filenames.GetPaymentAuditFilename() );
}

//**********************************************************************

bool CPaymentAuditRecord::SaveEntry(const char* szFilename)
{
	bool bReply = FALSE;

	CSSFile file;
	if ((bReply = file.Open(szFilename, "ab")) == TRUE)
	{
		bReply = WriteLine(&file);
		file.Close();
	}

	return bReply;
}

//**********************************************************************

bool CPaymentAuditRecord::WriteLine(CSSFile* pFile)
{
	CCSV csv(',', '"', TRUE, TRUE, TRUE);

	csv.Add(m_dateFiled.GetCSVDate());				// audit filed date
	csv.Add(m_timeFiled.GetCSVTime());				// audit filed time
	csv.Add(nVERSIONNO);
	csv.Add(m_strUserID);							// UserID if any
	csv.Add(m_dateLastUsed.GetCSVDate());			// date
	csv.Add(m_timeLastUsed.GetCSVTime());			// time
	csv.Add(m_strOperatorID);
	csv.Add(m_nTerminalNo);							// terminalID or EOD CCNo reference
	csv.Add(m_strTransactionNo);
	csv.Add(m_nPaymentType);
	csv.Add(m_dPaymentValue1, 2);
	csv.Add(m_dPaymentValue2, 2);
	csv.Add(m_dPaymentValue3, 2);
	csv.Add(m_bExternalMode);
	csv.Add(m_nFolderSet);

	return pFile->Write(csv);
}

//*********************************************************************

bool CPaymentAuditRecord::ReadEntry(CSSFile* file)
{
	CCSV csv{};
	while (TRUE)
	{
		if (file->Read(csv) == FALSE)
		{
			return FALSE;							// end of file
		}

		if (csv.GetSize() > 0)					// lose blank lines, just in case
		{
			break;
		}
	}

	return ExtractData(&csv);
}

//*********************************************************************

bool CPaymentAuditRecord::ExtractData(CCSV* pCsv)
{
	m_dateFiled.SetDate(pCsv->GetString(0));		// date filed
	m_timeFiled.SetTime(pCsv->GetString(1));		// time filed
	// version no	
	m_strUserID = pCsv->GetString(3);
	m_dateLastUsed.SetDate(pCsv->GetString(4));		// date used
	m_timeLastUsed.SetTime(pCsv->GetString(5));		// time used
	m_strOperatorID = pCsv->GetString(6);
	m_nTerminalNo = pCsv->GetInt(7);
	m_strTransactionNo = pCsv->GetString(8);
	m_nPaymentType = pCsv->GetInt(9);
	m_dPaymentValue1 = pCsv->GetDouble(10);			// Purse1 spend / Other
	m_dPaymentValue2 = pCsv->GetDouble(11);			// Purse2 spend
	m_dPaymentValue3 = pCsv->GetDouble(12);			// Purse3 spend
	m_bExternalMode = pCsv->GetBool(13);
	m_nFolderSet = pCsv->GetInt(14);
	return TRUE;
}

//**********************************************************************

