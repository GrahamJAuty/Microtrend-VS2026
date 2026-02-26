#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\ComboListCtrl.h"
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/
#include "EditEnter.h"
#include "PluChangesBuffer.h"
/**********************************************************************/
#include "afxwin.h"
/**********************************************************************/

class CPropPagePluLink : public CSSPropertyPage
{
public:
	CPropPagePluLink();
	~CPropPagePluLink();
	//{{AFX_DATA(CPropPagePluLink)
	enum { IDD = IDD_PROPPAGE_PLU_LINK };
	//}}AFX_DATA
	CSSComboBox m_comboLinkType;
	CEditEnter m_editPluNo;
	CButton m_buttonBrowse;
	CButton m_buttonAdd;
	CButton m_buttonRemove;
	CButton m_buttonScan;
	CButton m_checkSellTiedItems;

	//{{AFX_VIRTUAL(CPropPagePluLink)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	//}}AFX_VIRTUAL

public:
	void SetReadOnlyFlag( bool b ){ m_bReadOnly = b; }

protected:
	//{{AFX_MSG(CPropPagePluLink)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	//afx_msg void OnSelectItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillFocusPlu();
	afx_msg void OnSelectLinkType();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonRemove();
	afx_msg void OnButtonBrowse();
	afx_msg void OnButtonScan();
	afx_msg long OnAddPluMessage ( WPARAM wIndex, LPARAM lParam );
	afx_msg long OnAddScannedPlu ( WPARAM wIndex, LPARAM lParam );
	afx_msg long OnSetDefaultFocus ( WPARAM wIndex, LPARAM lParam );
	afx_msg LRESULT PopulateComboList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT GetCellType(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndLabelEditVariableCriteria(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual BOOL OnApply();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

public:
	BOOL PreTranslateMessage(MSG* pMsg);

private:
	void AddList();
	bool AddPluNoToList( __int64 nPluNo );
	void AddField( CCSV& csv );
	void UpdateLinkPluNoArray();
	void RefreshInternal();

public:
	void SetPluChangesBuffer( CPluChangesBuffer* pBuffer ) { m_pPluChangesBuffer = pBuffer; }
	void SetPluRecord ( CPluCSVRecord* pPluRecord ) { m_pPluRecord = pPluRecord; }

public:
	void SaveRecord();
	//void GetRecordData();
	void SetRecordControls();
	void Refresh();
	bool UpdateRecord();

private:
	void AddScannedPlu();
	
private:
	CPluFilterArray m_FilterArray;

private:
	CComboListCtrl m_listLinkPluNo;
	CArray<CellTypeInfo,CellTypeInfo> m_arrayCellTypes;
	CellTypeInfoTextBuffer m_ModifierTextBuffer;
	
private:
	CPluCSVRecord* m_pPluRecord;
	CArray<__int64,__int64> m_arrayLinkPluNo;
	CPluChangesBuffer* m_pPluChangesBuffer;
	bool m_bCanEnableSellTieldItems;
	bool m_bReadOnly;	
};

/**********************************************************************/
#endif
/**********************************************************************/
