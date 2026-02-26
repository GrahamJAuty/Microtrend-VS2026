#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPropPagePosTrayOptionsGeneral : public CSSPropertyPage
{
public:
	CPropPagePosTrayOptionsGeneral();
	~CPropPagePosTrayOptionsGeneral();

	//{{AFX_DATA(CPropPagePosTrayOptionsGeneral)
	enum { IDD = IDD_PROPPAGE_POSTRAY_OPTIONS_GENERAL };
	//}}AFX_DATA
	CButton m_checkStartUp;
	CButton m_checkRealTime;
	CButton m_checkProcessTran;
	CSSComboBox m_comboSpeed;
	CButton m_checkHibernate;
	CButton m_checkReportEpos;
	CButton m_checkReportHistory;
	CButton m_checkReportTimeAttend;
	CButton m_checkManageBackup;
	CButton m_checkExportSales;
	CButton m_checkExportPayment;
	CButton m_checkExportVoid;
	CButton m_checkExportDiscount;
	CButton m_checkFNB;
	CButton m_checkChartwells;
	CButton m_checkS4Labour;
	CButton m_checkFilename;
	CButton m_checkFieldname;
	CButton m_checkManual;
	CButton m_checkBusinessDate;
	CButton m_checkSkipEmpty;
	CButton m_checkSkipNoChange;
	CSSComboBox m_comboDate;

	//{{AFX_VIRTUAL(CPropPagePosTrayOptionsGeneral)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	void SetPropertySheet( void* p ){ m_pPropertySheet = p; }

protected:
	//{{AFX_MSG(CPropPagePosTrayOptionsGeneral)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnToggleTasks();
	DECLARE_MESSAGE_MAP()

private:
	bool CheckStartupLink();

private:
	void SaveRecord();
	void GetRecordData();

private:
	void* m_pPropertySheet;
};

/**********************************************************************/
#endif
/**********************************************************************/
