#pragma once
//$$******************************************************************

class CReportInfoBirthday
{
public:
	CReportInfoBirthday();

public:
	//expiry
	bool GetExpiryFlag(){ return m_bExpiry; }
	const char* GetDateRangeType(){ return m_strDateRangeType; }
	const char* GetDateFromString();
	const char* GetDateToString();
	CSSDate GetDateFrom(){ return m_dateFrom; }
	CSSDate GetDateTo(){ return m_dateTo; }
	bool GetExpiryMonthFilterFlag(){ return m_bExpiryMonthFilter; }
	//dob
	bool GetDOBFlag(){ return m_bDOB; }
	bool GetMonthFlag( int nMonth );
			
public:
	const char* GetDateRangeString();

public:
	//expiry
	void SetExpiryFlag( bool b ){ m_bExpiry = b; }
	void SetDateRangeType( const char* sz ){ m_strDateRangeType = sz; }
	void SetDateFrom( CTime& date );
	void SetDateTo( CTime& date );
	void SetExpiryMonthFilterFlag( bool b ){ m_bExpiryMonthFilter = b; }
	//dob
	void SetDOBFlag( bool b ){ m_bDOB = b; }
	void SetMonthFlag( int nMonth, bool b );
		
private:
	//expiry
	bool m_bExpiry;
	CString m_strDateRangeType;
	CSSDate m_dateFrom;
	CSSDate m_dateTo;
	CString m_strDateFrom;
	CString m_strDateTo;
	bool m_bExpiryMonthFilter;
	//dob
	bool m_bDOB;
	bool m_bMonth[12];
	
private:
	CString m_strDateRange;
};

//$$******************************************************************

