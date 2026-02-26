/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "DepartmentSorter.h"
/**********************************************************************/

CDepartmentSorter::CDepartmentSorter()
{
	//first array is for unknown items
	//second array is for department zero
	for ( int n = -1; n <= Dept::DeptNo.Max; n++ )
		m_DepartmentTable.Add( NULL );
}

/**********************************************************************/

CDepartmentSorter::~CDepartmentSorter()
{
	Reset();
}

/**********************************************************************/

void CDepartmentSorter::Reset()
{
	for ( int n = 0; n < m_DepartmentTable.GetSize(); n++ )
	{
		void* pTable = m_DepartmentTable.GetAt(n);
		
		if ( pTable != NULL )
		{
			CArray<int,int>* pArray = (CArray<int,int>*) pTable;
			pArray -> RemoveAll();
			delete pArray;
			m_DepartmentTable.SetAt( n, NULL );
		}
	}
}

/**********************************************************************/

void CDepartmentSorter::Add( int nDeptNo, int nPluIdx )
{
	if ( ( nDeptNo >= -1 ) && ( nDeptNo <= Dept::DeptNo.Max ) )
	{
		void* pTable = m_DepartmentTable.GetAt( nDeptNo + 1 );

		if ( NULL == pTable )
		{
			CArray<int,int>* pArray = new CArray<int,int>;
			m_DepartmentTable.SetAt( nDeptNo + 1, pArray );
			pArray -> Add( nPluIdx );
		}
		else
		{
			CArray<int,int>* pArray = (CArray<int,int>*) pTable;
			pArray -> Add ( nPluIdx );
		}
	}
}

/**********************************************************************/

void CDepartmentSorter::Build( bool bIncludeUnknown )
{
	Reset();
	
	CPluCSVRecord PluRecord;
	int nCount = DataManager.Plu.GetSize();	

	StatusProgress.Lock( TRUE, "Building department table" );
	for ( int nPluIdx = 0; nPluIdx < nCount; nPluIdx++ )
	{
		StatusProgress.SetPos ( nPluIdx, nCount );
		DataManager.Plu.GetAt ( nPluIdx, PluRecord );
		Add( PluRecord.GetBaseDeptNo(), nPluIdx );
	}

	if ( bIncludeUnknown == TRUE )
		Add( -1, 0 );

	StatusProgress.Unlock();
}

/**********************************************************************/

int CDepartmentSorter::GetItemCount ( int nDeptNo )
{
	int nResult = 0;

	if ( ( nDeptNo >= -1 ) && ( nDeptNo <= Dept::DeptNo.Max ) )
	{
		void* pTable = m_DepartmentTable.GetAt( nDeptNo + 1 );

		if ( NULL != pTable )
		{
			CArray<int,int>* pArray = (CArray<int,int>*) pTable;
			nResult = pArray -> GetSize();
		}
	}

	return nResult;
}

/**********************************************************************/

int CDepartmentSorter::GetItemIndex ( int nDeptNo, int nIndex )
{
	int nResult = 0;

	if ( ( nDeptNo >= -1 ) && ( nDeptNo <= Dept::DeptNo.Max ) )
	{
		void* pTable = m_DepartmentTable.GetAt( nDeptNo + 1 );

		if ( NULL != pTable )
		{
			CArray<int,int>* pArray = (CArray<int,int>*) pTable;
			
			if ( ( nIndex >= 0 ) && ( nIndex < pArray -> GetSize() ) )
				nResult = pArray -> GetAt( nIndex );
		}
	}

	return nResult;
}

/**********************************************************************/


