//**********************************************************************
#include "MembershipDatabase.h"
//**********************************************************************
static const char* szHEADER		= "MemberID,UserID";
static const char* szSORTFIELD	= "MemberID,ascending";
//**********************************************************************

CMemberDatabase::CMemberDatabase(CString strFilename)
{
	m_strError = "";
	m_strKey = "";
	m_strFilename = strFilename;
}

/******************************************************************************/
//	mode = DB_READONLY, DB_READWRITE

bool CMemberDatabase::OpenForSharing ( int nMode )
{
	int nRetries = 10;

	if (m_strFilename == "")
	{
		m_strError = "Membership database : No filename specified";
		return FALSE;
	}

	CreateSSDatabase ( m_strFilename, "", szHEADER, szSORTFIELD, nRetries );	// create db if not found
	Close();

	int nReply = 0;
	if (nMode == DB_READONLY)
	{
		nReply = OpenSSDatabaseReadOnly(m_strFilename, szSORTFIELD, nRetries);
	}
	else
	{
		nReply = OpenSSDatabaseReadWrite(m_strFilename, "", szSORTFIELD, nRetries);
	}

	if (nReply == DB_ERR_NONE)
	{
		return TRUE;
	}

	m_strError.Format ( "Membership database error - %s", (const char*) CSSStringDatabase::GetError() );
	return FALSE;
}	

/**********************************************************************/

const char* CMemberDatabase::GetHeader()
{
	return szHEADER;
}

/**********************************************************************/

const char* CMemberDatabase::FormatKey ( const char* szKey )
{
	m_strKey = szKey;
	AddLeading ( m_strKey, Membership::MemberID.Max, ' ' );
	return m_strKey;
}

/**********************************************************************/

int CMemberDatabase::GetPercentPosition()
{
	return  ( GetCurrentRow() * 100L ) / GetRecordCount();
}

//*********************************************************************
//*** validation checking**********************************************
//*********************************************************************

void CMemberDatabase::SetField ( const char* strLabel, const char* szText, int nMax )
{
	CString strText = szText;
	::TrimSpacesFromString( strText );			// loose leading\trailing spaces

	int nLen = strText.GetLength();
	if ( nLen > nMax )
	{
		strText = strText.Left(nMax);
		nLen = nMax;
	}

	for ( int i = 0 ; i < nLen ; i++ )
	{
		if (strText.GetAt(i) < ' ')			//	conver Ctrl chars to space
		{
			strText.SetAt(i, ' ');
		}
	}

	CSSDatabase::Set ( strLabel, strText );
}

//*******************************************************************
