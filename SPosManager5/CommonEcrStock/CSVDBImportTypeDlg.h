#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CCSVDBImportTypeDlg : public CDialog
{
public:
	CCSVDBImportTypeDlg( int nNodeType, CWnd* pParent = NULL);   

	//{{AFX_DATA(CCSVDBImportTypeDlg)
	enum { IDD = IDD_CSVDB_IMPORTTYPE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CCSVDBImportTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	CButton m_radioAll;
	CButton m_radioNew;
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CCSVDBImportTypeDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int m_nNodeType;
	int m_nImportMethod;
};

/**********************************************************************/
