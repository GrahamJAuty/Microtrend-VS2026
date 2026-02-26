#pragma once
/*********************************************************************/
#ifdef STOCKMAN_SYSTEM
/*********************************************************************/
 
/**********************************************************************/
#define MAX_ALLOWANCES 9
/*********************************************************************/

class CDefTextAllowance : public CSharedStringArray 
{
public:
	int Open( const char* szFilename, int nMode );

public:
	const char* GetDefaultText ( int nAllowance );
	const char* GetText ( int nAllowance );
	bool GetReportEnable ( int nAllowance );
	int GetTaxBand ( int nAllowance );
	void SetAllowance ( int nAllowance, const char* szText, int nTaxBand, bool bEnable );
	int GetEnableCount();

private:
	CString m_strDefText;
	CString m_strText;
};

/**********************************************************************/
#endif
/*********************************************************************/
