#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "..\CommonEcrStock\SelectMultipleEntityDlg.h"
/**********************************************************************/

class CSelectMultipleLocationDlg : public CSelectMultipleEntityDlg
{
public:
	CSelectMultipleLocationDlg( const char* szTitle, CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo>& arrayEntityInfo, bool bAutoSelectSingle, CWnd* pParent = NULL);   
	
public:
	static void CreateLocInfoArray( CArray<int,int>& arrayLocIdx, CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo>& arrayLocInfo );
	static bool FilterLocListForDownload( CArray<int,int>& arrayLocIdx, const char* szTitle, CWnd* pParent );

private:
	virtual void SetListColumnns();
	virtual void GetListData();
};

/**********************************************************************/
#endif
/**********************************************************************/

