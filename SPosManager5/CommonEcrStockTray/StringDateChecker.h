#pragma once
/**********************************************************************/

class CStringDateChecker
{
public:
	CStringDateChecker();
	CStringDateChecker( const char* szDateFrom, const char* szDateTo );
	
public:
	bool IsValid(){ return m_bIsValid; }
	bool SetRange( const char* szDateFrom, const char* szDateTo );
	bool CheckDate( CString& strDate, COleDateTime& oleDate );
	bool CheckDate( bool bSaleTime, int nFileType, CString& strDateFolder, CString& strDateTran, CString& strDateSale );

private:
	bool m_bIsValid;
	CString m_strDateFrom;
	CString m_strDateTo;
};

/**********************************************************************/

