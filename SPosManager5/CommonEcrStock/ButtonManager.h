#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ButtonEditDlg.h"
#include "ButtonInfo.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CButtonManager
{
public:
	CButtonManager(void);
	~CButtonManager(void);
	
public:
	void ReadButtonActions();
	void WriteButtonActions();

public:
	bool CheckButtonClick( CPoint& point, CButtonInfo& infoDisplay, int& nButtonIdx );
	
public:
	void CalculateButtonRects( bool bForce = FALSE );
	int GetButtonCount(){ return m_ButtonDisplay.GetSize(); }
	int GetCurrentListNo(){ return m_nCurrentListNo; }

public:
	void SetButtonManagerMode( int n );
	int GetButtonManagerMode(){ return m_nButtonManagerMode; }

public:
	const char* GetListName( int n );
	void SetListName( int n, const char* sz );

public:
	void LoadCurrentDisplayList();
	bool GetDisplayButton( int nDisplayLine, int nDisplayButton, CButtonInfo& infoDisplay );
	bool SetDisplayButton( int nDisplayLine, int nDisplayButton, CButtonInfo& infoDisplay );
	bool SetDisplayButton( int nButtonIdx, CButtonInfo& infoDisplay );
	bool SetLibraryButton( CButtonInfo& infoDisplay );
	void RememberButtonLibrary();
	void RestoreButtonLibrary();
	bool CheckButtonLibraryChanges();

public:
	void SetEditingButton( CButtonInfo& infoDisplay );
	void ClearEditingButton();
	bool IsEditingButton( CButtonInfo& infoDisplay );
	
public:
	bool GotoList( CButtonInfo& infoDisplay );
	bool JumpToList( CButtonInfo& infoDisplay );
	bool GoBack();
	void GoHome();

public:
	void SetButtonEditDlg( CButtonEditDlg* pDlg ){ m_pButtonEditDlg = pDlg; }
	CButtonEditDlg* GetButtonEditDlg(){ return m_pButtonEditDlg; }

public:
	void TempImageModeOn();
	void TempImageModeOff( bool bSave );
	void InvalidateImage( CButtonInfo& infoDisplay );
	void LoadImage( int nListNo, CButtonInfo& infoDisplay, CDC* pDC );
	bool GotDisplayableImage( CButtonInfo& infoDisplay );
	CBitmapInfo* GetBitmapInfo( CButtonInfo& infoDisplay );

private:
	void LoadDisplayListNormal();
	void LoadDisplayListSetupLayout();
	void LoadDisplayListSetupLists();

private:
	CArray<int,int> m_arrayListTrail;
	CArray<CButtonInfo> m_arrayParentTrail;
	CButtonInfo m_ButtonParent;
	bool m_bCalculatedRects;
	
private:
	CArray<CBitmapInfo*> m_arrayBitmaps;
	CArray<CButtonInfo> m_ButtonDisplay;

private:
	CReportConsolidationArray<CButtonInfo> m_ButtonLibrary;
	CReportConsolidationArray<CButtonInfo> m_ButtonLibrarySaved;
	int m_nCurrentListNo;
	int m_nButtonManagerMode;
	bool m_bTempImageMode;

private:
	bool m_bGotEditingButton;
	CButtonInfo m_ButtonEditing;

private:
	CStringArray m_arrayListNames;
	CStringArray m_arrayListNamesSaved;
	CString m_strListName;

private:
	CButtonEditDlg* m_pButtonEditDlg;
};

extern CButtonManager ButtonManager;

/**********************************************************************/
#endif
/**********************************************************************/
