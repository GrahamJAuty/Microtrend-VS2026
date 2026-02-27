//*****************************************************************************
#include "CardCounterDatabase.h"
//*****************************************************************************

static const char* szHEADER		= "CardNo";
static const char* szSORTFIELD	= "CardNo,ascending";
static int nRETRIES				= 10;

/******************************************************************************/
//	mode = DB_READONLY, DB_READWRITE

int CCardCounterDatabase::Open ( const char* szFilename )
{
	int nRetries = 10;

	CreateSSDatabase( szFilename, "", szHEADER, szSORTFIELD, nRETRIES );	// create db if not found
	Close();

	return OpenSSDatabaseReadWrite ( szFilename, "", szSORTFIELD, nRETRIES );
}	

//******************************************************************
// return TRUE if not already added, else FALSE ( alreday added )

bool CCardCounterDatabase::AddCard ( const char* szCardNo )
{
	CString strLabel = System.FormatCardNo(szCardNo);

	if ( FindRecord ( strLabel ) == FALSE )
	{
		AddBlankRecord ( strLabel );
		return TRUE;
	}
	return FALSE;
}

//******************************************************************
