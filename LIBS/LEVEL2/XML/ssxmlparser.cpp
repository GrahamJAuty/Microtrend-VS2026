/**********************************************************************/

#include "local.h"

/**********************************************************************/

CString CSSXMLParser::ParseFile ( const char* szFilename, bool bValidate )
{
	return CInhouseXMLParser::ParseFile ( szFilename, bValidate );
}

/**********************************************************************/

int CSSXMLParser::GetNode ( CXMLNodeArray* pNodeStart, CXMLNodeArray& arNodesFound, const char* szName )	// eg "xml.Transactions.Transaction"
{
	int nCount = 0;

	CString strName;
	CXMLNodeArray* pNode;

	if ( pNodeStart != NULL )
	{
		pNode = pNodeStart;
		strName = pNodeStart->GetFullName();	// make full name from partial name eg "OrderItems.OrderItem" becomes "xml.Transactions.Transaction.OrderItems.OrderItem"
		strName += '.';
		strName += szName;
	}
	else
	{
		pNode = m_pNode;
		strName = szName;
	}

	if ( pNode != NULL )
	{
		GetNode ( arNodesFound, pNode, strName );
		nCount = arNodesFound.GetSize();
	}

	return nCount;
}

/**********************************************************************/

void CSSXMLParser::GetNode ( CXMLNodeArray& arNodesFound, CXMLNodeArray* pArray, const char* szName )
{
	for ( int i = 0 ; i < pArray->GetSize() ; i++ )
	{
		CXMLNodeArray* p = pArray->GetAt(i);

		CString strName = p->GetFullName();

		if ( strName == szName )
			arNodesFound.Add ( p );

		GetNode ( arNodesFound, p, szName );
	}
}

/**********************************************************************/

CString CSSXMLParser::GetValue ( CXMLNodeArray* pNode, const char* szName )		// eg "Id"
{
	CString strValue;

	for ( int i = 0 ; i < pNode->GetSize() ; i++ )
	{
		CXMLNodeArray* p = pNode->GetAt(i);
		if ( p->IsText() == TRUE )
		{
			if ( p->GetName() == szName )
			{
				strValue = p->GetValue();
				break;
			}
		}
	}

	return strValue;
}

/**********************************************************************/

CString CSSXMLParser::GetValue ( const char* szPath, const char* szName )	// eg GetValue ( "xml.WebService", "Version" )
{
	CString strValue;

	CXMLNodeArray ar;

	int nCount = GetNode ( NULL, ar, szPath );		// NULL means start at beginning

	if ( nCount == 1 )							// can only get value if not an array
		strValue = GetValue ( ar.GetAt(0), szName );

	return strValue;
}

/**********************************************************************/

void CSSXMLParser::Display ( CXMLNodeArray* pNodeStart, const char* szFilename )
{
	CSSFile file;
	if ( file.Open ( szFilename, "wb" ) == TRUE )
	{
		if ( pNodeStart == NULL )
			pNodeStart = m_pNode;

		Display ( pNodeStart, file );
		file.Close();
	}
}

/**********************************************************************/

void CSSXMLParser::Display ( CXMLNodeArray* pNode, CSSFile& file )
{
	for ( int i = 0 ; i < pNode->GetSize() ; i++ )		// work through all nodes recursively
	{
		CXMLNodeArray* p = pNode->GetAt(i);

		CString str;
		CString s;

		switch ( p->m_nType )
		{
		case TYPE_ELEMENT:		str = p->GetFullName();
							break;

		case TYPE_TEXT:		str.Format ( "%s=%s", (const char*) p->GetFullName(), (const char*) p->m_strValue );
							break;

		case TYPE_ATTRIBUTE:	str.Format ( "%s=\"%s\"", (const char*) p->GetFullName(), (const char*) p->m_strValue );
							break;

		default:				str = "Unknown";
							break;
		}

		file.WriteLine ( str );

		Display ( p, file );
	}
}

/**********************************************************************/
/*
<xml>
	<WebService>
		<Version>1.1</Version>
	</WebService>
	<Transactions>
		<Transaction>
			<Id>1635963</Id>
			<OrderItems>
				<OrderItem>
					<Id>2174616</Id>
				</OrderItem>
				<OrderItem>
				</OrderItem>
			</OrderItems>
		</Transaction>
		<Transaction>
		</Transaction>
		<Transaction>
		</Transaction>
		<Transaction>
		</Transaction>
	</Transactions>
</xml>
*/
/**********************************************************************/
/*
void SampleXmlParsing()
{
	CSSXMLParser x;

	CString strReply = x.ParseFile ( "sample.xml", TRUE );
	if ( strReply = "" )
	{
		strReply.Format ( "Version: %s\n", x.GetValue ( "xml.WebService", "Version" ) );

		CXMLNodeArray arTransaction;		// array to hold found Transaction records

		int nTransactionCount = x.GetNode ( NULL, arTransaction, "xml.Transactions.Transaction" );	// NULL means start at beginning

		for ( int i = 0 ; i < nTransactionCount ; i++ )
		{
			CXMLNodeArray* pTransaction = arTransaction.GetAt(i);

			CString strValue = x.GetValue ( pTransaction, "Id" );

			CString str;
			str.Format ( "xml.Transactions.Transaction[%d].Id=%s\n", i, strValue );
			strReply += str;

			CXMLNodeArray arOrderItem;		// array to hold found OrderItem records

			int nOrderItemCount = x.GetNode ( pTransaction, arOrderItem, "OrderItems.OrderItem" );	// pTransaction means start at pTransaction, effectively "xml.Transactions.Transaction.OrderItems.OrderItem"

			for ( int j = 0 ; j < nOrderItemCount ; j++ )
			{
				CXMLNodeArray* pOrderItem = arOrderItem.GetAt(j);

				CString strValue = x.GetValue ( pOrderItem, "Id" );

				CString str;
				str.Format ( "xml.Transactions.Transaction[%d].OrderItems.OrderItem[%d].Id=%s\n", i, j, strValue );
				strReply += str;
			}
		}
	}

	AfxMessageBox ( strReply );
}
*/
/**********************************************************************/
