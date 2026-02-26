 /**********************************************************************/
#include "BackgroundLayoutInfo.h"
#include "ButtonDefines.h"
#include "ButtonOptions.h"
#include "FileRemover.h"
/**********************************************************************/
#include "ButtonManager.h"
/**********************************************************************/

CButtonManager::CButtonManager(void)
{
	m_pButtonEditDlg = NULL;
	m_nCurrentListNo = 0;
	m_bCalculatedRects = FALSE;

	m_arrayListNames.Add( "Home List" );
	for ( int n = 1; n <= BUTMGR_MAX_LIST_NO; n++ )
	{
		CString str;
		str.Format( "List %d", n );
		m_arrayListNames.Add(str);
	}

	m_bGotEditingButton = FALSE;
	m_nButtonManagerMode = BUTMGR_MODE_USER;

	int nBitmapCount = ( BUTMGR_MAX_BUTTON_NO + 1 ) * ( BUTMGR_MAX_LINE_NO + 1 );
	for ( int n = 0; n < nBitmapCount; n++ )
		m_arrayBitmaps.Add( new ( CBitmapInfo ) );

	m_bTempImageMode = FALSE;
}

/**********************************************************************/

CButtonManager::~CButtonManager(void)
{
	for ( int n = 0; n < m_arrayBitmaps.GetSize(); n++ )
	{
		CBitmapInfo* p = m_arrayBitmaps.GetAt(n);
		p -> DeleteOwnDC();
		delete(p);
	}
}

/**********************************************************************/

void CButtonManager::SetButtonManagerMode( int n )
{
	switch(n)
	{
	case BUTMGR_MODE_USER:
	case BUTMGR_MODE_EDIT_LAYOUT:
	case BUTMGR_MODE_EDIT_LIST:
		m_nButtonManagerMode = n;
		break;
	}
}

/**********************************************************************/

const char* CButtonManager::GetListName( int n )
{
	m_strListName = "";
	if ( ( n >= 0 ) && ( n < m_arrayListNames.GetSize() ) )
		m_strListName = m_arrayListNames.GetAt(n);

	return m_strListName;
}

/**********************************************************************/

void CButtonManager::SetListName( int n, const char* sz )
{
	if ( ( n >= 0 ) && ( n < m_arrayListNames.GetSize() ) )
	{
		CString str = sz;
		if ( str.GetLength() <= BUTMGR_MAX_LISTNAME )
			m_arrayListNames.SetAt( n, str );
	}
}

/**********************************************************************/

void CButtonManager::CalculateButtonRects( bool bForce )
{
	if ( ( FALSE == m_bCalculatedRects ) || ( TRUE == bForce ) )
	{
		m_bCalculatedRects = TRUE;

		m_ButtonDisplay.RemoveAll();

		if (ButtonOptions.GetButtonBarLineSize() == 0)
		{
			return;
		}

		int nAreaWidth = BackgroundLayoutInfo.GetTotalWindowWidth();
		int nAreaHeight = BackgroundLayoutInfo.GetUpperAreaHeight();
		int nMarginTop = ButtonOptions.GetButtonBarMarginTop();
		int nMarginBottom = ButtonOptions.GetButtonBarMarginBottom();
		int nMarginLeft = ButtonOptions.GetButtonBarMarginLeft();
		int nMarginRight = ButtonOptions.GetButtonBarMarginRight();
		
		int nButtonXSpace = nAreaWidth - nMarginLeft - nMarginRight;
		int nButtonYSpace = nAreaHeight - nMarginTop - nMarginBottom;

		int nButtonXCount = 0;
		int nButtonYCount = 0;

		switch( ButtonOptions.GetButtonBarDirectionInt() )
		{
		case 0: //UP
		case 2:	//DOWN
			nButtonYCount = ButtonOptions.GetButtonBarLineSize();
			nButtonXCount = ButtonOptions.GetButtonBarLineCount();
			break;

		case 1:	//RIGHT
		case 3:	//LEFT
		default:
			nButtonXCount = ButtonOptions.GetButtonBarLineSize();
			nButtonYCount = ButtonOptions.GetButtonBarLineCount();
			break;
		}

		int nButtonXGap = ButtonOptions.GetButtonXGap();
		int nButtonYGap = ButtonOptions.GetButtonYGap();

		int nButtonWidth = 0;
		if (ButtonOptions.GetButtonWidthType() == 1)
		{
			nButtonWidth = ButtonOptions.GetButtonWidthPixel();
		}
		else
		{
			int nButtonXFill = ( nButtonXSpace * ButtonOptions.GetButtonWidthPercent() ) / 100;
			nButtonWidth = ( nButtonXFill - ( nButtonXGap * ( nButtonXCount - 1 ) ) ) / nButtonXCount;
			if ( nButtonWidth < 100 ) nButtonWidth = 100;
			if ( nButtonWidth > 300 ) nButtonWidth = 300;
		}

		int nButtonHeight = 0;
		if (ButtonOptions.GetButtonHeightType() == 1)
		{
			nButtonHeight = ButtonOptions.GetButtonHeightPixel();
		}
		else
		{
			int nButtonYFill = ( nButtonYSpace * ButtonOptions.GetButtonHeightPercent() ) / 100;
			nButtonHeight = ( nButtonYFill - ( nButtonYGap * ( nButtonYCount - 1 ) ) ) / nButtonYCount;
			if ( nButtonHeight < 100 ) nButtonHeight = 100;
			if ( nButtonHeight > 300 ) nButtonHeight = 300;
		}

		int nButtonXNeed = 0;
		nButtonXNeed += nButtonXCount * nButtonWidth;
		nButtonXNeed += ( nButtonXCount - 1 ) * nButtonXGap;

		if ( nButtonXNeed > nButtonXSpace )
		{
			BackgroundLayoutInfo.AddError( "Button bar is too wide for allocated area" );
			return;
		}

		int nButtonYNeed = 0;
		nButtonYNeed += nButtonYCount * nButtonHeight;
		nButtonYNeed += ( nButtonYCount - 1 ) * nButtonYGap;

		if ( nButtonYNeed > nButtonYSpace )
		{
			BackgroundLayoutInfo.AddError( "Button bar is too tall for allocated area" );
			return;
		}
	
		int nXPosStart = 0;
		int nYPosStart = 0;
		int nXButtonDelta = 0;
		int nYButtonDelta = 0;
		int nXLineDelta = 0;
		int nYLineDelta = 0;

		switch( ButtonOptions.GetButtonBarHomeInt() )
		{
		case 0:	//TOPLEFT
			switch( ButtonOptions.GetButtonBarDirectionInt() )
			{
			case 0:	//UP
				nXPosStart = nMarginLeft;
				nYPosStart = nMarginTop + ( ( nButtonHeight + nButtonYGap ) * ( nButtonYCount - 1 ) );
				nYButtonDelta = -(nButtonHeight + nButtonYGap);
				nXLineDelta = nButtonWidth + nButtonXGap;
				break;

			case 1: //RIGHT
				nXPosStart = nMarginLeft;
				nYPosStart = nMarginTop;
				nXButtonDelta = nButtonWidth + nButtonXGap;
				nYLineDelta = nButtonHeight + nButtonYGap;
				break;

			case 2:	//DOWN
				nXPosStart = nMarginLeft;
				nYPosStart = nMarginTop;
				nYButtonDelta = nButtonHeight + nButtonYGap;
				nXLineDelta = nButtonWidth + nButtonXGap;
				break;

			case 3:	//LEFT
				nXPosStart = nMarginLeft + ( ( nButtonWidth + nButtonXGap ) * ( nButtonXCount - 1 ) );
				nYPosStart = nMarginTop;
				nXButtonDelta = -(nButtonWidth + nButtonXGap);
				nYLineDelta = nButtonHeight + nButtonYGap;
				break;
			}
			break;

		case 1:	//TOPRIGHT
			switch( ButtonOptions.GetButtonBarDirectionInt() )
			{
			case 0:	//UP
				nXPosStart = nAreaWidth - nMarginRight - nButtonWidth;
				nYPosStart = nMarginTop + ( ( nButtonHeight + nButtonYGap ) * ( nButtonYCount - 1 ) );
				nYButtonDelta = -(nButtonHeight + nButtonYGap);
				nXLineDelta = -(nButtonWidth + nButtonXGap);
				break;

			case 1: //RIGHT
				nXPosStart = nAreaWidth - nMarginRight - ( nButtonWidth * nButtonXCount ) - ( nButtonXGap * ( nButtonXCount - 1 ) );
				nYPosStart = nMarginTop;
				nXButtonDelta = nButtonWidth + nButtonXGap;
				nYLineDelta = nButtonHeight + nButtonYGap;
				break;

			case 2:	//DOWN
				nXPosStart = nAreaWidth - nMarginRight - nButtonWidth;
				nYPosStart = nMarginTop;
				nYButtonDelta = nButtonHeight + nButtonYGap;
				nXLineDelta = -(nButtonWidth + nButtonXGap);
				break;

			case 3:	//LEFT
				nXPosStart = nAreaWidth - nMarginRight - nButtonWidth;
				nYPosStart = nMarginTop;
				nXButtonDelta = -(nButtonWidth + nButtonXGap);
				nYLineDelta = nButtonHeight + nButtonYGap;
				break;
			}
			break;

		case 2:	//BOTTOMRIGHT
			switch( ButtonOptions.GetButtonBarDirectionInt() )
			{
			case 0:	//UP
				nXPosStart = nAreaWidth - nMarginRight - nButtonWidth;
				nYPosStart = nAreaHeight - nMarginBottom - nButtonHeight;
				nYButtonDelta = -(nButtonHeight + nButtonYGap);
				nXLineDelta = -(nButtonWidth + nButtonXGap);
				break;

			case 1: //RIGHT
				nXPosStart = nAreaWidth - nMarginRight - ( nButtonWidth * nButtonXCount ) - ( nButtonXGap * ( nButtonXCount - 1 ) );
				nYPosStart = nAreaHeight - nMarginBottom - nButtonHeight;
				nXButtonDelta = nButtonWidth + nButtonXGap;
				nYLineDelta = -(nButtonHeight + nButtonYGap);
				break;

			case 2:	//DOWN
				nXPosStart = nAreaWidth - nMarginRight - nButtonWidth;
				nYPosStart = nAreaHeight - nMarginBottom - ( nButtonHeight * nButtonYCount ) - ( nButtonYGap * ( nButtonYCount - 1 ) );
				nYButtonDelta = nButtonHeight + nButtonYGap;
				nXLineDelta = -(nButtonWidth + nButtonXGap);
				break;

			case 3:	//LEFT
				nXPosStart = nAreaWidth - nMarginRight - nButtonWidth;
				nYPosStart = nAreaHeight - nMarginBottom - nButtonHeight;
				nXButtonDelta = -(nButtonWidth + nButtonXGap);
				nYLineDelta = -(nButtonHeight + nButtonYGap);
				break;
			}
			break;

		case 3:	//BOTTOMLEFT
			switch( ButtonOptions.GetButtonBarDirectionInt() )
			{
			case 0:	//UP
				nXPosStart = nMarginLeft;
				nYPosStart = nAreaHeight - nMarginBottom - nButtonHeight;
				nYButtonDelta = -(nButtonHeight + nButtonYGap);
				nXLineDelta = nButtonWidth + nButtonXGap;
				break;

			case 1: //RIGHT
				nXPosStart = nMarginLeft;
				nYPosStart = nAreaHeight - nMarginBottom - nButtonHeight;
				nXButtonDelta = nButtonWidth + nButtonXGap;
				nYLineDelta = -(nButtonHeight + nButtonYGap);
				break;

			case 2:	//DOWN
				nXPosStart = nMarginLeft;
				nYPosStart = nAreaHeight - nMarginBottom - ( nButtonHeight * nButtonYCount ) - ( nButtonYGap * ( nButtonYCount - 1 ) );
				nYButtonDelta = nButtonHeight + nButtonYGap;
				nXLineDelta = nButtonWidth + nButtonXGap;
				break;

			case 3:	//LEFT
				nXPosStart = nMarginLeft + ( ( nButtonWidth + nButtonXGap ) * ( nButtonXCount - 1 ) );
				nYPosStart = nAreaHeight - nMarginBottom - nButtonHeight;
				nXButtonDelta = -(nButtonWidth + nButtonXGap);
				nYLineDelta = -(nButtonHeight + nButtonYGap);
				break;
			}
			break;
		}

		if ( ButtonOptions.GetButtonBarCentre() == TRUE )
		{
			switch( ButtonOptions.GetButtonBarDirectionInt() )
			{
			case 0:	//UP
			case 2:	//DOWN
				switch( ButtonOptions.GetButtonBarHomeInt() )
				{
				case 0:	//TOPLEFT
				case 1:	//TOPRIGHT
					nYPosStart += ( nButtonYSpace - nButtonYNeed ) / 2;
					break;

				case 2: //BOTTOMRIGHT
				case 3: //BOTTOMLEFT
					nYPosStart -= ( nButtonYSpace - nButtonYNeed ) / 2;
					break;
				}
				break;

			case 1: //RIGHT
			case 3:	//LEFT
				switch( ButtonOptions.GetButtonBarHomeInt() )
				{
				case 0:	//TOPLEFT
				case 3: //BOTTOMLEFT
					nXPosStart += ( nButtonXSpace - nButtonXNeed ) / 2;
					break;

				case 1:	//TOPRIGHT
				case 2: //BOTTOMRIGHT
					nXPosStart -= ( nButtonXSpace - nButtonXNeed ) / 2;
					break;
				}
				break;
			}
		}

		for ( int nLine = 0; nLine < ButtonOptions.GetButtonBarLineCount(); nLine++ )
		{
			int nXPos = nXPosStart + ( nXLineDelta * nLine );
			int nYPos = nYPosStart + ( nYLineDelta * nLine );

			for ( int nButton = 0; nButton < ButtonOptions.GetButtonBarLineSize(); nButton++ )
			{
				CRect rect( nXPos, nYPos, nXPos + nButtonWidth - 1, nYPos + nButtonHeight - 1 );
				
				CButtonInfo infoDisplay;
				infoDisplay.SetDisplayRect( rect );
				infoDisplay.SetLineNo( nLine );
				infoDisplay.SetButtonNo( nButton );
				m_ButtonDisplay.Add( infoDisplay );
				
				nXPos += nXButtonDelta;
				nYPos += nYButtonDelta;
			}
		}

		GoHome();
	}
}

/**********************************************************************/

bool CButtonManager::CheckButtonClick( CPoint& point, CButtonInfo& infoDisplay, int& nButtonIdx )
{
	for ( nButtonIdx = 0; nButtonIdx < m_ButtonDisplay.GetSize(); nButtonIdx++ )
	{
		if ( PtInRect( m_ButtonDisplay[nButtonIdx].GetDisplayRect(), point ) == TRUE )
		{
			infoDisplay = m_ButtonDisplay.GetAt(nButtonIdx);
			return TRUE;
		}
	}
	return FALSE;
}

/**********************************************************************/

void CButtonManager::ReadButtonActions()
{
	CSSFile fileButtons;
	CFilenameUpdater FnUp( SysFiles::ButtonActions );
	if ( fileButtons.Open( FnUp.GetFilenameToUse(), "rb" ) == TRUE )
	{
		CString strBuffer;
		while( fileButtons.ReadString( strBuffer ) == TRUE )
		{
			CCSV csv( strBuffer );

			int nVer = csv.GetInt(0);

			switch( nVer )
			{
			case 5:
			case 6:
			case 7:
				{
					CButtonInfo infoButton;
					
					switch( csv.GetInt(1) )
					{
					case 0:
						{
							int nListNo = csv.GetInt(2);
							if ( ( nListNo >= 1 ) && ( nListNo <= BUTMGR_MAX_LIST_NO ) )
							{
								CString strListName = csv.GetString(3);
								if ( strListName.GetLength() <= BUTMGR_MAX_LISTNAME )
									m_arrayListNames.SetAt( nListNo, strListName );
							}
						}
						break;

					case 1:
						{
							int nPos = 2;
							
							infoButton.SetListNo( csv.GetInt(nPos++) );
							infoButton.SetLineNo( csv.GetInt(nPos++) );
							infoButton.SetButtonNo( csv.GetInt(nPos++) );
							infoButton.SetButtonType( csv.GetBool(nPos++) ? BUTTON_TYPE_LIST : BUTTON_TYPE_ACTION );
							infoButton.SetDetail1( csv.GetInt(nPos++) );
							infoButton.SetDetail2( csv.GetInt(nPos++) );
							infoButton.SetDetail3( csv.GetInt(nPos++) );

							if ( nVer >= 6 )
								infoButton.SetActionFlag1( csv.GetBool(nPos++) );
							
							infoButton.ParseFileText( csv.GetString(nPos++) );
							infoButton.SetOwnColourFlag( csv.GetBool(nPos++) );
						
							CString strBackColour = csv.GetString(nPos++);
							if ( strBackColour.GetLength() == 6 )
								infoButton.SetOwnBackColour( ::HexToInt( strBackColour ) );

							CString strTextColour = csv.GetString(nPos++);
							if ( strTextColour.GetLength() == 6 )
								infoButton.SetOwnTextColour( ::HexToInt( strTextColour ) );

							infoButton.SetFontSize( csv.GetInt(nPos++) );

							if ( nVer >= 7 )
							{
								infoButton.SetImageType( csv.GetString(nPos++) );
								infoButton.SetImageTransparentFlag( csv.GetBool(nPos++) );
							}
							
							int nArrayPos;
							if ( m_ButtonLibrary.Find( infoButton, nArrayPos ) == FALSE )
								m_ButtonLibrary.InsertAt( nArrayPos, infoButton );
						}
						break;
					}
				}
			}
		}
	}
}

/**********************************************************************/

void CButtonManager::WriteButtonActions()
{
	CSSFile fileButtons;
	CFilenameUpdater FnUp( SysFiles::ButtonActions );
	if ( fileButtons.Open( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
	{
		for ( int nListNo = 0; nListNo <= 99; nListNo++ )
		{
			CCSV csv;
			csv.Add(6);
			csv.Add(0);
			csv.Add(nListNo);
			csv.Add(m_arrayListNames.GetAt(nListNo) );
			fileButtons.WriteLine( csv.GetLine() );

			int nLineCount = ( 0 == nListNo ) ? 1 : ButtonOptions.GetButtonBarLineCount() - 1;

			for ( int nLineNo = 0; nLineNo < nLineCount; nLineNo++ )
			{
				for ( int nButtonNo = 0; nButtonNo < ButtonOptions.GetButtonBarLineSize(); nButtonNo++ )
				{
					CButtonInfo infoLibrary;
					infoLibrary.SetListNo( nListNo );
					infoLibrary.SetLineNo( nLineNo );
					infoLibrary.SetButtonNo( nButtonNo );

					int nPos;
					if ( m_ButtonLibrary.Find( infoLibrary, nPos ) == TRUE )
					{
						m_ButtonLibrary.GetAt( nPos, infoLibrary );
						
						CCSV csv;
						csv.Add(7);
						csv.Add(1);
						csv.Add(nListNo);
						csv.Add(nLineNo);
						csv.Add(nButtonNo);
						csv.Add(infoLibrary.GetButtonType() == BUTTON_TYPE_LIST ? 1 : 0 );
						csv.Add(infoLibrary.GetDetail1());
						csv.Add(infoLibrary.GetDetail2());
						csv.Add(infoLibrary.GetDetail3());
						csv.Add(infoLibrary.GetActionFlag1() );
						csv.Add(infoLibrary.CreateFileText());
						csv.Add(infoLibrary.GetOwnColourFlag() );

						CString strBackColour;
						strBackColour.Format( "%6.6X", infoLibrary.GetOwnBackColour() );
						csv.Add( strBackColour );

						CString strTextColour;
						strTextColour.Format( "%6.6X", infoLibrary.GetOwnTextColour() );
						csv.Add( strTextColour );

						csv.Add( infoLibrary.GetFontSize() );
						csv.Add( infoLibrary.GetImageType() );
						csv.Add( infoLibrary.GetImageTransparentFlag() );

						fileButtons.WriteLine( csv.GetLine() );
					}
				}
			}
		}
	}
}

/**********************************************************************/

void CButtonManager::RememberButtonLibrary()
{
	m_ButtonLibrarySaved.RemoveAll();
	m_arrayListNamesSaved.RemoveAll();

	CButtonInfo infoLibrary;
	for ( int n = 0; n < m_ButtonLibrary.GetSize(); n++ )
	{
		m_ButtonLibrary.GetAt( n, infoLibrary );
		m_ButtonLibrarySaved.DirectAdd( infoLibrary );
	}

	for ( int n = 0; n < m_arrayListNames.GetSize(); n++ )
		m_arrayListNamesSaved.Add( m_arrayListNames.GetAt(n) );
}

/**********************************************************************/

void CButtonManager::RestoreButtonLibrary()
{
	m_ButtonLibrary.RemoveAll();
	m_arrayListNames.RemoveAll();
	
	CButtonInfo infoLibrary;
	for ( int n = 0; n < m_ButtonLibrarySaved.GetSize(); n++ )
	{
		m_ButtonLibrarySaved.GetAt( n, infoLibrary );
		m_ButtonLibrary.DirectAdd( infoLibrary );
	}

	for ( int n = 0; n < m_arrayListNamesSaved.GetSize(); n++ )
		m_arrayListNames.Add( m_arrayListNamesSaved.GetAt(n) );
}

/**********************************************************************/

bool CButtonManager::CheckButtonLibraryChanges()
{
	if ( m_ButtonLibrary.GetSize() != m_ButtonLibrarySaved.GetSize() )
		return TRUE;

	if ( m_arrayListNames.GetSize() != m_arrayListNamesSaved.GetSize() )
		return TRUE;

	CButtonInfo infoLibrary1;
	CButtonInfo infoLibrary2;
	for ( int n = 0; n < m_ButtonLibrary.GetSize(); n++ )
	{
		m_ButtonLibrary.GetAt( n, infoLibrary1 );
		m_ButtonLibrarySaved.GetAt( n, infoLibrary2 );

		if ( infoLibrary1.Equals( infoLibrary2 ) == FALSE )
			return TRUE;

		CString strImageType = infoLibrary1.GetImageTypeTemp();
		if ( strImageType != "" )
			return TRUE;
	}

	for ( int n = 0; n < m_arrayListNames.GetSize(); n++ )
	{
		CString strTest = m_arrayListNames.GetAt(n);
		if ( strTest != m_arrayListNamesSaved.GetAt(n) )
			return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

bool CButtonManager::GetDisplayButton( int nDisplayLine, int nDisplayButton, CButtonInfo& infoDisplay )
{
	bool bResult = FALSE;

	int nButtonIdx = ( nDisplayLine * ButtonOptions.GetButtonBarLineSize() ) + nDisplayButton;
	if ( ( nButtonIdx >= 0 ) && ( nButtonIdx < m_ButtonDisplay.GetSize() ) )
	{
		infoDisplay = m_ButtonDisplay.GetAt( nButtonIdx );
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

bool CButtonManager::SetDisplayButton( int nDisplayLine, int nDisplayButton, CButtonInfo& infoDisplay )
{
	bool bResult = FALSE;

	int nButtonIdx = ( nDisplayLine * ButtonOptions.GetButtonBarLineSize() ) + nDisplayButton;
	if ( ( nButtonIdx >= 0 ) && ( nButtonIdx < m_ButtonDisplay.GetSize() ) )
	{
		m_ButtonDisplay.SetAt( nButtonIdx, infoDisplay );
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

bool CButtonManager::SetDisplayButton( int nButtonIdx, CButtonInfo& infoDisplay )
{
	bool bResult = FALSE;

	if ( ( nButtonIdx >= 0 ) && ( nButtonIdx < m_ButtonDisplay.GetSize() ) )
	{
		m_ButtonDisplay.SetAt( nButtonIdx, infoDisplay );
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

bool CButtonManager::SetLibraryButton( CButtonInfo& infoDisplay )
{
	bool bResult = FALSE;
	
	switch( infoDisplay.GetButtonType() )
	{
	case BUTTON_TYPE_UNUSED:
		{
			bResult = TRUE;

			int nPos;
			if ( m_ButtonLibrary.Find( infoDisplay, nPos ) == TRUE )
				m_ButtonLibrary.RemoveAt( nPos );
		}
		break;

	case BUTTON_TYPE_ACTION:
	case BUTTON_TYPE_LIST:
		{
			bResult = TRUE;

			int nPos;
			if ( m_ButtonLibrary.Find( infoDisplay, nPos ) == TRUE )
				m_ButtonLibrary.SetAt( nPos, infoDisplay );
			else
				m_ButtonLibrary.InsertAt( nPos, infoDisplay );
		}
		break;
	}
	
	return bResult;
}

/**********************************************************************/

void CButtonManager::SetEditingButton( CButtonInfo& infoDisplay )
{
	m_bGotEditingButton = TRUE;
	m_ButtonEditing = infoDisplay;
}

/**********************************************************************/

void CButtonManager::ClearEditingButton()
{
	m_bGotEditingButton = FALSE;
}

/**********************************************************************/

bool CButtonManager::IsEditingButton( CButtonInfo& infoDisplay )
{
	if ( FALSE == m_bGotEditingButton )
		return FALSE;

	if ( infoDisplay.GetLineNo() != m_ButtonEditing.GetLineNo() )
		return FALSE;

	if ( infoDisplay.GetButtonNo() != m_ButtonEditing.GetButtonNo() )
		return FALSE;

	return TRUE;
}

/**********************************************************************/

void CButtonManager::LoadCurrentDisplayList()
{
	CString str;
	str.Format( "Load Display List %d %d \n",
		m_nButtonManagerMode,
		m_nCurrentListNo );
	TRACE( str );

	switch( m_nButtonManagerMode )
	{
	case BUTMGR_MODE_USER:
		LoadDisplayListNormal();
		break;

	case BUTMGR_MODE_EDIT_LAYOUT:
		LoadDisplayListSetupLayout();
		break;

	case BUTMGR_MODE_EDIT_LIST:
		LoadDisplayListSetupLists();
		break;
	}
}

/**********************************************************************/

void CButtonManager::LoadDisplayListNormal()
{
	for ( int n = 0; n < m_ButtonDisplay.GetSize(); n++ )
		m_ButtonDisplay[n].SetButtonType( BUTTON_TYPE_HIDE );

	if ( 0 == m_nCurrentListNo )
	{
		for ( int nButtonNo = 0; nButtonNo < ButtonOptions.GetButtonBarLineSize(); nButtonNo++ )
		{
			CButtonInfo infoLibrary;
			infoLibrary.SetListNo( 0 );
			infoLibrary.SetLineNo( 0 );
			infoLibrary.SetButtonNo( nButtonNo );

			CButtonInfo infoDisplay;
			GetDisplayButton( 0, nButtonNo, infoDisplay );
				
			int nPos;
			if ( m_ButtonLibrary.Find( infoLibrary, nPos ) == TRUE )
			{
				m_ButtonLibrary.GetAt( nPos, infoLibrary );
				infoDisplay.LibraryToDisplay( infoLibrary );
			}
			else
			{
				infoDisplay.CreateSpecialButton( BUTTON_TYPE_UNUSED );
			}

			SetDisplayButton( 0, nButtonNo, infoDisplay );
		}
	}
	else
	{
		int nMaxLineNo = 0;
		int nLineOffset = ( ButtonOptions.GetButtonBarControlLinePosInt() == 1 ) ? 0 : 1;
		for ( int nLineNo = 0; nLineNo < ButtonOptions.GetButtonBarLineCount() - 1; nLineNo++ )
		{
			for ( int nButtonNo = 0; nButtonNo < ButtonOptions.GetButtonBarLineSize(); nButtonNo++ )
			{
				CButtonInfo infoLibrary;
				infoLibrary.SetListNo( m_nCurrentListNo );
				infoLibrary.SetLineNo( nLineNo );
				infoLibrary.SetButtonNo( nButtonNo );

				int nPos;
				if ( m_ButtonLibrary.Find( infoLibrary, nPos ) == TRUE )
				{
					m_ButtonLibrary.GetAt( nPos, infoLibrary );
					
					CButtonInfo infoDisplay;
					GetDisplayButton( nLineNo + nLineOffset, nButtonNo, infoDisplay );
					infoDisplay.LibraryToDisplay( infoLibrary );
					SetDisplayButton( nLineNo + nLineOffset, nButtonNo, infoDisplay );
					
					nMaxLineNo = nLineNo;
				}
			}
		}

		for ( int nLineNo = 0; nLineNo <= nMaxLineNo; nLineNo++ )
		{
			for ( int nButtonNo = 0; nButtonNo < ButtonOptions.GetButtonBarLineSize(); nButtonNo++ )
			{	
				CButtonInfo infoDisplay;
				GetDisplayButton( nLineNo + nLineOffset, nButtonNo, infoDisplay );
				
				if ( BUTTON_TYPE_HIDE == infoDisplay.GetButtonType() )
				{
					infoDisplay.CreateSpecialButton( BUTTON_TYPE_UNUSED );
					SetDisplayButton( nLineNo + nLineOffset, nButtonNo, infoDisplay );
				}
			}
		}

		int nControlLineNo = ( ButtonOptions.GetButtonBarControlLinePosInt() == 1 ) ? nMaxLineNo + 1 : 0;
		
		int nHomeButtonPos, nBackButtonPos, nParentButtonPos = 0;
		if ( ButtonOptions.GetButtonBarControlButtonPosInt() == 0 )
		{
			nHomeButtonPos = 0;
			nBackButtonPos = 1;
			nParentButtonPos = ButtonOptions.GetButtonBarLineSize() - 1;
		}
		else
		{
			nHomeButtonPos = ButtonOptions.GetButtonBarLineSize() - 1;
			nBackButtonPos = ButtonOptions.GetButtonBarLineSize() - 2;
			nParentButtonPos = 0;
		}

		CButtonInfo infoDisplay;
		GetDisplayButton( nControlLineNo, nHomeButtonPos, infoDisplay );
		infoDisplay.CreateSpecialButton( BUTTON_TYPE_HOME );
		SetDisplayButton( nControlLineNo, nHomeButtonPos, infoDisplay );

		GetDisplayButton( nControlLineNo, nBackButtonPos, infoDisplay );
		infoDisplay.CreateSpecialButton( BUTTON_TYPE_BACK );
		SetDisplayButton( nControlLineNo, nBackButtonPos, infoDisplay );
		
		if ( ButtonOptions.GetButtonBarParentButtonFlag() == TRUE )
		{
			int nSize = m_arrayParentTrail.GetSize(); 
			if ( nSize > 0 )
			{
				CButtonInfo infoParent = m_arrayParentTrail.GetAt( nSize - 1 );
			
				GetDisplayButton( nControlLineNo, nParentButtonPos, infoDisplay );
				infoDisplay.SetButtonType( BUTTON_TYPE_PARENT );
				infoDisplay.SetOwnBackColour( ButtonOptions.GetButtonColourBackParentInt() );
				infoDisplay.SetOwnTextColour( ButtonOptions.GetButtonColourTextParentInt() );
				infoDisplay.CopyTextLines( infoParent );
				infoDisplay.SetFontSize(14);
				SetDisplayButton( nControlLineNo, nParentButtonPos, infoDisplay );
			}
		}
	}
}

/**********************************************************************/

void CButtonManager::LoadDisplayListSetupLayout()
{
	int nMenuLine = 0;
	int nContLine = ButtonOptions.GetButtonBarLineCount() - 1;
	if ( ButtonOptions.GetButtonBarControlLinePosInt() == 0 )
	{
		nMenuLine = 1;
		nContLine = 0;
	}

	for ( int nLineNo = 0; nLineNo < ButtonOptions.GetButtonBarLineCount(); nLineNo++ )
	{
		int nButtonType = ( nLineNo == nContLine ) ? BUTTON_TYPE_HIDE : BUTTON_TYPE_UNUSED;
		for ( int nButtonNo = 0; nButtonNo < ButtonOptions.GetButtonBarLineSize(); nButtonNo++ )
		{
			CButtonInfo infoDisplay;
			GetDisplayButton( nLineNo, nButtonNo, infoDisplay );
			infoDisplay.CreateSpecialButton( nButtonType );
			SetDisplayButton( nLineNo, nButtonNo, infoDisplay );
		}
	}

	int nHomeButtonPos, nBackButtonPos, nParentButtonPos = 0;
	if ( ButtonOptions.GetButtonBarControlButtonPosInt() == 0 )
	{
		nHomeButtonPos = 0;
		nBackButtonPos = 1;
		nParentButtonPos = ButtonOptions.GetButtonBarLineSize() - 1;
	}
	else
	{
		nHomeButtonPos = ButtonOptions.GetButtonBarLineSize() - 1;
		nBackButtonPos = ButtonOptions.GetButtonBarLineSize() - 2;
		nParentButtonPos = 0;
	}

	CButtonInfo infoDisplay;
	GetDisplayButton( nMenuLine, 0, infoDisplay );
	infoDisplay.SetButtonType( BUTTON_TYPE_ACTION );
	infoDisplay.SetOwnBackColour( ButtonOptions.GetButtonColourBackActionInt() );
	infoDisplay.SetOwnTextColour( ButtonOptions.GetButtonColourTextActionInt() );
	infoDisplay.SetDetail1( 0 );
	infoDisplay.SetDetail2( 0 );
	infoDisplay.SetDetail3( 0 );
	infoDisplay.SetTextLines( "Action,Button" );
	SetDisplayButton( nMenuLine, 0, infoDisplay );

	GetDisplayButton( nMenuLine, 1, infoDisplay );
	infoDisplay.SetButtonType( BUTTON_TYPE_LIST );
	infoDisplay.SetOwnBackColour( ButtonOptions.GetButtonColourBackListInt() );
	infoDisplay.SetOwnTextColour( ButtonOptions.GetButtonColourTextListInt() );
	infoDisplay.SetDetail1( 0 );
	infoDisplay.SetDetail2( 0 );
	infoDisplay.SetDetail3( 0 );
	infoDisplay.SetTextLines( "List,Button" );
	SetDisplayButton( nMenuLine, 1, infoDisplay );

	if ( ButtonOptions.GetButtonBarParentButtonFlag() == TRUE )
	{
		GetDisplayButton( nContLine, nParentButtonPos, infoDisplay );
		infoDisplay.SetButtonType( BUTTON_TYPE_PARENT );
		infoDisplay.SetOwnBackColour( ButtonOptions.GetButtonColourBackParentInt() );
		infoDisplay.SetOwnTextColour( ButtonOptions.GetButtonColourTextParentInt() );
		infoDisplay.SetDetail1( 0 );
		infoDisplay.SetDetail2( 0 );
		infoDisplay.SetDetail3( 0 );
		infoDisplay.SetTextLines( "Parent,Button" );
		infoDisplay.SetFontSize(14);
		SetDisplayButton( nContLine, nParentButtonPos, infoDisplay );
	}

	GetDisplayButton( nContLine, nBackButtonPos, infoDisplay );
	infoDisplay.CreateSpecialButton( BUTTON_TYPE_BACK );
	infoDisplay.SetTextLines( "Back,Button" );
	SetDisplayButton( nContLine, nBackButtonPos, infoDisplay );

	GetDisplayButton( nContLine, nHomeButtonPos, infoDisplay );
	infoDisplay.SetButtonType( BUTTON_TYPE_HOME );
	infoDisplay.CreateSpecialButton( BUTTON_TYPE_HOME );
	infoDisplay.SetTextLines( "Home,Button" );
	SetDisplayButton( nContLine, nHomeButtonPos, infoDisplay );
}

/**********************************************************************/

void CButtonManager::LoadDisplayListSetupLists()
{
	for ( int n = 0; n < m_ButtonDisplay.GetSize(); n++ )
	{
		m_ButtonDisplay[n].SetListNo( m_nCurrentListNo );
		m_ButtonDisplay[n].SetButtonType( BUTTON_TYPE_HIDE );
	}

	int nLineOffset = ( ButtonOptions.GetButtonBarControlLinePosInt() == 1 ) ? 0 : 1;
	int nMaxLineNo = ( 0 == m_nCurrentListNo ) ? 0 : ButtonOptions.GetButtonBarLineCount() - 2;

	for ( int nLineNo = 0; nLineNo <= nMaxLineNo; nLineNo++ )
	{
		for ( int nButtonNo = 0; nButtonNo < ButtonOptions.GetButtonBarLineSize(); nButtonNo++ )
		{
			CButtonInfo infoDisplay;
			GetDisplayButton( nLineNo + nLineOffset, nButtonNo, infoDisplay );

			CButtonInfo infoLibrary;
			infoLibrary.SetListNo( m_nCurrentListNo );
			infoLibrary.SetLineNo( nLineNo );
			infoLibrary.SetButtonNo( nButtonNo );
				
			int nPos;
			if ( m_ButtonLibrary.Find( infoLibrary, nPos ) == TRUE )
			{
				m_ButtonLibrary.GetAt( nPos, infoLibrary );
				infoDisplay.LibraryToDisplay( infoLibrary );
			}
			else
			{
				infoDisplay.LibraryToDisplay( infoLibrary );
				infoDisplay.CreateSpecialButton( BUTTON_TYPE_UNUSED );
			}

			SetDisplayButton( nLineNo + nLineOffset, nButtonNo, infoDisplay );
		}
	}

	int nControlLineNo = 0;
	if ( ButtonOptions.GetButtonBarControlLinePosInt() != 0 )
		nControlLineNo = ( 0 == m_nCurrentListNo ) ? 1 : ButtonOptions.GetButtonBarLineCount() - 1;
	
	for ( int nLineNo = 0; nLineNo < ButtonOptions.GetButtonBarLineCount(); nLineNo++ )
	{
		if ( nLineNo != nControlLineNo )
		{
			if ( ( m_nCurrentListNo != 0 ) || ( nLineNo < 2 ) )
			{
				for ( int nButtonNo = 0; nButtonNo < ButtonOptions.GetButtonBarLineSize(); nButtonNo++ )
				{	
					CButtonInfo infoDisplay;
					GetDisplayButton( nLineNo, nButtonNo, infoDisplay );
					
					if ( BUTTON_TYPE_HIDE == infoDisplay.GetButtonType() )
					{
						infoDisplay.CreateSpecialButton( BUTTON_TYPE_UNUSED );
						SetDisplayButton( nLineNo, nButtonNo, infoDisplay );
					}
				}
			}
		}
	}

	CButtonInfo infoDisplay;
	GetDisplayButton( nControlLineNo, 0, infoDisplay );
	infoDisplay.CreateSpecialButton( BUTTON_TYPE_LIST_SELECT );
	SetDisplayButton( nControlLineNo, 0, infoDisplay );

	if ( 0 != m_nCurrentListNo )
	{
		GetDisplayButton( nControlLineNo, 1, infoDisplay );
		infoDisplay.CreateSpecialButton( BUTTON_TYPE_LIST_NAME );
		SetDisplayButton( nControlLineNo, 1, infoDisplay );
	}

	{
		int nButtonNo = ButtonOptions.GetButtonBarLineSize() - 1;
		GetDisplayButton( nControlLineNo, nButtonNo, infoDisplay );
		infoDisplay.CreateSpecialButton( BUTTON_TYPE_LIST_SAVE );
		SetDisplayButton( nControlLineNo, nButtonNo, infoDisplay );
	}
}

/**********************************************************************/

bool CButtonManager::GotoList( CButtonInfo& infoDisplay )
{
	int nListNo = infoDisplay.GetDetail1();
	if ( ( nListNo < 1 ) || ( nListNo > BUTMGR_MAX_LIST_NO ) )
		return FALSE;

	if ( nListNo == m_nCurrentListNo )
		return FALSE;

	for ( int n = 0; n < m_arrayListTrail.GetSize(); n++ )
		if ( m_arrayListTrail[n] == nListNo )
			return FALSE;

	if ( m_nCurrentListNo != 0 )
		m_arrayListTrail.Add( m_nCurrentListNo );

	m_arrayParentTrail.Add( infoDisplay );

	m_nCurrentListNo = nListNo;
	LoadCurrentDisplayList();

	return TRUE;
}

/**********************************************************************/

bool CButtonManager::JumpToList( CButtonInfo& infoDisplay )
{
	bool bResult = FALSE;

	int nListNo = infoDisplay.GetDetail1();
	if ( ( nListNo >= 1 ) && ( nListNo <= BUTMGR_MAX_LIST_NO ) )
	{
		m_nCurrentListNo = nListNo;
		LoadCurrentDisplayList();
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

bool CButtonManager::GoBack()
{
	bool bResult = FALSE; 

	if ( 0 != m_nCurrentListNo )
	{
		{
			int nSize = m_arrayListTrail.GetSize();
			
			if ( nSize > 0 )
			{
				m_nCurrentListNo = m_arrayListTrail.GetAt( nSize - 1 );
				m_arrayListTrail.RemoveAt( nSize - 1 );
			}
			else
			{
				m_nCurrentListNo = 0;
			}
		}

		{
			int nSize = m_arrayParentTrail.GetSize();

			if ( nSize > 0 )
				m_arrayParentTrail.RemoveAt( nSize - 1 );
		}
	
		LoadCurrentDisplayList();
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

void CButtonManager::GoHome()
{
	m_nCurrentListNo = 0;
	m_arrayListTrail.RemoveAll();
	m_arrayParentTrail.RemoveAll();
	LoadCurrentDisplayList();
}

/**********************************************************************/

void CButtonManager::InvalidateImage( CButtonInfo& infoDisplay )
{
	int nButtonIdx = ( infoDisplay.GetLineNo() * ButtonOptions.GetButtonBarLineSize() ) + infoDisplay.GetButtonNo();
	if ( ( nButtonIdx >= 0 ) && ( nButtonIdx < m_arrayBitmaps.GetSize() ) )
	{
		CBitmapInfo* pBitmap = m_arrayBitmaps.GetAt(nButtonIdx);
		pBitmap -> DeleteOwnDC();
	}
}

/**********************************************************************/

void CButtonManager::LoadImage( int nListNo, CButtonInfo& infoDisplay, CDC* pDC )
{
	int nLineNo = infoDisplay.GetLineNo();
	int nButtonNo = infoDisplay.GetButtonNo();
	int nButtonIdx = ( nLineNo * ButtonOptions.GetButtonBarLineSize() ) + nButtonNo;

	if ( ( nButtonIdx >= 0 ) && ( nButtonIdx < m_arrayBitmaps.GetSize() ) )
	{
		CBitmapInfo* pBitmap = m_arrayBitmaps.GetAt(nButtonIdx);

		CString strImageFilename = "";
		
		if ( TRUE == m_bTempImageMode )
		{
			if ( infoDisplay.GetButtonImageFilename( nListNo, 1, strImageFilename ) == FALSE )
				infoDisplay.GetButtonImageFilename( nListNo, 0, strImageFilename );
		}
		else
		{
			infoDisplay.GetButtonImageFilename( nListNo, 0, strImageFilename );
		}

		if ( ( strImageFilename == "" ) || ( ::FileExists( strImageFilename ) == FALSE ) )
		{
			pBitmap -> DeleteOwnDC();
		}
		else
		{	
			if ( FALSE == pBitmap -> CreatedDC() )
				pBitmap -> CreateOwnDC( pDC );
			
			pBitmap -> LoadImage( strImageFilename );
		}
	}
}

/**********************************************************************/

bool CButtonManager::GotDisplayableImage( CButtonInfo& infoDisplay )
{
	int nButtonIdx = ( infoDisplay.GetLineNo() * ButtonOptions.GetButtonBarLineSize() ) + infoDisplay.GetButtonNo();
	if ( ( nButtonIdx >= 0 ) && ( nButtonIdx < m_arrayBitmaps.GetSize() ) )
	{
		CBitmapInfo* pBitmap = m_arrayBitmaps.GetAt(nButtonIdx);
		return ( ( pBitmap -> GotImage() ) && ( pBitmap -> GetHeight() <= 350 ) && ( pBitmap -> GetWidth() <= 350 ) );
	}
	return FALSE;
}

/**********************************************************************/

CBitmapInfo* CButtonManager::GetBitmapInfo( CButtonInfo& infoDisplay )
{
	int nButtonIdx = ( infoDisplay.GetLineNo() * ButtonOptions.GetButtonBarLineSize() ) + infoDisplay.GetButtonNo();
	if ( ( nButtonIdx >= 0 ) && ( nButtonIdx < m_arrayBitmaps.GetSize() ) )
		return m_arrayBitmaps.GetAt(nButtonIdx);
	else
		return NULL;
}

/**********************************************************************/

void CButtonManager::TempImageModeOn()
{
	BackgroundLayoutInfo.CreateLayoutFolders();

	int nFileNo = 0;
	CFileRemover FileRemover;
	FileRemover.RemoveFolderContents( BackgroundLayoutInfo.GetButtonImagePathTemp(), nFileNo );

	for ( int n = 0; n < m_ButtonLibrary.GetSize(); n++ )
	{
		CButtonInfo infoButton;
		m_ButtonLibrary.GetAt( n, infoButton );
		infoButton.SetImageTypeTemp( "" );
		m_ButtonLibrary.SetAt( n, infoButton );
	}

	for ( int n = 0; n < m_ButtonDisplay.GetSize(); n++ )
	{
		CButtonInfo infoButton = m_ButtonDisplay.GetAt( n );
		infoButton.SetImageTypeTemp( "" );
		m_ButtonDisplay.SetAt( n, infoButton );
	}

	m_bTempImageMode = TRUE;
}

/**********************************************************************/

void CButtonManager::TempImageModeOff( bool bSave )
{
	if ( TRUE == bSave )
	{
		CStringArray arrayTypes;
		arrayTypes.Add( "BMP" );
		arrayTypes.Add( "GIF" );
		arrayTypes.Add( "JPG" );
		arrayTypes.Add( "PNG" );
			
		for ( int n = 0; n < m_ButtonLibrary.GetSize(); n++ )
		{
			CButtonInfo infoButton;
			m_ButtonLibrary.GetAt( n, infoButton );
			
			CString strImageType = infoButton.GetImageTypeTemp();
			
			if ( strImageType != "" )
			{
				for ( int x = 0; x < arrayTypes.GetSize(); x++ )
				{
					infoButton.SetImageType( arrayTypes.GetAt(x) );
					
					if ( strImageType != arrayTypes.GetAt(x) )
					{
						CString strFilename = "";
						infoButton.GetButtonImageFilename( infoButton.GetListNo(), 0, strFilename );
						CFileRemover FileRemover( strFilename );
					}
					else
					{
						CString strSource, strDest;
						infoButton.GetButtonImageFilename( infoButton.GetListNo(), 1, strSource );
						infoButton.GetButtonImageFilename( infoButton.GetListNo(), 0, strDest );
						CopyFile( strSource, strDest, FALSE );
						CFileRemover FileRemover( strSource );
					}
				}

				if ( strImageType != "XXX" )
					infoButton.SetImageType( strImageType );
				else
					infoButton.SetImageType( "" );
			}

			m_ButtonLibrary.SetAt( n, infoButton );
		}
	}

	m_bTempImageMode = FALSE;
}

/**********************************************************************/
