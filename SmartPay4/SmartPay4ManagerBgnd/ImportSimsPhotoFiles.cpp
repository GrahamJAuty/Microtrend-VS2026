/*********************************************************************/
#include "..\SmartPay4Shared\ImageHandler.h"
#include "..\SmartPay4Shared\MembershipDatabase.h"
#include "..\SmartPay4Shared\RepmanDefines.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\WaitDlg.h"
/*********************************************************************/
#include "PhotoIDCopy.h"
/*********************************************************************/
#include "ImportSimsPhotoFiles.h"
/*********************************************************************/

void CImportSimsPhotoFiles::Init()
{
	m_nRead = 0;
	m_nIgnored = 0;
	m_nUsed = 0;
	m_nInvalid = 0;
	m_nAdded = 0;
	m_strError = "";
}

//*******************************************************************

CImportSimsPhotoFiles::~CImportSimsPhotoFiles()
{
}

//*******************************************************************

bool CImportSimsPhotoFiles::Import()
{
	m_ImportFlag.SetImportFlag( TRUE );

	if ( ProcessPhotos() == nREPORT_NOERROR )
	{
		ShowStatistics();
		return TRUE;
	}
	return FALSE;
}

//*******************************************************************
// On process copy sims file into PhotoID foler & Exports\PhotoID folder

int CImportSimsPhotoFiles::ProcessPhotos()
{
	CWaitDlg dlgWait ( m_pParent );
	dlgWait.SetMessageText ( "Checking for SIMS photo files" );

	CPhotoIDCopy photoID{};

	CString strMask = "";
	strMask.Format ( "%s\\*.*", 
		(const char*) m_pSims->m_strSimsPhotoFolder );		// folder where SIMS photo are located

	CFileFind FileFinder;	
	BOOL bWorking = FileFinder.FindFile ( strMask );

	while ( bWorking )   
	{
		bWorking = FileFinder.FindNextFile();
		if ( FileFinder.IsDots() == TRUE || FileFinder.IsDirectory() == TRUE )
			continue;

		CString strFilename = FileFinder.GetFileName();
		strFilename.MakeUpper();
		if ( strFilename == "THUMBS.DB" )								// skip windows generated file 
			continue;

		++m_nRead;
		CString strPhotoReference = FileFinder.GetFileTitle();			// nnnnnn		

		CSQLRepositoryAccount RepoAccount;
		__int64 nUserID = RepoAccount.LookupIndex(m_pSims->GetLocatePhotoByLabel(), strPhotoReference, NULL).GetSQLResult();
		
		if ( nUserID != 0 )			// see if found 
		{
			CString strCardNo = FormatInt64Value(nUserID);

			CImageHandler image(FALSE);
			bool bReplacement = ( image.GetImageFilepath( strCardNo ) == "" ) ? FALSE : TRUE;

			CString strSourcePathname = FileFinder.GetFilePath();		// full name of Sims Photo file

			bool bOk = TRUE;
			if ( m_ImportFlag.GetImportFlag() == TRUE )	
			{
				bOk = photoID.CopyPhotoFile ( strSourcePathname, strCardNo );
			}
			else						
			{
				ShowImportLine ( strPhotoReference, strCardNo, strSourcePathname, bReplacement );
			}

			if ( bOk == TRUE )
			{
				++m_nUsed;
		
				if ( bReplacement == FALSE )					// see if new photo added
				{
					++m_nAdded;
				}

				if ( ( m_ImportFlag.GetImportFlag() == TRUE ) && ( m_pSims->m_bDeletePhotoFile == TRUE ) )
				{
					CFileRecycler::SendToRecycleBin ( strSourcePathname );
				}
			}
		}
		else
		{
			++m_nIgnored;

			if ( m_ImportFlag.GetImportFlag() == FALSE )
			{
				ShowImportLine ( strPhotoReference, "", FileFinder.GetFilePath(), FALSE );
			}
		}
	}

	return nREPORT_NOERROR;
}

//*******************************************************************
