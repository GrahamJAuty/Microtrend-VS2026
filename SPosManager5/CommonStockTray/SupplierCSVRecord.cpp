/**********************************************************************/
 
/**********************************************************************/
//#include "globalfunctions.h"
/**********************************************************************/
#include "SupplierCSVArray.h"
/**********************************************************************/

CSupplierCSVRecord::CSupplierCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CSupplierCSVRecord::ClearRecord()
{
	m_strKey = "1";
	m_nSuppNo = 1;
	m_strName = "";
	m_strAddress1 = "";
	m_strAddress2 = "";
	m_strAddress3 = "";
	m_strAddress4 = "";
	m_strAddress5 = "";
	m_strPostcode = "";
	m_strPhone = "";
	m_strFax = "";
	m_strEmail = "";
	m_strAccountNo = "";
	m_strFAO = "";
}

/**********************************************************************/

void CSupplierCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_strKey );
	csv.Add ( m_nSuppNo );
	csv.Add ( SUPPLIERDB_VERSION );				
	csv.Add ( m_strName );
	csv.Add ( m_strAddress1 );
	csv.Add ( m_strAddress2 );
	csv.Add ( m_strAddress3 );
	csv.Add ( m_strAddress4 );
	csv.Add ( m_strAddress5 );
	csv.Add ( m_strPostcode );
	csv.Add ( m_strPhone );
	csv.Add ( m_strFax );
	csv.Add ( m_strEmail );
	csv.Add ( m_strAccountNo );
	csv.Add ( m_strFAO );
}

/**********************************************************************/

void CSupplierCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	m_strKey = csv.GetString(0);
	m_nSuppNo = csv.GetInt(1);
	
	switch ( csv.GetInt(2) )
	{
	case 1:	V1ConvertFromCSV( csv );	break;
	}
}

/**********************************************************************/

void CSupplierCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	int n = 3;
	m_strName = csv.GetString(n++);
	m_strAddress1 = csv.GetString(n++);
	m_strAddress2 = csv.GetString(n++);
	m_strAddress3 = csv.GetString(n++);
	m_strAddress4 = csv.GetString(n++);
	m_strAddress5 = csv.GetString(n++);
	m_strPostcode = csv.GetString(n++);
	m_strPhone = csv.GetString(n++);
	m_strFax = csv.GetString(n++);
	m_strEmail = csv.GetString(n++);
	m_strAccountNo = csv.GetString(n++);
	m_strFAO = csv.GetString(n++);
}

/**********************************************************************/
//	Get header
/**********************************************************************/

CString CSupplierCSVRecord::GetHeader()
{
	CString strHeader;
	CCSV csv;
	csv.Add ( Supplier::SupplierKey.Label );
	csv.Add ( Supplier::SupplierNo.Label );	//PROTECTED SUPPLIER NUMBER FIELD
	csv.Add ( "" );					//PROTECTED VERSION NUMBER FIELD
	csv.Add ( Supplier::Name.Label );
	csv.Add ( Supplier::Address1.Label );
	csv.Add ( Supplier::Address2.Label );
	csv.Add ( Supplier::Address3.Label );
	csv.Add ( Supplier::Address4.Label );
	csv.Add ( Supplier::Address5.Label );
	csv.Add ( Supplier::Postcode.Label );
	csv.Add ( Supplier::Phone.Label );
	csv.Add ( Supplier::Fax.Label );
	csv.Add ( Supplier::Email.Label );
	csv.Add ( Supplier::AccountNo.Label );
	csv.Add ( Supplier::FAO.Label );
	strHeader = csv.GetLine();
	return strHeader;
}

/**********************************************************************/

void CSupplierCSVRecord::SetSuppNo ( int nSuppNo )
{
	Set ( m_nSuppNo, nSuppNo, Supplier::SupplierNo );
}

/**********************************************************************/

void CSupplierCSVRecord::SetName( const char* szName )
{
	CString strName = StripSpaces ( szName );

	if ( strName == "" )
		strName.Format ( "Supplier %d", m_nSuppNo );

	Set ( m_strName, strName, Supplier::Name );
}

/**********************************************************************/

void CSupplierCSVRecord::SetKey( const char* szKey )
{
	CString strKey = StripSpaces ( szKey );

	if ( strKey == "" )
		strKey.Format ( "%d", m_nSuppNo );

	Set ( m_strKey, strKey, Supplier::Name );
}

/**********************************************************************/

void CSupplierCSVRecord::SetAddress1( const char* szAddress1 )
{
	Set ( m_strAddress1, StripSpaces ( szAddress1 ), Supplier::Address1 );
}

/**********************************************************************/

void CSupplierCSVRecord::SetAddress2( const char* szAddress2 )
{
	Set ( m_strAddress2, StripSpaces ( szAddress2 ), Supplier::Address2 );
}

/**********************************************************************/

void CSupplierCSVRecord::SetAddress3( const char* szAddress3 )
{
	Set ( m_strAddress3, StripSpaces ( szAddress3 ), Supplier::Address3 );
}

/**********************************************************************/

void CSupplierCSVRecord::SetAddress4( const char* szAddress4 )
{
	Set ( m_strAddress4, StripSpaces ( szAddress4 ), Supplier::Address4 );
}

/**********************************************************************/

void CSupplierCSVRecord::SetAddress5( const char* szAddress5 )
{
	Set ( m_strAddress5, StripSpaces ( szAddress5 ), Supplier::Address5 );
}

/**********************************************************************/

void CSupplierCSVRecord::SetPostcode( const char* szPostcode )
{
	Set ( m_strPostcode, StripSpaces ( szPostcode ), Supplier::Postcode );
}

/**********************************************************************/

void CSupplierCSVRecord::SetPhone( const char* szPhone )
{
	Set ( m_strPhone, StripSpaces ( szPhone ), Supplier::Phone );
}

/**********************************************************************/

void CSupplierCSVRecord::SetFax( const char* szFax )
{
	Set ( m_strFax, StripSpaces ( szFax ), Supplier::Fax );
}

/**********************************************************************/

void CSupplierCSVRecord::SetEmail( const char* szEmail )
{
	Set ( m_strEmail, StripSpaces ( szEmail ), Supplier::Email );
}

/**********************************************************************/

void CSupplierCSVRecord::SetAccountNo( const char* szAccountNo )
{
	Set ( m_strAccountNo, StripSpaces ( szAccountNo ), Supplier::AccountNo );
}

/**********************************************************************/

void CSupplierCSVRecord::SetFAO( const char* szFAO )
{
	Set ( m_strFAO, StripSpaces ( szFAO ), Supplier::FAO );
}

/**********************************************************************/
//	Merge
/**********************************************************************/

CString CSupplierCSVRecord::StripSpaces ( const char* szText )
{
	CString strText = szText;
	::TrimSpaces ( strText, FALSE );
	return strText;
}

/**********************************************************************/

void CSupplierCSVRecord::ImportLine ( CSSImportFile& import, CCSV& csv, bool bImportSuppNo )
{
	int n = 0;
	if ( import.IsRequired(n) == TRUE ) Import ( StripSpaces ( csv.GetString (n) ), m_strKey, Supplier::SupplierKey ); n++;
	
	if ( bImportSuppNo == TRUE )
	{
		if ( import.IsRequired(n) == TRUE ) Import ( StripSpaces ( csv.GetString (n) ), m_nSuppNo, Supplier::SupplierNo ); n++;
	}
	else
		n++;

	n++;	//SKIP PROTECTED VERSION NUMBER
	if ( import.IsRequired(n) == TRUE ) Import ( StripSpaces ( csv.GetString (n) ), m_strName, Supplier::Name ); n++;
	if ( import.IsRequired(n) == TRUE ) Import ( StripSpaces ( csv.GetString (n) ), m_strAddress1, Supplier::Address1 ); n++;
	if ( import.IsRequired(n) == TRUE ) Import ( StripSpaces ( csv.GetString (n) ), m_strAddress2, Supplier::Address2 ); n++;
	if ( import.IsRequired(n) == TRUE ) Import ( StripSpaces ( csv.GetString (n) ), m_strAddress3, Supplier::Address3 ); n++;
	if ( import.IsRequired(n) == TRUE ) Import ( StripSpaces ( csv.GetString (n) ), m_strAddress4, Supplier::Address4 ); n++;
	if ( import.IsRequired(n) == TRUE ) Import ( StripSpaces ( csv.GetString (n) ), m_strAddress5, Supplier::Address5 ); n++;
	if ( import.IsRequired(n) == TRUE ) Import ( StripSpaces ( csv.GetString (n) ), m_strPostcode, Supplier::Postcode ); n++;
	if ( import.IsRequired(n) == TRUE ) Import ( StripSpaces ( csv.GetString (n) ), m_strPhone, Supplier::Phone ); n++;
	if ( import.IsRequired(n) == TRUE ) Import ( StripSpaces ( csv.GetString (n) ), m_strFax, Supplier::Fax ); n++;
	if ( import.IsRequired(n) == TRUE ) Import ( StripSpaces ( csv.GetString (n) ), m_strEmail, Supplier::Email ); n++;
	if ( import.IsRequired(n) == TRUE ) Import ( StripSpaces ( csv.GetString (n) ), m_strAccountNo, Supplier::AccountNo ); n++;
	if ( import.IsRequired(n) == TRUE ) Import ( StripSpaces ( csv.GetString (n) ), m_strFAO, Supplier::FAO ); n++;
}

/**********************************************************************/

const char* CSupplierCSVRecord::GetOrderFilename()
{
	m_strOrderFilename.Format ( "Supp%3.3d.dat", m_nSuppNo );
	return m_strOrderFilename;
}

/**********************************************************************/

const char* CSupplierCSVRecord::GetAuditFilename()
{
	m_strAuditFilename.Format ( "SupAd%3.3d.dat", m_nSuppNo );
	return m_strAuditFilename;
}

/**********************************************************************/

const char* CSupplierCSVRecord::GetTempFilename()
{
	m_strTempFilename.Format ( "SupTm%3.3d.dat", m_nSuppNo );
	return m_strTempFilename;
}

/**********************************************************************/