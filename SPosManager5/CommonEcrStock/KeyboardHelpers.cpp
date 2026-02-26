/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "FileRemover.h"
#include "LocationCSVArray.h"
#include "PriceSetItemManager.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "KeyboardHelpers.h"
/**********************************************************************/

void CKeyboardHelpers::CreatePluFile( int nDbIdx, int nLocIdx, CString& strPluFilename, bool bKbSet )
{
	strPluFilename = "";

	int nPriceSet = 0;

	if ( FALSE == bKbSet )
	{
		if ( EcrmanOptions.GetPluPriceSetsFlag() == TRUE )
			if ( EcrmanOptions.GetFeaturesKeyboardPriceSetFlag() == TRUE )
				nPriceSet = dbLocation.GetDefaultPriceSet(nLocIdx);
	}

	if ( nDbIdx < 0 || nDbIdx >= dbDatabase.GetSize() )
		return;

	if ( FALSE == bKbSet )
	{
		if ( nLocIdx < 0 || nLocIdx >= dbLocation.GetSize() )
			return;
	}

	CFilenameUpdater FnUp ( SysFiles::KeyboardPlu, nDbIdx, nPriceSet );
	strPluFilename = FnUp.GetFilenameToUse();
	
	if ( 0 == nPriceSet )
		if ( ::FileExists ( strPluFilename ) == TRUE )
			return;

	CSSFile fileKeyboardPlu;
	if ( fileKeyboardPlu.Open ( strPluFilename, "wb" ) == FALSE )
		return;

	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );

	if ( nPriceSet != 0 )
	{
		PriceSetItemManager.SelectPriceSet ( nLocIdx, nPriceSet );
		PriceSetItemManager.ReadPriceSetFile( FALSE, DataManager.Plu );
	}

	int nCount = DataManager.Plu.GetSize();

	StatusProgress.Lock( TRUE, "Creating keyboard PLU file" );
	for ( int nPluIdx = 0; nPluIdx < nCount; nPluIdx++ )
	{
		StatusProgress.SetPos ( nPluIdx, nCount );	
		
		CPluCSVRecord Record;
		DataManager.Plu.GetAt ( nPluIdx, Record );

		__int64 nPluNo = Record.GetPluNoInt();

		if ( SysInfo.IsModifiableSSPluNo( nPluNo ) == TRUE )
		{
			for ( int nMod = 0; nMod <= 9; nMod++ )
			{
				if ( ( nMod == 0 ) || Record.GetModifierEnable( nMod ) )
				{
					CCSV csv;
					csv.Add ( ( nPluNo * 10 ) + nMod );
					csv.Add ( 5 );
					csv.Add ( Record.GetModifierDeptNoForReport( nMod ) );
					csv.Add ( Record.GetModifierAnalysisCategoryForReport( nMod ) );

					{
						CString strText = Record.GetModifierEposText( nMod );
						if ( Record.GetModifierEposTextFlag( nMod ) == FALSE )
						{
							CString strCheck = Record.GetEposText();
							if ( strCheck == "" )
								strText = "";
						}
						csv.Add( strText );
					}

					{
						CString strText = Record.GetModifierKeyText( nMod );
						if ( Record.GetModifierKeyTextFlag( nMod ) == FALSE )
						{
							CString strCheck = Record.GetKeyText();
							if ( strCheck == "" )
								strText = "";
						}
						csv.Add( strText );
					}

					csv.Add ( Record.GetRepText() );
					csv.Add ( ( nMod == 0 ) ? "" : Record.GetEposText() );
					csv.Add ( Record.GetImageFilename() );
					fileKeyboardPlu.WriteLine ( csv.GetLine() );
				}
			}
		}
		else
		{
			CCSV csv;
			csv.Add ( nPluNo );
			csv.Add ( 5 );
			csv.Add ( Record.GetBaseDeptNo() );
			csv.Add ( Record.GetBaseAnalysisCategory() );
			csv.Add ( Record.GetEposText() );
			csv.Add ( Record.GetKeyText() );
			csv.Add ( Record.GetRepText() );
			csv.Add ( "" );
			csv.Add ( Record.GetImageFilename() );
			fileKeyboardPlu.WriteLine ( csv.GetLine() );
		}
	}
	StatusProgress.Unlock();
}

/**********************************************************************/

void CKeyboardHelpers::RemovePluFile ( int nDbIdx )
{
	if ( nDbIdx < 0 || nDbIdx >= dbDatabase.GetSize() )
		return;

	CFilenameUpdater FnUp ( SysFiles::KeyboardPlu, nDbIdx, 0 );
	CFileRemover FileRemover( FnUp.GetFilenameToUse() );
}

/**********************************************************************/

void CKeyboardHelpers::CloneFolder ( const char* szSource, const char* szDest )
{
	CString strSource = szSource;
	CString strDest = szDest;
	
	CFileRemover FileRemover;
	FileRemover.RemoveFolder( strDest );
	::CreateSubdirectory ( strDest );

	strSource += "\\";
	strDest += "\\";

	CFileFind FileFinder;	
	bool bWorking = ( FileFinder.FindFile ( strSource + "*.*" ) != 0 );

	while (bWorking)   
	{
		( bWorking = FileFinder.FindNextFile() != 0 );

		//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
		if ( FileFinder.IsDots() == TRUE )
			continue;

		//WE ONLY NEED TO CHECK FILES
		if ( FileFinder.IsDirectory() == TRUE )
			continue;

		CString strFilename = FileFinder.GetFileName();
		CopyFile ( strSource + strFilename, strDest + strFilename, FALSE );
	}
}

/**********************************************************************/
