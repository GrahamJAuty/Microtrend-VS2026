/**********************************************************************/
#include "FileRemover.h"
#include "LocationCSVArray.h"
#include "PluDatabaseHeader.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "PriceSetHeaderManager.h"
/**********************************************************************/

CPriceSetHeaderManager::CPriceSetHeaderManager()
{
	m_arrayFields.RemoveAll();
	m_strCurrentHeader = "";
	m_strNewHeader = "";
}

/**********************************************************************/

void CPriceSetHeaderManager::LoadHeaderForLocationEdit( int nLocIdx )
{
	m_arrayFields.RemoveAll();
	m_strCurrentHeader = "";
	m_strNewHeader = "";
		
	bool bIgnoreRequest = FALSE;
	if (nLocIdx < 0 || nLocIdx >= dbLocation.GetSize())
	{
		bIgnoreRequest = TRUE;
	}
	else if (EcrmanOptions.GetPluPriceSetsFlag() == FALSE)
	{
		bIgnoreRequest = TRUE;
	}

	if (bIgnoreRequest == TRUE)
	{
		return;
	}

	CString strFieldFilename = dbLocation.GetFilePathDataLocalPluField( nLocIdx, 1, TRUE );

	CSSFile fileHeader; 
	if (fileHeader.Open(strFieldFilename, "rb") == FALSE)
	{
		return;
	}

	CString strHeader = "";
	fileHeader.ReadString ( strHeader );
	fileHeader.Close();

	strHeader = PluDatabaseHeader.GetUpdatedHeader( strHeader );

	::TrimSpaces ( strHeader, FALSE );
	CCSV csvIn ( strHeader );
	CCSV csvOut;

	for ( int nIndex = 0; nIndex < csvIn.GetSize(); nIndex++ )
	{
		CString strField = csvIn.GetString(nIndex);
		::TrimSpaces ( strField, FALSE );
		
		if ( strField != "" )
		{
			csvOut.Add ( strField );
			strField.MakeUpper();

			CSortedStringItem item;
			item.m_strItem = strField;
			m_arrayFields.Consolidate ( item );
		}
	}

	m_strCurrentHeader = csvOut.GetLine();
	m_strNewHeader = csvOut.GetLine();
}

/**********************************************************************/

void CPriceSetHeaderManager::LoadHeaderForDatabaseEdit(int nDbIdx, int nSet)
{
	m_arrayFields.RemoveAll();
	m_strCurrentHeader = "";
	m_strNewHeader = "";

	bool bIgnoreRequest = FALSE;
	if (nDbIdx < 0 || nDbIdx >= dbDatabase.GetSize())
	{
		bIgnoreRequest = TRUE;
	}
	else if (EcrmanOptions.GetPluPriceSetsFlag() == FALSE)
	{
		bIgnoreRequest = TRUE;
	}
	else if ((nSet < 1 )|| (nSet > 20))
	{
		bIgnoreRequest = TRUE;
	}
	
	if (bIgnoreRequest == TRUE)
	{
		return;
	}

	CString strFieldFilename = dbDatabase.GetFilePathDataLocalPluField( nDbIdx, nSet );
	
	CSSFile fileHeader; 
	if (fileHeader.Open(strFieldFilename, "rb") == FALSE)
	{
		return;
	}

	CString strHeader = "";
	fileHeader.ReadString ( strHeader );
	fileHeader.Close();

	strHeader = PluDatabaseHeader.GetUpdatedHeader( strHeader );

	::TrimSpaces ( strHeader, FALSE );
	CCSV csvIn ( strHeader );
	CCSV csvOut;

	for ( int nIndex = 0; nIndex < csvIn.GetSize(); nIndex++ )
	{
		CString strField = csvIn.GetString(nIndex);
		::TrimSpaces ( strField, FALSE );
		
		if ( strField != "" )
		{
			csvOut.Add ( strField );
			strField.MakeUpper();

			CSortedStringItem item;
			item.m_strItem = strField;
			m_arrayFields.Consolidate ( item );
		}
	}

	m_strCurrentHeader = csvOut.GetLine();
	m_strNewHeader = csvOut.GetLine();
}

/**********************************************************************/

void CPriceSetHeaderManager::CheckHeaderChanges ( bool& bAdded, bool& bRemoved, bool& bWasEmpty )
{
	bAdded = FALSE;
	bRemoved = FALSE;
	bWasEmpty = ( m_strCurrentHeader == "" );
	
	if (m_strCurrentHeader == m_strNewHeader)
	{
		return;
	}

	if ( m_strCurrentHeader == "" && m_strNewHeader != "" )
	{
		bAdded = TRUE;
		return;
	}

	if ( m_strCurrentHeader != "" && m_strNewHeader == "" )
	{
		bRemoved = TRUE;
		return;
	}

	CReportConsolidationArray<CSortedStringItem> CurrentHeader, NewHeader;
	CCSV csvCurrent ( m_strCurrentHeader );
	CCSV csvNew ( m_strNewHeader );

	for ( int nIndex = 0; nIndex < csvCurrent.GetSize(); nIndex++ )
	{
		CSortedStringItem item;
		item.m_strItem = csvCurrent.GetString(nIndex);
		CurrentHeader.Consolidate( item );
	}

	for ( int nIndex = 0; nIndex < csvNew.GetSize(); nIndex++ )
	{
		CSortedStringItem item;
		item.m_strItem = csvNew.GetString(nIndex);
		NewHeader.Consolidate( item );
	}

	int nPos = 0;

	for ( int nIndex = 0; nIndex < CurrentHeader.GetSize(); nIndex++ )
	{
		CSortedStringItem item;
		CurrentHeader.GetAt( nIndex, item );

		if ( NewHeader.Find ( item, nPos ) == FALSE )
		{
			bRemoved = TRUE;
			break;
		}
	}

	for ( int nIndex = 0; nIndex < NewHeader.GetSize(); nIndex++ )
	{
		CSortedStringItem item;
		NewHeader.GetAt( nIndex, item );

		if ( CurrentHeader.Find ( item, nPos ) == FALSE )
		{
			bAdded = TRUE;
			break;
		}
	}
}

/**********************************************************************/

void CPriceSetHeaderManager::UpdateLocationSpecificPriceSetFiles ( int nLocIdx, int nLocalPluSetCount )
{	
	StatusProgress.Lock( TRUE, "Updating price set files" );

	for ( int nSet = 1; nSet <= nLocalPluSetCount; nSet++ )
	{
		CString strSourceFilename = dbLocation.GetFilePathDataLocalPluField ( nLocIdx, nSet, TRUE );
		CString strDestFilename = dbLocation.GetFilePathDataLocalPluFieldTemp ( nLocIdx, nSet, TRUE );
		
		if ( m_strNewHeader == "" )
		{
			CFileRemover FileRemover;
			FileRemover.RemoveFile ( strSourceFilename );
			FileRemover.RemoveFile ( strDestFilename );
		}
		else
		{			
			CSSFile fileDest;
			if ( fileDest.Open ( strDestFilename, "wb" ) == FALSE )
				continue;
			
			fileDest.WriteLine ( m_strNewHeader );

			CSSImportFile fileSource ( PluDatabaseHeader.GetCompatibilityHeader( strSourceFilename, m_strNewHeader ) );
			if ( fileSource.Open ( strSourceFilename, "rb" ) == TRUE )
			{
				CString str = "";
				str.Format( "(%s, set %d)", dbLocation.GetName( nLocIdx ), nSet );
				StatusProgress.SetDialogText( str );

				CString strData = "";
				while ( fileSource.ReadData ( strData ) == TRUE )
				{
					StatusProgress.SetPos ( fileSource );
					fileDest.WriteLine ( strData );
				}
				
				fileSource.Close();
			}
			fileDest.Close();			
			CFileRemover FileRemover ( strSourceFilename );
			rename ( strDestFilename, strSourceFilename );
		}
	}
	
	StatusProgress.Unlock();	
}

/**********************************************************************/

void CPriceSetHeaderManager::CreateLocationSpecificPriceSetFiles( int nLocIdx, int nLocalPluSetCount )
{
	for ( int nSet = 1; nSet <= nLocalPluSetCount; nSet++ )
	{
		CString strTempFilename = dbLocation.GetFilePathDataLocalPluFieldTemp ( nLocIdx, nSet, TRUE );
		CString strFieldFilename = dbLocation.GetFilePathDataLocalPluField ( nLocIdx, nSet, TRUE );
		
		if ( m_strNewHeader == "" )
		{
			CFileRemover FileRemover;
			FileRemover.RemoveFile ( strTempFilename );
			FileRemover.RemoveFile ( strFieldFilename );
		}
		else
		{	
			if (::FileExists(strFieldFilename) == FALSE)
			{
				CSSFile fileField;
				if (fileField.Open(strFieldFilename, "wb") == TRUE)
				{
					fileField.WriteLine(m_strNewHeader);
				}
			}
		}
	}
}

/**********************************************************************/

void CPriceSetHeaderManager::UpdateDatabasePriceSetFile ( int nDbIdx, int nSet )
{	
	if ((nSet < 1) || (nSet > 20))
	{
		return;
	}

	CString strSourceFilename = dbDatabase.GetFilePathDataLocalPluField ( nDbIdx, nSet );
	CString strDestFilename = dbDatabase.GetFilePathDataLocalPluTemp ( nDbIdx, nSet );
		
	if ( m_strNewHeader == "" )
	{
		CFileRemover FileRemover;
		FileRemover.RemoveFile ( strSourceFilename );
		FileRemover.RemoveFile ( strDestFilename );
	}
	else
	{		
		CSSFile fileDest;
		if (fileDest.Open(strDestFilename, "wb") == FALSE)
		{
			return;
		}

		fileDest.WriteLine ( m_strNewHeader );

		CSSImportFile fileSource ( PluDatabaseHeader.GetCompatibilityHeader( strSourceFilename, m_strNewHeader ) );
		if ( fileSource.Open ( strSourceFilename, "rb" ) == TRUE )
		{
			CString strData = "";

			StatusProgress.Lock( TRUE, "Updating price set file" );
			
			CString str = "";
			str.Format ( "(%s, set %d)", dbDatabase.GetName( nDbIdx ), nSet );
			StatusProgress.SetDialogText( str );

			while ( fileSource.ReadData ( strData ) == TRUE )
			{
				StatusProgress.SetPos ( fileSource );
				fileDest.WriteLine ( strData );
			}
			StatusProgress.Unlock();
				
			fileSource.Close();
		}

		fileDest.Close();			
		CFileRemover FileRemover ( strSourceFilename );
		rename ( strDestFilename, strSourceFilename );
	}
}

/**********************************************************************/

void CPriceSetHeaderManager::CreateDatabasePriceSetFile( int nDbIdx, int nSet )
{
	if ((nSet < 1) || (nSet > 20))
	{
		return;
	}

	CString strTempFilename = dbDatabase.GetFilePathDataLocalPluTemp ( nDbIdx, nSet );
	CString strFieldFilename = dbDatabase.GetFilePathDataLocalPluField ( nDbIdx, nSet );
		
	if ( m_strNewHeader == "" )
	{
		CFileRemover FileRemover;
		FileRemover.RemoveFile ( strTempFilename );
		FileRemover.RemoveFile ( strFieldFilename );
	}
	else
	{	
		if (::FileExists(strFieldFilename) == FALSE)
		{
			CSSFile fileField;
			if (fileField.Open(strFieldFilename, "wb") == TRUE)
			{
				fileField.WriteLine(m_strNewHeader);
			}
		}
	}
}

/**********************************************************************/

