#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "..\CommonEcrStock\PresetReportBase.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/
#include "ListDataDlg.h"
/**********************************************************************/

class CSingleListContentHelpersPreset : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersPreset( int nNodeType, CPresetReportArray& arrayPresets );

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );

private:
	int m_nNodeType;
	CPresetReportArray& m_arrayPresets;
};

/**********************************************************************/

class CListDataPresetDlg : public CListDataDlg
{
public:
	CListDataPresetDlg( int nNodeType, int nInitialSel, CPresetReportArray& arrayPresets, CWnd* pParent = NULL); 
	~CListDataPresetDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	virtual void PrepareList();

	virtual UINT GetDefaultButtonID() { return IDC_BUTTON1; }
	virtual void HandleButton1();	//EDIT NAME
	virtual void HandleButton2();	//DELETE
	virtual void HandleButton4();	//MOVE UP
	virtual void HandleButton5();	//MOVE DOWN
	virtual void HandleDefaultButton() { HandleButton1(); }
	
private:
	CPresetReportArray& m_arrayPresets;
	int m_nNodeType;
	int m_nInitialSel;
};

/**********************************************************************/
#endif
/**********************************************************************/
