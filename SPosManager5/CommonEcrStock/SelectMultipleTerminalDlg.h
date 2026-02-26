#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "..\CommonEcrStock\SelectMultipleEntityDlg.h"
/**********************************************************************/

class CSelectMultipleTerminalDlg : public CSelectMultipleEntityDlg
{
public:
	CSelectMultipleTerminalDlg( const char* szTitle, CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo>& arrayEntityInfo, CWnd* pParent = NULL);   
	
private:
	virtual void SetListColumnns();
	virtual void GetListData();
};

/**********************************************************************/
#endif		//__SelectMultipleTerminalDlg_H__
/**********************************************************************/

