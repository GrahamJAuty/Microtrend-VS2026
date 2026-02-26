/**********************************************************************/

class CLockArrayObj : public CObject
{
public:
	CLockArrayObj();
	~CLockArrayObj();
	bool Lock ( const char* szFilename, IdentityInfo& UserIdentityInfo, const char* szPCIdHost, const char* szPCIdClient );
	const char* GetFilename() { return m_strFilename; }

private:
	CSSFile m_file;
	CString m_strFilename;
};

/**********************************************************************/

CLockArrayObj::CLockArrayObj()
{
	m_strFilename = "";
}

CLockArrayObj::~CLockArrayObj()
{
	if ( m_strFilename != "" )
	{
		m_file.Close();
		remove ( m_strFilename );
		m_strFilename = "";
	}
}

bool CLockArrayObj::Lock(const char* szFilename, IdentityInfo& UserIdentityInfo, const char* szPCIdHost, const char* szPCIdClient)
{
	if (m_file.Open(szFilename, "wb", SH_DENYWR, 0) == TRUE)
	{
		m_strFilename = szFilename;

		CTime tm = CTime::GetCurrentTime();
		CString strDate = tm.Format("%d/%m/%Y");
		CString strTime = tm.Format("%H:%M:%S");

		CCSV csv;
		csv.Add(UserIdentityInfo.m_strUserName);
		csv.Add(UserIdentityInfo.m_strComputerName);
		csv.Add(UserIdentityInfo.m_strClientName);
		csv.Add(UserIdentityInfo.m_strSessionName);
		csv.Add(szPCIdHost);		// PCnnnnnnnnnn
		csv.Add(szPCIdClient);		// PDnnnnnnnnnn
		csv.Add(strDate);			// dd/mm/yyyy
		csv.Add(strTime);			// hh:mm:ss

		m_file.WriteLine(csv.GetLine());
		m_file.Flush();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

CLockArray::~CLockArray()
{
	for ( int i = 0 ; i < GetSize() ; i++ )
	{
		CLockArrayObj* pObj = (CLockArrayObj*)GetAt(i);
		delete pObj;
	}

	RemoveAll();
}

bool CLockArray::LockUser ( const char* szFilename, IdentityInfo& UserIdentityInfo, const char* szPCIdHost, const char* szPCIdClient )
{
	CLockArrayObj* pObj = new CLockArrayObj();

	if ( pObj != NULL )
	{
		if ( Add ( pObj ) >= 0 )
		{
			if ( pObj->Lock ( szFilename, UserIdentityInfo, szPCIdHost, szPCIdClient ) == TRUE )
				return TRUE;
		}
	}
	return FALSE;
}

void CLockArray::UnlockAllExcept ( const char* szFilename )
{
	if ( GetSize() > 1 )
	{
		CString strFilename = szFilename;

		for ( int i = 0 ; i < GetSize() ; i++ )
		{
			CLockArrayObj* pObj = (CLockArrayObj*)GetAt(i);

			if ( strFilename == pObj->GetFilename() )
				SetAt ( 0, pObj );	// move to top of array
			else
				delete pObj;
		}

		RemoveAt ( 1, GetSize()-1 );	// remove all but first element
	}
}

void CLockArray::UnlockOtherProgram()
{
	for ( int i = 0 ; i < GetSize() ; i++ )
	{
		CLockArrayObj* pObj = (CLockArrayObj*)GetAt(i);
		delete pObj;
	}

	RemoveAll();	// remove all but first element
}

/**********************************************************************/
