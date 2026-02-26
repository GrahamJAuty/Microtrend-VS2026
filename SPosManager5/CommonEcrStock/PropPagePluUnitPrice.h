#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "PluChangesBuffer.h"
/**********************************************************************/
#include "afxwin.h"
/**********************************************************************/

class CPropPagePluUnitPrice : public CSSPropertyPage
{
public:
	CPropPagePluUnitPrice();
	~CPropPagePluUnitPrice();
	//{{AFX_DATA(CPropPagePluUnitPrice)
	enum { IDD = IDD_PROPPAGE_PLU_UNITPRICE };
	CStatic	m_staticPriceType;
	CEdit	m_editLabel;
	CSSComboBox	m_comboPluUnit;
	CButton m_checkShelf;
	CButton m_checkProduct;
	CString	m_strPluUnit;
	double	m_dPrice;
	double	m_dPluSize;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPagePluUnitPrice)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	void SetReadOnlyFlag( bool b ){ m_bReadOnly = b; }

protected:
	//{{AFX_MSG(CPropPagePluUnitPrice)
	afx_msg void OnButtonSetUnit();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeLabel();
	afx_msg void OnChangePluSize();
	afx_msg void OnButtonRemove();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonSettings();
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetPluChangesBuffer( CPluChangesBuffer* pBuffer ) { m_pPluChangesBuffer = pBuffer; }
	void FillPluUnitCombo();
	void SetPluRecord ( CPluCSVRecord* pPluRecord ) { m_pPluRecord = pPluRecord; }
	void Refresh();
	bool UpdateRecord();

private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();
	void AddList( int nTargetBarcodeIdx );

private:
	CEdit* GetEditPluSize();

private:
	CSSListCtrl m_listBarcodes;
	
private:
	CPluCSVRecord* m_pPluRecord;
	CPluChangesBuffer* m_pPluChangesBuffer;
	CArray<int,int> m_BarcodeIdxArray;
	CString m_strPriceType;
	bool m_bReadOnly;
	CButton m_buttonSetUnit;
	CButton m_buttonBarcodeAdd;
	CButton m_buttonBarcodeDelete;
	CButton m_buttonBarcodeSettings;
};

/**********************************************************************/
#endif
/**********************************************************************/
