//$$******************************************************************
#include "WebPaymentLogRecord.h"
//$$******************************************************************

CWebPaymentLogRecord::CWebPaymentLogRecord()
{
	m_strPrefix = "";									// gets set to cardno or error message
	m_strField1 = "";
	m_strField2 = "";

	m_nPaymentID = 0;
	m_nServiceID = 0;
	m_dPaidAmount = 0.0;
	m_strPaidDate = "";
	m_strReceiptCode = "";
	m_strTransactionRef = "";
	m_strPupilID = "";
	m_strPupilName = "";
	m_strDOB = "";
	m_strNotes = "";

	m_date.SetCurrentDate();
	m_time.SetCurrentTime();
}

//$$******************************************************************

bool CWebPaymentLogRecord::SaveLine(CSSFile* fpFile)
{
	CCSV csv;

	csv.Add(m_strPrefix);								// card no or error message
	csv.Add(m_nPaymentID);								// payment id
	csv.Add(m_nServiceID);								// 
	csv.Add(m_dPaidAmount, 2);							// amount
	csv.Add(m_strPaidDate);								// date amount paid
	csv.Add(m_strReceiptCode);
	csv.Add(m_strTransactionRef);
	csv.Add(m_strPupilID);								// pupil id
	csv.Add(m_strPupilName);
	csv.Add(m_strDOB);
	csv.Add(m_strNotes);

	return fpFile->Write(csv);
}

//$$******************************************************************

bool CWebPaymentLogRecord::ReadLine(CSSFile* fpFile)
{
	CCSV csv;
	if (fpFile->Read(csv) == FALSE)
	{
		return FALSE;
	}

	m_strPrefix = csv.GetString(0);							// gets set to cardno or error message or /date
	m_strField1 = csv.GetString(1);							// paymentid or importdate
	m_strField2 = csv.GetString(2);							// serviceid or importtime

	m_nPaymentID = _atoi64(m_strField1);
	m_nServiceID = _atoi64(m_strField2);
	m_dPaidAmount = csv.GetDouble(3);							// PaidAmount

	m_strPaidDate = csv.GetString(4);
	m_strReceiptCode = csv.GetString(5);
	m_strTransactionRef = csv.GetString(6);
	m_strPupilID = csv.GetString(7);
	m_strPupilName = csv.GetString(8);
	m_strDOB = csv.GetString(9);
	m_strNotes = csv.GetString(10);

	return TRUE;
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

CString CWebPaymentLogRecord::GetLogFileName()
{
	CString strLogFileName;
	strLogFileName.Format ( "In%d%2.2d", m_date.GetYear(), m_date.GetMonth() );
	return strLogFileName;
}

//$$******************************************************************

CString CWebPaymentLogRecord::GetExceptionFileName()
{
	CString strExceptionName;
	strExceptionName.Format ( "Ex%d%2.2d", m_date.GetYear(), m_date.GetMonth() );
	return strExceptionName;
}

//$$******************************************************************

CString CWebPaymentLogRecord::GetNewExceptionFileName()
{
	CString strExceptionName = "";
	strExceptionName.Format("NewEx%d%2.2d", m_date.GetYear(), m_date.GetMonth());
	return strExceptionName;
}

//$$******************************************************************

const char* CWebPaymentLogRecord::GetHeaderDate()
{ 
	return m_strField1; 
}

//$$******************************************************************

const char* CWebPaymentLogRecord::GetHeaderTime()	
{ 
	return m_strField2; 
}

//$$******************************************************************

bool CWebPaymentLogRecord::IsHeaderLine()
{
	return (m_strPrefix == "/DATE");
}

//$$******************************************************************

bool CWebPaymentLogRecord::LogValidImport(const char* szFilename, const char* szCardNo, int& nValidCounter)
{
	return LogImport(szFilename, szCardNo, nValidCounter);
}

//$$******************************************************************

bool CWebPaymentLogRecord::LogAlreadyProcessed(const char* szFilename, const char* szDate, const char* szTime, int& nExceptionCounter)
{
	CString strMsg = "";
	strMsg.Format("Already processed ( %s %s )", szDate, szTime);

	return LogImport(szFilename, strMsg, nExceptionCounter);
}

//$$******************************************************************

bool CWebPaymentLogRecord::LogException ( const char* szFilename, const char* szIndicator, const char* szErrorText, int& nExceptionCounter )
{
	CSSFile file;
	file.Open ( szIndicator, "ab" );					// create indicator file to show have some new exceptions
	file.Close();

	return LogImport ( szFilename, szErrorText, nExceptionCounter );
}

//$$******************************************************************
// NB: increments counter
//$$******************************************************************

bool CWebPaymentLogRecord::LogImport(const char* szFilename, const char* szText, int& nCounter)
{
	bool bReply = FALSE;

	CSSFile file;
	if ((bReply = file.Open(szFilename, "ab")) == TRUE)
	{
		if (0 == nCounter)					// save date import line if first new line
		{
			CString strLine;
			strLine.Format("/DATE,%2.2d/%2.2d/%4.4d,%2.2d:%2.2d:%2.2d", m_date.GetDay(), m_date.GetMonth(), m_date.GetYear(), m_time.GetHour(), m_time.GetMinute(), m_time.GetSecond());
			file.WriteLine(strLine);
		}

		SetPrefix(szText);
		SaveLine(&file);						// write log line

		++nCounter;
		file.Close();
	}

	return bReply;
}

//$$******************************************************************
