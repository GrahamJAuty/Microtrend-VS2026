#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ServerCSVArray.h"
#include "ServerRestrictionCSVArray.h"
/**********************************************************************/

class CPropPageServerRestriction : public CSSPropertyPage
{
public:
	CPropPageServerRestriction();
	~CPropPageServerRestriction();

public:
	void SetFullEditFlag(bool b) { m_bFullEdit = b; }

	//{{AFX_DATA(CPropPageServerRestriction)
	enum { IDD = IDD_PROPPAGE_SERVER_RESTRICTION };
	CEdit			m_editNameReceipt;
	CEdit			m_editNameFull;
	CStatic			m_staticLocation;
	CSSListTaggedSelectCtrl m_listTerminal;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPageServerRestriction)
public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPageServerRestriction)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAll();
	afx_msg void OnButtonNone();
	//}}AFX_MSG
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	DECLARE_MESSAGE_MAP()

public:
	void SetServerRecord(CServerCSVRecord* pServerRecord) { m_pServerRecord = pServerRecord; }
	void SetRestrictionRecord(CServerRestrictionCSVRecord* pRestrictionRecord) { m_pRestrictionRecord = pRestrictionRecord; }
	void Refresh();
	bool UpdateRecord();

private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();

private:
	CServerCSVRecord* m_pServerRecord;
	CServerRestrictionCSVRecord* m_pRestrictionRecord;
	bool m_bFullEdit;
};

/**********************************************************************/
#endif
/**********************************************************************/

