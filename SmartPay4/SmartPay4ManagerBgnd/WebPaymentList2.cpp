//**********************************************************************************
// Used by Tucasi to convert Tucasi transactionItemNumbers from 9999 to range 0 - 99
//**********************************************************************************

#include "WebPaymentList2.h"

//******************************************************************************

static const char* szHEADER		= "PaymentID,ItemList";
static const char* szSORTFIELD	= "PaymentID,ascending";
static int nRETRIES				= 10;

//******************************************************************************
//	mode = DB_READONLY, DB_READWRITE

int CWebPaymentList2Database::Open ( const char* szFilename, int nMode )
{
	int nRetries = 10;

	CreateSSDatabase ( szFilename, "", szHEADER, szSORTFIELD, nRETRIES );	// create db if not found
	Close();

	if ( nMode == DB_READONLY )
		return OpenSSDatabaseReadOnly ( szFilename, szSORTFIELD, nRETRIES );

	return OpenSSDatabaseReadWrite ( szFilename, "", szSORTFIELD, nRETRIES );
}	

//******************************************************************
// as any Tucasi 1 transactioNumber can only ever have a max of 99 itemNumbers
// convert tucasi item number into an internal range of 0 to 99

int CWebPaymentList2Database::ConvertItemNo ( __int64 nTransactionNo, const char* szItemNo )
{
	if (CSSInt64Database::FindRecord(nTransactionNo) == FALSE)		// see if transactionNumber been used before
	{
		CSSInt64Database::AddBlankRecord(nTransactionNo);					// no - add record
	}

	CCSV csv ( CSSInt64Database::GetString ( "ItemList" ) );			// get list of already assoicated itemNumbers
	int nItemCount = csv.GetSize();

	int nIndex;
	for ( nIndex = 0 ; nIndex < nItemCount ; nIndex++ )
	{
		if ( lstrcmp ( csv.GetString(nIndex), szItemNo ) == 0 )			// match found - return index position 
			break;
	}

	if ( nIndex == nItemCount )											// not found
	{
		csv.Add ( szItemNo );
		CSSInt64Database::Set ( "ItemList", csv.GetLine() );			// add to end of line & return end index position
	}

	return nIndex;
}

//******************************************************************

