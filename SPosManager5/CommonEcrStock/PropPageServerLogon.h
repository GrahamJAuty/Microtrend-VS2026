#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "MiFareTerminalData.h"
#include "MyMifare.h"
#include "ServerCSVArray.h"
/**********************************************************************/
#include <string>
/**********************************************************************/

class CPropPageServerLogon : public CSSPropertyPage
{
public:
	CPropPageServerLogon();
	~CPropPageServerLogon();

public:
	void SetFullEditFlag(bool b) { m_bFullEdit = b; }

	//{{AFX_DATA(CPropPageServerLogon)
	enum { IDD = IDD_PROPPAGE_SERVER_LOGON };
	CEdit m_editNameReceipt;
	CEdit m_editNameFull;
	CSSListCtrlVirtual m_listLogon;
	CButton m_buttonNewCard;
	CButton m_buttonRemoveCard;
	CEdit m_editLogon;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPageServerLogon)
public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPageServerLogon)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonRemoveCard();
	afx_msg void OnButtonNewCard();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetServerRecord(CServerCSVRecord* pServerRecord) { m_pServerRecord = pServerRecord; }
	void Refresh();
	bool UpdateRecord();

private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();

private:
	bool ValidateMiFareNo(CString strMiFareNo, CString& strNumberError) const;

private:
	CMiFareTerminalData m_terminal;
	CMyMifare m_reader;

private:
	CArray<__int64, __int64> m_arrayLogons;
	CServerCSVRecord* m_pServerRecord = NULL;
	bool m_bFullEdit = TRUE;
	bool m_bReaderMode = FALSE;
};

/**********************************************************************/
#endif
/**********************************************************************/

