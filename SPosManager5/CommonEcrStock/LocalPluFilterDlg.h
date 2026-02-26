#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/

class CLocalPluFilterDlg : public CDialog
{
public:
	CLocalPluFilterDlg( CPluFilterArray& FilterArray, CWnd* pParent = NULL);   

	//{{AFX_DATA(CLocalPluFilterDlg)
	enum { IDD = IDD_LOCALPLU_FILTER };
	BOOL	m_bExclude;
	BOOL	m_bLocation;
	BOOL	m_bMaster;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CLocalPluFilterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CLocalPluFilterDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CPluFilterArray& m_FilterArray;
};

//{{AFX_INSERT_LOCATION}}

/**********************************************************************/

