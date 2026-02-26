#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "..\CommonEcrStock\SelectMultipleEntityDlg.h"
/**********************************************************************/

class CSelectMultipleKbdCloneTargetDlg : public CSelectMultipleEntityDlg
{
public:
	CSelectMultipleKbdCloneTargetDlg( const char* szTitle, CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo>& arrayEntityInfo, bool bSingle, CWnd* pParent = NULL);   

public:
	bool GetCopyNameFlag(){ return m_bCopyNames; }

private:
	virtual void SetListColumnns();
	virtual void GetListData();
	virtual bool HandleOK();

private:
	bool m_bSingle;
	bool m_bCopyNames;
};

/**********************************************************************/
#endif
/**********************************************************************/

