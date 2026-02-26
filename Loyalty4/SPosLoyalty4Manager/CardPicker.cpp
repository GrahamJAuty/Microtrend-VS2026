//#include "systemdata.h"
//#include "globalfunctions.h"
#include "CardPicker.h"

CCardPicker::CCardPicker()
{
	Reset();
}

void CCardPicker::Reset()
{
	m_array.RemoveAll();								// force read on nex validate
}

bool CCardPicker::IsValid ( const char* szCardNo, const char* szFilename )
{
	if ( m_array.GetSize() == 0 )						// see if file has ben read yet
	{
		CSSFile file;
		if ( file.Open ( szFilename, "rb" ) == TRUE )	// no read data
		{
			CString strCardNo;
			while ( file.ReadString(strCardNo) == TRUE )
				m_array.Add ( strCardNo );

			file.Close();
		}
	}

	int nIndex;
	return FindCardNo ( szCardNo, nIndex );
}

//*****************************************************************************
// return true if cardno if found in array - index updated with pointer to record

bool CCardPicker::FindCardNo ( const char* szCardNo, int& nIndex )
{
	int nLow	= 0;
	int nLowest = m_array.GetSize();
	int nHigh	= nLowest - 1;

	CString strCardNo = System.FormatCardNo ( szCardNo );		// padout to maximum

	while ( nLow <= nHigh )
	{
		int nMid = ( nLow + nHigh ) / 2;
		CString strListCardNo = m_array.GetAt ( nMid );

		if ( strCardNo < strListCardNo )
			nHigh = nMid - 1;
		else
		{
			if ( strCardNo > strListCardNo )
				nLow = nMid + 1;
			else									// strings compare/
			{
				nHigh = nMid - 1;					// assume comp < 0/
				if ( nMid < nLowest )
					nLowest = nMid;					// save lowest value
			}
		}
	}

	if ( nLowest < m_array.GetSize() )				// lowest has been modified
	{
		nIndex = nLowest;
		return TRUE;								// found
	}

	nIndex = nLow;
	return FALSE;									// not found
}

//*******************************************************************
