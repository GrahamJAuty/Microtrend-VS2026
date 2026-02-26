#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "TaxArray.h"
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CEditRatesDlg : public CSSDialog
{
public:
	CEditRatesDlg( CWnd* pParent = NULL);   

	//{{AFX_DATA(CEditRatesDlg)
	enum { IDD = IDD_EDIT_RATES };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CEditRatesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CEditRatesDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	afx_msg void OnButtonDownload();
	DECLARE_MESSAGE_MAP()

private:
	void HandleDownload( int nDbIdx, CArray<int,int>& arrayLocIdx );

private:
	//ARRAYS OF CONTROLS
	CButton m_buttonDownload;
	CEdit m_editText[MAX_TAX_BANDS];

	//ARRAYS OF CONTROL IDS
	WORD m_IDEditRate[MAX_TAX_BANDS];
	WORD m_IDEditText[MAX_TAX_BANDS];

	//ARRAYS OF VALUES
	double m_dRate[MAX_TAX_BANDS];
	
private:
	bool m_bWarnCancelAfterDownload;
	CString m_strType;
	int m_nMaxLen;
	int m_nDPQty;
};

//{{AFX_INSERT_LOCATION}}

/**********************************************************************/
#endif
/**********************************************************************/
