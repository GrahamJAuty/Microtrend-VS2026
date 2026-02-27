//*****************************************************************************
#include "..\SmartPay4ManagerBgnd\DbExportHandler.h"
//*****************************************************************************
#include "PurchaseBreakdownDatabase.h"
//*****************************************************************************
#define nMAX_ORDERBTLENGTH		10
#define nMAX_TEXTLENGTH			20
//*****************************************************************************
static const char* szHEADER		= "K1,X1,X2,X3,X4,X5,X6,X7,X8,X9";
static const char* szSORTFIELD	= "K1,ascending";
static int nRETRIES				= 10;
/******************************************************************************/
//	mode = DB_READONLY, DB_READWRITE

int CPurchaseBreakdownDatabase::Open ( const char* szFilename, int nMode )
{
	int nRetries = 10;

	CreateSSDatabase ( szFilename, "", szHEADER, szSORTFIELD, nRETRIES );	// create db if not found
	Close();

	if ( nMode == DB_READONLY )
		return OpenSSDatabaseReadOnly ( szFilename, szSORTFIELD, nRETRIES );

	return OpenSSDatabaseReadWrite ( szFilename, "", szSORTFIELD, nRETRIES );
}	

//******************************************************************

CString CPurchaseBreakdownDatabase::MakeKey ( const char* szOrderBy, int nGroupNo, const char* szText )
{
	CString strKey, strBuf;

	strBuf = szOrderBy;
	CString strOrderBy = strBuf.Left(nMAX_ORDERBTLENGTH);			// first 10 characters of text
	::AddLeading ( strOrderBy, nMAX_ORDERBTLENGTH, ' ' );			// pad out

	strBuf = szText;
	CString strText = strBuf.Left(nMAX_TEXTLENGTH	);				// first 20 characters of text
	::AddTrailing ( strText, nMAX_TEXTLENGTH, ' ' );				// pad out

	strKey.Format ( "%s%2.2d%s", 
		(const char*) strOrderBy, 
		nGroupNo, 
		(const char*) strText );

	strKey.MakeUpper();
	return strKey;
}

//******************************************************************

void CPurchaseBreakdownDatabase::AddQty ( double dValue )
{
	double dNewValue = GetQty() + dValue;
	CSSDatabase::Set ( DBPurchaseBreakdown::Qty.Label, dNewValue, System.GetDPValue() );
}

void CPurchaseBreakdownDatabase::AddRefundQty ( double dValue )
{
	double dNewValue = GetRefundQty() + dValue;
	CSSDatabase::Set ( DBPurchaseBreakdown::RefundQty.Label, dNewValue, System.GetDPValue() );
}

void CPurchaseBreakdownDatabase::AddValue ( double dValue )
{
	double dNewValue = GetValue() + dValue;
	CSSDatabase::Set ( DBPurchaseBreakdown::Value.Label, dNewValue, System.GetDPValue() );
}

void CPurchaseBreakdownDatabase::AddPoints ( int nValue )
{
	int nNewValue = GetPoints() + nValue;
	CSSDatabase::Set ( DBPurchaseBreakdown::Points.Label, nNewValue );
}

void CPurchaseBreakdownDatabase::IncCount()
{
	int nNewValue = GetCount() + 1;
	CSSDatabase::Set ( DBPurchaseBreakdown::Count.Label, nNewValue );
}
