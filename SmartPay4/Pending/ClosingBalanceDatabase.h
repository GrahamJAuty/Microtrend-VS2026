#pragma once
//**********************************************************************

namespace DBClosing
{
const recString		Date			= { "D1",	 0, 0 };	// Date
const recString		Time			= { "T1",	 0, 0 };	// Time
const recString		ArchiveDate		= { "D2",	 0, 0 };	// Archived Date
const recString		ArchiveTime		= { "T2",	 0, 0 };	// Archived Time
const recDouble		Purse1Liability	= { "A1",	 0, 0 };	// A1 = purse1
const recDouble		Purse2Balance	= { "A2",	 0, 0 };	// A2 = purse2
const recDouble		Purse3Liability	= { "A3",	 0, 0 };	// A3 = purse3
const recDouble		Purse1Credit	= { "C1",	 0, 0 };	// C1 = purse1 credit
const recDouble		Purse3Credit	= { "C3",	 0, 0 };	// C3 = purse1 credit
};

//**********************************************************************

class CClosingBalanceDatabase : public CSSStringDatabase
{
public:
	CClosingBalanceDatabase() {}
	int Open ( const char* szFilename, int nMode );
	CString MakeKey ( CSSDate* pDate );

	void SetDate ( const char* szCsvDate ){ CSSDatabase::Set ( DBClosing::Date.Label, szCsvDate ); }
	CString GetDate();

	void SetTime ( const char* szCsvTime = "" );
	CString GetTime();

	void SetArchived();
	bool IsArchived();

	void SetPurse1Liability ( double dPurse1 ){ CSSDatabase::Set ( DBClosing::Purse1Liability.Label, dPurse1, System.m_nDPValue ); }
	double GetPurse1Liability(){ return CSSDatabase::GetDouble ( DBClosing::Purse1Liability.Label ); }

	void SetPurse2Balance ( double dPurse2 ){ CSSDatabase::Set ( DBClosing::Purse2Balance.Label, dPurse2, System.m_nDPValue ); }
	double GetPurse2Balance(){ return CSSDatabase::GetDouble ( DBClosing::Purse2Balance.Label ); }

	void SetPurse3Liability ( double dPurse3 ){ CSSDatabase::Set ( DBClosing::Purse3Liability.Label, dPurse3, System.m_nDPValue ); }
	double GetPurse3Liability(){ return CSSDatabase::GetDouble ( DBClosing::Purse3Liability.Label ); }

	void SetPurse1Credit(double dPurse1) { CSSDatabase::Set(DBClosing::Purse1Credit.Label, dPurse1, System.m_nDPValue); }
	double GetPurse1Credit() { return CSSDatabase::GetDouble(DBClosing::Purse1Credit.Label); }

	void SetPurse3Credit(double dPurse3) { CSSDatabase::Set(DBClosing::Purse3Credit.Label, dPurse3, System.m_nDPValue); }
	double GetPurse3Credit() { return CSSDatabase::GetDouble(DBClosing::Purse3Credit.Label); }
};

//**********************************************************************
