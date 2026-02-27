//**********************************************************************
#include "WebPaymentExportTnLog.h"
#include "SchoolcommsData.h"
#include "Schoolcomms.hpp"
//**********************************************************************
#include "SchoolcommsExport.h"
//**********************************************************************

CSchoolcommsExport::CSchoolcommsExport()
{
	m_strError = "";
}

//**********************************************************************
// csvfile in schoolcomms transaction file format ( with header line )

bool CSchoolcommsExport::SendBalances ( const char* szWebCsvFilename, const char* szLoginReplyFile )
{
	CString strTmpFile = Filenames.GetWebPaymentPath( ::GetUniqueTempFilename("$10"), nWEBPAYMENT_SCHOOLCOMMS);
	::CopyFile ( szWebCsvFilename, strTmpFile, FALSE );				// make copy as upload function will clear the file as it sends it

	SendUpdates ( strTmpFile, szLoginReplyFile );					// sets m_error on error

	return (m_strError == "");
}

//**********************************************************************
// csvfile in schoolcomms transaction file format ( with header line )

bool CSchoolcommsExport::SendTransactions ( const char* szWebCsvFilename, const char* szLoginReplyFile )
{
	CString strTmpFile = Filenames.GetWebPaymentPath( ::GetUniqueTempFilename("$11"), nWEBPAYMENT_SCHOOLCOMMS);
	::CopyFile ( szWebCsvFilename, strTmpFile, FALSE );				// make copy as upload function will clear the file as it sends it

	SendUpdates ( strTmpFile, szLoginReplyFile );					// sets m_error on error

	return ( m_strError == "" )	? TRUE : FALSE; 
}

//**********************************************************************

void CSchoolcommsExport::SendUpdates(const char* szTmpFile, const char* szLoginReply)
{
	m_strError = "";

	CSchoolcommsData data;
	data.Read();

	int nTxCount = 0;
	CSSSchoolcomms sc(&data);

	while (TRUE)
	{
		int nLineCount = ::GetFileLineCount(szTmpFile) - 1;	// ignore header line

		nTxCount = (nLineCount > data.m_nMaxTransactionLines) ? data.m_nMaxTransactionLines : nLineCount;
		if (nTxCount > 0)
		{
			if ((m_strError = sc.SendTransactions(szTmpFile, szLoginReply, nTxCount)) != "")
			{
				break;
			}

			::RemoveFileLines(szTmpFile, nTxCount, TRUE);
		}
		else
		{
			break;
		}
	}

	if (nTxCount == 0)												// check for empty file
	{
		CFileRecycler::SendToRecycleBin(szTmpFile);								// file should just have a header line if all send ok 
	}
}

//**********************************************************************
