/*********************************************************************/
#include "..\SmartPay4Shared\ImageHandler.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\WaitDlg.h"
/*********************************************************************/
#include "PhotoIDCopy.h"
/*********************************************************************/
#include "ImportWondePhotoFiles.h"
/*********************************************************************/

void CImportWondePhotoFiles::Init()
{
	m_nRead = 0;
	m_nIgnored = 0;
	m_nUsed = 0;
	m_nInvalid = 0;
	m_nAdded = 0;
}

//******************************************************************

CImportWondePhotoFiles::~CImportWondePhotoFiles()
{
}

//*******************************************************************

int CImportWondePhotoFiles::Import()
{
	m_ImportFlag.SetImportFlag( TRUE );
	ProcessPhotos ();
	ShowStatistics();
	return m_nUsed;													// return number of photo imported
}

//*******************************************************************

void CImportWondePhotoFiles::ProcessPhotos()
{
	CWaitDlg dlgWait ( m_pParent );
	dlgWait.SetMessageText ( "Checking for Wonde photo files" );

	CPhotoIDCopy photoID{};								// will create ecr download files if required

	CString strMask;
	strMask.Format ( "%s\\*.*", m_pData->m_strPhotoFolder );			// folder where photo are located

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

		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserIDString(strPhotoReference);
		
		CSQLRepositoryAccount RepoAccount;
		if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE )			// see if found 
		{
			CString strCardNo = RowAccount.GetUserIDString();

			CImageHandler image(FALSE);
			bool bReplacement = ( image.GetImageFilepath( strCardNo ) == "" ) ? FALSE : TRUE;

			CString strSourcePathname = FileFinder.GetFilePath();		// full name of Photo file

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

				if ( ( m_ImportFlag.GetImportFlag() == TRUE ) && ( m_pData->m_bDeletePhotoFile == TRUE ) )
				{
					CFileRecycler::SendToRecycleBin ( strSourcePathname );
				}
			}
		}
		else
		{
			++m_nIgnored;

			if ( m_ImportFlag.GetImportFlag() == FALSE  )
			{
				ShowImportLine ( strPhotoReference, "", FileFinder.GetFilePath(), FALSE );
			}
		}
	}
}

//**********************************************************************
