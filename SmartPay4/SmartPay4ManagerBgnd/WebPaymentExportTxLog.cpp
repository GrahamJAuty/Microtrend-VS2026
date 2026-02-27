//$$******************************************************************
#include "WebPaymentExport.h"
//$$******************************************************************
#include "WebPaymentExportTxLog.h"
//$$******************************************************************
#define nVERSION_NO		1
//$$******************************************************************

CWebPaymentExportTxLog::CWebPaymentExportTxLog()
{
}

//$$******************************************************************
// Type		= nWEBEXPORT_BALANCES / nWEBEXPORT_UPDATES
// TxOK		= Tx successful 
// comment	= Good lines sent / Error text
//$$******************************************************************

bool CWebPaymentExportTxLog::SaveLog ( int nType, int nResultCode, const char* szComment )
{
	CSSDate date;
	date.SetCurrentDate();

	CString strFilename;
	strFilename.Format ( "Tx%d%2.2d.dat", date.GetYear(), date.GetMonth() );

	CString strLogFilePath = System.GetWebPaymentPath ( strFilename );

	bool bReply;
	CSSFile file;
	if ( ( bReply = file.Open ( strLogFilePath, "ab" ) ) == TRUE )
	{
		CCSV csv;
		csv.Add ( CSSDate::GetCurrentDate().GetCSVDate() );
		csv.Add ( CSSTime::GetCurrentTime().GetCSVTime() );
		csv.Add ( nVERSION_NO );
		csv.Add ( nType );
		csv.Add ( nResultCode );
		csv.Add ( szComment );
		file.Write ( csv );
		file.Close();
	}

	return bReply;
}

//$$******************************************************************

bool CWebPaymentExportTxLog::ReadLine ( CSSFile* fp )
{
	CCSV csv;
	if ( fp->Read(csv) == TRUE )
	{
		m_date.SetDate ( csv.GetString(0) );
		m_time.SetTime ( csv.GetString(1) );
//		m_nVersion		= csv.GetInt(2);
		m_nType			= csv.GetInt(3);
		m_nResultCode	= csv.GetInt(4);
		m_strComment	= csv.GetString(5);
		return TRUE;
	}
	return FALSE;
}

//$$******************************************************************

CString CWebPaymentExportTxLog::GetTypeString()
{
	CString strText;

	switch ( m_nType )
	{
	case nWEBEXPORT_BALANCES:		strText = "Balance";		break;
	case nWEBEXPORT_UPDATES:		strText = "Transaction";	break;
	}
	return strText;
}

//$$******************************************************************

CString CWebPaymentExportTxLog::GetResultString()
{
	CString strText;

	switch ( m_nResultCode )
	{
	case 0:	strText = "OK";			break;
	case 1:	strText = "Failed";		break;
	}
	return strText;
}

//$$******************************************************************
