/**********************************************************************/

#include "local.h"

/**********************************************************************/

CXMLNodeArray::CXMLNodeArray()
	: m_pParent(NULL), m_strName(""), m_strLocalName(""), m_nType(TYPE_NONE)
{
}

CXMLNodeArray::CXMLNodeArray ( CXMLNodeArray* pParent, const char* szName )
	: m_pParent(pParent), m_strName(szName), m_nType(TYPE_ELEMENT)
{
	CCSV csv ( szName, ':' );	// extract local name from namespace
	m_strLocalName = ( csv.GetSize() > 1 ) ? csv.GetString(1) : szName;
}

/**********************************************************************/

bool CXMLNodeArray::IsElement()
{
	return ( m_nType == TYPE_ELEMENT ) ? TRUE : FALSE;
}

/**********************************************************************/

bool CXMLNodeArray::IsAttribute()
{
	return ( m_nType == TYPE_ATTRIBUTE ) ? TRUE : FALSE;
}

/**********************************************************************/

bool CXMLNodeArray::IsText()
{
	return ( m_nType == TYPE_TEXT ) ? TRUE : FALSE;
}

/**********************************************************************/

CString CXMLNodeArray::GetFullName()
{
	CStringArray arNames;

	arNames.Add ( GetName() );

	CXMLNodeArray* p = m_pParent;

	while ( p != NULL )
	{
		CString strName = p->GetName();
		
		if ( strName != "" )				// discard blank name at root
			arNames.InsertAt ( 0, strName );	// reverse order by inserting

		p = p->m_pParent;
	}

	CString strFullName;

	for ( int i = 0 ; i < arNames.GetSize() ; i++ )
	{
		if ( i > 0 )
			strFullName += '.';

		strFullName += arNames.GetAt(i);
	}

	return strFullName;
}

/**********************************************************************/
