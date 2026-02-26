#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "OfferGroupsCSVArray.h"
#include "PropPageOfferGroupEntryPlu.h"
/**********************************************************************/

class CPropertySheetPluFilter : public CSpinPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetPluFilter)

public:
	CPropertySheetPluFilter( WORD wSpinID, int nIndex, bool bSystem, bool bAllowSpin, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pWndParent = NULL);
	virtual ~CPropertySheetPluFilter();
		
public:
	//{{AFX_VIRTUAL(CPropertySheetPluFilter)
	protected:
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropertySheetPluFilter)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetListDataDlg( CListDataDlg* pDlg ) { m_pListDataDlg = pDlg; }
	void SaveRecord();
	int GetIndex(){ return m_nIndex; }

private:
	virtual void SpinPrevious();
	virtual void SpinNext();

private:
	void UpdateTitle();

private:
	int m_nIndex;
	bool m_bAllowSpin;

private:
	bool m_bSystem;
	COfferGroupsCSVArray* m_pFilterNames;
	COfferGroupEntriesCSVArray* m_pFilterItems;

private:
	COfferGroupsCSVRecord m_PluFilterRecord;

public:
	CPropPageOfferGroupEntryPlu m_Page1;
	
private:
	CListDataDlg* m_pListDataDlg;
	CListDataDlgChangeFlag& m_WarnCancelAfterChange;
};

/**********************************************************************/
#endif
/**********************************************************************/
