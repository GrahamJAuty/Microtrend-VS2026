#pragma once
/**********************************************************************/
#include "Defines.h"
/**********************************************************************/

void DisplayMessage ( CWnd* pParent, const char* szMsg, const char* szTitle, UINT uType = MB_ICONSTOP );

bool FileExists ( const char* szFile );
bool AppendFile ( const char* szSourceFilename, const char* szDestFilename );

bool ValidateFolderAccess ( const char* szPath );

void SendToRecycleBin ( const char* szFileMask);
void SendToRecycleBin ( CStringArray* pArray );

bool GetAdminPassword( CWnd* pParent);

void PlayMessage ( int nMessageNo, bool bWaitFinshed = FALSE );

bool CheckEventDue(LONGLONG& nLastTime, int nInterval);
void ResetEventIfFired(LONGLONG& nLastTime);
void ResetEvent(LONGLONG& nLastTime);

/**********************************************************************/
