/**********************************************************************/
/*	PageObject											*/
/**********************************************************************/

class CPageObject : public CObject
{
public:
	CPageObject();
	void SetIndex ( int nIndex ) { m_nIndex = nIndex; }
	void SetCount ( int nCount ) { m_nCount = nCount; }
	void SetPageNo ( int nPageNo ) { m_nPageNo = nPageNo; }
	int GetIndex() { return m_nIndex; }
	int GetCount() { return m_nCount; }
	int GetPageNo() { return m_nPageNo; }

private:
	int m_nIndex;
	int m_nCount;
	int m_nPageNo;
};

CPageObject::CPageObject()
{
	m_nIndex = 0;
	m_nCount = 0;
	m_nPageNo = 0;
}

/**********************************************************************/
/*	PageArray												*/
/**********************************************************************/

CPageArray::~CPageArray()
{
	RemoveAll();
}

/**********************************************************************/

void CPageArray::RemoveAll()
{
	for ( int i = 0 ; i < GetSize() ; i++ )
		delete GetAt(i);

	CObArray::RemoveAll();
}

/**********************************************************************/

int CPageArray::Paginate ( CStringArray& TextArray, int nLinesPerPage )
{
	int i = 0;		// index to document
	int j = 0;		// lines on page
	int p = 1;		// displayed page number

	RemoveAll();
	AddPage();

	while ( (i+j) < TextArray.GetSize() )
	{
		if ( j == 0 )
			SetIndex(i+j);

		if ( strncmp ( TextArray.GetAt(i+j), "<NEWPAGE>", 9 ) == 0 )
		{
			CCSV csv ( TextArray.GetAt(i+j) );

			i += ( j + 1 );			// discard new page line
			if ( j != 0 )				// do not allow blank pages
			{
				SetCount(j);
				SetPageNo(p++);
				AddPage();
				j = 0;
			}

			if ( csv.GetInt(1) > 0 )		// get page number
				p = csv.GetInt(1);		// set page number
		}

		else
		{
			if ( j >= nLinesPerPage )
			{
				i += j;
				SetCount(j);
				SetPageNo(p++);
				AddPage();
				j = 0;
			}
			else
				j++;
		}
	}

	SetCount(j);		// last page
	SetPageNo(p);

	return GetSize();	// number of pages
}

/**********************************************************************/

void CPageArray::AddPage()
{
	CPageObject* pPageObject = new CPageObject;
	if ( pPageObject != NULL )
	{
		TRY
		{
			CObArray::Add ( pPageObject );
		}
		CATCH ( CMemoryException, e )
		{
			delete pPageObject;
		}
		END_CATCH
	}
}

/**********************************************************************/

void CPageArray::SetIndex ( int nIndex )
{
	CPageObject* pPageObject = (CPageObject*) GetAt ( GetSize() - 1 );
	pPageObject->SetIndex ( nIndex );
}

void CPageArray::SetCount ( int nCount )
{
	CPageObject* pPageObject = (CPageObject*) GetAt ( GetSize() - 1 );
	pPageObject->SetCount ( nCount );
}

void CPageArray::SetPageNo ( int nPageNo )
{
	CPageObject* pPageObject = (CPageObject*) GetAt ( GetSize() - 1 );
	pPageObject->SetPageNo ( nPageNo );
}

/**********************************************************************/

int CPageArray::GetIndex ( int nIndex )
{
	CPageObject* pPageObject = (CPageObject*) GetAt ( nIndex );
	return pPageObject->GetIndex();
}

int CPageArray::GetCount ( int nIndex )
{
	CPageObject* pPageObject = (CPageObject*) GetAt ( nIndex );
	return pPageObject->GetCount();
}

int CPageArray::GetPageNo ( int nIndex )
{
	CPageObject* pPageObject = (CPageObject*) GetAt ( nIndex );
	return pPageObject->GetPageNo();
}

/**********************************************************************/
