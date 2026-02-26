/**********************************************************************/
#pragma once
/**********************************************************************/

#include <afxcmn.h>

/**********************************************************************/

class CSimpleListCtrl : public CListCtrl
{
public:
	void AddColumn ( const char* szHeaderText, int nWidth, int nAlign );	// LVCFMT_LEFT, LVCFMT_RIGHT or LVCFMT_CENTER
	void AddRow ( const char* szData );							// csv data line
};

/**********************************************************************/

