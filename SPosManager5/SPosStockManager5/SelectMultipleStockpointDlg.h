#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "..\CommonEcrStock\SelectMultipleEntityDlg.h"
/**********************************************************************/

class CSelectMultipleStockpointDlg : public CSelectMultipleEntityDlg
{
public:
	CSelectMultipleStockpointDlg( const char* szTitle, CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo>& arrayEntityInfo, CWnd* pParent = NULL);   
	
private:
	virtual void SetListColumnns();
	virtual void GetListData();
};

/**********************************************************************/
#endif
/**********************************************************************/

