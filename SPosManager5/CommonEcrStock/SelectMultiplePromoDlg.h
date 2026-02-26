#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "..\CommonEcrStock\SelectMultipleEntityDlg.h"
/**********************************************************************/

class CSelectMultiplePromoDlg : public CSelectMultipleEntityDlg
{
public:
	CSelectMultiplePromoDlg( CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo>& arrayEntityInfo, CWnd* pParent = NULL);   

private:
	virtual void SetListColumnns();
	virtual void GetListData();
	virtual void HandleAll();
	virtual bool HandleOK();

private:
	bool m_bSorted;
};

/**********************************************************************/
#endif
/**********************************************************************/
