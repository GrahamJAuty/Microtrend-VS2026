#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\PebbleHelpers.h"
//*******************************************************************
#include "..\SmartPay4Shared\SQLTable_PebbleConfig\SQLRepositoryPebbleConfig.h"
//*******************************************************************
#include "ImportStatsDlg.h"
//*******************************************************************

class CAccountLinkPebbleDlg : public CSSAutoShutdownDialog
{
public:
	CAccountLinkPebbleDlg ( CWnd* pParent = NULL);   // standard constructor
	~CAccountLinkPebbleDlg();
	
// Dialog Data
	enum { IDD = IDD_ACCOUNTLINK_PEBBLE };
	CButton m_buttonStart;
	CButton m_buttonImport;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnButtonStart();
	afx_msg void OnButtonImport();
	//afx_msg void OnButtonPurses();
	//afx_msg void OnButtonZero();
	afx_msg void OnOK();
	afx_msg void OnCancel();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	void SetDisplay();
	CString AccessPebble();
	CString ImportPebble();
	//bool CreateCreditPurses(CImportStatsDlgConfig& ImportStatsConfig);
	//void DeleteCreditPurses();
	
private:
	CWnd* m_pParent;
	CString m_strError;
	
private:
	CSQLRepositoryPebbleConfig m_configPebble;
	CArray<CPebbleMemberBuffer, CPebbleMemberBuffer> m_arrayPebbleMembers;
	bool m_bDonePreview;
	bool m_bDoneImport;
};

//*******************************************************************
