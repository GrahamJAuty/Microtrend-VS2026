#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "Consolidation.h"
#include "EditEnter.h"
#include "OfferGroupsCSVArray.h"
#include "OfferGroupItem.h"
#include "..\CommonEcrStock\PluFilterArray.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CPropPageOfferGroupEntryPlu : public CSSPropertyPage
{
public:
	CPropPageOfferGroupEntryPlu();
	~CPropPageOfferGroupEntryPlu();

	void SetEntryType( int nEntryType );
	void SetNodeType( int nEntryType );
	void SetPluFilterMode( bool bSystem);
	
	//{{AFX_DATA(CPropPageOfferGroupEntryPlu)
	enum { IDD = IDD_PROPPAGE_OFFERGROUP_ENTRY_PLU };
	//}}AFX_DATA
	CEditEnter m_editPluNo;
	CEdit m_editName;
	CButton m_buttonModifier;
	CButton m_buttonScan;
	CButton m_buttonBrowse;
	CButton m_buttonAdd;

public:
	//{{AFX_VIRTUAL(CPropPageOfferGroupEntryPlu)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnKillFocusPlu();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonModifier();
	afx_msg void OnButtonRemove();
	afx_msg void OnButtonEditPlu();
	afx_msg void OnButtonBrowse();
	afx_msg void OnButtonScan();
	afx_msg long OnAddPluMessage ( WPARAM wIndex, LPARAM lParam );
	afx_msg long OnAddScannedPlu ( WPARAM wIndex, LPARAM lParam );
	afx_msg long OnSetDefaultFocus ( WPARAM wIndex, LPARAM lParam );
	virtual BOOL OnApply();
	
protected:
	//{{AFX_MSG(CPropPageOfferGroupEntryPlu)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SetRecordControls();
	void AddScannedPlu();

public:
	COfferGroupEntriesCSVArray* GetEntryArray();
	void SetOfferGroupRecord ( COfferGroupsCSVRecord* pOfferGroupRecord ) { m_pOfferGroupRecord = pOfferGroupRecord; }
	void Refresh();
	void SaveRecord();
	void SaveOfferGroupEntries();
	void SetWalletStampRuleFlag( bool b ) { m_bWalletStampRule = b; }
	
private:
	void SelectSingleItem( int nIndex );

public:
	void BuildPluArray();

private:
	void UpdateSelectedItemArray();
	void GetSelectedItem( int nIdx, COfferGroupItem& item );
	void SetSelectedItem( int nIdx, COfferGroupItem& item );
	void GetPluDeletionList( CStringArray& arrayText );
	
private:
	CSSListCtrlVirtual m_listPlu;

private:
	COfferGroupsCSVRecord* m_pOfferGroupRecord;
	CReportConsolidationArray<COfferGroupItem> m_arrayPluNo;
	CReportConsolidationArray<CSortedIntItem> m_arraySelected;
	CPluFilterArray m_FilterArray;

private:
	int m_nEntryType;
	int m_nNodeType;
	bool m_bDialogReady;
	bool m_bPluFilterMode;
	bool m_bPluFilterSystem;
	bool m_bWalletStampRule;
};

/**********************************************************************/
#endif
/**********************************************************************/
