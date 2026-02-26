#pragma once
//*******************************************************************

class CLoyaltyAuditAppend
{
public:
	CLoyaltyAuditAppend(const char* szFilename = "");

public:
	bool AppendFile ( const char* szFilename );
	bool AppendArray( CStringArray& array );

private:
	CString m_strAuditFile;
};

//*******************************************************************

