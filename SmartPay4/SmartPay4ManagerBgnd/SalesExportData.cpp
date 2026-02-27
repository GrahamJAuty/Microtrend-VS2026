//**********************************************************************
#include "SalesExportData.h"
//**********************************************************************

#define szSTANDARD_HEADER "SequenceNo,Date,Time,Type,Description,Purse1Value,Purse2Value,Purse3Value,CashValue,CCNo,Purse1Balance,Purse2Balance,Purse3Balance"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSalesExportData::CSalesExportData()
{
	m_nCsvExternalDelimiter = ',';

	m_strFilename = System.GetWebPaymentPath ( "SalesExp.dat", nWEBPAYMENT_EXPORTSALES );

}

CString CSalesExportData::GetUploadHeader ( const char* szAdditionalHeader )
{
	CString strExtraHeader = szAdditionalHeader;

	CString strHeader = szSTANDARD_HEADER;
	if ( strExtraHeader != "" )
	{
		strHeader +=",";
		strHeader += strExtraHeader;					// add to end of line
	}

	return strHeader;	
}

//******************************************************************

void CSalesExportData::Read()
{
	CSSIniFile file;
	if ( file.Read ( m_strFilename ) == TRUE )
	{
		CString strBuf;		strBuf = file.GetString ( "Delimiter", "," );
		m_nCsvExternalDelimiter = strBuf.GetAt(0);
	}
}

//*********************************************************************

//void CSalesExportData::Save()
//{
//	CSSIniFile file;
//
//	file.Set ( "Delimiter", m_nCsvExternalDelimiter );
//
//	file.Write ( m_strFilename );
//}

//******************************************************************

