/**********************************************************************/

class CCommsLockArrayObj : public CObject
{
public:
	CCommsLockArrayObj();
	~CCommsLockArrayObj();
	bool Lock ( const char* szFilename );
	const char* GetFilename() { return m_strFilename; }

private:
	CSSFile m_file;
	CString m_strFilename;
};

/**********************************************************************/

CCommsLockArrayObj::CCommsLockArrayObj()
{
	m_strFilename = "";
}

CCommsLockArrayObj::~CCommsLockArrayObj()
{
	if ( m_strFilename != "" )
	{
		m_file.Close();
		remove ( m_strFilename );
		m_strFilename = "";
	}
}

bool CCommsLockArrayObj::Lock ( const char* szFilename )
{
	if ( m_file.Open ( szFilename, "wb", SH_DENYRW, 0 ) == TRUE )
	{
		m_strFilename = szFilename;
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

CCommsLockArray::~CCommsLockArray()
{
	for ( int i = 0 ; i < GetSize() ; i++ )
	{
		CCommsLockArrayObj* pObj = (CCommsLockArrayObj*)GetAt(i);
		delete pObj;
	}

	RemoveAll();
}

bool CCommsLockArray::Lock ( const char* szFilename )
{
	CCommsLockArrayObj* pObj = new CCommsLockArrayObj();

	if ( pObj != NULL )
	{
		if ( Add ( pObj ) >= 0 )
		{
			if ( pObj->Lock ( szFilename ) == TRUE )
				return TRUE;
		}
	}
	return FALSE;
}

void CCommsLockArray::Unlock ( const char* szFilename )
{
	CString strFilename = szFilename;

	for ( int i = 0 ; i < GetSize() ; i++ )
	{
		CCommsLockArrayObj* pObj = (CCommsLockArrayObj*)GetAt(i);

		if ( strFilename == pObj->GetFilename() )
		{
			RemoveAt ( i );	// remove from array
			delete pObj;
			break;
		}
	}
}

/**********************************************************************/
