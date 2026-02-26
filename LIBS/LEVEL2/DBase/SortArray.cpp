
/**********************************************************************/
/*	CSortObj												*/
/**********************************************************************/

CSortObj::CSortObj ( int nColumn, int nType, int nDirection )
	: m_nColumn(nColumn), m_nType(nType), m_nDirection(nDirection)
{
}

/**********************************************************************/
/*	CSortArray											*/
/**********************************************************************/

CSortArray::CSortArray()
	: m_pUserCompare ( NULL )
{
}

/**********************************************************************/

CSortArray::~CSortArray()
{
	RemoveAll();
}

/**********************************************************************/

void CSortArray::SetFunction ( int (__cdecl *pUserCompare)(const char* arg1, const char* arg2, int nType, int nDirection) )
{
	m_pUserCompare = pUserCompare;
}

/**********************************************************************/

void CSortArray::Add ( int nColumn, int nType, int nDirection )
{
	CSortObj* pObj = new CSortObj ( nColumn, nType, nDirection );
	CPtrArray::Add ( pObj );
}

/**********************************************************************/

void CSortArray::RemoveAll()
{
	for ( int i = 0 ; i < GetSize() ; i++ )
		delete (CSortObj*) CPtrArray::GetAt(i);

	CPtrArray::RemoveAll();
	m_pUserCompare = NULL;
}

/**********************************************************************/
