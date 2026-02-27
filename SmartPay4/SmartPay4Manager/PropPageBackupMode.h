#pragma once
/**********************************************************************/
#include "Resource.h"
/**********************************************************************/

class CPropPageBackupMode : public CSSAutoShutdownPropertyPage
{
public:
	CPropPageBackupMode();
	~CPropPageBackupMode();

	//{{AFX_DATA(CPropPageBackupMode)
	enum { IDD = IDD_PROPPAGE_BACKUP_MODE };
	int		m_nBackupMode;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPageBackMode)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

public:
	void SetPropertySheet( void* p ){ m_pPropertySheet = p; }
	void SetMinutes( int n ){ m_nMinutes = n; }

private:
	void SetBackupMode( int n );
	
protected:
	//{{AFX_MSG(CPropPageBackupMode)
	afx_msg void OnRadioMode1();
	afx_msg void OnRadioMode2();
	afx_msg void OnRadioMode3();
	afx_msg void OnRadioMode4();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void* m_pPropertySheet;
	int m_nMinutes;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/

