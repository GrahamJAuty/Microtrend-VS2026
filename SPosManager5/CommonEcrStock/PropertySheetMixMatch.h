#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "PropPageMixMatchBuckets.h"
#include "PropPageMixMatchGeneral.h"
#include "PropPageMixMatchTimeplan.h"
#include "MixMatchCSVArray.h"
/**********************************************************************/

class CPropertySheetMixMatch : public CSpinPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetMixMatch)

public:
	CPropertySheetMixMatch( WORD wSpinID, bool bAllowSpin, int nMixMatchIdx, CWnd* pWndParent = NULL);
	virtual ~CPropertySheetMixMatch();

public:
	CPropPageMixMatchGeneral m_Page1;
	CPropPageMixMatchBuckets m_Page2;
	CPropPageMixMatchTimeplan m_PageTimeplan;

public:
	//{{AFX_VIRTUAL(CPropertySheetMixMatch)
	//}}AFX_VIRTUAL

public:
	void SaveRecord();
	int GetIndex() { return m_nMixMatchIdx; }

public:
	void SetListDataDlg( CListDataDlg* pDlg ) { m_pListDataDlg = pDlg; }

private:
	void UpdateTitle();
	virtual void SpinPrevious();
	virtual void SpinNext();

protected:
	//{{AFX_MSG(CPropertySheetMixMatch)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nMixMatchIdx;
	CMixMatchCSVRecord m_MixMatchRecord;
	CByteArray m_PageArray;

private:
	CListDataDlg* m_pListDataDlg;
};

/**********************************************************************/
#endif
/**********************************************************************/
