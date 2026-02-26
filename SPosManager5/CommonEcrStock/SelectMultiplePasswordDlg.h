#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\SelectMultipleEntityDlg.h"
/**********************************************************************/

class CSelectMultiplePasswordDlg : public CSelectMultipleEntityDlg
{
public:
	CSelectMultiplePasswordDlg( CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo>& arrayEntityInfo, int nSourceIdx, CWnd* pParent = NULL);   
	
private:
	virtual void SetListColumnns();
	virtual void GetListData();
};

/**********************************************************************/
#endif		//__SELECTPASSWORDMULTIPLEDLG_H__
/**********************************************************************/

