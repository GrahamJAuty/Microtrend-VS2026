//*******************************************************************************/
#include "TLIDDatabase.h"
//*******************************************************************************/

CTLIDDatabase::CTLIDDatabase()
 : CSharedCSVArray ( ',', '"', TRUE, FALSE, TRUE )
{
}

//**********************************************************************

bool CTLIDDatabase::GetRecordByIndex ( int nIndex, CTLIDRecord& Record )
{
	return GetAt ( nIndex, Record );
}

//**********************************************************************
// Get record create if not found

bool CTLIDDatabase::GetRecord ( int nTerminalNo, CTLIDRecord& Record )
{
	int nIndex;
	if ( FindRecord ( nTerminalNo, nIndex ) == FALSE )
	{
		Record.SetTerminalNo ( nTerminalNo );
		return InsertAt ( nIndex, Record );
	}

	return GetAt ( nIndex, Record );
}

//*******************************************************************

bool CTLIDDatabase::SaveRecord ( CTLIDRecord& Record )
{
	int nIndex;
	if ( FindRecord ( Record.GetTerminalNo(), nIndex ) == FALSE )
		return InsertAt ( nIndex, Record );

	return SetAt ( nIndex, Record );
}

//**********************************************************************

bool CTLIDDatabase::FindRecord ( int nTerminalNo, int& nIndex )
{
	int nLow	= 0;
	int nLowest	= GetSize();
	int nHigh	= nLowest - 1;
	
	CTLIDRecord Record;

	while ( nLow <= nHigh )
	{
		int nMid = ( nLow + nHigh ) / 2;
		GetAt ( nMid, Record );

		int nRecordTNo = Record.GetTerminalNo();

		if ( nTerminalNo < nRecordTNo )
			nHigh = nMid - 1;
		else
		{
			if ( nTerminalNo > nRecordTNo )
				nLow = nMid + 1;
			else									/* strings compare */
			{
				nHigh = nMid - 1;
				if ( nMid < nLowest )
					nLowest = nMid;					/* save lowest value */
			}
		}
	}

	if ( nLowest < GetSize() )
	{
		nIndex = nLowest;
		return TRUE;
	}

	nIndex = nLow;
	return FALSE;
}

//*********************************************************************
