#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "..\CommonEcrStock\SelectMultipleEntityDlg.h"
/**********************************************************************/

class CSelectMultiplePluDlg : public CSelectMultipleEntityDlg
{
public:
	CSelectMultiplePluDlg( CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo>& arrayEntityInfo, const char* szStockCode, CWnd* pParent = NULL);   

private:
	virtual void SetListColumnns();
	virtual void GetListData();
	virtual void HandleCancel();
};

/**********************************************************************/
#endif
/**********************************************************************/
