#pragma once
//$$******************************************************************
#include "..\SmartPay4Shared\BackgroundOptions.h"
#include "..\SmartPay4Shared\EODDefines.h"
#include "..\SmartPay4Shared\ReportEndOfDay.h"
//$$******************************************************************
#include "SmartPay4Manager.h"
//$$******************************************************************


class CEODListDlg : public CSSAutoShutdownDialog
{
public:
	CEODListDlg( CWnd* pParent = NULL);
	virtual ~CEODListDlg();

	enum { IDD = IDD_ENDOFDAY };
	
private:
	CSSListCtrl m_listboxDate;
	CComboBox m_comboDateRange;
	CButton m_buttonAltKey1;
	CButton m_buttonAltKey2;
	CButton m_buttonConfirm;
	CButton m_buttonView;
	CButton m_buttonRegenerate;
	CButton m_buttonPrint;
	CButton m_checkStart;
	CStatic m_staticEODBoxText;
	CTime m_ctimeFrom;
	CTime m_ctimeTo;
	CString m_strDateRange;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	virtual void OnOK();
	afx_msg void OnButtonAltKey1();
	afx_msg void OnButtonAltKey2();
	afx_msg void OnToggleStart();
	afx_msg void OnButtonView();
	afx_msg void OnButtonPrint();
	afx_msg void OnButtonConsolidate();
	afx_msg void OnButtonConfirm();
	afx_msg void OnButtonRegenerate();
	afx_msg void OnSelectDateRange();
	afx_msg void OnDoubleClickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRightDoubleClickList(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

public:
	int GetReprintDateCount() { return m_arrayReprintDates.GetCount(); }
	CString GetReprintDate(int n);

private:
	virtual void CSSAutoShutdownPreInitDialog();
	virtual BOOL CSSAutoShutdownPostInitDialog();

public:
	int m_nEODType;
	CString m_strReprintFilename;
	CString m_strKeyDateFrom;
	CString m_strKeyDateTo;
	CString GetDateString();

private:
	void SetDisplay();
	void SelectEODReport();
	void ShowNoSalesReport ( const char* szKey );
	void SetReprintDate(CString strDate);
	void AddReprintDate(CString strDate);
	void CloseAsOK();

private:
	CBackgroundOptions m_options;

	int m_nListCurSel;
	int m_nConsolCurSel;
	int m_nShowRegenerate;

	CString m_strKeyDateToday;
	CString m_strDisplayDateToday;
	bool m_bTodayListed;

private:
	CArray<CEODListInfo, CEODListInfo> m_arrayEODListInfo;
	CStringArray m_arrayReprintDates;
};

//$$******************************************************************
