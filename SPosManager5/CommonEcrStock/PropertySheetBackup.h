#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "PropPageBackupMode.h"
#include "PropPageBackupSchedule.h"
/**********************************************************************/

class CPropertySheetBackup : public CSSPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetBackup)

public:
	CPropertySheetBackup( int nMinutes, CWnd* pWndParent = NULL);
		
public:
	//{{AFX_VIRTUAL(CPropertySheetBackup)
	protected:
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropertySheetBackup)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void AddDayPages( int nMinutes );

private:
	CPropPageBackupMode m_PropPageMode;
	CPropPageBackupSchedule m_PageArray[7];
	int m_nPageCount;

	friend class CPropPageBackupMode;
};

/**********************************************************************/
#endif
/**********************************************************************/
