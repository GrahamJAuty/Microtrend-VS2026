/**********************************************************************/
#pragma once
/**********************************************************************/
#include "DepartmentCSVArray.h"
/**********************************************************************/

class CDepartmentSorter
{
public:
	CDepartmentSorter();
	~CDepartmentSorter();

public:
	void Build( bool bIncludeUnknown = FALSE );
	int GetItemCount ( int nDeptNo );
	int GetItemIndex ( int nDeptNo, int nIndex );

private:
	void Reset();
	void Add( int nDeptNo, int nPluIdx );

private:
	CPtrArray m_DepartmentTable;
};

/**********************************************************************/

