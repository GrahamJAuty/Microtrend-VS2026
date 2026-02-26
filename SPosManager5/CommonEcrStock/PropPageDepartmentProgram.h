#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DepartmentCSVArray.h"
/**********************************************************************/

class CPropPageDepartmentProgram : public CSSPropertyPage
{
public:
	CPropPageDepartmentProgram();
	~CPropPageDepartmentProgram();

	//{{AFX_DATA(CPropPageDepartmentProgram)
	enum { IDD = IDD_PROPPAGE_DEPARTMENT_PROGRAM };

	CSSComboBox m_comboTicketMode;
	int		m_nTickets;

	CButton	m_checkDPQty;
	CButton	m_checkNegative;
	CButton m_checkNonAdd;
	CStatic m_staticEffects;
	CStatic m_staticLoyalty;
	CStatic m_staticTrip;
	CButton m_radioPoints;
	CButton m_radioValue;
	CButton m_radioPercent;
	CButton m_checkGeneralBonus;

	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPageDepartmentProgram)
public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnRadioPoints();
	afx_msg void OnRadioValue();
	afx_msg void OnRadioPercent();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPageDepartmentProgram)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditTickets();
	CEdit* GetEditTrip();
	CEdit* GetEditPoints();
	CEdit* GetEditValue();
	CEdit* GetEditPercent();

public:
	void SetDepartmentRecord(CDepartmentCSVRecord* pDeptRecord) { m_pDeptRecord = pDeptRecord; }
	void Refresh();
	bool UpdateRecord();

private:
	bool SaveRecord();
	void GetRecordData();
	void SetRecordControls();

private:
	void UpdateBonusEditBoxes(int nType);

private:
	CDepartmentCSVRecord* m_pDeptRecord;
	bool m_bShowLoyalty;
	int m_nLoyaltyBonusType;
};

/**********************************************************************/
#endif
/**********************************************************************/

