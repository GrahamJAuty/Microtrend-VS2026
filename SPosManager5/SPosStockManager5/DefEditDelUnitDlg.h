#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CDefEditDelUnitDlg : public CSSDialog
{
public:
	CDefEditDelUnitDlg( const char* szTitle, CWnd* pParent = NULL);   

	//{{AFX_DATA(CDefEditDelUnitDlg)
	enum { IDD = IDD_DEF_EDIT_DELUNIT };
	CEdit	m_EditDelUnit;
	CString	m_strDeliveryUnit;
	double	m_dDUItemSize;
	int		m_nDUItems;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDefEditDelUnitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDefEditDelUnitDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocusYield();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void UpdateYieldDisplay();

private:
	CString m_strTitle;
};

/**********************************************************************/
#endif		
/**********************************************************************/

