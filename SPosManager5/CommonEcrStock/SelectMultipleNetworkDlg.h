#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "..\CommonEcrStock\SelectMultipleEntityDlg.h"
/**********************************************************************/

class CSelectMultipleNetworkDlg : public CSelectMultipleEntityDlg
{
public:
	CSelectMultipleNetworkDlg( const char* szTitle, CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo>& arrayEntityInfo, CWnd* pParent = NULL);   
	
private:
	virtual void SetListColumnns();
	virtual void GetListData();
};

/**********************************************************************/
#endif
/**********************************************************************/
