#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "PropPagePMSRoomMonitor.h"
/**********************************************************************/

class CPropertySheetPMSRoomMonitor : public CSSPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetPMSRoomMonitor)

public:
	CPropertySheetPMSRoomMonitor( CWnd* pWndParent = NULL);
	virtual ~CPropertySheetPMSRoomMonitor();

private:
	void AddRoomMonitorPage( CPropPagePMSRoomMonitor& page, CString& strTitle );

public:
	//{{AFX_VIRTUAL(CPropertySheetPMSRoomMonitor)
	protected:
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropertySheetPMSRoomMonitor)
	//}}AFX_MSG
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	CPropPagePMSRoomMonitor m_page1;
	CPropPagePMSRoomMonitor m_page2;
	CPropPagePMSRoomMonitor m_page3;
	CPropPagePMSRoomMonitor m_page4;
	CPropPagePMSRoomMonitor m_page5;
	CPropPagePMSRoomMonitor m_page6;

private:
	CString m_strTitle1;
	CString m_strTitle2;
	CString m_strTitle3;
	CString m_strTitle4;
	CString m_strTitle5;
	CString m_strTitle6;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
