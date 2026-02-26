#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPropPagePMSOptionsQuickPlu : public CSSPropertyPage
{
public:
	CPropPagePMSOptionsQuickPlu();
	~CPropPagePMSOptionsQuickPlu();

	//{{AFX_DATA(CPropPageQuickPlu)
	enum { IDD = IDD_PROPPAGE_PMS_OPTIONS_QUICKPLU };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPagePMSOptionsQuickPlu)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropPagePMSOptionsQuickPlu)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnKillFocusPluNo1();
	afx_msg void OnKillFocusPluNo2();
	afx_msg void OnKillFocusPluNo3();
	afx_msg void OnKillFocusPluNo4();
	afx_msg void OnKillFocusPluNo5();
	afx_msg void OnKillFocusPluNo6();
	afx_msg void OnKillFocusPluNo7();
	afx_msg void OnKillFocusPluNo8();
	afx_msg void OnKillFocusPluNo9();
	afx_msg void OnKillFocusPluNo10();
	afx_msg void OnButtonBrowse1();
	afx_msg void OnButtonBrowse2();
	afx_msg void OnButtonBrowse3();
	afx_msg void OnButtonBrowse4();
	afx_msg void OnButtonBrowse5();
	afx_msg void OnButtonBrowse6();
	afx_msg void OnButtonBrowse7();
	afx_msg void OnButtonBrowse8();
	afx_msg void OnButtonBrowse9();
	afx_msg void OnButtonBrowse10();
	afx_msg void OnToggleEnable1();
	afx_msg void OnToggleEnable2();
	afx_msg void OnToggleEnable3();
	afx_msg void OnToggleEnable4();
	afx_msg void OnToggleEnable5();
	afx_msg void OnToggleEnable6();
	afx_msg void OnToggleEnable7();
	afx_msg void OnToggleEnable8();
	afx_msg void OnToggleEnable9();
	afx_msg void OnToggleEnable10();
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditPluNo( int n );

private:
	void ToggleEnable( int n );
	void KillFocusPluNo( int n );
	void BrowsePlu( int n );

private:
	void UpdateDescription( int n, __int64 nPluNo );
	void SaveRecord();

private:
	CPluFilterArray m_PluFilterArray;

private:
	int m_nEnableID[10];
	int m_nPluNoID[10];
	int m_nDescriptionID[10];
	int m_nQtyID[10];
	int m_nBrowseID[10];

private:
	CButton m_checkEnable[10];
	CEdit m_editDescription[10];
	CSSComboBox m_comboQty[10];
	CButton m_buttonBrowse[10];
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
