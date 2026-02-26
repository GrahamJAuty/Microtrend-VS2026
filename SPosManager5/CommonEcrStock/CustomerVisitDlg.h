#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CCustomerVisitDlg : public CSSDialog
{
public:
	CCustomerVisitDlg( int nCustIdx, CWnd* pParent = NULL);   

	//{{AFX_DATA(CCustomerVisitDlg)
	enum { IDD = IDD_CUSTOMER_VISIT };
	CEdit m_editName;
	int m_nBeforeThis;
	int m_nAfterThis;
	int m_nBeforeOther;
	int m_nAfterOther;
	int m_nVisit;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CCustomerVisitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CCustomerVisitDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditBeforeThis();
	CEdit* GetEditAfterThis();
	CEdit* GetEditBeforeOther();
	CEdit* GetEditAfterOther();
	CEdit* GetEditVisit();

private:
	int m_nCustIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/
