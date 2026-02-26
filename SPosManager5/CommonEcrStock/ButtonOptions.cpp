/**********************************************************************/
//#include "globalfunctions.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ButtonOptions.h"
/**********************************************************************/

CButtonOptions::CButtonOptions() 
{
	m_LayoutEnable.Create				( "LayoutEnable", FALSE );
	/*****/
	m_WindowWidthAuto.Create			( "WindowWidthAuto", TRUE );
	m_WindowWidthManual.Create			( "WindowWidth", 1000, 3000, 1000 );
	/*****/
	m_UpperAreaHeightAuto.Create		( "UpperAreaHeightAuto", TRUE );
	m_UpperAreaHeightManual.Create		( "UpperAreaHeight", 400, 3000, 400 );
	m_UpperAreaFillColour.Create		( "UpperAreaFillColour", 6, "FFFFFF" );
	/*****/
	m_UpperImageType.Create				( "UpperImageType", 4, "NONE" );
	m_UpperImageMarginTop.Create		( "UpperImageMarginTop", 0, 999, 0 );
	m_UpperImageMarginBottom.Create		( "UpperImageMarginBottom", 0, 999, 0 );
	m_UpperImageMarginLeft.Create		( "UpperImageMarginLeft", 0, 999, 0 );
	m_UpperImageMarginRight.Create		( "UpperImageMarginRight", 0, 999, 0 );
	m_UpperImageXAlign.Create			( "UpperImageXAlign", 6, "CENTRE" );
	m_UpperImageYAlign.Create			( "UpperImageYAlign", 6, "CENTRE" );
	m_UpperImageStretch.Create			( "UpperImageStretch", FALSE );
	/*****/
	m_LowerAreaHeightManual.Create		( "LowerAreaHeight", 100, 400, 200 );
	m_LowerAreaFillColour.Create		( "LowerAreaFillColour", 6, "FFFFFF" );
	m_LowerAreaTaskbarGap.Create		( "TaskbarGap", 0, 150, 150 );
	/*****/
	m_LowerImageType.Create				( "LowerImageType", 4, "NONE" );
	m_LowerImageMarginTop.Create		( "LowerImageMarginTop", 0, 999, 0 );
	m_LowerImageMarginBottom.Create		( "LowerImageMarginBottom", 0, 999, 0 );
	m_LowerImageMarginLeft.Create		( "LowerImageMarginLeft", 0, 999, 0 );
	m_LowerImageMarginRight.Create		( "LowerImageMarginRight", 0, 999, 0 );
	m_LowerImageXAlign.Create			( "LowerImageXAlign", 6, "CENTRE" );
	m_LowerImageYAlign.Create			( "LowerImageYAlign", 6, "CENTRE" );
	m_LowerImageStretch.Create			( "LowerImageStretch", FALSE );
	/*****/
	m_ButtonBarHome.Create				( "ButtonBarHome", 11, "BOTTOMLEFT" );
	m_ButtonBarDirection.Create			( "ButtonBarDirection", 5, "RIGHT" );
	m_ButtonBarControlLinePos.Create	( "ButtonBarControlLinePos", 6, "AFTER" );
	m_ButtonBarControlButtonPos.Create	( "ButtonBarControlButtonPos", 5, "END" );
	m_ButtonBarParentButton.Create		( "ButtonBarParentButton", TRUE );
	m_ButtonBarCentre.Create			( "ButtonBarCentre", TRUE );
	m_ButtonBarMarginTop.Create			( "ButtonBarMarginTop", 0, 999, 0 );
	m_ButtonBarMarginBottom.Create		( "ButtonBarMarginBottom", 0, 999, 0 );
	m_ButtonBarMarginLeft.Create		( "ButtonBarMarginLeft", 0, 999, 0 );
	m_ButtonBarMarginRight.Create		( "ButtonBarMarginRight", 0, 999, 0 );
	m_ButtonBarLineSize.Create			( "ButtonBarLineSize", 3, 10, 5 );
	m_ButtonBarLineCount.Create			( "ButtonBarLineCount", 2, 5, 2 );
	/*****/
	m_ButtonWidthType.Create			( "ButtonWidthType", 0, 1, 0 );
	m_ButtonWidthPercent.Create			( "ButtonWidthPercent", 0, 99, 60 );
	m_ButtonWidthPixel.Create			( "ButtonWidthPixel", 100, 300, 100 );
	m_ButtonHeightType.Create			( "ButtonHeightType", 0, 1, 0 );
	m_ButtonHeightPercent.Create		( "ButtonHeightPercent", 0, 99, 60 );
	m_ButtonHeightPixel.Create			( "ButtonHeightPixel", 100, 300, 100 ); 
	m_ButtonXGap.Create					( "ButtonXGap", 5, 50, 20 );
	m_ButtonYGap.Create					( "ButtonYGap", 5, 50, 20 );
	m_ButtonRounding.Create				( "ButtonRounding", 0, 5, 3 );
	m_ButtonColourBackAction.Create		( "ButtonColourBackAction", 6, "00CC00" );
	m_ButtonColourBackList.Create		( "ButtonColourBackList", 6, "CCCC00" );
	m_ButtonColourBackUnused.Create		( "ButtonColourBackUnused", 6, "777777" );
	m_ButtonColourBackControl.Create	( "ButtonColourBackControl", 6, "00CCCC" );
	m_ButtonColourBackParent.Create		( "ButtonColourBackParent", 6, "CCCCCC" );
	m_ButtonColourTextAction.Create		( "ButtonColourTextAction", 6, "000000" );
	m_ButtonColourTextList.Create		( "ButtonColourTextList", 6, "000000" );
	m_ButtonColourTextControl.Create	( "ButtonColourTextControl", 6, "000000" );
	m_ButtonColourTextParent.Create		( "ButtonColourTextParent", 6, "000000" );
	/*****/
	m_ButtonImageTopMarginPercent.Create( "ButtonImageTopMarginPercent", 0, 20, 10 );
	m_ButtonImageBottomMarginPercent.Create( "ButtonImageBottomMarginPercent", 0, 40, 10 );
	m_ButtonImageLeftMarginPercent.Create( "ButtonImageLeftMarginPercent", 0, 20, 10 );
	m_ButtonImageRightMarginPercent.Create( "ButtonImageRightMarginPercent", 0, 20, 10 );

	Reset();
	m_bFatalReadError = FALSE;
}

/**********************************************************************/

void CButtonOptions::CopyFrom( CButtonOptions& source )
{
	CEnhancedIniFile iniFile;
	source.PrepareIniFile( iniFile );
	ReloadFromIniFile( iniFile );
}

/**********************************************************************/

int CButtonOptions::MakeColourInt( CIniFileString& iniString )
{
	int nResult = 0xFFFFFF;

	CString strColour = iniString.GetValue();
	if ( strColour.GetLength() == 6 )
	{
		int n = ::HexToInt( strColour.Right(2) + strColour.Mid(2,2) + strColour.Left(2) );
		if ( ( n >= 0 ) && ( n <= 0xFFFFFF ) )
			nResult = n;
	}

	return nResult;
}

/**********************************************************************/

bool CButtonOptions::MakeColourString( int nColour, CString& strColour )
{
	bool bResult = FALSE;
	if ( ( nColour >= 0 ) && ( nColour <= 0xFFFFFF ) )
	{
		CString strHex;
		strHex.Format( "%6.6X", nColour );
		strColour = strHex.Right(2) + strHex.Mid(2,2) + strHex.Left(2);
		bResult = TRUE;
	}
	return bResult;
}

/**********************************************************************/

void CButtonOptions::SetUpperAreaFillColourInt( int n )
{
	CString strColour;
	if ( MakeColourString( n, strColour ) == TRUE )
	{
		m_UpperAreaFillColour.SetValue( strColour );
		m_nUpperAreaFillColour = MakeColourInt( m_UpperAreaFillColour );
	}
}

/**********************************************************************/

void CButtonOptions::SetLowerAreaFillColourInt( int n )
{
	CString strColour;
	if ( MakeColourString( n, strColour ) == TRUE )
	{
		m_LowerAreaFillColour.SetValue( strColour );
		m_nLowerAreaFillColour = MakeColourInt( m_LowerAreaFillColour );
	}
}

/**********************************************************************/

void CButtonOptions::SetButtonColourBackActionInt( int n )
{
	CString strColour;
	if ( MakeColourString( n, strColour ) == TRUE )
	{
		m_ButtonColourBackAction.SetValue( strColour );
		m_nButtonColourBackAction = MakeColourInt( m_ButtonColourBackAction );
	}
}

/**********************************************************************/

void CButtonOptions::SetButtonColourBackListInt( int n )
{
	CString strColour;
	if ( MakeColourString( n, strColour ) == TRUE )
	{
		m_ButtonColourBackList.SetValue( strColour );
		m_nButtonColourBackList = MakeColourInt( m_ButtonColourBackList );
	}
}

/**********************************************************************/

void CButtonOptions::SetButtonColourBackParentInt( int n )
{
	CString strColour;
	if ( MakeColourString( n, strColour ) == TRUE )
	{
		m_ButtonColourBackParent.SetValue( strColour );
		m_nButtonColourBackParent = MakeColourInt( m_ButtonColourBackParent );
	}
}

/**********************************************************************/

void CButtonOptions::SetButtonColourBackControlInt( int n )
{
	CString strColour;
	if ( MakeColourString( n, strColour ) == TRUE )
	{
		m_ButtonColourBackControl.SetValue( strColour );
		m_nButtonColourBackControl = MakeColourInt( m_ButtonColourBackControl );
	}
}

/**********************************************************************/

void CButtonOptions::SetButtonColourBackUnusedInt( int n )
{
	CString strColour;
	if ( MakeColourString( n, strColour ) == TRUE )
	{
		m_ButtonColourBackUnused.SetValue( strColour );
		m_nButtonColourBackUnused = MakeColourInt( m_ButtonColourBackUnused );
	}
}

/**********************************************************************/

void CButtonOptions::SetButtonColourTextActionInt( int n )
{
	CString strColour;
	if ( MakeColourString( n, strColour ) == TRUE )
	{
		m_ButtonColourTextAction.SetValue( strColour );
		m_nButtonColourTextAction = MakeColourInt( m_ButtonColourTextAction );
	}
}

/**********************************************************************/

void CButtonOptions::SetButtonColourTextListInt( int n )
{
	CString strColour;
	if ( MakeColourString( n, strColour ) == TRUE )
	{
		m_ButtonColourTextList.SetValue( strColour );
		m_nButtonColourTextList = MakeColourInt( m_ButtonColourTextList );
	}
}

/**********************************************************************/

void CButtonOptions::SetButtonColourTextParentInt( int n )
{
	CString strColour;
	if ( MakeColourString( n, strColour ) == TRUE )
	{
		m_ButtonColourTextParent.SetValue( strColour );
		m_nButtonColourTextParent = MakeColourInt( m_ButtonColourTextParent );
	}
}

/**********************************************************************/

void CButtonOptions::SetButtonColourTextControlInt( int n )
{
	CString strColour;
	if ( MakeColourString( n, strColour ) == TRUE )
	{
		m_ButtonColourTextControl.SetValue( strColour );
		m_nButtonColourTextControl = MakeColourInt( m_ButtonColourTextControl );
	}
}

/**********************************************************************/

int CButtonOptions::MakeImageTypeInt( CIniFileString& iniString )
{
	CString strType = iniString.GetValue();
	strType.MakeUpper();
	if ( strType == "NONE" )	return 0;
	if ( strType == "GIF" )		return 1;
	if ( strType == "JPG" )		return 2;
	if ( strType == "BMP" )		return 3;
	if ( strType == "PNG" )		return 4;
	return 0;
}

/**********************************************************************/

void CButtonOptions::SetUpperImageTypeInt( int n )
{
	switch(n)
	{ 
	case 0:		m_UpperImageType.SetValue( "NONE" );	break;
	case 1:		m_UpperImageType.SetValue( "GIF" );		break;
	case 2:		m_UpperImageType.SetValue( "JPG" );		break;
	case 3:		m_UpperImageType.SetValue( "BMP" );		break;
	case 4:		m_UpperImageType.SetValue( "PNG" );		break;
	}
	m_nUpperImageType = MakeImageTypeInt( m_UpperImageType );
}

/**********************************************************************/

void CButtonOptions::SetLowerImageTypeInt( int n )
{
	switch(n)
	{ 
	case 0:		m_LowerImageType.SetValue( "NONE" );	break;
	case 1:		m_LowerImageType.SetValue( "GIF" );		break;
	case 2:		m_LowerImageType.SetValue( "JPG" );		break;
	case 3:		m_LowerImageType.SetValue( "BMP" );		break;
	case 4:		m_LowerImageType.SetValue( "PNG" );		break;
	}
	m_nLowerImageType = MakeImageTypeInt( m_LowerImageType );
}

/**********************************************************************/

int CButtonOptions::MakeImageXAlignInt( CIniFileString& iniString )
{
	CString strType = iniString.GetValue();
	strType.MakeUpper();
	if ( strType == "LEFT" )	return 0;
	if ( strType == "CENTRE" )	return 1;
	if ( strType == "RIGHT" )	return 2;
	return 1;
}

/**********************************************************************/

void CButtonOptions::SetUpperImageXAlignInt( int n  )
{
	switch(n)
	{
	case 0:	m_UpperImageXAlign.SetValue( "LEFT" );		break;
	case 1:	m_UpperImageXAlign.SetValue( "CENTRE" );	break;
	case 2:	m_UpperImageXAlign.SetValue( "RIGHT" );		break;
	}
	m_nUpperImageXAlign = MakeImageXAlignInt( m_UpperImageXAlign );
}

/**********************************************************************/

void CButtonOptions::SetLowerImageXAlignInt( int n  )
{
	switch(n)
	{
	case 0:	m_LowerImageXAlign.SetValue( "LEFT" );		break;
	case 1:	m_LowerImageXAlign.SetValue( "CENTRE" );	break;
	case 2:	m_LowerImageXAlign.SetValue( "RIGHT" );		break;
	}
	m_nLowerImageXAlign = MakeImageXAlignInt( m_LowerImageXAlign );
}

/**********************************************************************/

int CButtonOptions::MakeImageYAlignInt( CIniFileString& iniString )
{
	CString strType = iniString.GetValue();
	strType.MakeUpper();
	if ( strType == "TOP" )		return 0;
	if ( strType == "CENTRE" )	return 1;
	if ( strType == "BOTTOM" )	return 2;
	return 1;
}

/**********************************************************************/

void CButtonOptions::SetUpperImageYAlignInt( int n  )
{
	switch(n)
	{
	case 0:	m_UpperImageYAlign.SetValue( "TOP" );		break;
	case 1:	m_UpperImageYAlign.SetValue( "CENTRE" );	break;
	case 2:	m_UpperImageYAlign.SetValue( "BOTTOM" );	break;
	}
	m_nUpperImageYAlign = MakeImageYAlignInt( m_UpperImageYAlign );
}

/**********************************************************************/

void CButtonOptions::SetLowerImageYAlignInt( int n  )
{
	switch(n)
	{
	case 0:	m_LowerImageYAlign.SetValue( "TOP" );		break;
	case 1:	m_LowerImageYAlign.SetValue( "CENTRE" );	break;
	case 2:	m_LowerImageYAlign.SetValue( "BOTTOM" );	break;
	}
	m_nLowerImageYAlign = MakeImageYAlignInt( m_LowerImageYAlign );
}

/**********************************************************************/

int CButtonOptions::MakeButtonBarHomeInt( CIniFileString& iniString )
{
	CString strType = iniString.GetValue();
	strType.MakeUpper();
	if ( strType == "TOPLEFT" )		return 0;
	if ( strType == "TOPRIGHT" )	return 1;
	if ( strType == "BOTTOMRIGHT" )	return 2;
	if ( strType == "BOTTOMLEFT" )	return 3;
	return 3;
}

/**********************************************************************/

void CButtonOptions::SetButtonBarHomeInt( int n )
{
	switch( n )
	{
	case 0:	m_ButtonBarHome.SetValue( "TOPLEFT" );		break;
	case 1:	m_ButtonBarHome.SetValue( "TOPRIGHT" );		break;
	case 2:	m_ButtonBarHome.SetValue( "BOTTOMRIGHT" );	break;
	case 3:	m_ButtonBarHome.SetValue( "BOTTOMLEFT" );	break;
	}
	m_nButtonBarHome = MakeButtonBarHomeInt( m_ButtonBarHome );
}

/**********************************************************************/

int CButtonOptions::MakeButtonBarDirectionInt( CIniFileString& iniString )
{
	CString strType = iniString.GetValue();
	strType.MakeUpper();
	if ( strType == "UP" )		return 0;
	if ( strType == "RIGHT" )	return 1;
	if ( strType == "DOWN" )	return 2;
	if ( strType == "LEFT" )	return 3;
	return 1;
}

/**********************************************************************/

void CButtonOptions::SetButtonBarDirectionInt( int n )
{
	switch( n )
	{
	case 0:	m_ButtonBarDirection.SetValue( "UP" );		break;
	case 1:	m_ButtonBarDirection.SetValue( "RIGHT" );	break;
	case 2:	m_ButtonBarDirection.SetValue( "DOWN" );	break;
	case 3:	m_ButtonBarDirection.SetValue( "LEFT" );	break;
	}
	m_nButtonBarDirection = MakeButtonBarDirectionInt( m_ButtonBarDirection );
}

/**********************************************************************/

int CButtonOptions::MakeButtonBarControlLinePosInt( CIniFileString& iniString )
{
	CString strType = iniString.GetValue();
	strType.MakeUpper();
	if ( strType == "BEFORE" )		return 0;
	if ( strType == "AFTER" )		return 1;
	return 1;
}

/**********************************************************************/

void CButtonOptions::SetButtonBarControlLinePosInt( int n )
{
	switch(n)
	{
	case 0:	m_ButtonBarControlLinePos.SetValue( "BEFORE" );	break;
	case 1:	m_ButtonBarControlLinePos.SetValue( "AFTER" );	break;
	}
	m_nButtonBarControlLinePos = MakeButtonBarControlLinePosInt( m_ButtonBarControlLinePos );
}

/**********************************************************************/

int CButtonOptions::MakeButtonBarControlButtonPosInt( CIniFileString& iniString )
{
	CString strType = iniString.GetValue();
	strType.MakeUpper();
	if ( strType == "START" )		return 0;
	if ( strType == "END" )			return 1;
	return 1;
}

/**********************************************************************/

void CButtonOptions::SetButtonBarControlButtonPosInt( int n )
{
	switch(n)
	{
	case 0:	m_ButtonBarControlButtonPos.SetValue( "START" );	break;
	case 1:	m_ButtonBarControlButtonPos.SetValue( "END" );	break;
	}
	m_nButtonBarControlButtonPos = MakeButtonBarControlButtonPosInt( m_ButtonBarControlButtonPos );
}

/**********************************************************************/

void CButtonOptions::ReloadFromIniFile( CEnhancedIniFile& iniFile )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		iniFile.ReadBool( m_LayoutEnable );
		/*****/
		iniFile.ReadBool( m_WindowWidthAuto );
		iniFile.ReadInt( m_WindowWidthManual );
		/*****/
		iniFile.ReadBool( m_UpperAreaHeightAuto );
		iniFile.ReadInt( m_UpperAreaHeightManual );
		iniFile.ReadString( m_UpperAreaFillColour );
		/*****/
		iniFile.ReadString( m_UpperImageType );
		iniFile.ReadInt( m_UpperImageMarginTop );
		iniFile.ReadInt( m_UpperImageMarginBottom );
		iniFile.ReadInt( m_UpperImageMarginLeft );
		iniFile.ReadInt( m_UpperImageMarginRight );
		iniFile.ReadString( m_UpperImageXAlign );
		iniFile.ReadString( m_UpperImageYAlign );
		iniFile.ReadBool( m_UpperImageStretch );
		/*****/
		iniFile.ReadInt( m_LowerAreaHeightManual );
		iniFile.ReadString( m_LowerAreaFillColour );
		iniFile.ReadInt( m_LowerAreaTaskbarGap );
		/*****/
		iniFile.ReadString( m_LowerImageType );
		iniFile.ReadInt( m_LowerImageMarginTop );
		iniFile.ReadInt( m_LowerImageMarginBottom );
		iniFile.ReadInt( m_LowerImageMarginLeft );
		iniFile.ReadInt( m_LowerImageMarginRight );
		iniFile.ReadString( m_LowerImageXAlign );
		iniFile.ReadString( m_LowerImageYAlign );
		iniFile.ReadBool( m_LowerImageStretch );
		/*****/
		iniFile.ReadString( m_ButtonBarHome );
		iniFile.ReadString( m_ButtonBarDirection );
		iniFile.ReadString( m_ButtonBarControlLinePos );
		iniFile.ReadString( m_ButtonBarControlButtonPos );
		iniFile.ReadBool( m_ButtonBarParentButton );
		iniFile.ReadBool( m_ButtonBarCentre );
		iniFile.ReadInt( m_ButtonBarMarginTop );
		iniFile.ReadInt( m_ButtonBarMarginBottom );
		iniFile.ReadInt( m_ButtonBarMarginLeft );
		iniFile.ReadInt( m_ButtonBarMarginRight );
		iniFile.ReadInt( m_ButtonBarLineSize );
		iniFile.ReadInt( m_ButtonBarLineCount );
		/*****/
		iniFile.ReadInt( m_ButtonWidthType );
		iniFile.ReadInt( m_ButtonWidthPercent );
		iniFile.ReadInt( m_ButtonWidthPixel );
		iniFile.ReadInt( m_ButtonHeightType );
		iniFile.ReadInt( m_ButtonHeightPercent );
		iniFile.ReadInt( m_ButtonHeightPixel );
		iniFile.ReadInt( m_ButtonXGap );
		iniFile.ReadInt( m_ButtonYGap );
		iniFile.ReadInt( m_ButtonRounding );
		iniFile.ReadString( m_ButtonColourBackAction );
		iniFile.ReadString( m_ButtonColourBackList );
		iniFile.ReadString( m_ButtonColourBackUnused );
		iniFile.ReadString( m_ButtonColourBackControl );
		iniFile.ReadString( m_ButtonColourBackParent );
		iniFile.ReadString( m_ButtonColourTextAction );
		iniFile.ReadString( m_ButtonColourTextList );
		iniFile.ReadString( m_ButtonColourTextControl );
		iniFile.ReadString( m_ButtonColourTextParent );
		/*****/
		iniFile.ReadInt( m_ButtonImageTopMarginPercent );
		iniFile.ReadInt( m_ButtonImageBottomMarginPercent );
		iniFile.ReadInt( m_ButtonImageLeftMarginPercent );
		iniFile.ReadInt( m_ButtonImageRightMarginPercent );
		/*****/
		
		MakeIntSettings();
	}
}

/**********************************************************************/

bool CButtonOptions::Write()
{
	bool bResult = FALSE;

	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		CEnhancedIniFile iniFile;
		PrepareIniFile( iniFile );
			
		CFilenameUpdater FnUp( SysFiles::ButtonOptions );
		bResult = iniFile.Write( FnUp.GetFilenameToUse() );
	}

	return bResult;
}

/**********************************************************************/

void CButtonOptions::PrepareIniFile( CEnhancedIniFile& iniFile )
{
	iniFile.WriteBool( m_LayoutEnable );
	/*****/
	iniFile.WriteBool( m_WindowWidthAuto );
	iniFile.WriteInt( m_WindowWidthManual );
	/*****/
	iniFile.WriteBool( m_UpperAreaHeightAuto );
	iniFile.WriteInt( m_UpperAreaHeightManual );
	iniFile.WriteString( m_UpperAreaFillColour );
	/*****/
	iniFile.WriteString( m_UpperImageType );
	iniFile.WriteInt( m_UpperImageMarginTop );
	iniFile.WriteInt( m_UpperImageMarginBottom );
	iniFile.WriteInt( m_UpperImageMarginLeft );
	iniFile.WriteInt( m_UpperImageMarginRight );
	iniFile.WriteString( m_UpperImageXAlign );
	iniFile.WriteString( m_UpperImageYAlign );
	iniFile.WriteBool( m_UpperImageStretch );
	/*****/
	iniFile.WriteInt( m_LowerAreaHeightManual );
	iniFile.WriteString( m_LowerAreaFillColour );
	iniFile.WriteInt( m_LowerAreaTaskbarGap );
	/*****/
	iniFile.WriteString( m_LowerImageType );
	iniFile.WriteInt( m_LowerImageMarginTop );
	iniFile.WriteInt( m_LowerImageMarginBottom );
	iniFile.WriteInt( m_LowerImageMarginLeft );
	iniFile.WriteInt( m_LowerImageMarginRight );
	iniFile.WriteString( m_LowerImageXAlign );
	iniFile.WriteString( m_LowerImageYAlign );
	iniFile.WriteBool( m_LowerImageStretch );
	/*****/
	iniFile.WriteString( m_ButtonBarHome );
	iniFile.WriteString( m_ButtonBarDirection );
	iniFile.WriteString( m_ButtonBarControlLinePos );
	iniFile.WriteString( m_ButtonBarControlButtonPos );
	iniFile.WriteBool( m_ButtonBarParentButton );
	iniFile.WriteBool( m_ButtonBarCentre );
	iniFile.WriteInt( m_ButtonBarMarginTop );
	iniFile.WriteInt( m_ButtonBarMarginBottom );
	iniFile.WriteInt( m_ButtonBarMarginLeft );
	iniFile.WriteInt( m_ButtonBarMarginRight );
	iniFile.WriteInt( m_ButtonBarLineSize );
	iniFile.WriteInt( m_ButtonBarLineCount );
	/*****/
	iniFile.WriteInt( m_ButtonWidthType );
	iniFile.WriteInt( m_ButtonWidthPercent );
	iniFile.WriteInt( m_ButtonWidthPixel );
	iniFile.WriteInt( m_ButtonHeightType );
	iniFile.WriteInt( m_ButtonHeightPercent );
	iniFile.WriteInt( m_ButtonHeightPixel );
	iniFile.WriteInt( m_ButtonXGap );
	iniFile.WriteInt( m_ButtonYGap );
	iniFile.WriteInt( m_ButtonRounding );
	iniFile.WriteString( m_ButtonColourBackAction );
	iniFile.WriteString( m_ButtonColourBackList );
	iniFile.WriteString( m_ButtonColourBackUnused );
	iniFile.WriteString( m_ButtonColourBackControl );
	iniFile.WriteString( m_ButtonColourBackParent );
	iniFile.WriteString( m_ButtonColourTextAction );
	iniFile.WriteString( m_ButtonColourTextList );
	iniFile.WriteString( m_ButtonColourTextControl );
	iniFile.WriteString( m_ButtonColourTextParent );
	/*****/
	iniFile.WriteInt( m_ButtonImageTopMarginPercent );
	iniFile.WriteInt( m_ButtonImageBottomMarginPercent );
	iniFile.WriteInt( m_ButtonImageLeftMarginPercent );
	iniFile.WriteInt( m_ButtonImageRightMarginPercent );
}

/**********************************************************************/

void CButtonOptions::Reset()
{
	m_LayoutEnable.Reset();
	/*****/
	m_WindowWidthAuto.Reset();
	m_WindowWidthManual.Reset();
	/*****/
	m_UpperAreaHeightAuto.Reset();
	m_UpperAreaHeightManual.Reset();
	m_UpperAreaFillColour.Reset();
	/*****/
	m_UpperImageType.Reset();
	m_UpperImageMarginTop.Reset();
	m_UpperImageMarginBottom.Reset();
	m_UpperImageMarginLeft.Reset();
	m_UpperImageMarginRight.Reset();
	m_UpperImageXAlign.Reset();
	m_UpperImageYAlign.Reset();
	m_UpperImageStretch.Reset();
	/*****/
	m_LowerAreaHeightManual.Reset();
	m_LowerAreaFillColour.Reset();
	m_LowerAreaTaskbarGap.Reset();
	/*****/
	m_LowerImageType.Reset();
	m_LowerImageMarginTop.Reset();
	m_LowerImageMarginBottom.Reset();
	m_LowerImageMarginLeft.Reset();
	m_LowerImageMarginRight.Reset();
	m_LowerImageXAlign.Reset();
	m_LowerImageYAlign.Reset();
	m_LowerImageStretch.Reset();
	/*****/
	m_ButtonBarHome.Reset();
	m_ButtonBarDirection.Reset();
	m_ButtonBarControlLinePos.Reset();
	m_ButtonBarControlButtonPos.Reset();
	m_ButtonBarParentButton.Reset();
	m_ButtonBarCentre.Reset();
	m_ButtonBarMarginTop.Reset();
	m_ButtonBarMarginBottom.Reset();
	m_ButtonBarMarginLeft.Reset();
	m_ButtonBarMarginRight.Reset();
	m_ButtonBarLineSize.Reset();
	m_ButtonBarLineCount.Reset();
	/*****/
	m_ButtonWidthType.Reset();
	m_ButtonWidthPercent.Reset();
	m_ButtonWidthPixel.Reset();
	m_ButtonHeightType.Reset();
	m_ButtonHeightPercent.Reset();
	m_ButtonHeightPixel.Reset();
	m_ButtonXGap.Reset();
	m_ButtonYGap.Reset();
	m_ButtonRounding.Reset();
	m_ButtonColourBackAction.Reset();
	m_ButtonColourBackList.Reset();
	m_ButtonColourBackUnused.Reset();
	m_ButtonColourBackControl.Reset();
	m_ButtonColourBackParent.Reset();
	m_ButtonColourTextAction.Reset();
	m_ButtonColourTextList.Reset();
	m_ButtonColourTextControl.Reset();
	m_ButtonColourTextParent.Reset();
	/*****/
	m_ButtonImageTopMarginPercent.Reset();
	m_ButtonImageBottomMarginPercent.Reset();
	m_ButtonImageLeftMarginPercent.Reset();
	m_ButtonImageRightMarginPercent.Reset();
	/*****/
	MakeIntSettings();
}

/**********************************************************************/

void CButtonOptions::MakeIntSettings()
{
	m_nUpperAreaFillColour = MakeColourInt( m_UpperAreaFillColour );
	m_nUpperImageType = MakeImageTypeInt( m_UpperImageType );
	m_nUpperImageXAlign = MakeImageXAlignInt( m_UpperImageXAlign );
	m_nUpperImageYAlign = MakeImageYAlignInt( m_UpperImageYAlign );
	/*****/
	m_nLowerAreaFillColour = MakeColourInt( m_LowerAreaFillColour );
	m_nLowerImageType = MakeImageTypeInt( m_LowerImageType );
	m_nLowerImageXAlign = MakeImageXAlignInt( m_LowerImageXAlign );
	m_nLowerImageYAlign = MakeImageYAlignInt( m_LowerImageYAlign );
	/*****/
	m_nButtonBarHome = MakeButtonBarHomeInt( m_ButtonBarHome );
	m_nButtonBarDirection = MakeButtonBarDirectionInt( m_ButtonBarDirection );
	m_nButtonBarControlLinePos = MakeButtonBarControlLinePosInt( m_ButtonBarControlLinePos );
	m_nButtonBarControlButtonPos = MakeButtonBarControlButtonPosInt( m_ButtonBarControlButtonPos );
	/*****/
	m_nButtonColourBackAction = MakeColourInt( m_ButtonColourBackAction );
	m_nButtonColourBackList = MakeColourInt( m_ButtonColourBackList );
	m_nButtonColourBackUnused = MakeColourInt( m_ButtonColourBackUnused );
	m_nButtonColourBackControl = MakeColourInt( m_ButtonColourBackControl );
	m_nButtonColourBackParent = MakeColourInt( m_ButtonColourBackParent );
	m_nButtonColourTextAction = MakeColourInt( m_ButtonColourTextAction );
	m_nButtonColourTextList = MakeColourInt( m_ButtonColourTextList );
	m_nButtonColourTextControl = MakeColourInt( m_ButtonColourTextControl );
	m_nButtonColourTextParent = MakeColourInt( m_ButtonColourTextParent );
}

/**********************************************************************/

void CButtonOptions::RememberSettings()
{
	m_LayoutEnable.SaveValue();
	/*****/
	m_WindowWidthAuto.SaveValue();
	m_WindowWidthManual.SaveValue();
	/*****/
	m_UpperAreaHeightAuto.SaveValue();
	m_UpperAreaHeightManual.SaveValue();
	m_UpperAreaFillColour.SaveValue();
	/*****/
	m_UpperImageType.SaveValue();
	m_UpperImageMarginTop.SaveValue();
	m_UpperImageMarginBottom.SaveValue();
	m_UpperImageMarginLeft.SaveValue();
	m_UpperImageMarginRight.SaveValue();
	m_UpperImageXAlign.SaveValue();
	m_UpperImageYAlign.SaveValue();
	m_UpperImageStretch.SaveValue();
	/*****/
	m_LowerAreaHeightManual.SaveValue();
	m_LowerAreaFillColour.SaveValue();
	m_LowerAreaTaskbarGap.SaveValue();
	/*****/
	m_LowerImageType.SaveValue();
	m_LowerImageMarginTop.SaveValue();
	m_LowerImageMarginBottom.SaveValue();
	m_LowerImageMarginLeft.SaveValue();
	m_LowerImageMarginRight.SaveValue();
	m_LowerImageXAlign.SaveValue();
	m_LowerImageYAlign.SaveValue();
	m_LowerImageStretch.SaveValue();
	/*****/
	m_ButtonBarHome.SaveValue();
	m_ButtonBarDirection.SaveValue();
	m_ButtonBarControlLinePos.SaveValue();
	m_ButtonBarControlButtonPos.SaveValue();
	m_ButtonBarParentButton.SaveValue();
	m_ButtonBarCentre.SaveValue();
	m_ButtonBarMarginTop.SaveValue();
	m_ButtonBarMarginBottom.SaveValue();
	m_ButtonBarMarginLeft.SaveValue();
	m_ButtonBarMarginRight.SaveValue();
	m_ButtonBarLineSize.SaveValue();
	m_ButtonBarLineCount.SaveValue();
	/*****/
	m_ButtonWidthType.SaveValue();
	m_ButtonWidthPercent.SaveValue();
	m_ButtonWidthPixel.SaveValue();
	m_ButtonHeightType.SaveValue();
	m_ButtonHeightPercent.SaveValue();
	m_ButtonHeightPixel.SaveValue();
	m_ButtonXGap.SaveValue();
	m_ButtonYGap.SaveValue();
	m_ButtonRounding.SaveValue();
	m_ButtonColourBackAction.SaveValue();
	m_ButtonColourBackList.SaveValue();
	m_ButtonColourBackUnused.SaveValue();
	m_ButtonColourBackControl.SaveValue();
	m_ButtonColourBackParent.SaveValue();
	m_ButtonColourTextAction.SaveValue();
	m_ButtonColourTextList.SaveValue();
	m_ButtonColourTextControl.SaveValue();
	m_ButtonColourTextParent.SaveValue();
}

/**********************************************************************/

void CButtonOptions::RestoreSettings()
{
	m_LayoutEnable.RestoreValue();
	/*****/
	m_WindowWidthAuto.RestoreValue();
	m_WindowWidthManual.RestoreValue();
	/*****/
	m_UpperAreaHeightAuto.RestoreValue();
	m_UpperAreaHeightManual.RestoreValue();
	m_UpperAreaFillColour.RestoreValue();
	/*****/
	m_UpperImageType.RestoreValue();
	m_UpperImageMarginTop.RestoreValue();
	m_UpperImageMarginBottom.RestoreValue();
	m_UpperImageMarginLeft.RestoreValue();
	m_UpperImageMarginRight.RestoreValue();
	m_UpperImageXAlign.RestoreValue();
	m_UpperImageYAlign.RestoreValue();
	m_UpperImageStretch.RestoreValue();
	/*****/
	m_LowerAreaHeightManual.RestoreValue();
	m_LowerAreaFillColour.RestoreValue();
	m_LowerAreaTaskbarGap.RestoreValue();
	/*****/
	m_LowerImageType.RestoreValue();
	m_LowerImageMarginTop.RestoreValue();
	m_LowerImageMarginBottom.RestoreValue();
	m_LowerImageMarginLeft.RestoreValue();
	m_LowerImageMarginRight.RestoreValue();
	m_LowerImageXAlign.RestoreValue();
	m_LowerImageYAlign.RestoreValue();
	m_LowerImageStretch.RestoreValue();
	/*****/
	m_ButtonBarHome.RestoreValue();
	m_ButtonBarDirection.RestoreValue();
	m_ButtonBarControlLinePos.RestoreValue();
	m_ButtonBarControlButtonPos.RestoreValue();
	m_ButtonBarParentButton.RestoreValue();
	m_ButtonBarCentre.RestoreValue();
	m_ButtonBarMarginTop.RestoreValue();
	m_ButtonBarMarginBottom.RestoreValue();
	m_ButtonBarMarginLeft.RestoreValue();
	m_ButtonBarMarginRight.RestoreValue();
	m_ButtonBarLineSize.RestoreValue();
	m_ButtonBarLineCount.RestoreValue();
	/*****/
	m_ButtonWidthType.RestoreValue();
	m_ButtonWidthPercent.RestoreValue();
	m_ButtonWidthPixel.RestoreValue();
	m_ButtonHeightType.RestoreValue();
	m_ButtonHeightPercent.RestoreValue();
	m_ButtonHeightPixel.RestoreValue();
	m_ButtonXGap.RestoreValue();
	m_ButtonYGap.RestoreValue();
	m_ButtonRounding.RestoreValue();
	m_ButtonColourBackAction.RestoreValue();
	m_ButtonColourBackList.RestoreValue();
	m_ButtonColourBackUnused.RestoreValue();
	m_ButtonColourBackControl.RestoreValue();
	m_ButtonColourBackParent.RestoreValue();
	m_ButtonColourTextAction.RestoreValue();
	m_ButtonColourTextList.RestoreValue();
	m_ButtonColourTextControl.RestoreValue();
	m_ButtonColourTextParent.RestoreValue();
	/*****/
	MakeIntSettings();
}

/**********************************************************************/

void CButtonOptions::SetDefaults()
{
	Reset();
}

/**********************************************************************/

bool CButtonOptions::Read()
{
	bool bResult = ReadInternal();
	return bResult;
}

/**********************************************************************/

bool CButtonOptions::ReadInternal()
{
	CFilenameUpdater FnUp( SysFiles::ButtonOptions );
	CString strFilename = FnUp.GetFilenameToUse();
	
	if ( ::FileExists( strFilename ) == TRUE )
	{
		CEnhancedIniFile iniFile( SS_INI_NORMAL );
		if ( iniFile.Read( strFilename ) == FALSE )
		{
			m_bFatalReadError = TRUE;
			return FALSE;
		}

		ReloadFromIniFile( iniFile );
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/
