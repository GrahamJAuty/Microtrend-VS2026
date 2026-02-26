#pragma once
/**********************************************************************/
#ifndef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\EposTaskDatabase.h"
/**********************************************************************/

class CJobDownloadDlgEcrLite : public CSSDialog
{
public:
	CJobDownloadDlgEcrLite( CEposTaskDatabase& TaskDatabase, CWnd* pParent = NULL);  

	//{{AFX_DATA(CJobDownloadDlgEcrLite)
	enum { IDD = IDD_JOB_DOWNLOAD_ECRLITE };
	CButton	m_checkPlu;
	CButton	m_checkBarcode;
	CButton	m_checkModifier;
	CButton	m_checkCategory;
	CButton	m_checkAllergen;
	CButton	m_checkLoyaltyScheme;
	CButton	m_checkDepartment;
	CButton	m_checkGroup;
	CButton m_checkTax;
	CButton m_checkPayment;
	CButton	m_checkKeyboard;
	CButton	m_checkImages;
	int		m_nKbNo;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CJobDownloadDlgEcrLite)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CJobDownloadDlgEcrLite)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnToggleKeyboard();
	DECLARE_MESSAGE_MAP()

private:
	void AddJobsToTask();
	void ProcessTick( CButton& tick, bool bShow, int& nNextYPos );

private:
	CEdit* GetEditKeyboard();

private:
	CEposTaskDatabase& m_TaskDatabase;
};

/**********************************************************************/
#endif
/**********************************************************************/

