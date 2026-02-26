#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "LoyaltySchemeCSVArray.h"
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPropPageLoyaltySchemeStaffLimits : public CSSPropertyPage
{
public:
	CPropPageLoyaltySchemeStaffLimits();
	~CPropPageLoyaltySchemeStaffLimits();

	//{{AFX_DATA(CPropPageLoyaltySchemeStaffLimits)
	enum { IDD = IDD_PROPPAGE_LOYALTYSCHEME_STAFFLIMITS };	
	CEdit	m_editName;
	CButton m_checkActive;
	CButton m_buttonCopy;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CPropPageLoyaltySchemeStaffLimits)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPageLoyaltySchemeStaffLimits)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCopyJan();
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditDay();
	CEdit* GetEditWeek();
	CEdit* GetEditMonth( int nMth );

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
	WORD m_wIDMonth[12];
};

/**********************************************************************/
#endif
/**********************************************************************/

