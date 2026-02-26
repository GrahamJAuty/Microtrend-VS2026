#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************

static const char nMaxColumns = 17;

/**********************************************************************/

class CColumnHandler
{
public:
	CColumnHandler();
	bool Read ( const char* szLabel, int nColumnCount );
	int GetHeight() { return m_nHeight; }
	void SetHeight ( int nHeight ) { m_nHeight = nHeight; }
	int GetWidth ( int nIndex );
//	int GetTotalWidth();
	void SetWidth ( int nIndex, int nWidth );
	void Save ( const char* szLabel, int nHeight, int nColumns, CListCtrl& list );

private:
	int m_nHeight;
	int m_nWidths[nMaxColumns];
	CString m_strFilename;
};

/**********************************************************************/

