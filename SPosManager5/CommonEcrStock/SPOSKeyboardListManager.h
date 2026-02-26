#pragma once
/**********************************************************************/
#include "SPOSKeyboardKeyRecord.h"
#include "SPOSKeyboardListRecord.h"
#include "SPOSKeyboardScreenRecord.h"
/**********************************************************************/
#define SCREENLIST_TYPE_TRADING_TOPLEVEL 0
#define SCREENLIST_TYPE_TRADING_FUNCTION 1
#define SCREENLIST_TYPE_LOGGEDOFF_FUNCTION 2
#define SCREENLIST_TYPE_LOGGEDOFF_FUNCTION_LARGE 3
#define SCREENLIST_TYPE_PAYMENT_TOPLEVEL 4
#define SCREENLIST_TYPE_PAYMENT_FUNCTION 5
/**********************************************************************/

struct CSPOSKeyboardListComboItem
{
public:
	CSPOSKeyboardListComboItem() {
		m_strText = "";
		m_nListNo = 0;
		m_nPageNo = 0;
		m_nScreenNo = 0;
		m_nScreenListType = 0;
	}

public:
	CString m_strText;
	__int64 m_nListNo;
	int m_nPageNo;
	int m_nScreenNo;
	int m_nScreenListType;
};

/**********************************************************************/

struct CSPOSKeyboardKeyExtent
{
public:
	CSPOSKeyboardKeyExtent() {
		m_nKeyNo = 0;
		m_nRowStart = 0;
		m_nColStart = 0;
		m_nRowSize = 0;
		m_nColSize = 0;
	}

public:
	int m_nKeyNo;
	int m_nRowStart;
	int m_nColStart;
	int m_nRowSize;
	int m_nColSize;
	CRect m_rectDisplay;
};

/**********************************************************************/

struct CSPOSKeyboardGlobalChangeRequest
{
	int m_nFontSize;
	int m_nHorizontal;
	int m_nVertical;
	int m_nBold;
	int m_nHide;
	int m_nDefault;
	int m_nColourMode;
	int m_nForeground;
	int m_nBackground;
	bool m_bKeyText;
	bool m_bRemoveImage;
	bool m_bEmpty;
	bool m_bFunction;
	bool m_bPayment;
	bool m_bModifier;
	bool m_bControl;
	bool m_bList;
	bool m_bServer;
	bool m_bMacro;
	bool m_bPlu;
	int m_nChangesMode;
	bool m_bChangeItemStandard;
	bool m_bChangeItemLarge;
	bool m_bChangeFunctionStandard;
	bool m_bChangeFunctionLarge;
	bool m_bChangeBottomBar;
	__int64 m_nVisibleListNo;
};

/**********************************************************************/

class CSPOSKeyboardListManager 
{
public:
	CSPOSKeyboardListManager();
	
public:
	//SCREENS
	bool FindScreen( int nScreenNo, int& nScreenIdx );	
	bool FindOrInsertScreen( int nScreenNo, int& nScreenIdx );
	bool GetNextFreeScreenNo( int nScreenNo, int& nFreeScreenNo );
	bool CopyScreen( int nSourceScreenNo, int nDestScreenNo );
	bool DeleteScreen( int nScreenIdx );
	
	int GetScreenCount();
	bool GetScreenRecord( int nScreenIdx, CSPOSKeyboardScreenRecord& ScreenRecord );
	bool SetScreenRecord( int nScreenIdx, CSPOSKeyboardScreenRecord& ScreenRecord );

public:
	//LISTS
	bool FindList( __int64 nListNo, int& nListIdx );	
	bool FindOrInsertList( __int64 nListNo, int& nListIdx );
	bool GetNextFreeListNo( __int64 nListNo, __int64& nFreeListNo );
	bool CopyList( __int64 nSourceListNo, __int64 nDestListNo );
	bool DeleteList( int nListIdx );
	bool InitialiseKeysForList( int nListIdx );
	bool UpdateKeysForList( int nListIdx, int nOldFormat );

	bool GetKeyExtentMap( __int64 nListNo, int nPageNo, CArray<CSPOSKeyboardKeyExtent,CSPOSKeyboardKeyExtent>& map ); 
	bool GroupKey( __int64 nListNo, int nPageNo, int nSourceKeyPos, int nDestKeyPos );
	
	int GetListCount();
	bool GetListRecord( int nListIdx, CSPOSKeyboardListRecord& ListRecord );
	bool SetListRecord( int nListIdx, CSPOSKeyboardListRecord& ListRecord );

	const char* GetListNameExportFolder() { return m_strListNameExportFolder; }
	void SetListNameExportFolder( const char* sz ) { m_strListNameExportFolder = sz; }
	void WriteListNames();

public:
	bool MovePages( __int64 nListNo, CWordArray& NewPages );

public:
	//KEYS
	bool FindKey( __int64 nListNo, int nPageNo, int nKeyNo, int& nKeyIdx );	
	bool FindOrInsertKey( __int64 nListNo, int nPageNo, int nKeyNo, int& nKeyIdx );
	bool SplitKey( __int64 nListNo, int nPageNo, int nKeyNo );
	bool DeleteKey( int nKeyIdx );

	void GetPageKeyRange( __int64 nListNo, int nPageNo, int& nStartIdx, int& nEndIdx );
	void GetListKeyRange( __int64 nListNo, int& nStartIdx, int& nEndIdx );

	void UpdateSettingsGlobally( CSPOSKeyboardGlobalChangeRequest& ChangeRequest );

	int GetKeyCount();
	bool GetKeyRecord( int nKeyIdx, CSPOSKeyboardKeyRecord& KeyRecord );
	bool SetKeyRecord( int nKeyIdx, CSPOSKeyboardKeyRecord& KeyRecord );

	void RememberKeysForList( __int64 nListNo );
	void RestoreKeysForList( __int64 nListNo );
	
public:
	void GetListComboInfo( int nMode, CArray<CSPOSKeyboardListComboItem,CSPOSKeyboardListComboItem>& array );

public:
	bool IsModified() { return m_bIsModified; }
	void SetModifiedFlag() { m_bIsModified = TRUE; }
	void ClearModifiedFlag() { m_bIsModified = FALSE; }

public:
	void Reset();

private:
	CArray<CSPOSKeyboardListRecord,CSPOSKeyboardListRecord> m_ListArray;
	CArray<CSPOSKeyboardScreenRecord,CSPOSKeyboardScreenRecord> m_ScreenArray;
	CArray<CSPOSKeyboardKeyRecord,CSPOSKeyboardKeyRecord> m_KeyArray;
	
private:
	CArray<CSPOSKeyboardKeyRecord,CSPOSKeyboardKeyRecord> m_StoredPageKeyArray;
	__int64 m_nStoredPageListNo;
	int m_nStoredPagePageNo;

private:
	bool m_bIsModified;

private:
	CString m_strListNameExportFolder;
	CString m_strTextBuffer;
};

/**********************************************************************/

