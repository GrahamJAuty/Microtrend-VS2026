#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ColourButton.h"
/**********************************************************************/

class CPropPagePMSRoomMonitor : public CPropertyPage
{
public:
	CPropPagePMSRoomMonitor();
	~CPropPagePMSRoomMonitor();

	void SetRoomIdxRange( int nStart, int nEnd );
	int GetRoomIdxStart(){ return m_nRoomIdxStart; }
	int GetRoomIdxEnd(){ return m_nRoomIdxEnd; }

	//{{AFX_DATA(CPropPagePMSRoomMonitor)
	enum { IDD = IDD_PROPPAGE_PMS_ROOM_MONITOR };
	//}}AFX_DATA
	CColourButtonPMS m_buttonRoom[15];
	
	//{{AFX_VIRTUAL(CPropPagePMSRoomMonitor)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();

private:
	void UpdateButtonGrid();
	void UpdateButton( int nButtonIdx );

private:
	void StartUpdateTimer();
	void KillUpdateTimer();

protected:
	//{{AFX_MSG(CPropPagePMSRoomMonitor)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT nIDEvent);
	
private:
	COleDateTime m_CheckBalanceTime;
	UINT m_hTimer;

private:
	DWORD m_nButtonID[15];
	bool m_bFiltered[15];

private:
	int m_nRoomIdxStart;
	int m_nRoomIdxEnd;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
