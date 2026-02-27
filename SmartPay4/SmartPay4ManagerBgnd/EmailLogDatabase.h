#pragma once

//**********************************************************************

//#include "SystemData.h"

namespace DBEmailLog
{
const recString		CardNo			= { "K1",	 0, 0 };	// Cardno
const recString		CardName		= { "N1",	 0, 0 };	// Name
const recString		DateLastSent	= { "D1",	 0, 0 };	// Date emailed sent
const recString		TimeLastSent	= { "T1",	 0, 0 };	// Time emailed sent
const recString		EmailAddress	= { "E1",	 0, 0 };	// email address

const recDouble		Purse1			= { "AA",	 0, 0 };	// AA = purse1 balance
const recString		P1DateLastUsed	= { "AD",	 0, 0 };	// Date last used
const recString		P1TimeLastUsed	= { "AT",	 0, 0 };	// time last used

const recDouble		Purse2			= { "BA",	 0, 0 };	// AB = purse2 balance
const recString		P2DateLastUsed	= { "BD",	 0, 0 };	// Date last used
const recString		P2TimeLastUsed	= { "BT",	 0, 0 };	// time last used

const recDouble		Purse3			= { "CA",	 0, 0 };	// AC = purse3 balance
const recString		P3DateLastUsed	= { "CD",	 0, 0 };	// Date last used
const recString		P3TimeLastUsed	= { "CT",	 0, 0 };	// time last used

const recInt		Points			= { "DA",	 0, 0 };	// BA = points balance
};


//**********************************************************************

class CEmailLogDatabase : public CSSStringDatabase
{
public:
	CEmailLogDatabase();
	int Open ( const char* szFilename, int nMode );

	const char* FormatKey ( const char* szCardNo );

	void SetCardName ( const char* szText ) { CSSDatabase::Set ( DBEmailLog::CardName.Label, szText ); }

	void SetDateLastSent ( const char* szDate = "" );
	CString GetDateLastSent(){ return CSSDatabase::GetString ( DBEmailLog::DateLastSent.Label ); }

	void SetTimeLastSent ( const char* szTime = "" );
	CString GetTimeLastSent(){ return CSSDatabase::GetString ( DBEmailLog::TimeLastSent.Label ); }

	void SetEmailAddress ( const char* szText ) { CSSDatabase::Set ( DBEmailLog::EmailAddress.Label, szText ); }

// purse1

	void SetPurse1 ( double dPurse1 ){ CSSDatabase::Set ( DBEmailLog::Purse1.Label, dPurse1, System.GetDPValue() ); }
	double GetPurse1(){ return CSSDatabase::GetDouble ( DBEmailLog::Purse1.Label ); }

	void SetPurse1DateLastUsed ( const char* szDate ){ CSSDatabase::Set ( DBEmailLog::P1DateLastUsed.Label, szDate ); }
	CString GetPurse1DateLastUsed(){ return CSSDatabase::GetString ( DBEmailLog::P1DateLastUsed.Label ); }

	void SetPurse1TimeLastUsed ( const char* szTime ) { CSSDatabase::Set ( DBEmailLog::P1TimeLastUsed.Label, szTime ); }
	CString GetPurse1TimeLastUsed(){ return CSSDatabase::GetString ( DBEmailLog::P1TimeLastUsed.Label ); }

// purse2

	void SetPurse2 ( double dPurse2 ){ CSSDatabase::Set ( DBEmailLog::Purse2.Label, dPurse2, System.GetDPValue() ); }
	double GetPurse2(){ return CSSDatabase::GetDouble ( DBEmailLog::Purse2.Label ); }

	void SetPurse2DateLastUsed ( const char* szDate ){ CSSDatabase::Set ( DBEmailLog::P2DateLastUsed.Label, szDate ); }
	CString GetPurse2DateLastUsed(){ return CSSDatabase::GetString ( DBEmailLog::P2DateLastUsed.Label ); }

	void SetPurse2TimeLastUsed ( const char* szTime ) { CSSDatabase::Set ( DBEmailLog::P2TimeLastUsed.Label, szTime ); }
	CString GetPurse2TimeLastUsed(){ return CSSDatabase::GetString ( DBEmailLog::P2TimeLastUsed.Label ); }

// purse 3

	void SetPurse3 ( double dPurse3 ){ CSSDatabase::Set ( DBEmailLog::Purse3.Label, dPurse3, System.GetDPValue() ); }
	double GetPurse3(){ return CSSDatabase::GetDouble ( DBEmailLog::Purse3.Label ); }

	void SetPurse3DateLastUsed ( const char* szDate ){ CSSDatabase::Set ( DBEmailLog::P3DateLastUsed.Label, szDate ); }
	CString GetPurse3DateLastUsed(){ return CSSDatabase::GetString ( DBEmailLog::P3DateLastUsed.Label ); }

	void SetPurse3TimeLastUsed ( const char* szTime ) { CSSDatabase::Set ( DBEmailLog::P3TimeLastUsed.Label, szTime ); }
	CString GetPurse3TimeLastUsed(){ return CSSDatabase::GetString ( DBEmailLog::P3TimeLastUsed.Label ); }

// points

	void SetPoints ( int nPoints ){ CSSDatabase::Set ( DBEmailLog::Points.Label, nPoints ); }
	int GetPoints(){ return CSSDatabase::GetInt ( DBEmailLog::Points.Label ); }

private:
	CString m_strKey;
};
 