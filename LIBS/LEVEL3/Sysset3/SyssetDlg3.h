/**********************************************************************/
#pragma once
/**********************************************************************/

#include "sysset3.h"

/**********************************************************************/

class CSyssetDlg3 : public CDialog
{
	DECLARE_DYNAMIC(CSyssetDlg3)

public:
	CSyssetDlg3(const char* szText, CWnd* pParent = NULL);
	virtual ~CSyssetDlg3();

	enum { IDD = IDD_SYSSET_DLG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()

private:
	CStatic m_staticWaitMessage;
};

/**********************************************************************/
