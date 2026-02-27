#pragma once
//$$******************************************************************
#include "..\SmartPay4Shared\MembershipDatabase.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CCardPickerDlg : public CSSAutoShutdownDialog
{
public:
	CCardPickerDlg ( const char* szTitle, const char* szDestFilename, CArray<CSQLRowAccountList, CSQLRowAccountList>& SourceItems, CWnd* pParent = NULL );
	~CCardPickerDlg();

	enum { IDD = IDD_CARDPICKER };
	CSSTabbedComboBox m_tabcomboGroup;
	CSSListTaggedSelectCtrl m_listCardPicker;
	CSpinButtonCtrl m_spinCardPicker;
	CStatic m_staticCardNoText;
	CStatic m_staticReportOnBoxText;
	CButton m_buttonPrevious;
	CEdit m_editCardNo;
	CEdit m_editTotal;
	CEdit m_editCount;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonCardNo();
	afx_msg void OnSelectGroup();
	afx_msg void OnSpinCardPicker(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnButtonAll();
	afx_msg void OnButtonInvert();
	afx_msg void OnButtonNone();
	afx_msg void OnButtonImport();
	afx_msg void OnButtonPrevious();
	afx_msg void OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	LONG OnColumnClick(WPARAM wParam, LONG lParam);
	LONG OnToggleTickBox(WPARAM wParam, LONG lParam);
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	void UpdateCount();
	void UpdateTotal();
	void ReloadPrevious();
	void SaveColumns();

public:
	void DisablePreviousSelection(){ m_bNoPreviousSelection = TRUE; }
	int GetCount() { return m_nTaggedCount; }

private:
	void FillList();
	void ShowLine(CSQLRowAccountList& RowAccount, int nIndex);
	bool IsValidRecord(CSQLRowAccountList& RowAccount);
	int GetSelectedCount();
	CString GetAddedMessage ( int nCount );
	bool FindCardNo ( const char* szCardNo, int& nIndex );
	void EnableSpinFind ( int nCount );

private:
	void ImportViaMemberID();
	void ImportViaUserID();
	void ImportFromFile(int nImportType);

public:
	CString m_strBoxText;
	bool m_bValidAllowed;
	bool m_bExpiredAllowed;
	bool m_bInactiveAllowed;
	bool m_bAlertAllowed;

private:
	CString m_strTitle;
	CString m_strDestFilename;
	int m_nMaxCardNoLength;
	int m_nSingleGroupNo;
	int m_nColumnCount;

private:
	int m_nTaggedCount;

public:
	bool m_bMembershipList;
	bool m_bGroupsAllowed;

private:
	CArray<CSSListTaggedSelectItem, CSSListTaggedSelectItem> m_arrayListItems;
	CArray<CSQLRowAccountList, CSQLRowAccountList>& m_arraySourceItems;
	bool m_bNoPreviousSelection;
};

//$$******************************************************************
