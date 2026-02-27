#pragma once

class CWebPaymentExportCsvLog
{
public:
	CWebPaymentExportCsvLog();
	bool SaveLog ( const char* szPreFix, const char* szUpdatesFile );
	bool SaveLog ( const char* szPreFix, const char* szUpdatesFile, int nLineCount );

	int StripLog ( const char* szSource, const char* szDestination );

private:
	bool AppendLog ( const char* szPrefix, const char* szUpdatesFile, int nLineCount );
};
