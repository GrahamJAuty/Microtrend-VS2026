/**********************************************************************/
#include "SimpleListCtrl.h"
/**********************************************************************/

void CSimpleListCtrl::AddColumn ( const char* szHeaderText, int nWidth, int nAlign )
{
	int nColumnCount = GetHeaderCtrl()->GetItemCount();

	InsertColumn ( nColumnCount, szHeaderText, nAlign, nWidth );
}

/**********************************************************************/

void CSimpleListCtrl::AddRow ( const char* szData )
{
	CCSV csv ( szData );
	int nRowCount = GetItemCount();
	int nColumnCount = GetHeaderCtrl()->GetItemCount();

	for ( int i = 0 ; i < nColumnCount ; i++ )
	{
		if ( i == 0 )
			InsertItem ( nRowCount, csv.GetString(i) );
		else
			SetItemText ( nRowCount, i, csv.GetString(i) );
	}
}

/**********************************************************************/
