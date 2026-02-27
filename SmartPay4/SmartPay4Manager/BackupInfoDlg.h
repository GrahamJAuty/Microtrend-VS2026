#pragma once
/**********************************************************************/
#include "Resource.h"
/**********************************************************************/

class CBackupInfoDlg : public CSSAutoShutdownDialog
{
	DECLARE_DYNAMIC(CBackupInfoDlg)

public:
	CBackupInfoDlg(CWnd* pParent = NULL);   
	virtual ~CBackupInfoDlg();

	enum { IDD = IDD_BACKUP_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	CEdit m_editLast;
	CEdit m_editNext;
	CEdit m_editAttempt;
	CEdit m_editReason;
	CEdit m_editPrompt;
	
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	const char* FormatTimeDate( COleDateTime& timeDate );

private:
	CString m_strTimeDate;
};

/**********************************************************************/

