#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "CascadingMacroSelector.h"
#include "LocationSelector.h"
#include "ServerCSVArray.h"
/**********************************************************************/

class CPropPageServerGeneral : public CSSPropertyPage
{
public:
	CPropPageServerGeneral();
	~CPropPageServerGeneral();

	//{{AFX_DATA(CPropPageServerGeneral)
	enum { IDD = IDD_PROPPAGE_SERVER_GENERAL };
	CEdit		m_editNameReceipt;
	CEdit		m_editNameFull;
	CButton		m_checkTraining;
	CEdit		m_editMacro;
	CSSComboBox m_comboMacro;
	CEdit		m_editPassword;
	CSSComboBox	m_comboDrawer;
	CSSComboBox	m_comboView;
	CSSComboBox m_comboOpenTable;
	CStatic		m_staticLocation;
	CSSComboBoxEx m_comboLocation;
	CButton m_checkMidnightShift;
	
	CStatic		m_static1;
	CStatic		m_static2;
	CStatic		m_static3;
	CStatic		m_static4;
	CStatic		m_static5;
	CStatic		m_static6;
	CStatic		m_staticHourlyRate;

	CString		m_strPassword;
	int			m_nAuthority;
	double		m_dHourlyRate;
	int			m_nDailyHours;
	int			m_nColourScheme;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CPropPageServerGeneral)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropPageServerGeneral)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnKillFocusMacro();
	afx_msg void OnSelectMacro();
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditAuthority();
	CEdit* GetEditHourlyRate();
	CEdit* GetEditDailyHours();
	CEdit* GetEditColourScheme();
	
public:
	void SetServerRecord ( CServerCSVRecord* pServerRecord ) { m_pServerRecord = pServerRecord; }
	void SetFullEditFlag( bool b ){ m_bFullEdit = b; }
	void SetServerDbIdx(int n) { m_nServerDbIdx = n; }
	void SetServerLocIdx(int n) { m_nServerLocIdx = n; }
	void Refresh();
	bool UpdateRecord();

private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();

private:
	CServerCSVRecord* m_pServerRecord;
	CCascadingMacroSelector m_MacroSelector;
	CLocationSelector m_LocationSelector;

private:
	bool m_bFullEdit;
	int m_nMacroNo;
	int m_nServerDbIdx;
	int m_nServerLocIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/

