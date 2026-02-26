#pragma once
/**********************************************************************/

namespace FamilyDatabase
{
const recString		Forename		= { "Forename",		0, 30 };
const recString		Surname			= { "Surname",		0, 30 };
const recString		Gender			= { "Gender",		0, 1 };
const recString		Dob				= { "DOB",			0, 10 };
const recInt		MemberType		= { "MType",		0, 2 };
const recString		AddedDate		= { "Added",		0, 8 };
}

/**********************************************************************/

class CFamilyDatabase : public CSSStringDatabase
{
public:
	CFamilyDatabase();
	virtual const char* FormatKey ( const char* szKey );
	bool OpenForSharing ( const char* szPathname, int nMode = DB_READONLY );
	CString GetError() { return m_strError; }

	void CopyToDatabase ( CFamilyDatabase* pDbCopy );

	CString GetForename()					{ return (CString)CSSDatabase::GetString ( FamilyDatabase::Forename.Label ); }
	void SetForename ( const char* szText )	{ CSSDatabase::Set ( FamilyDatabase::Forename.Label, szText ); }

	CString GetSurname()					{ return (CString)CSSDatabase::GetString ( FamilyDatabase::Surname.Label ); }
	void SetSurname ( const char* szText )	{ CSSDatabase::Set ( FamilyDatabase::Surname.Label, szText ); }

	void SetDateAdded ( const char* szDate = "" );
	CString GetDateAdded()					{ return (CString)CSSDatabase::GetString ( FamilyDatabase::AddedDate.Label ); }

	CString GetGender() { return (CString)CSSDatabase::GetString ( FamilyDatabase::Gender.Label ); }
	void SetGender ( const char* szGender );
	void SetGender ( int nGender );
	CString GetGenderText();
	bool IsGenderMale()		{ return ( GetGender() == "M" ) ? TRUE : FALSE;  }
	bool IsGenderFemale()	{ return ( GetGender() == "F" ) ? TRUE : FALSE; }
	bool IsGenderUnknown()	{ return ( GetGender() == "" ) ? TRUE : FALSE; }

	CString GetDobDate() { return (CString)CSSDatabase::GetString ( FamilyDatabase::Dob.Label ); }
	void SetDob ( const char* szDate )	{ SetDateField ( FamilyDatabase::Dob.Label, szDate ); }
	int GetAge();
	CString GetAgeText();

	int GetMemberType()					{ return (int)CSSDatabase::GetInt ( FamilyDatabase::MemberType.Label ); }
	void SetMemberType ( int nStatus )	{ CSSDatabase::Set ( FamilyDatabase::MemberType.Label, nStatus ); }
	CString GetMemberTypeText();

private:
	void SetDateField ( const char* szLabel, const char* szDate );
	void SetTimeField ( const char* szLabel, const char* szTime );

private:
	CString m_strKey;
	CString m_strFilename;
};

/**********************************************************************/