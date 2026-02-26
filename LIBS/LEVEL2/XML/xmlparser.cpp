/**********************************************************************/

#include "local.h"

/**********************************************************************/

CInhouseXMLParser::CInhouseXMLParser()
	: m_pNode(NULL), m_pRoot(NULL)
{
}

CInhouseXMLParser::~CInhouseXMLParser()
{
	RemoveAll();
}

/**********************************************************************/

void CInhouseXMLParser::RemoveAll()
{
	if ( m_pRoot != NULL )
	{
		RemoveAll ( m_pRoot );
		delete m_pRoot;
		m_pRoot = NULL;
	}
}

/**********************************************************************/

void CInhouseXMLParser::RemoveAll ( CXMLNodeArray* pArray )
{
	for ( int i = 0 ; i < pArray->GetSize() ; i++ )
	{
		CXMLNodeArray* p = pArray->GetAt(i);
		RemoveAll ( p );
		delete p;
	}
}

/**********************************************************************/

CString CInhouseXMLParser::ParseFile ( const char* szFilename, bool bValidate )
{
	CString strError;

	CSSFile file;
	if ( file.Open ( szFilename, "rb" ) == TRUE )
	{
		CString strXML;

		CString strBuf;
		while ( file.ReadString ( strBuf ) == TRUE )		// discard crlfs
			strXML += strBuf;

		file.Close();

		strError = Parse ( strXML, bValidate );
	}
	else
		strError = "Cannot open file";

	return strError;
}

/**********************************************************************/

CString CInhouseXMLParser::Parse ( CString& strXML, bool bValidate )
{
	CString strError;

	if ( bValidate == TRUE )		// look for something like <?xml version="1.0" encoding="utf-8"?>
	{
		int n1;
		if (( n1 = strXML.Find ( '<' )) >= 0 )
		{
			int n2;
			if (( n2 = strXML.Find ( '>', n1 )) > n1 )
			{
				CString str = strXML.Mid(n1,n2-n1+1).MakeLower();
				if ( str.Left(5) == "<?xml" && str.Right(2) == "?>" )
					bValidate = FALSE;		// ok
			}
		}
	}

	if ( bValidate == FALSE )
	{
		m_pRoot = new CXMLNodeArray();
		m_pNode = m_pRoot;

		int nOffset = 0;
		int nLength = strXML.GetLength();

		while ( nOffset < nLength )
		{
			if ( GetNext ( strXML, nOffset ) == FALSE )
			{
				strError = "XML badly formed";
				break;
			}
		}

		if ( strError == "" )
		{
			if ( m_pNode != m_pRoot )	// number of begins != number of ends
				strError = "XML nesting error";
		}
	}
	else
		strError = "XML declaration not found";

	return strError;
}

/**********************************************************************/

bool CInhouseXMLParser::GetNext ( CString& strBuf, int& nOffset )
{
	int nCount;
	int nStart = strBuf.Find ( '<', nOffset );
	if ( nStart == nOffset )
	{
		int nEnd;

		if ( strBuf.Mid(nStart,5).MakeUpper() == "<?XML" )	// start of xml definition
		{
			if (( nEnd = strBuf.Find ( "?>", nStart )) >= 0 )	// end of xml definition
			{
				nCount = nEnd - nStart + 2;
				nOffset += nCount;
				return TRUE;
			}
		}
		else if ( strBuf.Mid(nStart,4) == "<!--" )		// start of comment
		{
			if (( nEnd = strBuf.Find ( "-->", nStart )) >= 0 )	// end of comment
			{
				nCount = nEnd - nStart + 3;
				nOffset += nCount;
				return TRUE;
			}
		}
		else if ( strBuf.Mid(nStart,9) == "<![CDATA[" )	// start of cdata section
		{
			if (( nEnd = strBuf.Find ( "]]>", nStart )) >= 0 )	// end of cdata section
			{
				nCount = nEnd - nStart + 3;
				nOffset += nCount;
				return TRUE;
			}
		}
		else if (( nEnd = strBuf.Find ( '>', nStart )) >= 0 )
		{
			nCount = nEnd - nStart + 1;
			nOffset += nCount;

			CString str = strBuf.Mid ( nStart+1, nCount-2 );	// discard leading '<' and trailing '>'

			if ( str.Right(1) == '/' )	// empty element eg <1 /> or <1/> or <1 a="0" /> etc
			{
				str.TrimRight('/');

				CCSV csv;
				int nPos = 0;
				CString strSeparators = " \t";	// spaces and tabs
				CString strToken = str.Tokenize ( strSeparators, nPos );
				while ( strToken != "" )
				{
					csv.Add ( strToken );
					strToken = str.Tokenize ( strSeparators, nPos );
				}

				int n = csv.GetSize();
				if ( n > 0 )
				{
					for ( int i = 0 ; i < n ; i++ )
					{
						if ( i == 0 )
							Begin ( csv.GetString(i) );
						else
							Attribute ( csv.GetString(i) );
					}
					End();
				}
				else
					return FALSE;
			}
			else		// normal element eg <1> or </1> or <1 a="0">
			{
				if ( str.Left(1) == '/' )	// end element eg </1>
				{
					if ( End() == FALSE )	// more ends than begins
						return FALSE;
				}
				else		// start element eg <1> or <1 a="0">
				{
					CCSV csv;
					int nPos = 0;
					CString strSeparators = " \t";	// spaces and tabs
					CString strToken = str.Tokenize ( strSeparators, nPos );
					while ( strToken != "" )
					{
						csv.Add ( strToken );
						strToken = str.Tokenize ( strSeparators, nPos );
					}

					int n = csv.GetSize();
					if ( n > 0 )
					{
						for ( int i = 0 ; i < n ; i++ )
						{
							if ( i == 0 )
								Begin ( csv.GetString(i) );
							else
								Attribute ( csv.GetString(i) );
						}
					}
					else
						return FALSE;
				}
			}
			return TRUE;
		}
	}
	else if ( nStart > nOffset )	// got text value (or whitespace between elements)
	{
		nCount = nStart - nOffset;
		CString str = strBuf.Mid ( nOffset, nCount );
		nOffset += nCount;
		str.TrimLeft ( " \t" );		// discard spaces and tabs
		str.TrimRight ( " \t" );		// discard spaces and tabs
		if ( str != "" )
			Value ( str, TYPE_TEXT );

		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CInhouseXMLParser::Begin ( const char* szElement )
{
	CXMLNodeArray* p = new CXMLNodeArray ( m_pNode, szElement );
	m_pNode->Add ( p );
	m_pNode = p;
}

/**********************************************************************/

bool CInhouseXMLParser::End()
{
	if ( m_pNode->m_pParent != NULL )
	{
		m_pNode = m_pNode->m_pParent;
		return TRUE;
	}
	return FALSE;	// more ends than begins
}

/**********************************************************************/

static int GetNumber ( CString str )
{
	int n;

	if ( str.Left(1) == 'x' )	// hex number
	{
		n = strtol ( str.Mid(1), NULL, 16 );
	}
	else						// decimal number
	{
		n = atoi ( str );
	}

	return n;
}

/**********************************************************************/

void CInhouseXMLParser::Value ( const char* szValue, int nType )
{
	if ( strstr ( szValue, "&" ) == NULL )		// value does not contain &
	{
		m_pNode->m_strValue = szValue;
	}
	else
	{
		m_pNode->m_strValue = "";

		CString strValue = szValue;
		for ( int i = 0 ; i < strValue.GetLength() ; i++ )
		{
			unsigned char c = strValue.GetAt(i);
			if ( c == '&' )
			{
				CString str;
				for ( int n = i+1 ; n < strValue.GetLength() ; n++ )
				{
					unsigned char cTemp = strValue.GetAt(n);
					if ( cTemp == ';' )
					{
						if ( str == "quot" )		c = '"';
						else if ( str == "apos" )	c = '\'';
						else if ( str == "amp" )		c = '&';
						else if ( str == "lt" )		c = '<';
						else if ( str == "gt" )		c = '>';
						else if ( str == "pound" )	c = 0xa3;		// '£'
						else if ( str == "euro" )	c = 0x80;		// '€'
						else if ( str.Left(1) == "#" )	// number eg &#163; = '£' or &#xa3; = '£'
						{
							int nNum = GetNumber ( str.Mid(1) );
							if ( nNum == 163 )		c = 0xa3;		// '£' unicode U+00A3
							else if ( nNum == 8364 )	c = 0x80;		// '€' unicode U+20AC
							else
								break;
						}
						else
							break;

						i = n;
						break;
					}
					else
						str += cTemp;
				}
			}

			m_pNode->m_strValue += c;
		}
	}

	m_pNode->m_nType = nType;
}

/**********************************************************************/

void CInhouseXMLParser::Attribute ( const char* szAttribute )
{
	CCSV csv ( szAttribute, '=' );	// eg id="000001"
	Begin ( csv.GetString(0) );
	Value ( csv.GetString(1), TYPE_ATTRIBUTE );
	End();
}

/**********************************************************************/

int CInhouseXMLParser::FindNodes ( CXMLNodeArray& arFound, const char* szName )
{
	int nCount = 0;
	if ( m_pNode != NULL )
	{
		bool bUseNamespace;
		CString strName ( szName );
		if ( strName.Left(2) == "*:" )
		{
			bUseNamespace = FALSE;
			strName = strName.Mid(2);
		}
		else
		{
			bUseNamespace = TRUE;
		}

		FindNodes ( arFound, m_pNode, strName, bUseNamespace );
		nCount = arFound.GetSize();
	}
	return nCount;
}

/**********************************************************************/

void CInhouseXMLParser::FindNodes ( CXMLNodeArray& arFound, CXMLNodeArray* pArray, const char* szName, bool bUseNamespace )
{
	for ( int i = 0 ; i < pArray->GetSize() ; i++ )
	{
		CXMLNodeArray* p = pArray->GetAt(i);

		CString strName = ( bUseNamespace == TRUE ) ? p->m_strName : p->m_strLocalName;
		if ( strName == szName )
			arFound.Add ( p );

		FindNodes ( arFound, p, szName, bUseNamespace );
	}
}

/**********************************************************************/

CXMLNodeArray* CInhouseXMLParser::FindNode ( const char* szName )
{
	if ( m_pNode != NULL )
	{
		bool bUseNamespace;
		CString strName ( szName );
		if ( strName.Left(2) == "*:" )
		{
			bUseNamespace = FALSE;
			strName = strName.Mid(2);
		}
		else
		{
			bUseNamespace = TRUE;
		}

		return FindNode ( m_pNode, strName, bUseNamespace );
	}

	return NULL;
}

/**********************************************************************/

CXMLNodeArray* CInhouseXMLParser::FindNode ( CXMLNodeArray* pArray, const char* szName, bool bUseNamespace )
{
	CXMLNodeArray* pFound = NULL;

	for ( int i = 0 ; i < pArray->GetSize() ; i++ )
	{
		CXMLNodeArray* p = pArray->GetAt(i);

		CString strName = ( bUseNamespace == TRUE ) ? p->m_strName : p->m_strLocalName;
		if ( strName == szName )
		{
			pFound = p;
			break;
		}

		if (( pFound = FindNode ( p, szName, bUseNamespace )) != NULL )
			break;
	}

	return pFound;
}

/**********************************************************************/

void CInhouseXMLParser::Display ( const char* szFilename )
{
	CSSFile file;
	if ( file.Open ( szFilename, "wb" ) == TRUE )
	{
		Display ( m_pNode, file );
		file.Close();
	}
}

/**********************************************************************/

void CInhouseXMLParser::Display ( CXMLNodeArray* pArray, CSSFile& file )
{
	for ( int i = 0 ; i < pArray->GetSize() ; i++ )
	{
		CXMLNodeArray* p = pArray->GetAt(i);

		CString str;
		switch ( p->m_nType )
		{
		case TYPE_ELEMENT:		str.Format ( "E: %s", (const char*) p->m_strName );					break;
		case TYPE_TEXT:		str.Format ( "T: %s=%s", (const char*) p->m_strName, (const char*) p->m_strValue );	break;
		case TYPE_ATTRIBUTE:	str.Format ( "A: %s=\"%s\"", (const char*) p->m_strName, (const char*) p->m_strValue );	break;
		default:				str = "Unknown";	break;
		}

		file.WriteLine ( str );

		Display ( p, file );
	}
}

/**********************************************************************/

