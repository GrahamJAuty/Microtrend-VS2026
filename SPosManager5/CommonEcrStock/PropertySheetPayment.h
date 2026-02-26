#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "PaymentCSVArray.h"
#include "PropPagePaymentGeneral.h"
#include "PropPagePaymentText.h"
/**********************************************************************/

class CPropertySheetPayment : public CSpinPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetPayment)

public:
	CPropertySheetPayment( WORD wSpinID, int nPaymentIdx, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pWndParent = NULL);
	virtual ~CPropertySheetPayment();

public:
	CPropPagePaymentGeneral m_Page1;
	CPropPagePaymentText m_Page2;
	
public:
	//{{AFX_VIRTUAL(CPropertySheetPayment)
	//}}AFX_VIRTUAL

public:
	void SaveRecord();
	int GetIndex() { return m_nPaymentIdx; }

public:
	void SetListDataDlg( CListDataDlg* pDlg ) { m_pListDataDlg = pDlg; }

private:
	void UpdateTitle();
	virtual void SpinPrevious();
	virtual void SpinNext();

protected:
	//{{AFX_MSG(CPropertySheetPayment)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nPaymentIdx;
	CPaymentCSVRecord m_PaymentRecord;
	CByteArray m_PageArray;

private:
	CListDataDlg* m_pListDataDlg;
	CListDataDlgChangeFlag& m_WarnCancelAfterChange;
};

/**********************************************************************/
#endif
/**********************************************************************/
