#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "PriceSetHeaderManager.h"
/**********************************************************************/

class CPropPageDatabasePriceSet : public CPropertyPage
{
public:
	CPropPageDatabasePriceSet();
	~CPropPageDatabasePriceSet();

	//{{AFX_DATA(CPropPageDatabasePriceSet)
	enum { IDD = IDD_PROPPAGE_DATABASE_PRICESET };
	CSSComboBox	m_comboPriceSet;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPageDatabasePriceSet)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPageDatabasePriceSet)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectPriceSet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	
public:
	int GetPriceSet() { return m_nPriceSet; }
	void SetDbIdx( int n ) { m_nDbIdx = n; }
	void SetDatabaseBuffer( CDatabaseCSVArray* pBuffer ) { m_pDatabaseBuffer = pBuffer; }
	void SetPriceSetHeaderManager( CPriceSetHeaderManager* pManager ) { m_pPriceSetHeaderManager = pManager; }

private:
	void SetLocalPluControls();
	void SelectPriceSetFields();
	void UpdateNewHeader();
	
private:
	CSSListTaggedSelectCtrl m_listFields;

private:
	int m_nDbIdx;
	int m_nPriceSet;
	CDatabaseCSVArray* m_pDatabaseBuffer;
	CPriceSetHeaderManager* m_pPriceSetHeaderManager;
};

/**********************************************************************/
#endif // __PROPPAGEDATABASEPRICESET_H__
/**********************************************************************/
