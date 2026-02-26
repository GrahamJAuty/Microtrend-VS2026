#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "SessionCSVArray.h"
/**********************************************************************/

class CSessionEditCombinedDlg : public CSSDialog
{
public:
	CSessionEditCombinedDlg( CSessionCSVRecord& SessionRecord, CWnd* pParent = NULL);   
	
	//{{AFX_DATA(CSessionEditCombinedDlg)
	enum { IDD = IDD_SESSION_EDIT_COMBINED };
	CEdit m_editName;
	CButton	m_radioStartTime2;
	//}}AFX_DATA
	int	m_nSessionStartType;

	//{{AFX_VIRTUAL(CSessionEditCombinedDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSessionEditCombinedDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnButtonUp();
	afx_msg void OnButtonDown();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CSSListTaggedSelectCtrl m_listSessions;	
	CImageList m_ImageList;

private:
	CSessionCSVRecord& m_SessionRecord;
};

/**********************************************************************/
#endif
/**********************************************************************/

