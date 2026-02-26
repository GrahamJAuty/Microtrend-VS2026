#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "MixMatchCSVArray.h"
/**********************************************************************/

class CPropPageMixMatchBuckets : public CSSPropertyPage
{
public:
	CPropPageMixMatchBuckets();
	~CPropPageMixMatchBuckets();

	//{{AFX_DATA(CPropPageMixMatchBuckets)
	enum { IDD = IDD_PROPPAGE_MIXMATCH_BUCKETS };
	CSSComboBox	m_comboOfferGroup5;
	CSSComboBox	m_comboOfferGroup4;
	CSSComboBox	m_comboOfferGroup3;
	CSSComboBox	m_comboOfferGroup2;
	CSSComboBox	m_comboOfferGroup1;
	CString	m_strDescription;
	BOOL	m_bEnable;
	int		m_nBucketA;
	int		m_nBucketB;
	int		m_nBucketC;
	int		m_nBucketD;
	int		m_nBucketE;
	double	m_dTrip;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CPropPageMixMatchBuckets)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPageMixMatchBuckets)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonEditGroup1();
	afx_msg void OnButtonEditGroup2();
	afx_msg void OnButtonEditGroup3();
	afx_msg void OnButtonEditGroup4();
	afx_msg void OnButtonEditGroup5();
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
	CEdit* GetEditTrip();
	CEdit* GetEditBucketA();
	CEdit* GetEditBucketB();
	CEdit* GetEditBucketC();
	CEdit* GetEditBucketD();
	CEdit* GetEditBucketE();
	
private:
	void EditOfferGroup( CSSComboBox& combo );

private:
	CMixMatchCSVRecord* m_pMixMatchRecord;
};

/**********************************************************************/
#endif
/**********************************************************************/
