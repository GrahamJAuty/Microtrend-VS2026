/**********************************************************************/
#include "MyCommandLineInfo.h"
/**********************************************************************/

void CMyCommandLineInfo::ParseParam ( const char* szParam, BOOL bFlag, BOOL bLast )
{
	if ( bFlag == FALSE )			/* discard /p for printing etc */
		m_arrayParam.Add ( szParam );

	CCommandLineInfo::ParseParam ( szParam, bFlag, bLast );
}

/**********************************************************************/
