#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/

class CPropPagePMSOptionsReports : public CSSPropertyPage
{
public:
	CPropPagePMSOptionsReports();
	~CPropPagePMSOptionsReports();

	void SetCanChangeDatabaseFlag( bool b ){ m_bCanChangeDatabase = b; }
	bool GetHasChangedDatabaseFlag(){ return m_bHasChangedDatabase; }

	//{{AFX_DATA(CPropPagePMSOptionsReports)
	enum { IDD = IDD_PROPPAGE_PMS_OPTIONS_REPORTS };
	//}}AFX_DATA
	CDateTimeCtrl m_TimePicker1;
	CDateTimeCtrl m_TimePicker2;
	CDateTimeCtrl m_TimePicker3;
	CButton m_checkFixedTime;
	CSSComboBox m_comboAccom;
	CEdit m_editDescription1;
	CEdit m_editDescription2;
	CEdit m_editDescription3;
	CEdit m_editDescription4;
	CEdit m_editDescription5;
	CEdit m_editDescription6;
	CEdit m_editDescription7;
	CEdit m_editDescription8;
	CButton m_buttonDefault;
	double m_dBreakfast;
	
private:
	void GetEditBoxes( int n, CEdit** pEditPlu, CEdit** pEditDescription1, CEdit** pEditDescription2S );
	CEdit* GetEditPluNo1();
	CEdit* GetEditPluNo2();
	CEdit* GetEditPluNo3();
	CEdit* GetEditPluNo4();
	CEdit* GetEditPluNo5();
	CEdit* GetEditPluNo6();
	CEdit* GetEditPluNo7();
	CEdit* GetEditBreakfast();
	
public:
	//{{AFX_VIRTUAL(CPropPagePMSOptionsReports)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	afx_msg void OnToggleFixedTime();
	afx_msg void OnKillFocusPluNo1();
	afx_msg void OnKillFocusPluNo2();
	afx_msg void OnKillFocusPluNo3();
	afx_msg void OnKillFocusPluNo4();
	afx_msg void OnKillFocusPluNo5();
	afx_msg void OnKillFocusPluNo6();
	afx_msg void OnKillFocusPluNo7();
	afx_msg void OnButtonBrowse1();
	afx_msg void OnButtonBrowse2();
	afx_msg void OnButtonBrowse3();
	afx_msg void OnButtonBrowse4();
	afx_msg void OnButtonBrowse5();
	afx_msg void OnButtonBrowse6();
	afx_msg void OnButtonBrowse7();
	afx_msg void OnButtonDefault();

private:
	void OnKillFocusPluNo( int n );
	void OnButtonBrowse( int n );
	
protected:
	//{{AFX_MSG(CPropPagePMSOptionsReports)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void UpdateDescription( __int64 nPluNo, int n );
	void SetInitialPluNo( __int64 nPluNo, int n );
	__int64 GetFinalPluNo( int n );

private:
	void SaveRecord();

private:
	CPluFilterArray m_PluFilterArray;

private:
	bool m_bCanChangeDatabase;
	bool m_bHasChangedDatabase;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
