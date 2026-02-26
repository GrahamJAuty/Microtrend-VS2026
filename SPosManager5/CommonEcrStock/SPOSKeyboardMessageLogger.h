#pragma once
/**********************************************************************/

class CSPOSKeyboardMessageLogger  
{
public:
	CSPOSKeyboardMessageLogger();
	void Reset();

public:
	void Initialise( const char* szFilename, bool bStockman, int nUserID );
	void LogFileWrite( const char* szFilename, int nResult, int nLastError );
	
private:
	CSSLogFile m_LogFileWrite;
	bool m_bStockman;
	int m_nUserID;
};

/**********************************************************************/
