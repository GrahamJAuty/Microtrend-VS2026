#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "PropPageCustomerGeneral.h"
#include "PropPageCustomerContact.h"
#include "PropPageCustomerAutoDelete.h"
/**********************************************************************/

class CPropertySheetCustomer : public CSpinPropertySheet
{
public:
	CPropertySheetCustomer( WORD wSpinID, bool bOneOff, int nIndex, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pWndParent = NULL);
	virtual ~CPropertySheetCustomer();

public:
	bool GetAdminEnabledFlag(){ return m_bAdminEnabled; }

public:
	CPropPageCustomerGeneral m_PageGeneral;
	CPropPageCustomerContact m_PageContact;
	CPropPageCustomerAutoDelete m_PageAutoDelete;
	
public:
	//{{AFX_VIRTUAL(CPropertySheetCustomer)
	//}}AFX_VIRTUAL
	virtual BOOL OnInitDialog();

public:
	void SaveRecord();
	void UndoChanges();
	int GetIndex() { return m_nIndex; }

public:
	void SetListDataDlg( CListDataDlg* pDlg ) { m_pListDataDlg = pDlg; }

private:
	void UpdateTitle();
	virtual void SpinPrevious();
	virtual void SpinNext();

protected:
	//{{AFX_MSG(CPropertySheetCustomer)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nIndex;
	bool m_bAdminEnabled;
	CByteArray m_PageArray;
	bool m_bOneOff;
	
private:
	CListDataDlg* m_pListDataDlg;
	CRect m_PageRect;

private:
	CListDataDlgChangeFlag& m_WarnCancelAfterChange;
};

/**********************************************************************/
#endif
/**********************************************************************/
