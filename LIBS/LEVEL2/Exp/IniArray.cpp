/**********************************************************************/

CIniObj::CIniObj ( const char* szSection, const char* szLabel, const char* szValue )
	: m_strSection(szSection), m_strLabel(szLabel), m_strValue(szValue)
{
}

/**********************************************************************/

CString CIniArray::GetValue ( int n )
{
	CIniObj* pObj = (CIniObj*) CObArray::GetAt(n);
	CString strValue = pObj->m_strValue;
	return strValue;
}

void CIniArray::RemoveAll()
{
	for ( int i = 0 ; i < GetSize() ; i++ )
		delete (CIniObj*) CObArray::GetAt(i);

	CObArray::RemoveAll();
}

/**********************************************************************/
