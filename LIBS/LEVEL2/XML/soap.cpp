/**********************************************************************/

#include "local.h"

/**********************************************************************/

CSSSoap::CSSSoap ( const char* szVersion )
{
	Init ( szVersion );
}

/**********************************************************************/

CSSSoap::CSSSoap ( const char* szDeclaration, const char* szVersion )
	: CSSXML ( szDeclaration )
{
	Init ( szVersion );
}

/**********************************************************************/

void CSSSoap::Init ( const char* szVersion )
{
	CString strVersion = szVersion;

	if ( strVersion == "1.2" )		// SOAP version 1.2
	{
		m_strSoapVersion = "soap12";
		m_strContentType = "Content-Type: application/soap+xml; charset=utf-8";
		m_strSoapAction = "";

		Begin ( "soap12:Envelope" );
		Attribute ( "xmlns:soap12", "http://www.w3.org/2003/05/soap-envelope" );
	}
	else							// SOAP version 1.1
	{
		m_strSoapVersion = "soap11";
		m_strContentType = "Content-Type: text/xml; charset=utf-8";
		m_strSoapAction = "";

		Begin ( "soap11:Envelope" );
		Attribute ( "xmlns:soap11", "http://schemas.xmlsoap.org/soap/envelope/" );
	}
}

/**********************************************************************/

void CSSSoap::SetSoapAction ( const char* szSoapAction )
{
	m_strSoapAction.Format ( "SOAPAction: \"%s\"", szSoapAction );
}

/**********************************************************************/

void CSSSoap::BeginSoap ( const char* szElement )
{
	CString strElement;
	strElement.Format ( "%s:%s", (const char*) m_strSoapVersion, szElement );
	Begin ( strElement );
}

/**********************************************************************/

void CSSSoap::AttributeSoap ( const char* szAttribute, const char* szValue )
{
	CString strAttribute;
	strAttribute.Format ( "%s:%s", (const char*) m_strSoapVersion, szAttribute );
	Attribute ( strAttribute, szValue );
}

/**********************************************************************/

CString CSSSoap::SendHttp ( const char* szIPAddress, const char* szObjectName, const char* szResponseFilename, bool bValidate, const char* szAgent )
{
	return Send ( FALSE, szIPAddress, szObjectName, szResponseFilename, bValidate, szAgent );
}

/**********************************************************************/

CString CSSSoap::SendHttps ( const char* szIPAddress, const char* szObjectName, const char* szResponseFilename, bool bValidate, const char* szAgent )
{
	return Send ( TRUE, szIPAddress, szObjectName, szResponseFilename, bValidate, szAgent );
}

/**********************************************************************/

CString CSSSoap::Send ( bool bUseHttps, const char* szIPAddress, const char* szObjectName, const char* szResponseFilename, bool bValidate, const char* szAgent )
{
	CString strError;

	if ( strlen(szIPAddress) > 0 )
	{
		CStringArray arHeaders;
		arHeaders.Add ( m_strContentType );

		if ( m_strSoapAction != "" )
			arHeaders.Add ( m_strSoapAction );

		strError = HttpPost ( bUseHttps, szIPAddress, szObjectName, arHeaders, GetXML(), szResponseFilename, szAgent );
	}

	if ( strError == "" )
	{
		if (( strError = m_Parse.ParseFile ( szResponseFilename, bValidate )) == "" )
		{
			if ( m_Parse.FindNode ( "*:Envelope" ) != NULL )	// ignore namespace
			{
				CXMLNodeArray* p1 = m_Parse.FindNode ( "*:Fault" );	// ignore namespace
				if ( p1 != NULL )
				{
					int i;
					for ( i = 0 ; i < p1->GetSize() ; i++ )
					{
						CXMLNodeArray* p2 = p1->GetAt(i);
						if ( p2->m_strName == "faultstring" )	// soap 1.1
						{
							strError = p2->m_strValue;
							break;
						}
						else if ( p2->m_strLocalName == "Reason" )	// soap 1.2 (namespace:Reason)
						{
							strError = p2->GetAt(0)->m_strValue;
							break;
						}
					}
					if ( i == p1->GetSize() )
					{
						strError = "Unknown soap error";
					}
				}
			}
			else
			{
				strError = "Soap envelope not found";
			}
		}
	}

	return strError;
}

/**********************************************************************/
