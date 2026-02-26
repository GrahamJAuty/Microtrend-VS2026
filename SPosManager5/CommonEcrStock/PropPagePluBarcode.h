#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "PluChangesBuffer.h"
/**********************************************************************/

class CPropPagePluBarcode : public CSSPropertyPage
{
public:
	CPropPagePluBarcode();
	~CPropPagePluBarcode();
	//{{AFX_DATA(CPropPagePluBarcode)
	enum { IDD = IDD_PROPPAGE_PLU_BARCODE };
	CButton m_buttonAdd;
	CButton m_buttonRemove;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPagePluBarcode)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	void SetReadOnlyFlag( bool b ){ m_bReadOnly = b; }

protected:
	//{{AFX_MSG(CPropPagePluBarcode)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonRemove();
	afx_msg void OnButtonAdd();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetPluChangesBuffer( CPluChangesBuffer* pBuffer ) { m_pPluChangesBuffer = pBuffer; }
	void SetPluRecord ( CPluCSVRecord* pPluRecord ) { m_pPluRecord = pPluRecord; }
	void Refresh();
	bool UpdateRecord();

private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();
	void AddList( int nTargetBarcodeIdx );

private:
	CSSListCtrl m_listBarcodes;
	
private:
	CPluCSVRecord* m_pPluRecord;
	CPluChangesBuffer* m_pPluChangesBuffer;
	CArray<int,int> m_BarcodeIdxArray;
	bool m_bReadOnly;
};

/**********************************************************************/
#endif
/**********************************************************************/
