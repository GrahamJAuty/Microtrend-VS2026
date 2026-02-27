#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SmartPay4Shared\SQLRowSetPebblePayment.h"
#include "..\SmartPay4Shared\SQLTable_PebbleConfig\SQLRepositoryPebbleConfig.h"
//*******************************************************************
#include "SingleListHelpers.h"
//*******************************************************************

class CSingleListContentHelpersPebblePayment : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersPebblePayment(CSQLRowSetPebblePayment& RowSet);

public:
	int GetArraySize();
	bool PrepareLine(int nIndex, CString& strLine);
	void DeleteDatabaseEntry(int nIndex);
	void SetColumns(CSSListCtrlVirtual& list, CWnd* pParent);

private:
	void FilterSelectedItemArray(CReportConsolidationArray<CSortedIntItem>& arraySelected);

private:
	CSQLRowSetPebblePayment& m_RowSet;
};

/**********************************************************************/

class CSetupPebbleDlg : public CSSAutoShutdownDialog
{
public:
	CSetupPebbleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetupPebbleDlg();

// Dialog Data
	enum { IDD = IDD_SETUP_PEBBLE };
	CEdit m_editInstallationID;
	CEdit m_editOrganisationID;
	CSSComboBox m_comboUPN;
	CSSComboBox m_comboPID;
	CDateTimeCtrl m_DatePickerFromTopup;
	CDateTimeCtrl m_TimePickerFromTopup;
	CDateTimeCtrl m_DatePickerFromSale;
	CDateTimeCtrl m_TimePickerFromSale;
	CButton m_checkClearRecent;
	CStatic m_staticRetainRecent;
	CButton m_checkClearPayment;
	CStatic m_staticRetainPayment;
	CEdit m_editOfficePLU;
	CSSComboBox m_comboOfficeVAT;
	CTabCtrl m_tabEPOS;
	CStatic m_staticVAT[12];
	CSSComboBox m_comboVATCode[12];
	CStatic m_staticVATNote[6];
	CSSListCtrlVirtual m_listPayment;
	CButton m_buttonPayAdd;
	CButton m_buttonPayEdit;
	CButton m_buttonPayDelete;

private:
	CSQLRowSetPebblePayment m_RowSetPayment;
	
protected:
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnTcnSelchangeTabs(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnToggleRecent();
	afx_msg void OnTogglePayment();
	afx_msg void OnKillFocusPaymentList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetFocusPaymentList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickPaymentList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoPaymentList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonPayAdd();
	afx_msg void OnButtonPayEdit();
	afx_msg void OnButtonPayDelete();
	afx_msg void OnSelectVATCode();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	virtual void CSSAutoShutdownPreInitDialog();
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	void SelectTab();
	CEdit* GetEditClearRecent() { return GetEdit(IDC_EDIT_CLEAR_RECENT); }
	CEdit* GetEditClearPayment() { return GetEdit(IDC_EDIT_CLEAR_PAYMENT); }
	CEdit* GetEditVAT(int n);
	
private:
	int GetPaymentArraySize();
	void SelectPaymentLine(int nIndex);
	bool PreparePaymentLine(int nIndex);
	int GetSafePaymentSelection(int nMin, int nMax);
	
private:
	CSQLRepositoryPebbleConfig m_config;
	CString m_strPaymentDisplayLine;

protected:
	CSingleListDisplayHelpers* m_pDisplayHelpers;
	CSingleListContentHelpers* m_pContentHelpers;
};

//*******************************************************************
