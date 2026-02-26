/**********************************************************************/
 
/**********************************************************************/
#include "ButtonOptions.h"
#include "FileRemover.h"
//#include "globalfunctions.h"
/**********************************************************************/
#include "BackgroundLayoutInfo.h"
/**********************************************************************/

CBackgroundLayoutInfo::CBackgroundLayoutInfo(void)
{
	m_bGotValidLayout = FALSE;
	m_bTempImageMode = FALSE;
	m_nFrameToViewAdjustX = 0;
	m_nFrameToViewAdjustY = 0;
	m_nNonClientAdjustX = 0;
	m_nNonClientAdjustY = 0;
	m_nStatusBarHeight = 0;
}

/**********************************************************************/

CBackgroundLayoutInfo::~CBackgroundLayoutInfo(void)
{
	m_ImageUpper.DeleteOwnDC();
	m_ImageLower.DeleteOwnDC();
}

/**********************************************************************/

const char* CBackgroundLayoutInfo::GetError( int n )
{
	m_strError = "";
	if ( ( n >= 0 ) && ( n < m_arrayErrors.GetSize() ) )
		m_strError = m_arrayErrors.GetAt(n);

	return m_strError;
}

/**********************************************************************/

void CBackgroundLayoutInfo::Initialise( CDC* pDC )
{
	m_bGotValidLayout = FALSE;
	m_nUpperImageXOffset = 0;
	m_nLowerImageXOffset = 0;
	m_nUpperImageYOffset = 0;
	m_nLowerImageYOffset = 0;
	m_nTotalWindowWidth = 0;
	m_nUpperAreaHeight = 0;
	m_nLowerAreaHeight = 0;
	m_ImageUpper.CreateOwnDC( pDC );
	m_ImageLower.CreateOwnDC( pDC );

	m_strLayoutPath = "Layout\\";
	::GetSyssetProgramPath( m_strLayoutPath );
	m_strLayoutPathTemp = m_strLayoutPath + "Temp\\";
	m_strButtonImagePath = m_strLayoutPath + "ButtonImages\\";
	m_strButtonImagePathTemp = m_strButtonImagePath + "Temp\\";
}

/**********************************************************************/

void CBackgroundLayoutInfo::SetStatusBarHeight( int n )
{
	if ( n >= 0 )
		m_nStatusBarHeight = n;
}

/**********************************************************************/

void CBackgroundLayoutInfo::SetFrameToViewAdjustX( int n )
{
	if ( n >= 0 )
		m_nFrameToViewAdjustX = n;
}

/**********************************************************************/

void CBackgroundLayoutInfo::SetFrameToViewAdjustY( int n )
{
	if ( n >= 0 )
		m_nFrameToViewAdjustY = n;
}

/**********************************************************************/

void CBackgroundLayoutInfo::SetNonClientAdjustX( int n )
{
	if ( n >= 0 )
		m_nNonClientAdjustX = n;
}

/**********************************************************************/

void CBackgroundLayoutInfo::SetNonClientAdjustY( int n )
{
	if ( n >= 0 )
		m_nNonClientAdjustY = n;
}

/**********************************************************************/

int CBackgroundLayoutInfo::LoadUpperImage()
{
	return LoadImage( m_ImageUpper, ButtonOptions.GetUpperImageTypeInt(), "UPPER" );
}

/**********************************************************************/

int CBackgroundLayoutInfo::LoadLowerImage()
{
	return LoadImage( m_ImageLower, ButtonOptions.GetLowerImageTypeInt(), "LOWER" );
}

/**********************************************************************/

int CBackgroundLayoutInfo::LoadImage( CBitmapInfo& BitmapInfo, int nImageType, const char* szFilename )
{
	BitmapInfo.SetWidth(0);
	BitmapInfo.SetHeight(0);
	BitmapInfo.SetGotImageFlag( FALSE );

	CStringArray arrayExt;
	switch( nImageType )
	{
	case 1:	
		arrayExt.Add( ".GIF" );	
		break;

	case 2:	
		arrayExt.Add( ".JPG" );	
		break;

	case 3:
		arrayExt.Add( ".BMP" );	
		break;

	case 4:	
		arrayExt.Add( ".PNG" );
		break;
	}

	if ( arrayExt.GetSize() == 0 )
		return BACKIMAGE_ERROR_NONE;

	if ( FALSE == m_ImageUpper.CreatedDC() )
		return BACKIMAGE_ERROR_NODC;

	CString strCommonPath = ( m_bTempImageMode ) ? m_strLayoutPathTemp : m_strLayoutPath;
	strCommonPath += szFilename;
	
	bool bFoundFile = FALSE;
	CString strFullPath = "";
	for ( int n = 0; n < arrayExt.GetSize(); n++ )
	{
		strFullPath = strCommonPath + arrayExt.GetAt(n);

		if ( ::FileExists( strFullPath ) == TRUE )
		{
			bFoundFile = TRUE;
			break;
		}
	}
	
	int nResult = 0;
	if ( TRUE == bFoundFile )
	{
		CImage Image;
		if ( ( Image.Load( strFullPath ) & 0x80000000 ) != 0 )
		{
			nResult = BACKIMAGE_ERROR_LOAD;
		}
		else
		{
			nResult = BACKIMAGE_ERROR_NONE;
			
			BitmapInfo.SetWidth ( Image.GetWidth() );
			BitmapInfo.SetHeight ( Image.GetHeight() );
			BitmapInfo.SetGotImageFlag ( TRUE );
			
			CBitmap bm; 
			bm.Attach( Image.Detach() );
			BitmapInfo.GetDCImage() -> SelectObject( &bm );
		}
	}
	else
	{
		nResult = BACKIMAGE_ERROR_NOFILE;
	}

	return nResult;
}

/**********************************************************************/

bool CBackgroundLayoutInfo::ValidateLayout()
{
	m_bGotValidLayout = FALSE;
	
	ClearErrors();

	int nScreenWidth = GetSystemMetrics( SM_CXSCREEN );
	int nScreenHeight = GetSystemMetrics( SM_CYSCREEN );
	int nTaskbarGap = ButtonOptions.GetLowerAreaTaskbarGap();
	
	if ( ButtonOptions.GetWindowWidthAuto() == TRUE )
		m_nTotalWindowWidth = nScreenWidth - ( m_nFrameToViewAdjustX + m_nNonClientAdjustX );
	else
		m_nTotalWindowWidth = ButtonOptions.GetWindowWidthManual();
	
	int nUpperLeftMargin = ButtonOptions.GetUpperImageMarginLeft();
	int nUpperRightMargin = ButtonOptions.GetUpperImageMarginRight();
	int nLowerLeftMargin = ButtonOptions.GetLowerImageMarginLeft();
	int nLowerRightMargin = ButtonOptions.GetLowerImageMarginRight();
	
	if ( m_nTotalWindowWidth + m_nFrameToViewAdjustX + m_nNonClientAdjustX > nScreenWidth )
	{
		AddError( "Requested window size is too wide for display" );
		return FALSE;
	}
	
	if ( nUpperLeftMargin + nUpperRightMargin + 50 > m_nTotalWindowWidth )
	{
		AddError( "Upper area margins are too wide" );
		return FALSE;
	}

	if ( nLowerLeftMargin + nLowerRightMargin + 50 > m_nTotalWindowWidth )
	{
		AddError( "Lower area margins are too wide" );
		return FALSE;
	}

	m_nLowerAreaHeight = ButtonOptions.GetLowerAreaHeightManual();

	if ( ButtonOptions.GetUpperAreaHeightAuto() == TRUE )
		m_nUpperAreaHeight = nScreenHeight - ( m_nLowerAreaHeight + m_nFrameToViewAdjustY + m_nNonClientAdjustY + nTaskbarGap + m_nStatusBarHeight );
	else
		m_nUpperAreaHeight = ButtonOptions.GetUpperAreaHeightManual();
		
	int nUpperTopMargin = ButtonOptions.GetUpperImageMarginTop();
	int nUpperBottomMargin = ButtonOptions.GetUpperImageMarginBottom();
	int nLowerTopMargin = ButtonOptions.GetLowerImageMarginTop();
	int nLowerBottomMargin = ButtonOptions.GetLowerImageMarginBottom();
		
	if ( m_nUpperAreaHeight + m_nLowerAreaHeight + m_nFrameToViewAdjustY + m_nNonClientAdjustY + nTaskbarGap + m_nStatusBarHeight > nScreenHeight  )
	{
		AddError( "Requested window size is too tall for display" );
		return FALSE;
	}

	if ( nUpperTopMargin + nUpperBottomMargin + 50 > m_nUpperAreaHeight )
	{
		AddError( "Upper area margins are too tall" );
		return FALSE;
	}

	if ( nLowerTopMargin + nLowerBottomMargin + 50 > m_nLowerAreaHeight )
	{
		AddError( "Lower area margins are too tall" );
		return FALSE;
	}
	
	if ( TRUE == m_ImageUpper.GotImage() )
	{
		if ( m_ImageUpper.GetWidth() > m_nTotalWindowWidth - ( nUpperLeftMargin + nUpperRightMargin ) )
		{
			AddError( "Upper image is too wide" );
			m_ImageUpper.SetGotImageFlag( FALSE );
		}
		else if ( m_ImageUpper.GetHeight() > m_nUpperAreaHeight - ( nUpperTopMargin + nUpperBottomMargin ) )
		{
			AddError( "Upper image is too tall" );
			m_ImageUpper.SetGotImageFlag( FALSE );
		}
	}

	if ( TRUE == m_ImageUpper.GotImage() )
	{
		switch( ButtonOptions.GetUpperImageXAlignInt() )
		{
		case 0:
			m_nUpperImageXOffset = nUpperLeftMargin;
			break;

		case 2:
			m_nUpperImageXOffset = m_nTotalWindowWidth - ( nUpperRightMargin + m_ImageUpper.GetWidth() );
			break;

		case 1:
		default:
			m_nUpperImageXOffset = ( nUpperLeftMargin + ( m_nTotalWindowWidth - nUpperRightMargin ) - m_ImageUpper.GetWidth() ) / 2;
			break;
		}

		switch( ButtonOptions.GetUpperImageYAlignInt() )
		{
		case 0:
			m_nUpperImageYOffset = nUpperTopMargin;
			break;

		case 2:
			m_nUpperImageYOffset = m_nUpperAreaHeight - ( nUpperBottomMargin + m_ImageUpper.GetHeight() );
			break;

		case 1:
		default:
			m_nUpperImageYOffset = ( nUpperTopMargin + ( m_nUpperAreaHeight - nUpperBottomMargin ) - m_ImageUpper.GetHeight() ) / 2;
			break;
		}
	}

	if ( TRUE == m_ImageLower.GotImage() )
	{
		if ( m_ImageLower.GetWidth() > m_nTotalWindowWidth - ( nLowerLeftMargin + nLowerRightMargin ) )
		{
			AddError( "Lower image is too wide" );
			m_ImageLower.SetGotImageFlag( FALSE );
		}
		else if ( m_ImageLower.GetHeight() > m_nLowerAreaHeight - ( nLowerTopMargin + nLowerBottomMargin ) )
		{
			AddError( "Lower image is too tall" );
			m_ImageLower.SetGotImageFlag( FALSE );
		}
	}

	if ( TRUE == m_ImageLower.GotImage() )
	{
		switch( ButtonOptions.GetLowerImageXAlignInt() )
		{
		case 0:
			m_nLowerImageXOffset = nLowerLeftMargin;
			break;

		case 2:
			m_nLowerImageXOffset = m_nTotalWindowWidth - ( nLowerRightMargin + m_ImageLower.GetWidth() );
			break;

		case 1:
		default:
			m_nLowerImageXOffset = ( nLowerLeftMargin + ( m_nTotalWindowWidth - nLowerRightMargin ) - m_ImageLower.GetWidth() ) / 2;
			break;
		}

		switch( ButtonOptions.GetLowerImageYAlignInt() )
		{
		case 0:
			m_nLowerImageYOffset = nLowerTopMargin;
			break;

		case 2:
			m_nLowerImageYOffset = m_nLowerAreaHeight - ( nLowerBottomMargin + m_ImageLower.GetHeight() );
			break;

		case 1:
		default:
			m_nLowerImageYOffset = ( nLowerTopMargin + ( m_nLowerAreaHeight - nLowerBottomMargin ) - m_ImageLower.GetHeight() ) / 2;
			break;
		}
	}
	
	m_bGotValidLayout = TRUE;
	return TRUE;
}

/**********************************************************************/

void CBackgroundLayoutInfo::TempImageModeOn()
{
	CreateLayoutFolders();
	
	CStringArray arrayFiles;
	arrayFiles.Add( "UPPER.BMP" );
	arrayFiles.Add( "UPPER.GIF" );
	arrayFiles.Add( "UPPER.JPG" );
	arrayFiles.Add( "UPPER.PNG" );
	arrayFiles.Add( "LOWER.BMP" );
	arrayFiles.Add( "LOWER.GIF" );
	arrayFiles.Add( "LOWER.JPG" );
	arrayFiles.Add( "LOWER.PNG" );

	CFileRemover FileRemover;
	for ( int n = 0; n < arrayFiles.GetSize(); n++ )
	{
		CString strFilename = arrayFiles.GetAt(n);
		if ( ::FileExists( m_strLayoutPath + strFilename ) == TRUE )
			CopyFile( m_strLayoutPath + strFilename, m_strLayoutPathTemp + strFilename, FALSE );
		else
			FileRemover.RemoveFile( m_strLayoutPathTemp + strFilename );
	}
	
	m_bTempImageMode = TRUE;
}

/**********************************************************************/

void CBackgroundLayoutInfo::TempImageModeOff( bool bSave )
{
	CreateLayoutFolders();
	
	CStringArray arrayFiles;
	arrayFiles.Add( "UPPER.BMP" );
	arrayFiles.Add( "UPPER.GIF" );
	arrayFiles.Add( "UPPER.JPG" );
	arrayFiles.Add( "UPPER.PNG" );
	arrayFiles.Add( "LOWER.BMP" );
	arrayFiles.Add( "LOWER.GIF" );
	arrayFiles.Add( "LOWER.JPG" );
	arrayFiles.Add( "LOWER.PNG" );

	CFileRemover FileRemover;

	if ( TRUE == bSave )
	{
		for ( int n = 0; n < arrayFiles.GetSize(); n++ )
		{
			CString strFilename = arrayFiles.GetAt(n);
			if ( ::FileExists( m_strLayoutPathTemp + strFilename ) == TRUE )
				CopyFile( m_strLayoutPathTemp + strFilename, m_strLayoutPath + strFilename, FALSE );
			else
				FileRemover.RemoveFile( m_strLayoutPath + strFilename );
		}
	}

	for ( int n = 0; n < arrayFiles.GetSize(); n++ )
	{
		CString strFilename = arrayFiles.GetAt(n);
		FileRemover.RemoveFile( m_strLayoutPathTemp + strFilename );
	}
	
	m_bTempImageMode = FALSE;
}

/**********************************************************************/

void CBackgroundLayoutInfo::CreateLayoutFolders()
{
	::MakeSubdirectory( m_strLayoutPath );
	::MakeSubdirectory( m_strLayoutPathTemp );
	::MakeSubdirectory( m_strButtonImagePath );
	::MakeSubdirectory( m_strButtonImagePathTemp );
}

/**********************************************************************/
