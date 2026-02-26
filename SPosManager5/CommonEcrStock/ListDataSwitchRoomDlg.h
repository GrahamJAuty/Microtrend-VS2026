#pragma once
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersSwitchRoom : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersSwitchRoom( CWordArray& arrayRoomIdx );

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex ){}
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent ){}

private:
	CWordArray& m_arrayRoomIdx;
};

/**********************************************************************/

class CListDataSwitchRoomDlg : public CListDataDlg
{
public:
	CListDataSwitchRoomDlg( const char* szRoomName, int nGuests, bool bTraining, CWordArray& arrayRoomIdx, CWnd* pParent = NULL); 
	~CListDataSwitchRoomDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	
	virtual UINT GetDefaultButtonID() { return IDOK; }
	virtual void HandleDefaultButton() { OnOK(); }
	virtual bool HandleOK();

public:
	int GetDestRoomIdx(){ return m_nDestRoomIdx; }

private:
	CString m_strRoomName;
	int m_nGuests;
	bool m_bTraining;
	CWordArray& m_arrayRoomIdx;
	int m_nDestRoomIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
