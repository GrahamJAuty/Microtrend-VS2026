#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "LocationCSVArray.h"
#include "PriceSetHeaderManager.h"
/**********************************************************************/

class CPropPageLocationPriceSet : public CSSPropertyPage
{
public:
	CPropPageLocationPriceSet();
	~CPropPageLocationPriceSet();

	//{{AFX_DATA(CPropPageLocationPriceSet)
	enum { IDD = IDD_PROPPAGE_LOCATION_PRICESET };
	CSSComboBox	m_comboDbPriceSet3;
	CSSComboBox	m_comboDbPriceSet2;
	CSSComboBox	m_comboDbPriceSet1;
	CSSComboBox	m_comboKeyboard3;
	CSSComboBox	m_comboKeyboard2;
	CSSComboBox	m_comboKeyboard1;
	CSSComboBox	m_comboLocalPlu;
	CSSComboBox	m_comboDefPriceSet;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPageLocationPriceSet)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	
protected:
	//{{AFX_MSG(CPropPageLocationPriceSet)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectPriceSetCount();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetLocIdx( int n ) { m_nLocIdx = n; }
	void SetLocationBuffer( CLocationCSVArray* pBuffer ) { m_pLocationBuffer = pBuffer; }
	void SetPriceSetHeaderManager( CPriceSetHeaderManager* pManager ) { m_pPriceSetHeaderManager = pManager; }

private:
	void SaveRecord();
	void GetRecordData();

private:
	void SetLocalPluControls();
	void SetLocalPluEnables();
	void SelectPriceSetFields();
	void LoadPriceSetCombo( CSSComboBox& combo, int nKbd );
	void LoadKeyboardCombo( CSSComboBox& combo, int nKbd );

private:
	CSSListTaggedSelectCtrl m_listFields;

private:
	int m_nLocIdx;
	CLocationCSVArray* m_pLocationBuffer;
	CPriceSetHeaderManager* m_pPriceSetHeaderManager;
};

/**********************************************************************/
#endif
/**********************************************************************/
