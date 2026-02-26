/**********************************************************************/
#include "SPOSKeyboardImageLibrary.h"
/**********************************************************************/

CSPOSKeyboardImageLibrary::CSPOSKeyboardImageLibrary(void) : m_arrayImageFilenames( FALSE, 1 )
{
	m_bGotFirstRefresh = FALSE;
}

/**********************************************************************/

void CSPOSKeyboardImageLibrary::Reset()
{
	m_arrayImageFilenames.RemoveAll();
}

/**********************************************************************/

void CSPOSKeyboardImageLibrary::Refresh( bool bForce )
{
	if ( ( TRUE == bForce ) || ( FALSE == m_bGotFirstRefresh ) )
	{
		m_bGotFirstRefresh = TRUE;
		m_arrayImageFilenames.RemoveAll();

		CString strPath = SPOSKeyboardManager.GetLayoutOptions() -> GetImageFolder();
		strPath += "\\";

		CFileFind FileFinder;
		bool bWorking = ( FileFinder.FindFile ( strPath + "*.*" ) != 0 );
		
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
			if ( strFilename.GetLength() < 5 || strFilename.GetLength() > MAX_BITMAP_FILENAME + 4 )
				continue;

			strFilename.MakeUpper();
			
			bool bAccept = FALSE;
			if ( strFilename.Right(4) == ".BMP" )		bAccept = TRUE;
			else if ( strFilename.Right(4) == ".GIF" )	bAccept = TRUE;
			else if ( strFilename.Right(4) == ".JPG" )	bAccept = TRUE;

			if ( bAccept == FALSE )
				continue;

			CSPOSKeyboardSortedString imageFilename;
			imageFilename.m_strItem = FileFinder.GetFileName();
			m_arrayImageFilenames.Consolidate( imageFilename );
		}
	}
}

/**********************************************************************/

const char* CSPOSKeyboardImageLibrary::GetFilename( int nIdx )
{
	m_strImageFilename = "";
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayImageFilenames.GetSize() ) )
	{
		CSPOSKeyboardSortedString item;
		m_arrayImageFilenames.GetAt( nIdx, item );
		m_strImageFilename = item.m_strItem;
	}
	return m_strImageFilename;
}

/**********************************************************************/

void CSPOSKeyboardImageLibrary::AddFilename( const char* sz )
{
	CSPOSKeyboardSortedString item;
	item.m_strItem = sz;
	m_arrayImageFilenames.Consolidate( item );
}

/**********************************************************************/

bool CSPOSKeyboardImageLibrary::FindFilename( const char* sz, int& nIdx )
{
	CSPOSKeyboardSortedString item;
	item.m_strItem = sz;
	return m_arrayImageFilenames.Find( item, nIdx );
}

/**********************************************************************/
