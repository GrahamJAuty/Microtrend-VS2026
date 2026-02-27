//*******************************************************************
//*** exactly same file used in KioskCardLink \ KioskBioLink ********
//*******************************************************************

#pragma once

//*******************************************************************

#define nPASSWORD_ERROR_OK		0
#define nPASSWORD_ERROR_NOFILE	1
#define nPASSWORD_ERROR_NOMATCH	2

//*******************************************************************

class CPasswordData
{
public:
	CPasswordData(void);
	CString GetPassword ( bool bDefaultPasswordAllowed = TRUE );
	bool CheckPassword ( const char* szPassword );
	bool SetPassword ( const char* szPassword );
	CString GetErrorMessage();

private:
	CString m_strHiddenPassword;
	CString m_strDefaultPassword;
	int m_nErrorNo;
};
