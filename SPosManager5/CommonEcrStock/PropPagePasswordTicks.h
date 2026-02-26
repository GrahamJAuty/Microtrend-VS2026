#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "Password.h"
/**********************************************************************/
#define PASSWORDTICKS_PAGE_FILE 0
#define PASSWORDTICKS_PAGE_SYSTEM 1
#define PASSWORDTICKS_PAGE_STOCKTAKE 2
#define PASSWORDTICKS_PAGE_TASKS 3
#define PASSWORDTICKS_PAGE_REPORTS 4
#define PASSWORDTICKS_PAGE_ADMIN 5
#define PASSWORDTICKS_PAGE_SETUP 6
#define PASSWORDTICKS_PAGE_SMARTCARD 7
#define PASSWORDTICKS_PAGE_PMS 8
/**********************************************************************/

class CPropPagePasswordTicks : public CPropertyPage
{
public:
	CPropPagePasswordTicks();
	~CPropPagePasswordTicks();

	void SetPageType ( int nType ) { m_nPageType = nType; }

	//{{AFX_DATA(CPropPagePasswordTicks)
	enum { IDD = IDD_PROPPAGE_PASSWORD_TICKS };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPagePasswordTicks)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnToggleTick(NMHDR* pNMHDR, LRESULT* pResult);
	
protected:
	//{{AFX_MSG(CPropPagePasswordTicks)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetPasswordRecord ( CPasswordCSVRecord* pPasswordRecord ) { m_pPasswordToEdit = pPasswordRecord; }
	void Refresh();
	bool UpdateRecord();
	
private:
	void SaveTicks();
	void SetTicks();
	void SetTickEnables();

private:
	CPasswordCSVRecord* m_pPasswordToEdit;
	CPasswordCSVRecord m_PasswordTemp;
	int m_nPageType; 

private:
	CSSListTaggedSelectCtrl m_listTick;
	CArray<CPasswordTickInfo, CPasswordTickInfo> m_arrayTicks;
};

/**********************************************************************/
#endif
/**********************************************************************/
