#pragma once

//**********************************************************************

#define nWONDETYPE_NONE			0
#define nWONDETYPE_PUPIL		1
#define nWONDETYPE_STAFF		2

//**********************************************************************

namespace DBWonde
{
const recString		UserID			= { "MIS_ID",			0, 0 };
const recString		Type			= { "Type",				0, 0 };			// Pupil / Staff
const recString		AdNo			= { "AdmissionNumber",	0, 0 };
const recString		UserName		= { "PreferredFullName",0, 0 };
const recString		Forename		= { "LegalForename",	0, 0 };
const recString		Surname			= { "LegalSurname",		0, 0 };
const recString		DOB				= { "DOB",				0, 0 };
const recString		UPN				= { "UPN",				0, 0 };
const recString		Year			= { "YearGroup",		0, 0 };
const recString		Reg				= { "RegGroup",			0, 0 };
const recString		House			= { "House",			0, 0 };
const recString		Gender			= { "Gender",			0, 0 };
const recString		FSM				= { "FSM",				0, 0 };
const recString		DietaryNeeds	= { "DietaryNeeds",		0, 0 };
const recString		InDate			= { "AdmissionDate",	0, 0 };  
const recString		OutDate			= { "LeavingDate",		0, 0 };
const recString		Email			= { "Email",			0, 0 };
const recString		Email2			= { "Email2",			0, 0 };
};
 
//**********************************************************************

class CWondeDatabase : public CSSStringDatabase
{
public:
	CWondeDatabase() {}
	virtual const char* FormatKey ( const char* szKey );

	int Open ( const char* szFilename, int nMode );
	int GetPercentPosition();

	void Set_MISID( const char* szText )		{ CSSDatabase::Set ( DBWonde::UserID.Label, szText ); }
	const char* GetMISID()						{ return CSSDatabase::GetString ( DBWonde::UserID.Label ); }

	void SetTypePupil()							{ CSSDatabase::Set ( DBWonde::Type.Label, "Pupil" ); }
	void SetTypeStaff()							{ CSSDatabase::Set ( DBWonde::Type.Label, "Staff" ); }
	const char* GetType()						{ return CSSDatabase::GetString ( DBWonde::Type.Label ); }
	int GetTypeNo();

	void SetUserName ( const char* szText )		{ CSSDatabase::Set ( DBWonde::UserName.Label, szText ); }
	const char* GetUserName()					{ return CSSDatabase::GetString ( DBWonde::UserName.Label ); }

	void SetForename ( const char* szText )		{ CSSDatabase::Set ( DBWonde::Forename.Label, szText ); }
	const char* GetForename()					{ return CSSDatabase::GetString ( DBWonde::Forename.Label ); }

	void SetSurname ( const char* szText )		{ CSSDatabase::Set ( DBWonde::Surname.Label, szText ); }
	const char* GetSurname()					{ return CSSDatabase::GetString ( DBWonde::Surname.Label ); }

	void SetAdNo ( const char* szText )			{ CSSDatabase::Set ( DBWonde::AdNo.Label, szText ); }
	const char* GetAdNo()						{ return CSSDatabase::GetString ( DBWonde::AdNo.Label ); }

	void SetUPN ( const char* szText )			{ CSSDatabase::Set ( DBWonde::UPN.Label, szText ); }
	const char* GetUPN()						{ return CSSDatabase::GetString ( DBWonde::UPN.Label ); }

	void SetYear ( const char* szText )			{ CSSDatabase::Set ( DBWonde::Year.Label, szText ); }
	const char* GetYear()						{ return CSSDatabase::GetString ( DBWonde::Year.Label ); }

	void SetReg ( const char* szText )			{ CSSDatabase::Set ( DBWonde::Reg.Label, szText ); }
	const char* GetReg()						{ return CSSDatabase::GetString ( DBWonde::Reg.Label ); }

	void SetHouse ( const char* szText )		{ CSSDatabase::Set ( DBWonde::House.Label, szText ); }
	const char* GetHouse()						{ return CSSDatabase::GetString ( DBWonde::House.Label ); }

	void SetDOB ( const char* szYYYYMMDD );
	const char* GetDOB()						{ return CSSDatabase::GetString ( DBWonde::DOB.Label ); }

	void SetGender ( const char* szText )		{ CSSDatabase::Set ( DBWonde::Gender.Label, szText ); }
	const char* GetGender()						{ return CSSDatabase::GetString ( DBWonde::Gender.Label ); }

	void SetFSM ( const char* szText )			{ CSSDatabase::Set ( DBWonde::FSM.Label, szText ); }
	const char* GetFSM()						{ return CSSDatabase::GetString ( DBWonde::FSM.Label ); }
	bool IsFSM();

	void SetDietaryNeed ( const char* szText );
	const char* GetDietaryNeeds()				{ return CSSDatabase::GetString ( DBWonde::DietaryNeeds.Label ); }
	
	void SetInDate ( const char* szYYYYMMDD );
	const char* GetInDate()						{ return CSSDatabase::GetString ( DBWonde::InDate.Label ); }

	void SetOutDate ( const char* szYYYYMMDD );
	const char* GetOutDate()					{ return CSSDatabase::GetString ( DBWonde::OutDate.Label ); }

	void SetEmail ( const char* szText )		{ CSSDatabase::Set ( DBWonde::Email.Label, szText ); }
	const char* GetEmail()						{ return CSSDatabase::GetString ( DBWonde::Email.Label ); }

	void SetEmail2 ( const char* szText )		{ CSSDatabase::Set ( DBWonde::Email2.Label, szText ); }
	const char* GetEmail2()						{ return CSSDatabase::GetString ( DBWonde::Email2.Label ); }

private:
	CString GetDate ( const char* szYYYYMMDD );

private:
	CString m_strKey;
};
 