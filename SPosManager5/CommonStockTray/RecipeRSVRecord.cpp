/**********************************************************************/
 
/**********************************************************************/
//#include "globalfunctions.h"
/**********************************************************************/
#include "RecipeCSVArray.h"
/**********************************************************************/

CRecipeCSVRecord::CRecipeCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CRecipeCSVRecord::ClearRecord()
{
	m_strPluNo = "";
	m_nSpNo = 0;
	m_bReverseSale = FALSE;
	EnablePluStockLink();
	RemoveAll();
}

/**********************************************************************/

CRecipeCSVRecord::~CRecipeCSVRecord()
{
	RemoveAll();
}

/**********************************************************************/
/*	convert to csv											*/
/**********************************************************************/

void CRecipeCSVRecord::ConvertToCSV ( CCSV& csv )
{
	int nDPValue = 3;		/* max system dp value */

	m_strPluNo.TrimLeft ( '0' );

	csv.Add ( m_strPluNo );
	csv.Add ( RECIPEDB_VERSION );
	csv.Add ( m_nSpNo );
	csv.Add ( m_bReverseSale );
	csv.Add ( m_bPluStockLink );

	if ( TRUE == m_bPluStockLink )
	{
		csv.Add ( m_Recipe.GetSize() );
		for ( int nIndex = 0 ; nIndex < GetSize() ; nIndex++ )
		{
			csv.Add ( GetStockCode ( nIndex ) );
			csv.Add ( GetStockQty ( nIndex ), 5 );
		}
	}
}

/**********************************************************************/
/*	convert from csv											  	*/
/**********************************************************************/

void CRecipeCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	switch ( csv.GetInt(1) )			/* version number */
	{
	case 1:	V1ConvertFromCSV ( csv );		break;
	case 2:	V2ConvertFromCSV ( csv );		break;
	case 3:	V3to4ConvertFromCSV ( csv, 3 );	break;
	case 4:	V3to4ConvertFromCSV ( csv, 4 );	break;
	}
}

/**********************************************************************/
/*	Version 1												*/
/**********************************************************************/

void CRecipeCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	int n = 0;
	SetRecipePluNo ( csv.GetString(n++) );
	n++;
	
	m_Recipe.RemoveAll();
	int nCount = csv.GetInt(n++);
	for ( int nItem = 0 ; nItem < nCount ; nItem++ )	// must use count from file
	{
		CString strCode = csv.GetString(n++);
		double dQty = csv.GetDouble(n++);
		AddStockCode ( strCode, dQty );
	}
}

/**********************************************************************/
/*	Version 2												*/
/**********************************************************************/

void CRecipeCSVRecord::V2ConvertFromCSV ( CCSV& csv )
{
	int n = 0;
	SetRecipePluNo ( csv.GetString(n++) );
	n++;
	SetSpNo( csv.GetInt(n++) );

	m_Recipe.RemoveAll();
	int nCount = csv.GetInt(n++);
	for ( int nItem = 0 ; nItem < nCount ; nItem++ )	// must use count from file
	{
		CString strCode = csv.GetString(n++);
		double dQty = csv.GetDouble(n++);
		AddStockCode ( strCode, dQty );
	}
}

/**********************************************************************/
/*	Version 3												*/
/**********************************************************************/

void CRecipeCSVRecord::V3to4ConvertFromCSV ( CCSV& csv, int nVer )
{
	int n = 0;
	SetRecipePluNo ( csv.GetString(n++) );
	n++;
	SetSpNo( csv.GetInt(n++) );

	if ( nVer >= 4 )
		SetReverseSaleFlag( csv.GetBool(n++) );

	if ( csv.GetBool(n++) == FALSE )
		DisablePluStockLink();
	else
	{
		EnablePluStockLink();
		m_Recipe.RemoveAll();
		
		int nCount = csv.GetInt(n++);
		for ( int nItem = 0 ; nItem < nCount ; nItem++ )	// must use count from file
		{
			CString strCode = csv.GetString(n++);
			double dQty = csv.GetDouble(n++);
			AddStockCode ( strCode, dQty );
		}
	}
}

/**********************************************************************/
//	Get header
/**********************************************************************/

CString CRecipeCSVRecord::GetHeader()
{
	CString strHeader;
	CCSV csv;
	
	csv.Add ( Recipe::PluNo.Label );
	csv.Add ( "" );	//Protected version number
	csv.Add ( Recipe::SpNo.Label );
	csv.Add ( Recipe::ReverseSale.Label );
	csv.Add ( Recipe::PluStockLink.Label );
	csv.Add ( "" );	//Protected stock code count
	csv.Add ( Recipe::StockCode.Label );
	csv.Add ( Recipe::StockQty.Label );
	
	strHeader = csv.GetLine();
	return strHeader;
}

/**********************************************************************/

void CRecipeCSVRecord::DisablePluStockLink()
{
	m_bPluStockLink = FALSE;
	RemoveAll();
}

/**********************************************************************/

void CRecipeCSVRecord::EnablePluStockLink()
{
	m_bPluStockLink = TRUE;
}

/**********************************************************************/

void CRecipeCSVRecord::RemoveAll()
{
	while ( GetSize() > 0 )
		RemoveItem ( 0 );
}

/**********************************************************************/

void CRecipeCSVRecord::SetRecipePluNo ( const char* szPluNo )
{
	CString strPluNo ( szPluNo );
	::AddLeading ( strPluNo, Recipe::PluNo.Max, '0' );
	Set( m_strPluNo, strPluNo, Recipe::PluNo );
	m_strPluNo = strPluNo;
}

/**********************************************************************/

bool CRecipeCSVRecord::FindStockCode ( const char* szCode, int& nIndex )
{
	CString strStockCode = szCode;

	for ( nIndex = 0; nIndex < GetSize(); nIndex++ )
		if ( strStockCode == GetStockCode ( nIndex ) )
			return TRUE;

	return FALSE;
}

/**********************************************************************/

bool CRecipeCSVRecord::AddStockCode ( const char* szCode, double dStockQty )
{
	CRecipeCSVItem item;
	item.m_strStockCode = szCode;
	item.m_dStockQty = dStockQty;
	m_Recipe.Add ( item );
	return TRUE;
}

/**********************************************************************/

bool CRecipeCSVRecord::RemoveItem ( int nIndex )
{
	if ( nIndex < 0 || nIndex >= m_Recipe.GetSize() )
		return FALSE;

	m_Recipe.RemoveAt ( nIndex );
	return TRUE;
}

/**********************************************************************/

bool CRecipeCSVRecord::operator!= ( CRecipeCSVRecord& recipe )
{
	if ( recipe.GetSize() != GetSize() )
		return TRUE;

	for ( int i = 0 ; i < GetSize() ; i++ )
	{
		if ( m_Recipe[i].m_strStockCode != recipe.GetStockCode(i) )
			return TRUE;

		if ( m_Recipe[i].m_dStockQty != recipe.GetStockQty(i) )
			return TRUE;
	}
	return FALSE;
}

/**********************************************************************/
//	Merge
/**********************************************************************/

CString CRecipeCSVRecord::StripSpaces ( const char* szText )
{
	CString strText = szText;
	::TrimSpaces ( strText, FALSE );
	return strText;
}

/**********************************************************************/

bool CRecipeCSVRecord::ImportLine ( CSSImportFile& import, CCSV& csv, bool& bIncompleteLine )
{
	int n = 0;
	bIncompleteLine = FALSE;
	
	//PLU NUMBER
	if ( import.IsRequired(n) == FALSE )
	{
		bIncompleteLine = TRUE;
		return FALSE;
	}
	n++;

	//VERSION NUMBER
	n++;

	//STOCKPOINT NUMBER
	int nSpNo = m_nSpNo;
	if ( import.IsRequired(n) == TRUE ) 
		Import ( StripSpaces ( csv.GetString(n) ), nSpNo, Recipe::SpNo ); 
	n++;

	//REVERSE SALE FLAG
	bool bReverseSale = m_bReverseSale;
	if ( import.IsRequired(n) == TRUE ) 
		Import ( StripSpaces ( csv.GetString(n) ), bReverseSale ); 
	n++;

	//NON STOCK FLAG
	bool bSetPluStockLink = FALSE;
	bool bPluStockLink = m_bPluStockLink;
	if ( import.IsRequired(n) == TRUE ) 
	{
		Import ( StripSpaces ( csv.GetString(n) ), bPluStockLink ); 
		bSetPluStockLink = TRUE;
	}
	n++;
	
	//ITEM COUNT
	n++;

	//STOCK CODE
	bool bSetStockCode = FALSE;
	CString strStockCode = "";
	if ( import.IsRequired(n) == TRUE )
	{
		Import ( StripSpaces( csv.GetString(n) ), strStockCode, Recipe::StockCode );
		bSetStockCode = TRUE;
	}
	n++;

	//MUST HAVE STOCK CODE AND / OR NON STOCK FLAG
	if ( ( FALSE == bSetPluStockLink ) && ( FALSE == bSetStockCode ) )
	{
		bIncompleteLine = TRUE;
		return FALSE;
	}
			
	//STOCK QTY
	double dStockQty = 1.0;
	
	if ( GetSize() == 1 )
		dStockQty = GetStockQty(0);
	
	if ( import.IsRequired(n) == TRUE ) 
		Import ( StripSpaces( csv.GetString(n) ), dStockQty, Recipe::StockQty );
	
	//MODIFY RECIPE RECORD
	bool bResult = FALSE;
	if ( ( TRUE == bSetPluStockLink ) && ( TRUE == bSetStockCode ) )
	{
		if ( FALSE == bPluStockLink )
		{
			if ( strStockCode == "" )
			{
				DisablePluStockLink();
				bResult = TRUE;
			}
		}
		else
		{
			if ( ( strStockCode != "" ) && ( dStockQty > 0.0 ) )
			{
				EnablePluStockLink();
				RemoveAll();
				AddStockCode( strStockCode, dStockQty );
				bResult = TRUE;
			}
		}
	}
	else if ( TRUE == bSetPluStockLink )
	{
		if ( FALSE == bPluStockLink )
		{
			DisablePluStockLink();
			bResult = TRUE;
		}
		else
		{
			EnablePluStockLink();
			bResult = TRUE;
		}
	}
	else
	{
		if ( ( strStockCode != "" ) && ( dStockQty > 0.0 ) )
		{
			EnablePluStockLink();
			RemoveAll();
			AddStockCode( strStockCode, dStockQty );
			bResult = TRUE;
		}
	}

	if ( TRUE == bResult )
	{
		SetSpNo( nSpNo );
		SetReverseSaleFlag( bReverseSale );
	}
		
	return bResult;
}

/**********************************************************************/





