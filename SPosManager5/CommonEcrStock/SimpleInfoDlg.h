#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CSimpleInfoDlg : public CDialog
{
public:
	CSimpleInfoDlg( const char* szTitle, const char* szLabel1, const char* szLabel2, int nNumber1, int nNumber2, CWnd* pParent = NULL);   

	//{{AFX_DATA(CSimpleInfoDlg)
	enum { IDD = IDD_SIMPLE_INFO };
	CStatic	m_staticLabel1;
	CStatic	m_staticLabel2;
	CStatic	m_staticNumber1;
	CStatic	m_staticNumber2;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSimpleInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSimpleInfoDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nNumber1;
	int m_nNumber2;
	CString m_strLabel1;
	CString m_strLabel2;
	CString m_strTitle;
};

//{{AFX_INSERT_LOCATION}}

/**********************************************************************/
