#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "PluCSVArray.h"
/**********************************************************************/

class CPluCreateLinkDlg : public CSSDialog
{
public:
	CPluCreateLinkDlg( CPluCSVRecord& PluRecord, CWnd* pParent = NULL); 

	//{{AFX_DATA(CPluCreateLinkDlg)
	enum { IDD = IDD_PLU_CREATE_LINK };
	CEdit	m_editStockCode;
	CStatic	m_staticPlu;
	CStatic	m_staticDescription;
	CStatic	m_staticDept;
	CString	m_strStockCode;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPluCreateLinkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPluCreateLinkDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocusStockCode();
	afx_msg void OnButtonBrowse();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CPluCSVRecord& m_PluRecord;
};


//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif 
/**********************************************************************/

