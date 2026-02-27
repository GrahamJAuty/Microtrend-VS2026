//*******************************************************************
//*** exactly same file used in KioskCardLink \ KioskBioLink ********
//*******************************************************************

#include <io.h>
#include "SystemData.h"
#include "PasswordData.h"

//*******************************************************************

CPasswordData::CPasswordData(void)
{
	m_nErrorNo = nPASSWORD_ERROR_NOFILE;
	m_strHiddenPassword = "75910170";					// reversed
	m_strDefaultPassword = "~'218dQk%-7s";				// junk password
}

//*******************************************************************

bool CPasswordData::CheckPassword ( const char* szUserPassword )
{
	CString strUserPassword   = szUserPassword;
	CString strAccessPassword = GetPassword ( FALSE );

	if ( strAccessPassword.MakeUpper() == strUserPassword.MakeUpper() )
		return TRUE;

	m_strHiddenPassword.MakeUpper();
	if ( strUserPassword == m_strHiddenPassword.MakeReverse() )
		return TRUE;

	if ( strUserPassword == m_strHiddenPassword.MakeReverse() )
		return TRUE;

	if ( _access ( System.m_strPasswordFile, 0 ) == 0 ) 
	{
		if ( strAccessPassword == "" )
			return TRUE;

		m_nErrorNo = nPASSWORD_ERROR_NOMATCH;
	}
	else
		m_nErrorNo = nPASSWORD_ERROR_NOFILE;

	return FALSE;
}

//*******************************************************************

CString CPasswordData::GetPassword ( bool bDefaultPasswordAllowed )
{
	CString strReply = "";

	CSSIniFile ini ( SS_INI_AES );
	if ( ini.Read ( System.m_strPasswordFile ) == TRUE )
	{
		strReply = ini.GetString ( "AccessPassword", m_strDefaultPassword );	// default is junk
	}																	// if using password MUST have a valid one

	if ( strReply == m_strDefaultPassword )				// see if default - ie not found in file
	{
		if ( bDefaultPasswordAllowed == TRUE )
			strReply = "";
	}

	return strReply;
}

//*******************************************************************

bool CPasswordData::SetPassword ( const char* szPassword )
{
	CSSIniFile ini ( SS_INI_AES );

	ini.Set ( "AccessPassword", szPassword );

	return ini.Write ( System.m_strPasswordFile );
}

//*******************************************************************

CString CPasswordData::GetErrorMessage()
{
	CString strReply = "Ok";

	switch ( m_nErrorNo )
	{
	case nPASSWORD_ERROR_NOFILE:	strReply = "Password file is missing";	break;
	case nPASSWORD_ERROR_NOMATCH:	strReply = "Password not recognised";	break;
	}
	return strReply;
}


