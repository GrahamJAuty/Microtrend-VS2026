#pragma once
//**********************************************************************

#define nMAX_ALLERGIES	14

//**********************************************************************

namespace EposAllergyData
{
const recInt	No		= { "No",	1, 50 };
const recString	Text	= { "Text",	0, 20 };
}

//**********************************************************************

class CEposAllergyDatabase: public CSSIntDatabase
{
public:
	CEposAllergyDatabase();
	int Read();

	int GetNo()			{ return GetInt ( EposAllergyData::No.Label ); }
	CString GetText()	{ return GetString ( EposAllergyData::Text.Label ); }

private:
	void CreateDefault();

private:
	CString m_strFilename;
};

/**********************************************************************/