#pragma once
/**********************************************************************/
#include "SPOSKeyboardListManager.h"
/**********************************************************************/

class CSPOSKeyboardKeymapTable
{
public:
	CSPOSKeyboardKeymapTable();
	~CSPOSKeyboardKeymapTable();
	void Reset();

	void BuildKeymap( __int64 nListNo, int nPageNo, int nZoomType );
	void Invalidate() { m_bGotKeymap = FALSE; }

	int GetSharpKeyPos ( int nCol, int nRow );
	int GetCol ( int nSharpKeyPos );
	int GetRow ( int nSharpKeyPos );

	int GetKeyAtPoint ( CPoint point );
	int GetSharpKeyPosAtPoint ( CPoint point );

	double GetZoomFactor() { return m_dZoomFactor; }

	int GetKeyCount(){ return m_KeyExtentMap.GetSize(); }
	void GetKeyInfoByIndex( int nIndex, CSPOSKeyboardKeyExtent& KeyExtent );
	void GetKeyInfoBySharpKeyPos( int nSharpKeyPos, CSPOSKeyboardKeyExtent& KeyExtent );

	int GetListWidth(){ return m_nListWidth; }
	int GetListHeight(){ return m_nListHeight; }

private:
	CArray<CSPOSKeyboardKeyExtent,CSPOSKeyboardKeyExtent> m_KeyExtentMap;
	CArray<CSPOSKeyboardKeyExtent,CSPOSKeyboardKeyExtent> m_SingleKeyMap;
	
private:
	double m_dZoomFactor;
	bool m_bGotKeymap;
	int m_nColumnCount;
	int m_nRowCount;
	int m_nListWidth;
	int m_nListHeight;
};

/**********************************************************************/
