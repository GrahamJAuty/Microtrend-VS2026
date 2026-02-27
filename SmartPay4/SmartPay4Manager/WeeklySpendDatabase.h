#pragma once

/**********************************************************************/

#define nSPEND_TTL		0
#define nSPEND_SUN		1			// SAME as days of week ( 1=sun, 2=Tue.. 6=sat )
#define nSPEND_MON		2
#define nSPEND_TUE		3
#define nSPEND_WED		4
#define nSPEND_THU		5
#define nSPEND_FRI		6
#define nSPEND_SAT		7

namespace WeeklySpend
{
const recString		UserID		= { "UID", 0, 10 };
const recString		Name		= { "N1", 0, 30 };
const recString		Surname		= { "N2", 0, 30 };
const recDouble		Sunday		= { "D1", 0, 9999999 };					
const recInt		SunCount	= { "C1", 0, 9999999 };
const recDouble		Monday		= { "D2", 0, 9999999 };					
const recInt		MonCount	= { "C2", 0, 9999999 };
const recDouble		Tuesday		= { "D3", 0, 9999999 };					
const recInt		TueCount	= { "C3", 0, 9999999 };
const recDouble		Wednesday	= { "D4", 0, 9999999 };					
const recInt		WedCount	= { "C4", 0, 9999999 };
const recDouble		Thursday	= { "D5", 0, 9999999 };					
const recInt		ThuCount	= { "C5", 0, 9999999 };
const recDouble		Friday		= { "D6", 0, 9999999 };					
const recInt		FriCount	= { "C6", 0, 9999999 };
const recDouble		Saturday	= { "D7", 0, 9999999 };					
const recInt		SatCount	= { "C7", 0, 9999999 };
}

/**********************************************************************/

class CWeeklySpendDatabase : public CSSStringDatabase
{
public:
	CWeeklySpendDatabase();
	virtual const char* FormatKey ( const char* szKey );
	bool OpenForSharing ( const char* szFilename );
	CString GetError() { return m_strError; }
	int GetPercentPosition();

	bool SetDayNo ( const char* szDate );
	void SaveSpend ( double dValue );
	void IncrementCount();

	CString GetUserID()						{ return (CString)CSSDatabase::GetString ( WeeklySpend::UserID.Label ); }
	void SetUserID ( const char* szText )	{ CSSDatabase::Set ( WeeklySpend::UserID.Label, szText ); }

	CString GetUserName()					{ return (CString)CSSDatabase::GetString ( WeeklySpend::Name.Label ); }
	void SetUserName ( const char* szText )	{ CSSDatabase::Set ( WeeklySpend::Name.Label, szText ); }

	CString GetSurname()					{ return (CString)CSSDatabase::GetString ( WeeklySpend::Surname.Label ); }
	void SetSurname ( const char* szText )	{ CSSDatabase::Set ( WeeklySpend::Surname.Label, szText ); }

	double GetSpend ( int nDayNo )			{ return GetSpend ( GetDayLabel ( nDayNo ) ); }
	int GetCount ( int nDayNo )				{ return GetCount ( GetCountLabel ( nDayNo ) ); }

private:
	CString GetDayLabel ( int nDayNo );
	CString GetCountLabel ( int nDayNo );

	double GetSpend ( const char*szLabel )		{ 	return (double)CSSDatabase::GetDouble ( szLabel );	}
	int GetCount ( const char* szLabel )		{	return CSSDatabase::GetInt ( szLabel ); }

private:
	CString m_strKey;
	int m_nDayNo;
};

/**********************************************************************/