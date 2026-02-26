#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "LocationCSVArray.h"
/**********************************************************************/
#include "..\CommonEcrStock\SelectMultipleEntityDlg.h"
/**********************************************************************/

class CSelectMultipleMasterDlg : public CSelectMultipleEntityDlg
{
public:
	CSelectMultipleMasterDlg( CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo>& arrayEntityInfo, CWnd* pParent = NULL);   
		
private:
	virtual void SetListColumnns();
	virtual void GetListData();
};

/**********************************************************************/
#endif
/**********************************************************************/

