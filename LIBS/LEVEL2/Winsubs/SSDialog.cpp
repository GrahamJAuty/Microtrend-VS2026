/**********************************************************************/

void AFXAPI DDV_IntegerString ( CDataExchange* pDX, CString const& strValue, CString const& strLow, CString const& strHigh );

/**********************************************************************/

CSSDialog::CSSDialog ( UINT nIDTemplate, CWnd* pParentWnd )
	: CDialog ( nIDTemplate, pParentWnd )
{
	m_bValidate = TRUE;
}

/**********************************************************************/

CSSDialog::CSSDialog ( LPCTSTR lpszTemplateName, CWnd* pParentWnd )
	: CDialog ( lpszTemplateName, pParentWnd )
{
	m_bValidate = TRUE;
}

/**********************************************************************/

CSSDialog::CSSDialog()
	: CDialog()
{
	m_bValidate = TRUE;
}

/**********************************************************************/

CSSDialog::~CSSDialog()
{
	POSITION pos = m_FilterArray.GetStartPosition();

	while ( pos != NULL )
	{
		CEditFilter* pFilter;
		WORD nIDC;

		m_FilterArray.GetNextAssoc ( pos, nIDC, (void*&) pFilter );
		delete pFilter;
	}

	m_FilterArray.RemoveAll();
}

/**********************************************************************/

void CSSDialog::SubclassEdit ( int nIDC, int nType, int nWidth, const char* szFmt )
{
	CEditFilter* pTemp;
	if ( m_FilterArray.Lookup ( nIDC, (void*&) pTemp ) == TRUE )	// check if already exists
	{
		if ( pTemp->GetSafeHwnd() != NULL )
		{
			pTemp->SetFormat ( nType, szFmt );

			if ( nWidth > 0 )
				pTemp->LimitText ( nWidth );

			pTemp->FormatWindowText();

			return;
		}
		else					// window may have gone if looping on domodal()
			delete pTemp;
	}

	CEditFilter* pFilter = new CEditFilter ( nType, szFmt );

	if ( pFilter != NULL )
	{
		m_FilterArray.SetAt ( nIDC, pFilter );

		if ( pFilter->SubclassDlgItem ( nIDC, this ) == TRUE )
		{
			if ( nWidth > 0 )
				pFilter->LimitText ( nWidth );

			pFilter->FormatWindowText();
		}
	}
}

/**********************************************************************/

void CSSDialog::SubclassEdit ( int nIDC, int nType, int nWidth, const char* szFmt, int n1 )
{
	CString strFmt;
	CCSV csv ( szFmt, '*' );

	if ( csv.GetSize() == 2 )
		strFmt.Format ( "%s%d%s", csv.GetString(0), n1, csv.GetString(1) );
	else
		strFmt = szFmt;

	SubclassEdit ( nIDC, nType, nWidth, strFmt );
}

void CSSDialog::SubclassEdit ( int nIDC, int nType, int nWidth, const char* szFmt, int n1, int n2 )
{
	CString strFmt;
	CCSV csv ( szFmt, '*' );

	if ( csv.GetSize() == 3 )
		strFmt.Format ( "%s%d%s%d%s", csv.GetString(0), n1, csv.GetString(1), n2, csv.GetString(2) );
	else
		strFmt = szFmt;

	SubclassEdit ( nIDC, nType, nWidth, strFmt );
}

/**********************************************************************/

bool CSSDialog::UpdateData ( bool bSave, bool bValidate )
{
	m_bValidate = bValidate;

	bool bReply = CDialog::UpdateData ( bSave ) != 0;

	m_bValidate = TRUE;

	return bReply;
}

/**********************************************************************/

void CSSDialog::SetText ( HWND hWndCtrl, int nIDC, const char* szBuf )
{
	CString strBuf;
	CEditFilter* pFilter;

	if ( m_FilterArray.Lookup ( nIDC, (void*&) pFilter ) == TRUE )
	{
		strBuf = pFilter->Format ( szBuf );
	}
	else
		strBuf = szBuf;

	char szOld[256];

	::GetWindowText ( hWndCtrl, szOld, sizeof(szOld) );

	if ( strBuf != szOld )	// check if text really changed (reduces flash in controls)
		::SetWindowText ( hWndCtrl, strBuf );
}

/**********************************************************************/
/*	DDX routines											*/
/**********************************************************************/

void CSSDialog::DDX_Text ( CDataExchange* pDX, int nIDC, CString& value )
{
	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);

	if ( pDX->m_bSaveAndValidate == TRUE )
	{
		int nLen = ::GetWindowTextLength ( hWndCtrl );
		::GetWindowText ( hWndCtrl, value.GetBufferSetLength(nLen), nLen+1 );
		value.ReleaseBuffer();
	}
	else
	{
		SetText ( hWndCtrl, nIDC, value );
	}
}

/**********************************************************************/

void CSSDialog::DDX_Text ( CDataExchange* pDX, int nIDC, int& value )
{
	char szBuf[32];

	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);

	if ( pDX->m_bSaveAndValidate == TRUE )
	{
		::GetWindowText ( hWndCtrl, szBuf, sizeof(szBuf) );
		value = atoi ( szBuf );
	}
	else
	{
		sprintf_s ( szBuf, sizeof(szBuf), "%d", value );
		SetText ( hWndCtrl, nIDC, szBuf );
	}
}

/**********************************************************************/

void CSSDialog::DDX_Text(CDataExchange* pDX, int nIDC, __int64& value)
{
	char szBuf[32];

	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);

	if (pDX->m_bSaveAndValidate == TRUE)
	{
		::GetWindowText(hWndCtrl, szBuf, sizeof(szBuf));
		value = _atoi64(szBuf);
	}
	else
	{
		sprintf_s(szBuf, sizeof(szBuf), "%I64d", value);
		SetText(hWndCtrl, nIDC, szBuf);
	}
}

/**********************************************************************/

void CSSDialog::DDX_Text ( CDataExchange* pDX, int nIDC, long& value )
{
	char szBuf[32];

	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);

	if ( pDX->m_bSaveAndValidate == TRUE )
	{
		::GetWindowText ( hWndCtrl, szBuf, sizeof(szBuf) );
		value = atol ( szBuf );
	}
	else
	{
		sprintf_s ( szBuf, sizeof(szBuf), "%ld", value );
		SetText ( hWndCtrl, nIDC, szBuf );
	}
}

/**********************************************************************/

void CSSDialog::DDX_Text ( CDataExchange* pDX, int nIDC, double& value )
{
	char szBuf[32];

	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);

	if ( pDX->m_bSaveAndValidate == TRUE )
	{
		::GetWindowText ( hWndCtrl, szBuf, sizeof(szBuf) );
		value = atof ( szBuf );
	}
	else
	{
		sprintf_s ( szBuf, sizeof(szBuf), "%.*g", 15, value );
		SetText ( hWndCtrl, nIDC, szBuf );
	}
}

/**********************************************************************/

void CSSDialog::DDX_IntegerString ( CDataExchange* pDX, int nIDC, CString& strValue )
{
	DDX_Text ( pDX, nIDC, strValue );
}

/**********************************************************************/
/*	DDV routines											*/
/**********************************************************************/

void CSSDialog::DDV_MaxChars ( CDataExchange* pDX, CString const& value, int nChars )
{
	if ( m_bValidate == TRUE )
		::DDV_MaxChars ( pDX, value, nChars );
}

void CSSDialog::DDV_MinMaxInt ( CDataExchange* pDX, int value, int minVal, int maxVal )
{
	if ( m_bValidate == TRUE )
		::DDV_MinMaxInt ( pDX, value, minVal, maxVal );
}

void CSSDialog::DDV_MinMaxLong ( CDataExchange* pDX, long value, long minVal, long maxVal )
{
	if ( m_bValidate == TRUE )
		::DDV_MinMaxLong ( pDX, value, minVal, maxVal );
}

void CSSDialog::DDV_MinMaxDouble ( CDataExchange* pDX, double const& value, double minVal, double maxVal )
{
	if ( m_bValidate == TRUE )
		::DDV_MinMaxDouble ( pDX, value, minVal, maxVal );
}

void CSSDialog::DDV_IntegerString ( CDataExchange* pDX, CString const& strValue, CString const& strLow, CString const& strHigh )
{
	if ( m_bValidate == TRUE )
		::DDV_IntegerString ( pDX, strValue, strLow, strHigh );
}

/**********************************************************************/

void CSSDialog::ResizeDialog( int nWidth, int nHeight )
{
	CRect rectWindow, rectClient;
	GetWindowRect( rectWindow );
	GetClientRect( rectClient );
	int nXBorder = rectWindow.Width() - rectClient.Width();
	int nYBorder = rectWindow.Height() - rectClient.Height();

	CRect rectDlg = CRect( 0, 0, nWidth, nHeight );
	MapDialogRect( &rectDlg );
	rectDlg.right += nXBorder;
	rectDlg.bottom += nYBorder;
	SetWindowPos( &wndTop, 0, 0, rectDlg.Width(), rectDlg.Height(), SWP_NOMOVE | SWP_NOZORDER );
}

/**********************************************************************/

void CSSDialog::ResizeControl( CWnd* pWnd, int nWidth, int nHeight )
{
	CRect rect ( 0, 0, nWidth, nHeight );
	MapDialogRect( &rect );
	pWnd -> SetWindowPos( &wndTop, 0, 0, rect.Width(), rect.Height(), SWP_NOMOVE | SWP_NOZORDER );
}

/**********************************************************************/

void CSSDialog::MoveControl( CWnd* pWnd, int nXPos, int nYPos )
{
	CRect rect ( nXPos, nYPos, 10, 10 );
	MapDialogRect( &rect );
	pWnd -> SetWindowPos( &wndTop, rect.left, rect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
}

/**********************************************************************/

void CSSDialog::MoveControl( CWnd* pWnd, int nXPos, int nYPos, int nWidth, int nHeight )
{
	CRect rect ( nXPos, nYPos, nXPos + nWidth, nYPos + nHeight );
	MapDialogRect( &rect );
	pWnd -> SetWindowPos( &wndTop, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER );
}

/**********************************************************************/

void CSSDialog::ShowAndEnableWindow( CWnd* pWnd, bool bShow, bool bEnable )
{
	if ( TRUE == bShow )
	{
		pWnd -> ShowWindow( SW_SHOW );
		pWnd -> EnableWindow( bEnable );
	}
	else
	{
		pWnd -> ShowWindow( SW_HIDE );
		pWnd -> EnableWindow( FALSE );
	}
}

/**********************************************************************/

void CSSDialog::AddDialogItem( CWnd* pWnd, int nXPos, int nYPos, bool bAlwaysShow )
{
	CSSDialogItem item;
	item.m_pWnd = pWnd;
	item.m_nXPos = nXPos;
	item.m_nYPos = nYPos;
	item.m_bAlwaysShow = bAlwaysShow;
	m_arrayDialogItems.Add( item );
}

/**********************************************************************/

void CSSDialog::ClearTabFixBuffer()
{
	m_arrayTabFixItems.RemoveAll();
}

/**********************************************************************/

void CSSDialog::AddTabFixItem( CWnd* pWnd, bool bTab, bool bShow, bool bEnable )
{
	if ( TRUE == bTab )
	{
		CSSTabFixItem item;
		item.m_pWnd = pWnd;
		item.m_bShow = bShow;
		item.m_bEnable = ( bShow && bEnable );
		m_arrayTabFixItems.Add( item );
	}
}

/**********************************************************************/

void CSSDialog::ProcessTabFixBuffer( CWnd* pParent )
{
	for ( int n = 0; n < m_arrayDialogItems.GetSize(); n++ )
	{
		CSSDialogItem item = m_arrayDialogItems.GetAt( n );
		
		if ( item.m_nXPos != 0 )
			MoveControl( item.m_pWnd, item.m_nXPos, item.m_nYPos );
		
		ShowAndEnableWindow( item.m_pWnd, item.m_bAlwaysShow );
	}

	for ( int n = 0; n < m_arrayTabFixItems.GetSize(); n++ )
	{
		CSSTabFixItem item = m_arrayTabFixItems.GetAt( n );
		item.m_pWnd -> SetWindowPos( pParent, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE );
		item.m_pWnd -> ShowWindow ( item.m_bShow ? SW_SHOW : SW_HIDE );
		item.m_pWnd -> EnableWindow( item.m_bEnable );
		pParent = item.m_pWnd;
	}

	ClearTabFixBuffer();
}

/**********************************************************************/

bool CSSDialog::IsTicked( CButton& button )
{
	return m_ControlContentHelpers.IsTicked(button);
}

/**********************************************************************/

const char* CSSDialog::GetEditBoxText( CEdit& edit )
{
	return m_ControlContentHelpers.GetEditBoxText(edit);
}

/**********************************************************************/

int CSSDialog::GetEditBoxInt( CEdit& edit )
{
	return m_ControlContentHelpers.GetEditBoxInt(edit);
}

/**********************************************************************/

void CSSDialog::SetEditBoxInt( CEdit& edit, int n )
{
	m_ControlContentHelpers.SetEditBoxInt(edit,n);
}

/**********************************************************************/

__int64 CSSDialog::GetEditBoxInt64(CEdit& edit)
{
	return m_ControlContentHelpers.GetEditBoxInt64(edit);
}

/**********************************************************************/

void CSSDialog::SetEditBoxInt64(CEdit& edit, __int64 n)
{
	m_ControlContentHelpers.SetEditBoxInt64(edit, n);
}

/**********************************************************************/

double CSSDialog::GetEditBoxDouble(CEdit& edit)
{
	return m_ControlContentHelpers.GetEditBoxDouble(edit);
}

/**********************************************************************/

void CSSDialog::SetEditBoxDouble(CEdit& edit, double d, int nDPs)
{
	m_ControlContentHelpers.SetEditBoxDouble(edit, d, nDPs);
}

/**********************************************************************/

const char* CSSDialog::GetComboBoxText(CComboBox& combo)
{
	return m_ControlContentHelpers.GetComboBoxText(combo);
}

/**********************************************************************/
