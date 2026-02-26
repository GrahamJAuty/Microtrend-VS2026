#pragma once
/**********************************************************************/
#include "Resource.h"
/**********************************************************************/

class CPropPageBackupSchedule : public CPropertyPage
{
public:
	CPropPageBackupSchedule();
	~CPropPageBackupSchedule();

	//{{AFX_DATA(CPropPageBackupSchedule)
	enum { IDD = IDD_PROPPAGE_BACKUP_SCHEDULE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPageBackupSchedule)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

public:
	void SetPropertySheet( void* p ){ m_pPropertySheet = p; }
	void SetDayFlags( int n ){ m_nDayFlags = n; }
	void SetMinutes( int n ){ m_nMinutes = n; }

protected:
	//{{AFX_MSG(CPropPageBackupSchedule)
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	//}}AFX_MSG
	afx_msg void OnButtonAll();
	afx_msg void OnButtonNone();
	DECLARE_MESSAGE_MAP()

private:
	void SetRecordControls();
	void SaveRecord();

private:
	void* m_pPropertySheet;
	int m_nDayFlags;
	int m_nMinutes;

	WORD m_wIDHour[24];
	CButton m_checkHour[24];
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/

