/**********************************************************************/

#include "local.h"

/**********************************************************************/

static BYTE ucCode[96] = 
{
	0x32,0x4e,0x60,0x2b,0x34,0x6a,0x40,0x2c,0x33,0x44,0x4d,0x2f,0x50,0x6f,0x52,0x43,
	0x54,0x29,0x70,0x53,0x41,0x78,0x4f,0x20,0x39,0x61,0x48,0x51,0x6b,0x30,0x28,0x67,
	0x55,0x2d,0x23,0x74,0x35,0x58,0x6c,0x4a,0x31,0x66,0x4b,0x65,0x59,0x42,0x26,0x71,
	0x2e,0x56,0x22,0x72,0x5a,0x57,0x21,0x5b,0x73,0x36,0x4c,0x7b,0x7e,0x37,0x2a,0x64,
	0x75,0x6d,0x5f,0x38,0x25,0x3f,0x68,0x7a,0x3a,0x6e,0x49,0x76,0x3c,0x62,0x69,0x7c,
	0x24,0x27,0x63,0x45,0x5e,0x3b,0x47,0x3d,0x79,0x46,0x77,0x7f,0x5d,0x7d,0x5c,0x3e,
};

/**********************************************************************/

static BYTE CodeChar ( BYTE ucChar )
{
	if ( ucChar >= 0x20 && ucChar <= 0x7f )
	{
		for ( int i = 0 ; i < 96 ; i++ )
			if ( ucCode[i] == ucChar )
				return ( i + 0x20 );
	}
	return ucChar;
}

/**********************************************************************/

static BYTE DecodeChar ( BYTE ucChar )
{
	if ( ucChar >= 0x20 && ucChar <= 0x7f )
		return ucCode[ucChar-0x20];

	return ucChar;
}

/**********************************************************************/

CString CSysset::CodeMessage ( const char* szText )
{
	CString strText;

	for ( int i = 0 ; i < (int) strlen(szText) ; i++ )
		strText += CodeChar(szText[i]);

	return strText;
}

/**********************************************************************/

CString CSysset::DecodeMessage ( const char* szText )
{
	CString strText;

	for ( int i = 0 ; i < (int) strlen(szText) ; i++ )
		strText += DecodeChar(szText[i]);

	return strText;
}

/**********************************************************************/

bool CSysset::LockAllUsers()
{
	if ( m_bSpawned == TRUE )	// spawned programs are not allowed to lock all users
	{
		m_strError = "Lock not allowed";
		return FALSE;
	}

	if ( m_bUsersLocked == FALSE )
	{
		int nMaxUsers = m_UserData.GetMaxUsers();

		if ( nMaxUsers > 1 )
		{
			for ( int nInstNo = 1 ; nInstNo <= nMaxUsers ; nInstNo++ )
			{
				if ( nInstNo != m_nInstanceNo )	// current instance is already locked (no need to lock again)
				{
					if ( m_LockArray.LockUser ( GetShareFilename(nInstNo), m_IdentityInfo, GetPCFolder(m_nPCIdHost), GetPCFolder(m_nPCIdClient)) == FALSE )	// another instance is in use, so we are unable to lock all users and must fail
					{
						// unlock any that we have just successfully locked

						m_LockArray.UnlockAllExcept ( GetShareFilename(m_nInstanceNo) );	// this will not unlock any created by other instances

						m_strError = "";	// no error message
						return FALSE;		// unable to lock all users
					}
				}
			}
		}
	}

	m_bUsersLocked = TRUE;
	return TRUE;
}

/**********************************************************************/

bool CSysset::LockOtherProgram( CString strMaster )
{
	if ( m_bUsersLocked == FALSE )
	{
		int nMaxUsers = m_UserData.GetMaxUsers();

		for ( int nInstNo = 1 ; nInstNo <= nMaxUsers ; nInstNo++ )
		{
			IdentityInfo info;
			info.m_strUserName = "Locked By";
			info.m_strComputerName = strMaster;

			if ( m_LockArray.LockUser ( GetShareFilename(nInstNo), info, "", "" ) == FALSE)	// another instance is in use, so we are unable to lock all users and must fail
			{
				// unlock any that we have just successfully locked

				m_LockArray.UnlockOtherProgram();	// this will not unlock any created by other instances

				m_strError = "";	// no error message
				return FALSE;		// unable to lock all users
			}
		}
	}

	m_bUsersLocked = TRUE;
	return TRUE;
}

/**********************************************************************/

void CSysset::UnlockAllUsers()
{
	if ( m_bUsersLocked == TRUE )
	{
		m_LockArray.UnlockAllExcept ( GetShareFilename(m_nInstanceNo) );
		m_bUsersLocked = FALSE;
	}
}

/**********************************************************************/

void CSysset::UnlockOtherProgram()
{
	if ( m_bUsersLocked == TRUE )
	{
		m_LockArray.UnlockOtherProgram();
		m_bUsersLocked = FALSE;
	}
}

/**********************************************************************/
