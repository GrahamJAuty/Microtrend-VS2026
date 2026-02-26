//*******************************************************************
#include "..\SPosLoyalty4Shared\FileRemover.h"
#include "..\SPosLoyalty4Shared\ImageHandler.h"
//*******************************************************************
#include "PhotoIDFilenameStack.h"
//*******************************************************************

CPhotoIDFilenameStack::CPhotoIDFilenameStack(void)
{
	m_strBaseFilename = "";
	m_strExt1 = "";
	m_strExt2 = "";
	m_strExt3 = "";
	m_strCardNo = "";
	m_bChange = FALSE;
}

//*******************************************************************

void CPhotoIDFilenameStack::Initialise( const char* szCardNo )
{
	m_strCardNo = szCardNo;

	CImageHandler ImageHandler;
	m_strBaseFilename = ImageHandler.GetImageFileTitle( m_strCardNo );
	
	m_strExt1 = "";
	m_strExt2 = "";
	m_strExt3 = "";

	CString strTemp = ImageHandler.GetFirst( m_strCardNo );

	if ( strTemp.GetLength() > 4 )
	{
		strTemp = strTemp.Right(4);
		if ( strTemp.Left(1) == "." )
		{	
			strTemp.MakeUpper();
			m_strExt1 = strTemp.Right(3);
		}
	}

	//FIDDLE 
	// ATTEMPT TO MITIGATE OLD BUG IN WHICH XX1 FILE COULD BE
	// LEFT BEHIND AFTER CHANGING IMAGE WITHOUT OTHER RECORD CHANGES
	if (m_strExt1 == "XX1")
	{
		if (rename(m_strBaseFilename + ".XX1", m_strBaseFilename + ".XX0 ") == 0)
		{
			m_strExt1 = "XX0";
		}
	}
}

//*******************************************************************

const char* CPhotoIDFilenameStack::GetImageFilename( int nLevel )
{
	m_strImageFilename = "";

	if ( ( nLevel >= 1 ) && ( nLevel <= 3 ) )
	{
		if ( 3 == nLevel )
		{
			if ( m_strExt3 != "" )
			{
				m_strImageFilename += m_strBaseFilename;
				m_strImageFilename += ".XX2";
			}
		}

		if ( nLevel >= 2 )
		{
			if ( ( m_strImageFilename == "" ) && ( m_strExt2 != "" ) )
			{
				m_strImageFilename += m_strBaseFilename;
				m_strImageFilename += ".XX1";
			}
		}

		if ( m_strImageFilename == "" )
		{
			if ( m_strExt1 != "" )
			{
				m_strImageFilename += m_strBaseFilename;
				m_strImageFilename += ".";
				m_strImageFilename += m_strExt1;
			}
		}
	}

	return m_strImageFilename;
}

//*******************************************************************

void CPhotoIDFilenameStack::ImportFile( int nLevel, const char* szFilename )
{
	if ( ( nLevel != 2 ) && ( nLevel != 3 ) )
		return;

	CString strFilename = szFilename;

	if ( strFilename.GetLength() < 5 )
		return;

	CString strExt = strFilename.Right(4);
	if ( strExt.Left(1) != "." )
		return;

	strExt = strExt.Right(3);
	strExt.MakeUpper();

	if ( 2 == nLevel )
	{
		::CopyFile( strFilename, m_strBaseFilename + ".XX1", FALSE );
		m_strExt2 = strExt;
	}
	else
	{
		::CopyFile( strFilename, m_strBaseFilename + ".XX2", FALSE );
		m_strExt3 = strExt;
	}
}

//*******************************************************************

void CPhotoIDFilenameStack::RemoveFile( int nLevel )
{
	if ( ( 3 == nLevel ) || ( 2 == nLevel ) )
	{
		CFileRemover FileRemover( m_strBaseFilename + ".XX2" );
		m_strExt3 = "";
	}

	if ( 2 == nLevel )
	{
		CFileRemover FileRemover( m_strBaseFilename + ".XX1" );
		m_strExt2 = "";
	}
}

//*******************************************************************

void CPhotoIDFilenameStack::PromoteFile( int nLevel )
{
	if ( ( 3 == nLevel ) && ( m_strExt3 != "" ) )
	{
		CopyFile( m_strBaseFilename + ".XX2", m_strBaseFilename + ".XX1", FALSE );
		m_strExt2 = m_strExt3;
		
		RemoveFile(3);
	}

	if ( ( 2 == nLevel ) && ( m_strExt2 != "" ) )
	{
		CString strDestFile = "";
		strDestFile = m_strBaseFilename;
		strDestFile += ".";
		strDestFile += m_strExt2;
		
		CopyFile( m_strBaseFilename + ".XX1", strDestFile, FALSE );
		m_strExt1 = m_strExt2;

		RemoveFile(2);
	}
}

//*******************************************************************

void CPhotoIDFilenameStack::RemoveOldFiles()
{
	CImageHandler ImageHandler;
	ImageHandler.DeleteAllExcept( m_strCardNo, m_strExt1 );
}

//*******************************************************************
