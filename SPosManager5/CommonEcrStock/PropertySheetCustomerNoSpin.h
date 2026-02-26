#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "PropPageCustomerGeneral.h"
#include "PropPageCustomerContact.h"
#include "PropPageCustomerAutoDelete.h"
/**********************************************************************/

class CPropertySheetCustomerNoSpin : public CSSPropertySheet
{
public:
	CPropertySheetCustomerNoSpin( int nIndex, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pWndParent = NULL);
	virtual ~CPropertySheetCustomerNoSpin();

public:
	CPropPageCustomerGeneral m_PageGeneral;
	CPropPageCustomerContact m_PageContact;
	CPropPageCustomerAutoDelete m_PageAutoDelete;
	
public:
	//{{AFX_VIRTUAL(CPropertySheetCustomerNoSpin)
	//}}AFX_VIRTUAL
	virtual BOOL OnInitDialog();

public:
	void SaveRecord();
	void UndoChanges();
	int GetIndex() { return m_nIndex; }

private:
	void UpdateTitle();

protected:
	//{{AFX_MSG(CPropertySheetCustomerNoSpin)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nIndex;
	CByteArray m_PageArray;
	bool m_bAdminEnabledDummy;
	
private:
	CRect m_PageRect;

private:
	CListDataDlgChangeFlag& m_WarnCancelAfterChange;
};

/**********************************************************************/
#endif
/**********************************************************************/
