#pragma once

class CSchoolcommsExport  
{
public:
	CSchoolcommsExport();
	bool SendTransactions ( const char* szCsvFilename, const char* szLoginReplyFile );
	bool SendBalances ( const char* szCsvFilename, const char* szLoginReplyFile );
	const char* GetError() { return m_strError; }

private:
	void SendUpdates ( const char* szSource, const char* szLoginReply );

private:
	CString m_strError;
};
