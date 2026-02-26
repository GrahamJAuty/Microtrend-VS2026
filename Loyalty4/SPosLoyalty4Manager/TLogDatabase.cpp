// Open a TLogFile purchase history as a Unsorted database
// & find the purchases made on a particular Date \ Time & Ecr CCNo
// 
// Used to add purchase items text to an Ecr transaction line in atc audit report line
//
//*****************************************************************************

#include "TLogRecord.h"
#include "TLogDatabase.h"

//*****************************************************************************

CTLogDatabase::CTLogDatabase()
{
	m_strSalesText = "";
}

//******************************************************************************
// as TLogFile does not have a header - uses implied oolumn field names ( col1, col2 .. colnn )
// see TlogRecord for colnn labels

int CTLogDatabase::Open ( const char* szFilename )
{
	return OpenSSDatabaseReadOnlyImplied ( szFilename, "<col>", 10 );
}

//******************************************************************

bool CTLogDatabase::Find(const char* szDate, const char* szTime, int nCCNo)
{
	bool bReply = FALSE;
	CString strKey = MakeKey(szDate, szTime, nCCNo);

	int nLow = 0;
	int nLowest = GetRecordCount();
	int nHigh = nLowest - 1;

	while (nLow <= nHigh)
	{
		int nMid = (nLow + nHigh) / 2;
		MoveTo(nMid);

		CTLogRecord tlog;
		tlog.ConvertRecord(this);					// convert database line to a tlog record

		CString strTlogKey = MakeKey(tlog.GetDate(), tlog.GetTime(), tlog.GetCCNo());

		if (strKey < strTlogKey)
		{
			nHigh = nMid - 1;
		}

		else if (strKey > strTlogKey)
		{
			nLow = nMid + 1;
		}
		else										// match
		{
			double dSalesValue = 0.0;						// sales value not used
			tlog.GetCombinedValues(m_strSalesText, dSalesValue);	// just extract purchases texts
			bReply = TRUE;
			break;
		}
	}

	return bReply;
}

//*******************************************************************

CString CTLogDatabase::MakeKey(const char* szDate, const char* szTime, int nCCNo)
{
	CSSDate date(szDate);
	CSSTime time(szTime);

	CString strKey = "";
	strKey.Format("%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d%4.4d", date.GetYear(), date.GetMonth(), date.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond(), nCCNo);
	return strKey;
}
 
//*******************************************************************
