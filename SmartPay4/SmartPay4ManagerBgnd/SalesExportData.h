#pragma once
//**********************************************************************

class CSalesExportData  
{
public:
	CSalesExportData();
	void Read();

	CString GetUploadHeader ( const char* szAdditionlHeader );
	CString GetUpdatesFilename()			{ return System.GetWebPaymentPath ( "SxFile03.dat", nWEBPAYMENT_EXPORTSALES ); }
	CString GetInternalExportFilename()		{ return System.GetWebPaymentPath ( "updates.csv", nWEBPAYMENT_EXPORTSALES ); }
	CString GetExternalExportFilename()		{ return System.GetWebPaymentPath ( "updates.txt", nWEBPAYMENT_EXPORTSALES ); }

	char m_nCsvExternalDelimiter;

private:
	CString m_strFilename;
};

//**********************************************************************
