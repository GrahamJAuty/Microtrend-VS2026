#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "PaidInOutTextCSVArray.h"
/**********************************************************************/

class CPaidInOutTextEditDlg : public CSSDialog
{
public:
	CPaidInOutTextEditDlg(CWnd* pParent = NULL); 

	//{{AFX_DATA(CPaidInOutTextEditDlg)
	enum { IDD = IDD_PAIDINOUT_EDIT };
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CPaidInOutTextEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL
	
private:
	int m_editTextID[PAIDIO_TEXT_COUNT];
	CEdit m_editText[PAIDIO_TEXT_COUNT];
	CButton m_buttonDownload;
	
protected:
	//{{AFX_MSG(CPaidInOutTextEditDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg void OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnButtonDownload();
	DECLARE_MESSAGE_MAP()

private:
	void SelectTab( int nPage );
	void HandleDownload( CArray<int,int>& arrayLocIdx );
	void BufferCurrentTabChanges();

private:
	bool m_bWarnCancelAfterDownload;
	
private:
	CTabCtrl m_TabPage;
	int m_nTabPage;

private:
	CString m_strPaidInBuffer[PAIDIO_TEXT_COUNT];
	CString m_strPaidOutBuffer[PAIDIO_TEXT_COUNT];
};

//{{AFX_INSERT_LOCATION}}

/**********************************************************************/
#endif
/**********************************************************************/
