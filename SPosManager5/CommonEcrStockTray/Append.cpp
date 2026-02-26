/**********************************************************************/
#include "Append.h"
/**********************************************************************/

bool Append ( const char* szSource, const char* szDest )
{
	CSSFile fileSource;
	CSSFile fileDest;

	if ( fileSource.Open ( szSource, "rb" ) == FALSE )
		return FALSE;

	if ( fileDest.Open ( szDest, "ab" ) == FALSE )
		return FALSE;
		
	CString strBuffer;
	while ( fileSource.ReadString ( strBuffer ) == TRUE )  
		fileDest.WriteLine ( strBuffer );
	
	return TRUE;
}

/**********************************************************************/
