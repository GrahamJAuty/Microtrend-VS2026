//**************************************************************************
#include "DatabaseImport.h"
#include "ImportSimsFile.h"
#include "ImportSimsPhotoFiles.h"
#include "PhotoIDCopy.h"
#include "SimsData.h"
#include "WondeData.h"
#include "ImportWondeFile.h"
#include "ImportWondePhotoFiles.h"
/**********************************************************************/
#include "AutoImportHandler.h"
//**************************************************************************

CAutoImportHandler::CAutoImportHandler ( CWnd* pParent )
{
	m_pParent = pParent;
	m_bBackgroundMode = FALSE;
	m_data.Read();
}

//******************************************************************
// Automatic account import file ( CSV \ SIMS format \ Wonde \ Custom )

int CAutoImportHandler::ImportAccounts()
{
	int nUsedCounter = 0;

	if ( ::FileExists ( m_data.m_strImportFilename ) == TRUE )								// see if import file exists
	{
		CString strImportFile = Filenames.GetImportsPath ( GetUniqueTempFilename("$$5") );	// rename & move import file
		if ( ::rename ( m_data.m_strImportFilename, strImportFile ) == 0 )
		{
			if ( m_data.m_nFileFormat == nIMPORT_FORMAT_SIMS )						//SIMS import ********
			{	
				CSimsData sims;
				sims.Read();

				CImportSimsFile simsFile ( &sims, m_pParent );
				simsFile.SetImportMethod ( m_data.m_nImportMethod );
				simsFile.Import ( strImportFile );
				nUsedCounter = simsFile.GetUsedCount();
			}

			else if ( m_data.m_nFileFormat == nIMPORT_FORMAT_WONDE )			//Wonde import ********
			{	
				CWondeData wonde;
				wonde.Read();

				CImportWondeFile wondeFile ( &wonde, m_pParent );
				wondeFile.SetImportMethod ( m_data.m_nImportMethod );
				wondeFile.Import ( strImportFile );
				nUsedCounter = wondeFile.GetUsedCount();
			}

			else																	// csv \ custom format ******
			{
				CSQLImportCounters Counters;
				CImportDatabase import ( m_pParent );
				import.AutoImportDatabase ( m_data.m_nFileFormat, strImportFile, m_data.m_nImportMethod, Counters );
				nUsedCounter = Counters.GetUsed();
			}

			CFileRecycler::SendToRecycleBin ( strImportFile );									//06/07/2016
		}
	}
	else
	{
		if ( m_bBackgroundMode == FALSE )
		{
			CString strMsg;		
			strMsg.Format ( "' %s ' not found!", (const char*) m_data.m_strImportFilename );
			Prompter.Error ( strMsg, "Import Auto File" ); 
		}
	}

	return nUsedCounter;
}

//******************************************************************

int CAutoImportHandler::ImportPhotoIDs()
{
	int nUsedCounter = 0;

	if ( m_data.m_bImportPhotoID == TRUE )
	{
		if ( m_data.m_nFileFormat == nIMPORT_FORMAT_SIMS )
		{	
			CSimsData sims;
			sims.Read();

			CImportSimsPhotoFiles photo ( &sims, m_pParent );
			if ( photo.Import() == TRUE )									// import photo & copy to PhotoID folder & Epos folder
				nUsedCounter = photo.GetUsedCount();						// number of photos used
		}

		else if ( m_data.m_nFileFormat == nIMPORT_FORMAT_WONDE )
		{	
			CWondeData data;
			data.Read();

			CImportWondePhotoFiles photo ( &data, m_pParent );
			nUsedCounter = photo.Import();									// import photo & copy to PhotoID folder & Epos folder
		}

		else
		{
			CPhotoIDCopy photo{};							// see if any photos to import from ..\Imports\PhotoID folder
			nUsedCounter = photo.Import();				// also copy to epos folder	if required
		}
	}

	return nUsedCounter;
}

//******************************************************************
