#pragma once
/**********************************************************************/
#include "SPOSKeyboardDefines.h"
#include "SPOSKeyboardListManager.h"
/**********************************************************************/

class CSPOSKeyboardMultipleKeySelectInfo
{
public:
	CSPOSKeyboardMultipleKeySelectInfo(void);
	void Reset();

public:
	void ClearSelection(){ m_bGotSelection = FALSE; }
	bool GotSelection(){ return m_bGotSelection; }
	void RememberSettings();
	void RestoreSettings();

public:
	void TagKey( __int64 nListNo, int nPageNo, CSPOSKeyboardKeyExtent& KeyExtent );
	
public:
	void BuildCopyBuffer();
	int GetCopyBufferSize(){ return m_arrayKeyRecordsToCopy.GetSize(); }
	void GetCopyBufferKey( int nIdx, CSPOSKeyboardKeyExtent& KeyExtent, CSPOSKeyboardKeyRecord& KeyRecord );

public:
	bool CheckPage( __int64 nListNo, int nPageNo );
	bool IsSelected( int nCol, int nRow );
	
public:
	__int64 GetListNo(){ return m_nListNo; }
	int GetColStart(){ return m_nColStart; }
	int GetRowStart(){ return m_nRowStart; }
	int GetCopyWidth(){ return m_nCopyWidth; }
	int GetCopyHeight(){ return m_nCopyHeight; }
	int GetKeyNoCorner1(){ return m_KeyExtent1.m_nKeyNo; }
	int GetKeyNoCorner2(){ return m_KeyExtent2.m_nKeyNo; }

private:
	void ResetSelectedKeys();
	void FlagSelectedKey( int nCol, int nRow, bool bFlag );
	
private:
	bool m_bGotSelection;
	__int64 m_nListNo;
	int m_nPageNo;
	CSPOSKeyboardKeyExtent m_KeyExtent1;
	CSPOSKeyboardKeyExtent m_KeyExtent2;
	int m_nColStart;
	int m_nRowStart;
	int m_nCopyHeight;
	int m_nCopyWidth;

private:
	bool m_arraySelected[20][20];
	CArray<CSPOSKeyboardKeyExtent,CSPOSKeyboardKeyExtent> m_arrayKeyExtentsToCopy;
	CArray<CSPOSKeyboardKeyRecord,CSPOSKeyboardKeyRecord> m_arrayKeyRecordsToCopy;

private:
	__int64 m_nStoredListNo;
	int m_nStoredPageNo;
	CSPOSKeyboardKeyExtent m_StoredKeyExtent1;
	CSPOSKeyboardKeyExtent m_StoredKeyExtent2;
	int m_nStoredColStart;
	int m_nStoredRowStart;
	int m_nStoredCopyHeight;
	int m_nStoredCopyWidth;

private:
	bool m_arrayStoredSelected[20][20];
	CArray<CSPOSKeyboardKeyExtent,CSPOSKeyboardKeyExtent> m_arrayStoredKeyExtentsToCopy;
	CArray<CSPOSKeyboardKeyRecord,CSPOSKeyboardKeyRecord> m_arrayStoredKeyRecordsToCopy;
};

/**********************************************************************/
