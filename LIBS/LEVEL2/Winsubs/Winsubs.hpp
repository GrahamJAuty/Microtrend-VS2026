/**********************************************************************/
#pragma once
/**********************************************************************/

#include <afxdlgs.h>
#include <afxcmn.h>
#include <afxdisp.h>
#include <afxpriv.h>
#include <atlimage.h>
#include <direct.h>

/**********************************************************************/

#define	SS_NUM		1
#define	SS_NUM_S		2
#define	SS_NUM_DP		3
#define	SS_NUM_SDP	4
#define	SS_DATE		5

#define	SS_BLANK_NUM		(0x80+1)
#define	SS_BLANK_NUM_S		(0x80+2)
#define	SS_BLANK_NUM_DP	(0x80+3)
#define	SS_BLANK_NUM_SDP	(0x80+4)

/**********************************************************************/

//MERGED IN FROM CMySSDialog FOR V1.36
struct CSSDialogItem
{
	CWnd* m_pWnd;
	int m_nXPos;
	int m_nYPos;
	bool m_bAlwaysShow;
};

/**********************************************************************/

//MERGED IN FROM CMySSDialog FOR V1.36
struct CSSTabFixItem
{
public:
	CWnd* m_pWnd;
	bool m_bShow;
	bool m_bEnable;
};

/**********************************************************************/

class CControlContentHelpers
{
public:
	bool IsTicked(CButton& button);
	const char* GetEditBoxText(CEdit& edit);
	int GetEditBoxInt(CEdit& edit);
	void SetEditBoxInt(CEdit& edit, int n);
	__int64 GetEditBoxInt64(CEdit& edit);
	void SetEditBoxInt64(CEdit& edit, __int64 n);
	double GetEditBoxDouble(CEdit& edit);
	void SetEditBoxDouble(CEdit& edit, double d, int nDPs);
	const char* GetComboBoxText(CComboBox& combo);

private:
	CString m_strEditBoxText;
	CString m_strComboBoxText;
};

/**********************************************************************/

struct CSSAutoShutdownHelper 
{
public:
	CSSAutoShutdownHelper();
	void Terminate();

public:
	void Init(ULONGLONG nMinutes);
	void ResetShutdownTime();
	void Trigger() { m_bTriggered = TRUE; }

public:
	void SetFinalShutdownSeconds(ULONGLONG nSeconds);

public:
	bool IsEnabled() { return m_bEnabled;  }
	bool IsTriggered();
	bool IsFinalShutdownMode() { return m_bFinalShutdownMode; }

public:
	//SHUTDOWN REQUEST MARKER FOR EXTERNAL USE
	//ALLOWS EXTERNAL CODE TO DETECT WHEN SHUTDOWN REQUEST HAS BEEN TRIGGERED
	//OTHER SHUTDOWN FLAGS MAY BE CLEARED DURING SHUTDOWN PROCESS
	void ClearShutdownRequestMarker() { m_bShutdownRequestMarker = FALSE;  }
	bool GetShutdownRequestMarker() { return m_bShutdownRequestMarker;  }

public:
	__int64 GetSecondsToShutdown();
	__int64 GetIdleMinutesAllowed();

public:
	void SetTimerBlock1( bool b ) { m_bTimerBlock1 = b;  }
	void SetTimerBlock2( bool b ) { m_bTimerBlock2 = b; }

public:
	bool IsTimerBlocked() { return (m_bTimerBlock1 || m_bTimerBlock2); }
	
public:
	void SetActiveWindow(CWnd* pWnd);
	CWnd* GetActiveWindow() { return m_pActiveWindow;  }

private:
	ULONGLONG m_nShutdownTime;
	ULONGLONG m_nIdleMinutesAllowed;

private:
	CWnd* m_pActiveWindow;
	bool m_bTimerBlock1;
	bool m_bTimerBlock2;
	bool m_bEnabled;
	bool m_bDoneInit;
	bool m_bTriggered;
	bool m_bFinalShutdownMode;
	bool m_bShutdownRequestMarker;
};

/**********************************************************************/
/*														*/
/* If using SubclassEdit, don't use DDX_Control on the same control	*/
/*														*/
/**********************************************************************/

class CSSDialog : public CDialog
{
public:
	CSSDialog();
	CSSDialog ( UINT nIDTemplate, CWnd* pParentWnd = NULL );
	CSSDialog ( LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL );
	~CSSDialog();

	bool UpdateData ( bool bSave=TRUE, bool bValidate=TRUE );
	void SubclassEdit ( int nIDC, int nType, int nWidth, const char* szFmt );
	void SubclassEdit ( int nIDC, int nType, int nWidth, const char* szFmt, int n1 );
	void SubclassEdit ( int nIDC, int nType, int nWidth, const char* szFmt, int n1, int n2 );

	CEdit* GetEdit ( int nIDC ) { return (CEdit*) GetDlgItem(nIDC); }

protected:
	void SetText ( HWND hWndCtrl, int nIDC, const char* szBuf );

protected:	// standard DDXs
	void AFXAPI DDX_Text ( CDataExchange* pDX, int nIDC, CString& value );
	void AFXAPI DDX_Text ( CDataExchange* pDX, int nIDC, int& value );
	void AFXAPI DDX_Text ( CDataExchange* pDX, int nIDC, __int64& value);
	void AFXAPI DDX_Text ( CDataExchange* pDX, int nIDC, long& value );
	void AFXAPI DDX_Text ( CDataExchange* pDX, int nIDC, double& value );

protected:	// standard DDVs
	void AFXAPI DDV_MaxChars ( CDataExchange* pDX, CString const& value, int nChars );
	void AFXAPI DDV_MinMaxInt ( CDataExchange* pDX, int value, int minVal, int maxVal );
	void AFXAPI DDV_MinMaxLong ( CDataExchange* pDX, long value, long minVal, long maxVal );
	void AFXAPI DDV_MinMaxDouble ( CDataExchange* pDX, double const& value, double minVal, double maxVal );

protected:	// sharpsoft DDXs
	void AFXAPI DDX_IntegerString ( CDataExchange* pDX, int nIDC, CString& strValue );

protected:	// sharpsoft DDVs
	void AFXAPI DDV_IntegerString ( CDataExchange* pDX, CString const& strValue, CString const& strLow, CString const& strHigh );

public:
	//MERGED IN FROM CMySSDialog FOR V1.36
	void MoveControl ( CWnd* pWnd, int XPos, int nYPos );
	void MoveControl ( CWnd* pWnd, int XPos, int nYPos, int nWidth, int nHeight );
	void ResizeControl( CWnd* pWnd, int nWidth, int nHeight );
	void ResizeDialog( int nWidth, int nHeight );
	void ShowAndEnableWindow( CWnd* pWnd, bool bShow, bool bEnable = TRUE );

public:
	//MERGED IN FROM CMySSDialog FOR V1.36
	void AddDialogItem( CWnd* pWnd, int nXPos, int nYPos, bool bAlwaysShow = FALSE );
	void ClearTabFixBuffer();
	void AddTabFixItem( CWnd* pWnd, bool bTab = TRUE, bool bShow = TRUE, bool bEnable = TRUE );
	void ProcessTabFixBuffer( CWnd* pParent );

public:
	bool IsTicked( CButton& button );
	const char* GetEditBoxText( CEdit& edit );
	int GetEditBoxInt( CEdit& edit );
	void SetEditBoxInt( CEdit& edit, int n );
	__int64 GetEditBoxInt64(CEdit& edit);
	void SetEditBoxInt64(CEdit& edit, __int64 n);
	double GetEditBoxDouble(CEdit& edit);
	void SetEditBoxDouble(CEdit& edit, double d, int nDPs);
	const char* GetComboBoxText(CComboBox& combo);

protected:
	//MERGED IN FROM CMySSDialog FOR V1.36
	CArray<CSSDialogItem,CSSDialogItem> m_arrayDialogItems;
	CArray<CSSTabFixItem,CSSTabFixItem> m_arrayTabFixItems;

protected:
	CMapWordToPtr m_FilterArray;
	bool m_bValidate;

protected:
	CControlContentHelpers m_ControlContentHelpers;
};


/**********************************************************************/

class CSSAutoShutdownDialog : public CSSDialog
{
public:
	CSSAutoShutdownDialog(CSSAutoShutdownHelper& AutoShutdownHelper);
	CSSAutoShutdownDialog(CSSAutoShutdownHelper& AutoShutdownHelper, UINT nIDTemplate, CWnd* pParentWnd = NULL);
	CSSAutoShutdownDialog(CSSAutoShutdownHelper& AutoShutdownHelper, LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);

protected:
	virtual bool UpdateData(bool bSave = TRUE, bool bValidate = TRUE);

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	virtual void PostNcDestroy();
	DECLARE_MESSAGE_MAP()

protected:
	virtual void CSSAutoShutdownPreInitDialog() {}
	virtual BOOL CSSAutoShutdownPostInitDialog() { return TRUE; }
	virtual void CSSAutoShutdownTimer(UINT nIDEvent) {}
	virtual void CSSAutoShutdownDestroy() {}

public:
	virtual void CSSAutoShutdownLogOff() { EndDialog(IDCANCEL);  }

protected:
	CSSAutoShutdownHelper& m_AutoShutdownHelper;

private:
	UINT m_hAutoShutdownTimer;
	CWnd* m_pPreviousShutdownWindow;
};

/**********************************************************************/

class CSSAutoShutdownPropertySheet : public CPropertySheet
{
public:
	CSSAutoShutdownPropertySheet(CSSAutoShutdownHelper& AutoShutdownHelper, const char* szTitle, CWnd* pWndParent, UINT nFirstPage = 0 );

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	virtual void PostNcDestroy();
	DECLARE_MESSAGE_MAP()

protected:
	virtual void CSSAutoShutdownPreInitDialog() {}
	virtual BOOL CSSAutoShutdownPostInitDialog() { return TRUE; }
	virtual void CSSAutoShutdownTimer(UINT nIDEvent) {}
	virtual void CSSAutoShutdownDestroy() {}

public:
	virtual void CSSAutoShutdownLogOff() { EndDialog(IDCANCEL); }

protected:
	CSSAutoShutdownHelper& m_AutoShutdownHelper;

private:
	UINT m_hAutoShutdownTimer;
	CWnd* m_pPreviousShutdownWindow;

	friend class CSSAutoShutdownPropertyPage;
};

/**********************************************************************/
/*														*/
/* If using SubclassEdit, don't use DDX_Control on the same control	*/
/*														*/
/**********************************************************************/

class CSSPropertyPage : public CPropertyPage
{
public:
	CSSPropertyPage( );
	CSSPropertyPage( UINT nIDTemplate, UINT nIDCaption = 0 );
	CSSPropertyPage( LPCTSTR lpszTemplateName, UINT nIDCaption = 0 );
	~CSSPropertyPage();

	bool UpdateData ( bool bSave=TRUE, bool bValidate=TRUE );
	void SubclassEdit ( int nIDC, int nType, int nWidth, const char* szFmt );
	void SubclassEdit ( int nIDC, int nType, int nWidth, const char* szFmt, int n1 );
	void SubclassEdit ( int nIDC, int nType, int nWidth, const char* szFmt, int n1, int n2 );

	CEdit* GetEdit ( int nIDC ) { return (CEdit*) GetDlgItem(nIDC); }

protected:
	void SetText ( HWND hWndCtrl, int nIDC, const char* szBuf );

protected:	// standard DDXs
	void AFXAPI DDX_Text ( CDataExchange* pDX, int nIDC, CString& value );
	void AFXAPI DDX_Text ( CDataExchange* pDX, int nIDC, int& value );
	void AFXAPI DDX_Text ( CDataExchange* pDX, int nIDC, long& value );
	void AFXAPI DDX_Text ( CDataExchange* pDX, int nIDC, double& value );

protected:	// standard DDVs
	void AFXAPI DDV_MaxChars ( CDataExchange* pDX, CString const& value, int nChars );
	void AFXAPI DDV_MinMaxInt ( CDataExchange* pDX, int value, int minVal, int maxVal );
	void AFXAPI DDV_MinMaxLong ( CDataExchange* pDX, long value, long minVal, long maxVal );
	void AFXAPI DDV_MinMaxDouble ( CDataExchange* pDX, double const& value, double minVal, double maxVal );

protected:	// sharpsoft DDXs
	void AFXAPI DDX_IntegerString ( CDataExchange* pDX, int nIDC, CString& strValue );

protected:	// sharpsoft DDVs
	void AFXAPI DDV_IntegerString ( CDataExchange* pDX, CString const& strValue, CString const& strLow, CString const& strHigh );

public:
	//MERGED IN FROM CMySSPropertyPage FOR V1.36
	void MoveControl ( CWnd* pWnd, int XPos, int nYPos, int nWidth, int nHeight );
	void MoveControl ( CWnd* pWnd, int XPos, int nYPos );
	void ResizeControl( CWnd* pWnd, int nWidth, int nHeight );
	void ShowAndEnableWindow( CWnd* pWnd, bool bShow, bool bEnable = TRUE );
	void ResizeDialog( int nWidth, int nHeight );

public:
	bool IsTicked( CButton& button );
	const char* GetEditBoxText( CEdit& edit );
	int GetEditBoxInt( CEdit& edit );
	void SetEditBoxInt( CEdit& edit, int n );
	__int64 GetEditBoxInt64(CEdit& edit);
	void SetEditBoxInt64(CEdit& edit, __int64 n);
	double GetEditBoxDouble(CEdit& edit);
	void SetEditBoxDouble(CEdit& edit, double d, int nDPs);
	const char* GetComboBoxText(CComboBox& combo);

protected:
	CMapWordToPtr m_FilterArray;
	bool m_bValidate;

protected:
	CControlContentHelpers m_ControlContentHelpers;
};

/**********************************************************************/

class CSSAutoShutdownPropertyPage : public CSSPropertyPage
{
public:
	CSSAutoShutdownPropertyPage(CSSAutoShutdownHelper& AutoShutdownHelper);
	CSSAutoShutdownPropertyPage(CSSAutoShutdownHelper& AutoShutdownHelper, UINT nIDTemplate, UINT nIDCaption = 0);
	CSSAutoShutdownPropertyPage(CSSAutoShutdownHelper& AutoShutdownHelper, LPCTSTR lpszTemplateName, UINT nIDCaption = 0);

protected:
	virtual bool UpdateData(bool bSave = TRUE, bool bValidate = TRUE);

protected:
	CSSAutoShutdownHelper& m_AutoShutdownHelper;
};

/**********************************************************************/
/*	Private class for use by CSSDialog and CSSPropertyPage only		*/
/**********************************************************************/

class CEditFilter : public CEdit
{
public:
	CEditFilter ( int nType, const char* szFmt );
	void SetFormat ( int nType, const char* szFmt );
	CString Format ( const char* szBuf );
	void FormatWindowText();

protected:
	bool ValidateNumber ( UINT nChar, UINT nRepCnt, UINT nFlags );
	bool ValidateNumberAfterPaste( CString& strNum );

protected:
	afx_msg void OnChar ( UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnKillFocus ( CWnd* pNewWnd );
	afx_msg LRESULT OnPaste( WPARAM, LPARAM );

	DECLARE_MESSAGE_MAP()

private:
	int m_nType;
	int m_nNumType;
	CString m_strFmt;
	bool m_bAllowBlank;
};

/**********************************************************************/
/*	SSLISTCTRLVIRTUAL.CPP									*/
/**********************************************************************/
/*														*/
/*	dialog properties:	need to tick "Owner data"				*/
/*	dialog properties:	need to tick "Owner draw fixed" for colour	*/
/*														*/
/*	If "Owner draw fixed" not ticked, then						*/
/*	SetExtendedStyle ( LVS_EX_FULLROWSELECT ) can be used to		*/
/*	highlight the whole line when an item is selected				*/
/*														*/
/**********************************************************************/
/*														*/
/*	N.B. the LVN_GETDISPINFO message must be processed - see below	*/
/*														*/
/*	If caching data use LVN_ODCACHEHINT message to reset the cache	*/
/*														*/
/**********************************************************************/
/*
static int nCacheIndex = -1;		// better if class member rather than static
static CCSV csvCache ( '\t' );	// better if class member rather than static

void CXXX::OnGetDispInfoList ( NMHDR* pNMHDR, LRESULT* pResult )
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*) pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;

		if ( nIndex != nCacheIndex )
		{
			csvCache.ParseLine ( GETDATA(nIndex) );
			nCacheIndex = nIndex;
		}

		strcpy ( pDispInfo->item.pszText,	csvCache.GetString(pDispInfo->item.iSubItem));
	}
	*pResult = 0;
}

void CXXX::OnOdCacheHintList ( NMHDR* pNMHDR, LRESULT* pResult ) 
{
	NMLVCACHEHINT* pCacheHint = (NMLVCACHEHINT*)pNMHDR;
	
	nCacheIndex = -1;		// reset cache index

	*pResult = 0;
}
*/
/**********************************************************************/

class CSSListCtrlVirtual : public CListCtrl
{
public:
	CSSListCtrlVirtual();
	void SetItemCountEx ( int nCount, DWORD dwFlags=LVSICF_NOINVALIDATEALL );
	void EnableColour();
	void SetColour ( int nIndex, int nColour );
	void SetColour ( int nIndex, COLORREF crColour );
	int SetCurSel ( int nSelect );
	int GetCurSel();

public:
	void GetBoldFont(CDC* pDC, CFont& Font);
	void SetBoldMode(bool b) { m_bBoldMode = b; }
	void SetFixedBackgroundColour(int n) { m_nFixedBackgroundColour = n; }
	void SetFixedForegroundColour(int n) { m_nFixedForegroundColour = n; }

public:
	void AdjustRowHeight( LPMEASUREITEMSTRUCT lpMeasureItemStruct, int nAdjust );
	void ClearRowHeightFlag(){ m_bAdjustedRowHeight = FALSE; }

public:
	virtual ~CSSListCtrlVirtual();

protected:
	virtual void DrawItem ( LPDRAWITEMSTRUCT lpDrawItemStruct );
	DWORD GetColour ( int nIndex );

protected:
	afx_msg BOOL OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	//MERGED IN FROM CMySSListCtrlVirtual FOR V1.36
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnRButtonDown(UINT nFlags, CPoint point);
	void OnLButtonDblClk(UINT nFlags, CPoint point);
	void OnRButtonDblClk(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

private:
	int m_nCurSel;
	bool m_bEnableColour;
	bool m_bAdjustedRowHeight;
	CDWordArray m_crColourArray;

private:
	bool m_bBoldMode;
	int m_nFixedBackgroundColour;
	int m_nFixedForegroundColour;
};

/**********************************************************************/
/*	SSLISTCTRL.CPP											*/
/**********************************************************************/
/*														*/
/*	dialog properties:	need to untick "Owner data"				*/
/*	dialog properties:	need to tick "Owner draw fixed" for colour	*/
/*														*/
/*	If "Owner draw fixed" not ticked, then						*/
/*	SetExtendedStyle ( LVS_EX_FULLROWSELECT ) can be used to		*/
/*	highlight the whole line when an item is selected				*/
/*														*/
/**********************************************************************/

class CSSListCtrl : public CListCtrl
{
public:
	CSSListCtrl();
	void EnableColour();
	int AddString ( const char* szText, int nColour=0 );
	int AddString ( const char* szText, COLORREF crColour );
	int InsertString ( int nIndex, const char* szText, int nColour=0 );
	int InsertString ( int nIndex, const char* szText, COLORREF crColour );
	int DeleteString ( int nIndex );
	void SetColour ( int nIndex, int nColour );
	void SetColour ( int nIndex, COLORREF crColour );
	int SetCurSel ( int nSelect );
	int GetCurSel();
	bool DeleteAllItems();

public:
	void AdjustRowHeight( LPMEASUREITEMSTRUCT lpMeasureItemStruct, int nAdjust );
	void ClearRowHeightFlag(){ m_bAdjustedRowHeight = FALSE; }

public:
	virtual ~CSSListCtrl();

protected:
	virtual void DrawItem ( LPDRAWITEMSTRUCT lpDrawItemStruct );

protected:
	afx_msg BOOL OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	//MERGED IN FROM CMySSListCtrl FOR V1.36
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnRButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual void OnRButtonDblClk(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

private:
	int m_nCurSel;
	bool m_bEnableColour;
	bool m_bAdjustedRowHeight;
};

/**********************************************************************/

class CSSListMultiSelectCtrl : public CSSListCtrl
{
public:
	CSSListMultiSelectCtrl();

public:
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnRButtonDown(UINT nFlags, CPoint point);
	void OnLButtonDblClk(UINT nFlags, CPoint point);
	void OnRButtonDblClk(UINT nFlags, CPoint point);

protected:
	void ClickHandler(UINT nFlags, CPoint point, bool bPassThrough);

protected:
	virtual void HandleLeftDoubleClick( UINT nFlags, CPoint point );
};

/**********************************************************************/

class CSSListMultiSelectCtrlDblClick : public CSSListMultiSelectCtrl
{
protected:
	virtual void HandleLeftDoubleClick( UINT nFlags, CPoint point );
};

/**********************************************************************/
/**********************************************************************/
#define TAGSTATETYPE_2STATEGREEN 0
#define TAGSTATETYPE_2STATEBLUE 1
#define TAGSTATETYPE_3STATEBLUEGREEN 2
/**********************************************************************/
/**********************************************************************/

struct CSSListTaggedSelectItem
{
public:
	CSSListTaggedSelectItem( int nTagStateType = TAGSTATETYPE_2STATEGREEN );
	
public:
	int GetTagStateType(){ return m_nTagStateType; }
	int GetTagState(){ return m_nTagState; }

public:
	void SetTagState( int nState );
	void NextTagState();

public:
	bool m_bLocked;
	CString m_strText;
	int m_nData;
	
private:
	int m_nTagStateType;
	int m_nTagState;
};

/**********************************************************************/

class CSSListTaggedSelectCtrl : public CSSListCtrl
{
public:
	CSSListTaggedSelectCtrl();
	void SetLockItemZeroFlag( bool b ){ m_bLockItemZero = b; }
	void SetMultiColumnsFlag(bool b) { m_bMultiColumns = b; }
	bool GetTagStateChangedFlag(){ return m_bTagStateChanged; }
	void SetTagStateNotifyMessage(CWnd* pWnd, int nMessage);
	int GetTaggedItemCount() { return m_nTaggedItemCount;  }
	
public:
	void ClearList();
	void AddItemToList( CSSListTaggedSelectItem& item );
	void AddSortedItemToList( CSSListTaggedSelectItem& item );
	void InsertItemIntoList( int nPos, CSSListTaggedSelectItem& item );
	void SwapItems( int nIndex1, int nIndex2 );	
	
public:
	void GetListItem( int nIndex, CSSListTaggedSelectItem& item );
	void SetListItem( int nIndex, CSSListTaggedSelectItem& item );
	void SetItemTagState( int nIndex, int nTagState );
	int GetItemTagState( int nIndex );
	void SelectAll();
	void SelectNone();

private:
	void FindArrayItem( CSSListTaggedSelectItem& item, int& nPos );
	void UpdateListItemInternal(int nIndex, CSSListTaggedSelectItem& itemNew);
	void SelectAllInternal(bool bSelect);
	void SendTagStateNotifyMessage();

public:
	DECLARE_MESSAGE_MAP()
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnRButtonDown(UINT nFlags, CPoint point);
	void OnLButtonDblClk(UINT nFlags, CPoint point);
	void OnRButtonDblClk(UINT nFlags, CPoint point);

public:
	void ClickHandler(UINT nFlags, CPoint point);
	void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );

public:
	void SetUseExternalArray(bool bUseExternalArray);
	void SetExternalArray(CArray<CSSListTaggedSelectItem, CSSListTaggedSelectItem>* pExternalArray);
	CArray<CSSListTaggedSelectItem, CSSListTaggedSelectItem>& GetArray();

protected:
	CArray<CSSListTaggedSelectItem, CSSListTaggedSelectItem> m_InternalArrayItems;
	CArray<CSSListTaggedSelectItem, CSSListTaggedSelectItem>* m_pExternalArrayItems;
	bool m_bUseExternalArray;

private:
	bool m_bLockItemZero;
	bool m_bTagStateChanged;
	
private:
	CWnd* m_pTagStateNotifyWnd;
	int m_nTagStateNotifyMessage;
	int m_nTaggedItemCount;
	bool m_bMultiColumns;
};

/**********************************************************************/
/**********************************************************************/
/* For internal use by CMenuChanger only						*/
/**********************************************************************/

#define MENU_ITEM_OPTION 0
#define MENU_ITEM_SEPARATOR 1
#define MENU_ITEM_SUBMENU 2
#define MENU_ITEM_ENDMENU 3

#define MENU_FLAG_SHOW 0
#define MENU_FLAG_ENABLE 1
#define MENU_FLAG_KILL 2

class CMenuChangerCSVRecord : public CCSVRecord
{
public:
	CMenuChangerCSVRecord(); 
	virtual ~CMenuChangerCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
	//basic data handling
	void SetItemDepth ( int nDepth ) { m_nItemDepth = nDepth; }
	void SetItemType ( int nType ) { m_nItemType = nType; }
	void SetItemId ( int nId ) { m_nItemId = nId; }
	void SetItemText ( const char* szText ) { m_strItemText = szText; }
	void SetEnableFlag ( bool bFlag ) { m_bEnable = bFlag; }
	void SetDisplayFlag ( bool bFlag ) { m_bDisplay = bFlag; }
	void SetKillFlag ( bool bFlag ) { m_bKill = bFlag; }
	
	int GetItemDepth() { return m_nItemDepth; }
	int GetItemType() { return m_nItemType; }
	int GetItemId() { return m_nItemId; }
	const char* GetItemText() { return m_strItemText; }
	bool GetEnableFlag() { return m_bEnable; }
	bool GetDisplayFlag() { return m_bDisplay; }
	bool GetKillFlag() { return m_bKill; }

private:
	int m_nItemDepth;
	int m_nItemType;
	int m_nItemId;
	CString m_strItemText;
	bool m_bEnable;
	bool m_bDisplay;
	bool m_bKill;
};

class CMenuChangerCSVArray : public CCSVArray
{
public:
	void AddOption ( int nId, const char* szText, int nItemDepth );
	void AddSeparator ( int nItemDepth );
	void AddSubMenu ( const char* szText, int nItemDepth );
	void AddEndMenu( int nItemDepth );

	void EnableAll( bool bEnable );
	void EnableItem ( int nId, bool bEnable );
	void EnableParent ( int nId, bool bEnable, int nDepth ) { SetParentStatus ( nId, MENU_FLAG_ENABLE, bEnable, nDepth ); }
	
	void ShowItem ( int nId, bool bShow );
	void ShowParent ( int nId, bool bShow, int nDepth ) { SetParentStatus ( nId, MENU_FLAG_SHOW, bShow, nDepth ); }
	
	void KillItem ( int nId );
	void KillParent ( int nId, int nDepth ) { SetParentStatus ( nId, MENU_FLAG_KILL, TRUE, nDepth ); }

	void SetParentText(int nId, CString strText, int nDepth = 1);

	void SetItemText ( int nId, const char* szText );
	bool InsertExtraMenu ( UINT nFirstNewID, const char* szMenuStructure, UINT& nNextFreeID );

private:
	void SetParentStatus ( int nId, int nType, bool bFlag, int nDepth );	
	int FindParentItem( int nId, int nDepth);
};

/**********************************************************************/
/*	MENUCHANGER.CPP										*/
/**********************************************************************/

class CMenuChanger
{
public:
	CMenuChanger() { m_bPopup = FALSE; }
	
	void SetItemText ( UINT nTargetID, const char* szText ) { m_MenuArray.SetItemText ( nTargetID, szText ); }
	void EnableItem ( UINT nTargetID, bool bEnable ) { m_MenuArray.EnableItem ( nTargetID, bEnable ); }
	void ShowItem ( UINT nTargetID, bool bShow ) { m_MenuArray.ShowItem ( nTargetID, bShow ); }
	void KillItem ( UINT nTargetID ) { m_MenuArray.KillItem ( nTargetID ); }
	
	void EnableParent ( UINT nTargetID, bool bEnable, int nDepth = 1 ) { m_MenuArray.EnableParent ( nTargetID, bEnable, nDepth ); }
	void ShowParent ( UINT nTargetID, bool bShow, int nDepth = 1 ) { m_MenuArray.ShowParent ( nTargetID, bShow, nDepth ); }
	void KillParent ( UINT nTargetID, int nDepth = 1 ) { m_MenuArray.KillParent ( nTargetID, nDepth ); }
	void SetParentText(UINT nTargetID, CString strText, int nDepth = 1) { m_MenuArray.SetParentText(nTargetID, strText, nDepth); }

	void EnableAll( bool bEnable ) { m_MenuArray.EnableAll( bEnable ); }
	
	bool InsertExtraMenu ( UINT nFirstNewID, const char* szMenuStructure, UINT& nNextFreeID ) { return m_MenuArray.InsertExtraMenu ( nFirstNewID, szMenuStructure, nNextFreeID ); }

	void EatMenuTopLevel ( CMenu* pMenu );
	void EatMenuPopup ( CMenu* pMenu );
	void BuildMenu ( CMenu* pMenu );
	
private:
	void EatMenuRecursive ( CMenu* pMenu, int nItemDepth );
	void TidySeparators ( CMenu* pMenu );
	
private:
	CMenuChangerCSVArray m_MenuArray;
	bool m_bPopup;
};

/**********************************************************************/
/*	CSPINPROPERTYSHEET.CPP									*/
/**********************************************************************/

class CMyDialogTemplate : public CDialogTemplate
{
public:
	void Attach(LPDLGTEMPLATE pTemplate)
	{
		m_hTemplate = ::GlobalHandle(pTemplate);
		m_dwTemplateSize = GetTemplateSize(pTemplate);
		m_bSystemFont = false;
	}
};

/**********************************************************************/

class CSSPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CSSPropertySheet)

public:
	CSSPropertySheet(const char* szTitle, CWnd* pWndParent = NULL, UINT nFirstPage = 0);

public:
	virtual int DoModal();

protected:
	virtual void BuildPropPageArray();

protected:
	static int CALLBACK PropSheetProc(HWND hwndDlg, UINT uMsg, LPARAM lParam);

private:
	CString m_strFont;
	int m_nSize;
};

/**********************************************************************/

class CSpinPropertySheet : public CSSPropertySheet
{
	DECLARE_DYNAMIC(CSpinPropertySheet)

public:
	CSpinPropertySheet( bool bEnableSpin, WORD wSpinID, const char* szTitle, CWnd* pWndParent = NULL, UINT nFirstPage = 0);

	virtual ~CSpinPropertySheet();
	virtual void SpinPrevious(){};
	virtual void SpinNext(){};

	//{{AFX_VIRTUAL(CSpinPropertySheet)
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSpinPropertySheet).
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CSpinButtonCtrl m_SpinButton;
	bool m_bEnableSpin;
	WORD m_wSpinID;
};

/**********************************************************************/
/*	SHORTCUT.CPP											*/
/**********************************************************************/

void CreateProgramsLink ( const char* szLinkFilename, const char* szOrigFilename, const char* szWorkingDirectory, const char* szArgs="" );

// eg szLinkFilename = "SharpSoft\\Ecr Manager.lnk";
//	 szOrigFilename = full path to original file
//	 szWorkingDirectory = full path to working directory (can be "" if not required)

void CreateDesktopLink ( const char* szLinkFilename, const char* szOrigFilename, const char* szWorkingDirectory, const char* szArgs="" );

// eg szLinkFilename = "Ecr Manager.lnk";
//	 szOrigFilename = full path to original file
//	 szWorkingDirectory = full path to working directory (can be "" if not required)

void CreateStartupLink ( const char* szLinkFilename, const char* szOrigFilename, const char* szWorkingDirectory, const char* szArgs="" );

// eg szLinkFilename = "Sys5000.lnk";
//	 szOrigFilename = full path to original file
//	 szWorkingDirectory = full path to working directory (can be "" if not required)

CString GetStartupPath();	// returns path to startup folder ( eg "c:\windows\start menu\programs\startup" )
						// N.B. no trailing backslash
						// or "" if function fails

/**********************************************************************/
/*	BROWSE.CPP											
/**********************************************************************/

bool BrowseFolder ( CString& strPathname, const char* szText, const char* szStartFolder, CWnd* pWnd );

// to browse everything make szStartFolder = NULL

/**********************************************************************/
/*	GLOBALS.CPP														
/**********************************************************************/

void DrawListText(CDC* pDC, CString strText, CRect& rect, int nAlign, int nYShift = 0);

/**********************************************************************/
/*
	How to use CSSFullScreenWnd:

	CSSFullScreenWnd* pScreen = new CSSFullScreenWnd();	// dont call delete (called automatically)
	if ( pScreen->Open ( "Window Title", "", RGB(0,0,255) ) == TRUE )
	{
		do something
		pScreen->Close();		// MUST be called
	}
*/
/**********************************************************************/

class CSSFullScreenWnd : public CFrameWnd
{
public:
	CSSFullScreenWnd();
	BOOL Open (  const char* szTitle, const char* szImageFilename, COLORREF crBackground );
	void Close();

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()

private:
	CImage m_image;
	COLORREF m_crBackground;
	bool m_bCreated;
};

/**********************************************************************/

class CSSColourDialog : public CDialog
{
public:
	CSSColourDialog ( UINT nIDTemplate, CWnd* pParent=NULL );	// standard constructor
	CSSColourDialog();
	~CSSColourDialog();
	void SetImage ( const char* szImageFilename, COLORREF crTransparent=-1 );		// -1 = no transparent colour in image

protected:
	virtual BOOL OnInitDialog ( COLORREF crBackground=-1, bool bRounded=FALSE );	// -1 = use default dialog colour
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()

private:
	CImage m_image;
	COLORREF m_crBackground;
	COLORREF m_crTransparent;
};

/**********************************************************************/

class CSSColourButton : public CButton
{
public:
	CSSColourButton();
	~CSSColourButton();
	void Init ( UINT nID, CWnd* pParent, COLORREF crText, COLORREF crBackground, const char* szFontName, int nFontSize=-1 );	// szFontName "" = Arial, -1 = auto font size
	void SetImage ( const char* szImageFilename, COLORREF crTransparent=-1 );		// -1 = no transparent colour in image
	COLORREF SetTextColour ( COLORREF crText );				// returns current text colour
	COLORREF SetBackgroundColour ( COLORREF crBackground );	// returns current background colour (-1 = transparent)

protected:
	virtual void DrawItem ( LPDRAWITEMSTRUCT lpDrawItemStruct );
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()

private:
	void InvalidateParentRect();

private:
	CWnd* m_pWndParent;
	CFont m_font;
	CImage m_image;
	CImage m_imageDisabled;
	COLORREF m_crText;
	COLORREF m_crBackground;
	COLORREF m_crTransparent;
	bool m_bInvalidateParent;
};

/**********************************************************************/

class CSSColourStatic : public CStatic
{
public:
	CSSColourStatic();
	~CSSColourStatic();
	void Init ( UINT nID, CWnd* pParent, COLORREF crText, COLORREF crBackground, const char* szFontName, int nFontSize=-1 );	// szFontName "" = Arial, -1 = auto font size
	void SetImage ( const char* szImageFilename, COLORREF crTransparent=-1 );		// -1 = no transparent colour in image
	COLORREF SetTextColour ( COLORREF crText );				// returns current text colour
	COLORREF SetBackgroundColour ( COLORREF crBackground );	// returns current background colour (-1 = transparent)

protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem ( LPDRAWITEMSTRUCT lpDrawItemStruct );

private:
	void InvalidateParentRect();

private:
	CWnd* m_pWndParent;
	CFont m_font;
	CImage m_image;
	COLORREF m_crText;
	COLORREF m_crBackground;
	COLORREF m_crTransparent;
	int m_nAlign;
	bool m_bInvalidateParent;
};

/**********************************************************************/

class CSSColourGroupBox : public CButton
{
public:
	CSSColourGroupBox();
	~CSSColourGroupBox();
	void Init ( UINT nID, CWnd* pParent, COLORREF crText, COLORREF crLine, const char* szFontName, int nFontSize );	// szFontName "" = Arial
	COLORREF SetTextColour ( COLORREF crText );		// returns current text colour
	COLORREF SetLineColour ( COLORREF crLine );		// returns current line colour

protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

private:
	CFont m_font;
	COLORREF m_crText;
	COLORREF m_crLine;
};

/**********************************************************************/
/*														*/
/*	ResizeImageToBGR565										*/
/*														*/
/*	Source file can be bmp jpg png or gif						*/
/*	Dest file must be bmp									*/
/*														*/
/**********************************************************************/

bool ResizeImageToBGR565 ( const char* szSourceFilename, const char* szDestFilename, int nMaxWidth, int nMaxHeight, bool bKeepAspectRatio=TRUE );

/**********************************************************************/

struct CComboTextInfo
{
	const char* szComboItems;
	const char* szDisplayItems;
};

/**********************************************************************/

class CSSComboBox : public CComboBox
{
public:
	CSSComboBox();

public:
	void LoadStrings( CComboTextInfo info, int nStart = 0 );
	bool SelectUsingItemData( int nData, int nDefault = -1 );
	int GetItemDataOfSelection();

public:
	//{{AFX_VIRTUAL(CSSComboBox)
	//}}AFX_VIRTUAL

public:
	virtual ~CSSComboBox();

protected:
	//{{AFX_MSG(CSSComboBox)
	afx_msg void OnDropDown();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/**********************************************************************/

class CSSComboBoxEx : public CComboBoxEx
{
public:
	CSSComboBoxEx();

public:
	void LoadStrings( CComboTextInfo info, int nStart = 0 );
	bool SelectUsingItemData( int nData, int nDefault = -1 );
	int GetItemDataOfSelection();

public:
	//{{AFX_VIRTUAL(CSSComboBoxEx)
	//}}AFX_VIRTUAL

public:
	virtual ~CSSComboBoxEx();

protected:
	//{{AFX_MSG(CSSComboBoxEx)
	afx_msg void OnDropDown();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/**********************************************************************/

class CSSTabbedComboBox : public CComboBox
{
public:
	CSSTabbedComboBox();
	void SetTabStops( CArray<int,int>& arrayTabStops );
	void SetMaxColumns( int n ){ m_nMaxColumns = n; }

public:
	void ResetContent();

public:
	int AddItem( const char* szItem );
	void InsertItem( int nPos, const char* szItem );
	int GetStringCount(){ return m_arrayStrings.GetSize(); } 
	bool SelectUsingItemData( int nData, int nDefault = -1 );

public:
	//{{AFX_VIRTUAL(CSSTabbedComboBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

public:
	virtual ~CSSTabbedComboBox();

protected:
	//{{AFX_MSG(CSSTabbedComboBox)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CArray<int,int> m_arrayTabStops;
	CStringArray m_arrayStrings;
	int m_nMaxColumns;
};

/**********************************************************************/

extern const char* szVERSION_WINSUBS;

/**********************************************************************/
