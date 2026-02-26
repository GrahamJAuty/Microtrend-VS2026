/******************************************************************************/
#include "..\SPosLoyalty4Shared\GlobalFunctions.h"
/**********************************************************************/

int GetFileLineCount ( const char* szFilename )
{
	CSSFile file{};
	int nCount = 0;

	CWaitCursor wait;
	if ( file.Open ( szFilename, "rb" ) == TRUE )
	{
		CString strBuf;
		while ( file.ReadLine ( strBuf ) == TRUE )
		{
			if (strBuf.GetLength() > 2)			// cr / lf
			{
				++nCount;
			}
		}

		file.Close();
	}

	return nCount;
}

//*******************************************************************
