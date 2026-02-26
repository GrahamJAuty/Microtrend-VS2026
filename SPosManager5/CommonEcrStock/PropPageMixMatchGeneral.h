#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/
#include "MixMatchCSVArray.h"
/**********************************************************************/

class CPropPageMixMatchGeneral : public CSSPropertyPage
{
public:
	CPropPageMixMatchGeneral();
	~CPropPageMixMatchGeneral();

	//{{AFX_DATA(CPropPageMixMatchGeneral)
	enum { IDD = IDD_PROPPAGE_MIXMATCH_GENERAL };
	CButton	m_buttonEditPromo;
	CButton	m_buttonEditPlu;
	CButton	m_buttonBrowse;
	CSSComboBox	m_comboPromo;
	CSSComboBox	m_comboBucket;
	CSSComboBox m_comboOrderMode;
	CEdit	m_editPluDesc;
	CEdit	m_editPluPrice;
	CStatic	m_staticPluNo3;
	CStatic	m_staticPluNo2;
	CStatic	m_staticPluNo;
	CStatic	m_staticPriceBand;
	CSSComboBox	m_comboPriceBand;
	CSSComboBox	m_comboAction;
	CSSComboBox m_comboActivation;
	CDateTimeCtrl	m_DatePickerStart;
	CDateTimeCtrl	m_DatePickerEnd;
	CString	m_strDescription;
	BOOL	m_bEnable;
	CString	m_strPluNo;
	CString m_strCost;
	BOOL	m_bAutoLock;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CPropPageMixMatchGeneral)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPageMixMatchGeneral)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectAction();
	afx_msg void OnSelectBucket();
	afx_msg void OnSelectPromo();
	afx_msg void OnButtonBrowse();
	afx_msg void OnButtonEditPlu();
	afx_msg void OnButtonEditPromo();
	afx_msg void OnKillFocusPluNo();
	afx_msg void OnKillFocusCost();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetMixMatchRecord ( CMixMatchCSVRecord* pMixMatchRecord ) { m_pMixMatchRecord = pMixMatchRecord; }
	void Refresh();
	bool UpdateRecord();

private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();
	
private:
	CEdit* GetEditPluNo();
	CEdit* GetEditCost();

private:
	void UpdatePluDetails( CString& strPluNo );
	void FillPromoCombo( int nPromoNo );
	void FillBucketComboFreeItem( int nBucketNo );
	void FillBucketComboFreeBucket( int nBucketNo );
	void KillFocusPluNo( bool bEdit );

private:
	CMixMatchCSVRecord* m_pMixMatchRecord;
	CPluFilterArray m_FilterArray;

private:
	int m_nLastPromoNo;
	int m_nLastBucketNoFreeItem;
	int m_nLastBucketNoFreeBucket;
	int m_nLastBucketNoFixedBucket;
	int m_nUseGate;
	CString m_strLastPluNo;
	CString m_strLastCost;
};

/**********************************************************************/
#endif
/**********************************************************************/
