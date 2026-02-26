/**********************************************************************/
#include "EnhancedIniFile.h"
/**********************************************************************/

CIniFileInt::CIniFileInt()
{
	m_strLabel = "";
	m_nValue = 0;
	m_nSavedValue = 0;
	m_nMin = 0;
	m_nMax = 0;
	m_nDefault = 0;
}

/**********************************************************************/

void CIniFileInt::Create( const char* szLabel, int nMin, int nMax, int nDefault )
{
	m_strLabel = szLabel;
	
	m_nMin = nMin;
	m_nMax = nMax;
	m_nDefault = nDefault;

	if ( m_nMax < m_nMin )
		m_nMax = m_nMin;

	if ( ( m_nDefault < m_nMin ) || ( m_nDefault > m_nMax ) )
		m_nDefault = m_nMin;

	m_nValue = m_nDefault;
}

/**********************************************************************/

void CIniFileInt::Reset( int nValue )
{
	if ( ( nValue >= m_nMin ) && ( nValue <= m_nMax ) )
		m_nValue = nValue;
	else
		m_nValue = m_nDefault;
}

/**********************************************************************/

void CIniFileInt::SetValue( int nValue )
{
	if ( ( nValue >= m_nMin ) && ( nValue <= m_nMax ) )
		m_nValue = nValue;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CIniFileInt64::CIniFileInt64()
{
	m_strLabel = "";
	m_nValue = 0;
	m_nSavedValue = 0;
	m_nMin = 0;
	m_nMax = 0;
	m_nDefault = 0;
}

/**********************************************************************/

void CIniFileInt64::Create( const char* szLabel, __int64 nMin, __int64 nMax, __int64 nDefault )
{
	m_strLabel = szLabel;
	
	m_nMin = nMin;
	m_nMax = nMax;
	m_nDefault = nDefault;

	if ( m_nMax < m_nMin )
		m_nMax = m_nMin;

	if ( ( m_nDefault < m_nMin ) || ( m_nDefault > m_nMax ) )
		m_nDefault = m_nMin;

	m_nValue = m_nDefault;
}

/**********************************************************************/

void CIniFileInt64::Reset( __int64 nValue )
{
	if ( ( nValue >= m_nMin ) && ( nValue <= m_nMax ) )
		m_nValue = nValue;
	else
		m_nValue = m_nDefault;
}

/**********************************************************************/

void CIniFileInt64::SetValue( __int64 nValue )
{
	if ( ( nValue >= m_nMin ) && ( nValue <= m_nMax ) )
		m_nValue = nValue;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CIniFileBool::CIniFileBool()
{
	m_strLabel = "";
	m_bValue = FALSE;
	m_bSavedValue = FALSE;
	m_bDefault = FALSE;
}

/**********************************************************************/

void CIniFileBool::Create( const char* szLabel, bool bDefault )
{
	m_strLabel = szLabel;
	m_bValue = bDefault;
	m_bDefault = bDefault;
}


/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CIniFileDouble::CIniFileDouble()
{
	m_strLabel = "";
	m_dValue = 0.0;
	m_dSavedValue = 0.0;
	m_dMin = 0.0;
	m_dMax = 0.0;
	m_dDefault = 0.0;
}

/**********************************************************************/

void CIniFileDouble::Create( const char* szLabel, double dMin, double dMax, double dDefault )
{
	m_strLabel = szLabel;
	
	m_dMin = dMin;
	m_dMax = dMax;
	m_dDefault = dDefault;

	if ( m_dMax < m_dMin )
		m_dMax = m_dMin;

	if ( ( m_dDefault < m_dMin ) || ( m_dDefault > m_dMax ) )
		m_dDefault = m_dMin;

	m_dValue = m_dDefault;
}

/**********************************************************************/

void CIniFileDouble::Reset( double dValue )
{
	if ( ( dValue >= m_dMin ) && ( dValue <= m_dMax ) )
		m_dValue = dValue;
	else
		m_dValue = m_dDefault;
}

/**********************************************************************/

void CIniFileDouble::SetValue( double dValue )
{
	if ( ( dValue >= m_dMin ) && ( dValue <= m_dMax ) )
		m_dValue = dValue;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CIniFileString::CIniFileString()
{
	m_strLabel = "";
	m_strValue = "";
	m_strSavedValue = "";
	m_nMaxLen = 0;
	m_strDefault = "";
}

/**********************************************************************/

void CIniFileString::Create( const char* szLabel, int nMaxLen, const char* szDefault )
{
	m_strLabel = szLabel;
	
	m_nMaxLen = nMaxLen;
	if ( m_nMaxLen < 0 )
		m_nMaxLen = 0;

	m_strDefault = szDefault;
	if ( m_strDefault.GetLength() > m_nMaxLen )
		m_strDefault = "";

	m_strValue = m_strDefault;
}

/**********************************************************************/

void CIniFileString::Reset( const char* szValue )
{
	CString strValue = szValue;
	
	if ( strValue.GetLength() <= m_nMaxLen )
		m_strValue = szValue;
	else
		m_strValue = m_strDefault;
}

/**********************************************************************/

void CIniFileString::SetValue( const char* szValue )
{
	CString strValue = szValue;
	
	if ( strValue.GetLength() <= m_nMaxLen )
		m_strValue = szValue;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CIniFileIntArray::CIniFileIntArray()
{
	m_strLabel = "";
	m_nMin = 0;
	m_nMax = 0;
	m_nDefault = 0;
	m_arrayValues.Add(0);
}

/**********************************************************************/

void CIniFileIntArray::Create( const char* szLabel, int nMin, int nMax, int nDefault, int nCount )
{
	m_strLabel = szLabel;
	
	m_nMin = nMin;
	m_nMax = nMax;
	m_nDefault = nDefault;

	if ( m_nMax < m_nMin )
		m_nMax = m_nMin;

	if ( ( m_nDefault < m_nMin ) || ( m_nDefault > m_nMax ) )
		m_nDefault = m_nMin;

	if ( nCount < 1 )
		nCount = 1;

	m_arrayValues.RemoveAll();
	for ( int n = 0; n < nCount; n++ )
		m_arrayValues.Add( m_nDefault );
}

/**********************************************************************/

void CIniFileIntArray::Reset( int nIdx, int nValue )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayValues.GetSize() ) )
	{
		if ( ( nValue >= m_nMin ) && ( nValue <= m_nMax ) )
			m_arrayValues[nIdx] = nValue;
		else
			m_arrayValues[nIdx] = m_nDefault;
	}
}

/**********************************************************************/

void CIniFileIntArray::Reset( int nIdx )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayValues.GetSize() ) )
		m_arrayValues[nIdx] = m_nDefault;
}

/**********************************************************************/

int CIniFileIntArray::GetValue( int nIdx )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayValues.GetSize() ) )
		return m_arrayValues[nIdx];
	else
		return m_nDefault;
}

/**********************************************************************/

void CIniFileIntArray::SetValue( int nIdx, int nValue )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayValues.GetSize() ) )
		if ( ( nValue >= m_nMin ) && ( nValue <= m_nMax ) )
			m_arrayValues[nIdx] = nValue;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CIniFileInt64Array::CIniFileInt64Array()
{
	m_strLabel = "";
	m_nMin = 0;
	m_nMax = 0;
	m_nDefault = 0;
	m_arrayValues.Add(0);
}

/**********************************************************************/

void CIniFileInt64Array::Create( const char* szLabel, __int64 nMin, __int64 nMax, __int64 nDefault, int nCount )
{
	m_strLabel = szLabel;
	
	m_nMin = nMin;
	m_nMax = nMax;
	m_nDefault = nDefault;

	if ( m_nMax < m_nMin )
		m_nMax = m_nMin;

	if ( ( m_nDefault < m_nMin ) || ( m_nDefault > m_nMax ) )
		m_nDefault = m_nMin;

	if ( nCount < 1 )
		nCount = 1;

	m_arrayValues.RemoveAll();
	for ( int n = 0; n < nCount; n++ )
		m_arrayValues.Add( m_nDefault );
}

/**********************************************************************/

void CIniFileInt64Array::Reset( int nIdx, __int64 nValue )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayValues.GetSize() ) )
	{
		if ( ( nValue >= m_nMin ) && ( nValue <= m_nMax ) )
			m_arrayValues[nIdx] = nValue;
		else
			m_arrayValues[nIdx] = m_nDefault;
	}
}

/**********************************************************************/

void CIniFileInt64Array::Reset( int nIdx )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayValues.GetSize() ) )
		m_arrayValues[nIdx] = m_nDefault;
}

/**********************************************************************/

__int64 CIniFileInt64Array::GetValue( int nIdx )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayValues.GetSize() ) )
		return m_arrayValues[nIdx];
	else
		return m_nDefault;
}

/**********************************************************************/

void CIniFileInt64Array::SetValue( int nIdx, __int64 nValue )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayValues.GetSize() ) )
		if ( ( nValue >= m_nMin ) && ( nValue <= m_nMax ) )
			m_arrayValues[nIdx] = nValue;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CIniFileStringArray::CIniFileStringArray()
{
	m_strLabel = "";
	m_nMaxLen = 0;
	m_strDefault = "";
	m_arrayValues.Add( "" );
}

/**********************************************************************/

void CIniFileStringArray::Create( const char* szLabel, int nMaxLen, const char* szDefault, int nCount )
{
	m_strLabel = szLabel;
	
	m_nMaxLen = nMaxLen;
	if ( m_nMaxLen < 0 )
		m_nMaxLen = 0;

	m_strDefault = szDefault;
	if ( m_strDefault.GetLength() > m_nMaxLen )
		m_strDefault = "";

	if ( nCount < 1 )
		nCount = 1;

	m_arrayValues.RemoveAll();
	for ( int n = 0; n < nCount; n++ )
		m_arrayValues.Add( m_strDefault );
}

/**********************************************************************/

void CIniFileStringArray::Reset( int nIdx, const char* szValue )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayValues.GetSize() ) )
	{
		CString strValue = szValue;
	
		if ( strValue.GetLength() <= m_nMaxLen )
			m_arrayValues.SetAt( nIdx, strValue );
		else
			m_arrayValues.SetAt( nIdx, m_strDefault );
	}
}

/**********************************************************************/

void CIniFileStringArray::Reset( int nIdx )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayValues.GetSize() ) )
		m_arrayValues.SetAt( nIdx, m_strDefault );
}

/**********************************************************************/

const char* CIniFileStringArray::GetValue( int nIdx )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayValues.GetSize() )  )
		m_strValue = m_arrayValues.GetAt( nIdx );
	else
		m_strValue = m_strDefault;

	return m_strValue;
}

/**********************************************************************/

void CIniFileStringArray::SetValue( int nIdx, const char* szValue )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayValues.GetSize() ) )
	{
		CString strValue = szValue;
		if ( strValue.GetLength() <= m_nMaxLen )
			m_arrayValues.SetAt( nIdx, strValue );
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEnhancedIniFile::CEnhancedIniFile( int nFlags ) : CSSIniFile( nFlags )
{
}

/**********************************************************************/

void CEnhancedIniFile::ReadInt( CIniFileInt& field )
{
	field.SetValue( GetInt( field.GetLabel(), field.GetDefault() ) );
}

/**********************************************************************/

void CEnhancedIniFile::ReadInt( CIniFileIntArray& field, int nIdx )
{
	CString strLabel;
	strLabel.Format( "%s%d", field.GetLabel(), nIdx + 1 );
	field.SetValue( nIdx, GetInt( strLabel, field.GetDefault() ) );
}

/**********************************************************************/

void CEnhancedIniFile::ReadInt64( CIniFileInt64& field )
{
	field.SetValue( GetInt64( field.GetLabel(), field.GetDefault() ) );
}

/**********************************************************************/

void CEnhancedIniFile::ReadInt64( CIniFileInt64Array& field, int nIdx )
{
	CString strLabel;
	strLabel.Format( "%s%d", field.GetLabel(), nIdx + 1 );
	field.SetValue( nIdx, GetInt64( strLabel, field.GetDefault() ) );
}

/**********************************************************************/

void CEnhancedIniFile::ReadBool( CIniFileBool& field )
{
	field.SetValue( GetBool( field.GetLabel(), field.GetDefault() ) );
}

/**********************************************************************/

void CEnhancedIniFile::ReadDouble( CIniFileDouble& field )
{
	field.SetValue( GetDouble( field.GetLabel(), field.GetDefault() ) );
}

/**********************************************************************/

void CEnhancedIniFile::ReadString( CIniFileString& field )
{
	field.SetValue( GetString( field.GetLabel(), field.GetDefault() ) );
}

/**********************************************************************/

void CEnhancedIniFile::ReadString( CIniFileStringArray& field, int nIdx )
{
	CString strLabel;
	strLabel.Format( "%s%d", field.GetLabel(), nIdx + 1 );
	field.SetValue( nIdx, GetString( strLabel, field.GetDefault() ) );
}

/**********************************************************************/

void CEnhancedIniFile::WriteInt( CIniFileInt& field )
{
	Set( field.GetLabel(), field.GetValue() );
}

/**********************************************************************/

void CEnhancedIniFile::WriteInt( CIniFileIntArray& field, int nIdx )
{
	CString strLabel;
	strLabel.Format( "%s%d", field.GetLabel(), nIdx + 1 );
	Set( strLabel, field.GetValue(nIdx) );
}

/**********************************************************************/

void CEnhancedIniFile::WriteInt64( CIniFileInt64& field )
{
	Set( field.GetLabel(), field.GetValue() );
}

/**********************************************************************/

void CEnhancedIniFile::WriteInt64( CIniFileInt64Array& field, int nIdx )
{
	CString strLabel;
	strLabel.Format( "%s%d", field.GetLabel(), nIdx + 1 );
	Set( strLabel, field.GetValue(nIdx) );
}

/**********************************************************************/

void CEnhancedIniFile::WriteBool( CIniFileBool& field )
{
	Set( field.GetLabel(), field.GetValue() );
}

/**********************************************************************/

void CEnhancedIniFile::WriteDouble( CIniFileDouble& field, int nDPs )
{
	Set( field.GetLabel(), field.GetValue(), nDPs );
}

/**********************************************************************/

void CEnhancedIniFile::WriteString( CIniFileString& field )
{
	Set( field.GetLabel(), field.GetValue() );
}

/**********************************************************************/

void CEnhancedIniFile::WriteString( CIniFileStringArray& field, int nIdx )
{
	CString strLabel;
	strLabel.Format( "%s%d", field.GetLabel(), nIdx + 1 );
	Set( strLabel, field.GetValue(nIdx) );
}

/**********************************************************************/
