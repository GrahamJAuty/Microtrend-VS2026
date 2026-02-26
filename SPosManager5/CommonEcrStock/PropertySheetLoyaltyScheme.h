#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "PropPageLoyaltySchemeGeneral.h"
#include "PropPageLoyaltySchemeAction.h"
#include "PropPageLoyaltySchemeStaffLimits.h"
#include "PropPageLoyaltySchemePayments.h"
/**********************************************************************/

class CPropertySheetLoyaltyScheme : public CSpinPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetLoyaltyScheme)

public:
	CPropertySheetLoyaltyScheme( WORD wSpinID, int nSchemeIdx, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pWndParent = NULL);
	virtual ~CPropertySheetLoyaltyScheme();

public:
	CPropPageLoyaltySchemeGeneral m_Page1;
	CPropPageLoyaltySchemeAction m_Page2;
	CPropPageLoyaltySchemeStaffLimits m_Page3;
	CPropPageLoyaltySchemePayments m_Page4;
	
public:
	//{{AFX_VIRTUAL(CPropertySheetLoyaltyScheme)
	//}}AFX_VIRTUAL

public:
	void SaveRecord();
	int GetIndex() { return m_nSchemeIdx; }

public:
	void SetListDataDlg( CListDataDlg* pDlg ) { m_pListDataDlg = pDlg; }
	
private:
	void UpdateTitle();
	virtual void SpinPrevious();
	virtual void SpinNext();

protected:
	//{{AFX_MSG(CPropertySheetLoyaltyScheme)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nSchemeIdx;
	CLoyaltySchemeCSVRecord m_SchemeRecord;
	CByteArray m_PageArray;

private:
	CListDataDlg* m_pListDataDlg;
	CListDataDlgChangeFlag& m_WarnCancelAfterChange;
};

/**********************************************************************/
#endif
/**********************************************************************/
