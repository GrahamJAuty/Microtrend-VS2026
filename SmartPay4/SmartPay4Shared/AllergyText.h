#pragma once
//*****************************************************
#include "EposAllergyDatabase.h"
//*****************************************************

class CAllergyText : public CEposAllergyDatabase
{
public:
	CAllergyText();
	int GetAllergyCount() { return GetRecordCount(); }
	int MatchAllergies ( int nCode );

	bool HaveAllergy ( int nAllergyCode );
	void SetAllergy ( int nAllergyCode, bool bSet = TRUE );
	void SetAllergy ( const char* szText );

	void SetAllergyCodes ( int nFlags ) { m_nAllergyFlags = nFlags; }
	int GetAllergyCodes() { return m_nAllergyFlags; }
	CString GetAllergyString ();

private:
	int m_nAllergyFlags;
};

//*****************************************************
