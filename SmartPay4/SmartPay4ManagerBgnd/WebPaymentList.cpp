/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

//#include "GlobalFunctions.h"
#include "WebPaymentList.h"

static const char* szHEADER = "PaymentID,ServiceID,Amount,Date,Time,UserID,Name";
static const char* szSORTFIELD = "PaymentID,ascending";
static int nRETRIES = 10;

/******************************************************************************/
//	mode = DB_READONLY, DB_READWRITE

int CWebPaymentListDatabase::Open ( const char* szFilename, int nMode )
{
	CreateSSDatabase ( szFilename, "", szHEADER, szSORTFIELD, nRETRIES );	// create db if not found
	Close();

	bool bHaveHeader = FALSE;
	CSSUnsortedDatabase db;
	if ( db.OpenSSDatabaseReadOnlyImplied ( szFilename, szHEADER, nRETRIES ) == DB_ERR_NONE )
	{
		CString strLabel = db.GetString ( "PaymentID" );
		if ( strLabel == "PaymentID" )
			bHaveHeader = TRUE;
		db.Close();
	}

	if ( bHaveHeader == TRUE )
	{
		if ( nMode == DB_READONLY )
			return OpenSSDatabaseReadOnly ( szFilename, szSORTFIELD, nRETRIES );

		return OpenSSDatabaseReadWrite ( szFilename, "", szSORTFIELD, nRETRIES );
	}
	else
	{
		if ( nMode == DB_READONLY )
			return OpenSSDatabaseReadOnlyImplied ( szFilename, szHEADER, szSORTFIELD, nRETRIES );

		return OpenSSDatabaseReadWriteImplied ( szFilename, "", szHEADER, szSORTFIELD, nRETRIES );
	}
}

//******************************************************************
