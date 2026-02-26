#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "LoyaltySchemeCSVArray.h"
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPropPageLoyaltySchemeGeneral : public CSSPropertyPage
{
public:
	CPropPageLoyaltySchemeGeneral();
	~CPropPageLoyaltySchemeGeneral();

	//{{AFX_DATA(CPropPageLoyaltySchemeGeneral)
	enum { IDD = IDD_PROPPAGE_LOYALTYSCHEME_GENERAL };	
	CEdit	m_editName;
	CButton m_checkActive;
	CEdit	m_editFormat;
	CEdit	m_editLower;
	CEdit	m_editUpper;
	CButton m_checkManual;
	CButton m_checkStaffCard;
	CButton m_checkStaffCredit;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CPropPageLoyaltySchemeGeneral)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	afx_msg void OnToggleStaffCard();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPageLoyaltySchemeGeneral)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetSchemeRecord ( CLoyaltySchemeCSVRecord* pSchemeRecord ) { m_pSchemeRecord = pSchemeRecord; }
	void Refresh();
	bool UpdateRecord();

private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();
		
private:
	CLoyaltySchemeCSVRecord* m_pSchemeRecord;
};

/**********************************************************************/
#endif
/**********************************************************************/

