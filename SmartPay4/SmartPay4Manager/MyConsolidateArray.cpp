#include "MyConsolidateArray.h"

//*******************************************************************

void CMyConsolidateObject::Consolidate ( double dValue1, double dValue2, double dValue3 )
{
	m_dValue1 += dValue1;
	m_dValue2 += dValue2;
	m_dValue3 += dValue3;
	CConsolidateObject::Consolidate();		// consolidate base class
}

//*******************************************************************

bool CMyConsolidateArray::Add ( const char* szLabel, double dValue1, double dValue2, double dValue3 )
{
	int nIndex;

	if ( Find ( szLabel, nIndex ) == TRUE )	// label exists
	{
		((CMyConsolidateObject*) (CObArray::GetAt(nIndex)))->Consolidate ( dValue1, dValue2, dValue3 );

		return TRUE;
	}
	else								// label not found
	{
		CMyConsolidateObject* pObj = new CMyConsolidateObject ( szLabel, dValue1, dValue2, dValue3 );

		return CConsolidateArray::Add ( nIndex, pObj );
	}
}

//*******************************************************************
