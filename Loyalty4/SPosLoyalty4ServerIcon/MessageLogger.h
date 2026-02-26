#pragma once
//*******************************************************************

class CMessageLogger
{
public:
	CMessageLogger(void);
	~CMessageLogger(void);

public:
	void LogSystemMessage( CString strMessage );
	
private:
	void LogMessage( CString strContext, CString strMessage );
};

//*******************************************************************
extern CMessageLogger MessageLogger;
//*******************************************************************
