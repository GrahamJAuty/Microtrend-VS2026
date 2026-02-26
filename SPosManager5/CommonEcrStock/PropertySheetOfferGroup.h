#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "OfferGroupsCSVArray.h"
#include "PropPageOfferGroupEntryPlu.h"
#include "PropPageOfferGroupEntryDept.h"
#include "PropPageOfferGroupEntryGroup.h"
/**********************************************************************/

class CPropertySheetOfferGroup : public CSpinPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetOfferGroup)

public:
	CPropertySheetOfferGroup( WORD wSpinID, int nIndex, bool bAllowSpin, bool bWalletStampRule, CWnd* pWndParent);
	virtual ~CPropertySheetOfferGroup();

public:
	bool GotEntryType( int nGroupNo, int nEntryType );
		
public:
	//{{AFX_VIRTUAL(CPropertySheetOfferGroup)
	protected:
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropertySheetOfferGroup)
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
	void InitWalletStampRule();
	void InitMixMatch();

private:
	int m_nIndex;
	bool m_bAllowSpin;
	bool m_bWalletStampRule;

private:
	COfferGroupsCSVRecord m_OfferGroupRecord;

public:
	CPropPageOfferGroupEntryPlu m_Page1;
	CPropPageOfferGroupEntryDept m_Page2;
	CPropPageOfferGroupEntryDept m_Page3;
	CPropPageOfferGroupEntryGroup m_Page4;
	CPropPageOfferGroupEntryPlu m_Page5;
	
private:
	CListDataDlg* m_pListDataDlg;
};

/**********************************************************************/
#endif
/**********************************************************************/
