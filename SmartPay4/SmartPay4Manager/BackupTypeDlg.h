#pragma once
/**********************************************************************/
#include "Resource.h"
/**********************************************************************/

class CBackupTypeDlg : public CSSAutoShutdownDialog
{
public:
	CBackupTypeDlg(CWnd* pParent = NULL);   
	virtual ~CBackupTypeDlg();

	enum { IDD = IDD_BACKUP_TYPE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

public:
	int m_nType;
};

/**********************************************************************/
