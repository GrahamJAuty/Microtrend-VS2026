#include "TransactionReferenceDatabase.h"

static const char* szHEADER		= "Account,RefNo";
static const char* szSORTFIELD	= "Account,ascending";
static int nRETRIES = 10;

/**********************************************************************/

CTransactionReferenceDatabase::CTransactionReferenceDatabase()
{
	m_strKey = "";
}

/******************************************************************************/
//	mode = DB_READONLY, DB_READWRITE

int CTransactionReferenceDatabase::OpenForSharing ( const char* szFilename, int nMode )
{
	m_strDatabaseFilename = szFilename;

	CreateSSDatabase ( m_strDatabaseFilename, "", szHEADER, szSORTFIELD, nRETRIES );	// create db if not found
	Close();

	if ( nMode == DB_READONLY )
		return OpenSSDatabaseReadOnly ( m_strDatabaseFilename, szSORTFIELD, nRETRIES );

	return OpenSSDatabaseReadWrite ( m_strDatabaseFilename, "", szSORTFIELD, nRETRIES );
}	

/**********************************************************************/

const char* CTransactionReferenceDatabase::GetHeader()
{
	return szHEADER;
}

//*********************************************************************
// CardNo = database key

const char* CTransactionReferenceDatabase::FormatKey ( const char* szKey )
{
	m_strKey = szKey;
	AddLeading ( m_strKey, TransRef::AccountNo.Max, '0' );
	return m_strKey;
}

//*********************************************************************

CString CTransactionReferenceDatabase::GetNextReference()
{
	__int64 nRefNo = CSSDatabase::GetInt64 ( TransRef::ReferenceNo.Label );

	CSSDatabase::Set ( TransRef::ReferenceNo.Label, ++nRefNo );

	CString strReply = ::FormatInt64Value( nRefNo );

	return strReply;
}

//*********************************************************************

