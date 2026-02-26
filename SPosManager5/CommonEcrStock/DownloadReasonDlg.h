#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CDownloadReasonDlg : public CSSDialog
{
public:
	CDownloadReasonDlg( CWnd* pParent = NULL);   
	
	//{{AFX_DATA(CDownloadReasonDlg)
	enum { IDD = IDD_DOWNLOAD_REASONTYPE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDownloadReasonDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDownloadReasonDlg)
	afx_msg void OnButtonAll();
	afx_msg void OnButtonNone();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	DECLARE_MESSAGE_MAP()

private:
	void AddList();

private:
	CButton m_checkClear;
	CSSListTaggedSelectCtrl m_listReasonTypes;

public:
	int m_nParam1;
};

/**********************************************************************/

