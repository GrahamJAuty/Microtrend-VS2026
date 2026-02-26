/**********************************************************************/

void AFXAPI DDV_IntegerString ( CDataExchange* pDX, CString const& strValue, CString const& strLow, CString const& strHigh );

/**********************************************************************/

CSSPropertyPage::CSSPropertyPage( UINT nIDTemplate, UINT nIDCaption )
	: CPropertyPage ( nIDTemplate, nIDCaption )
{
	m_bValidate = TRUE;
}

/**********************************************************************/

CSSPropertyPage::CSSPropertyPage( LPCTSTR lpszTemplateName, UINT nIDCaption )
	: CPropertyPage ( lpszTemplateName, nIDCaption )
{
	m_bValidate = TRUE;
}

/**********************************************************************/

CSSPropertyPage::CSSPropertyPage()
	: CPropertyPage()
{
	m_bValidate = TRUE;
}

/**********************************************************************/

CSSPropertyPage::~CSSPropertyPage()
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

void CSSPropertyPage::SubclassEdit ( int nIDC, int nType, int nWidth, const char* szFmt )
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

void CSSPropertyPage::SubclassEdit ( int nIDC, int nType, int nWidth, const char* szFmt, int n1 )
{
	CString strFmt;
	CCSV csv ( szFmt, '*' );

	if ( csv.GetSize() == 2 )
		strFmt.Format ( "%s%d%s", csv.GetString(0), n1, csv.GetString(1) );
	else
		strFmt = szFmt;

	SubclassEdit ( nIDC, nType, nWidth, strFmt );
}

void CSSPropertyPage::SubclassEdit ( int nIDC, int nType, int nWidth, const char* szFmt, int n1, int n2 )
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

bool CSSPropertyPage::UpdateData ( bool bSave, bool bValidate )
{
	m_bValidate = bValidate;

	bool bReply = CDialog::UpdateData ( bSave ) != 0;

	m_bValidate = TRUE;

	return bReply;
}

/**********************************************************************/

void CSSPropertyPage::SetText ( HWND hWndCtrl, int nIDC, const char* szBuf )
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

void CSSPropertyPage::DDX_Text ( CDataExchange* pDX, int nIDC, CString& value )
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

void CSSPropertyPage::DDX_Text ( CDataExchange* pDX, int nIDC, int& value )
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

void CSSPropertyPage::DDX_Text ( CDataExchange* pDX, int nIDC, long& value )
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

void CSSPropertyPage::DDX_Text ( CDataExchange* pDX, int nIDC, double& value )
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

void CSSPropertyPage::DDX_IntegerString ( CDataExchange* pDX, int nIDC, CString& strValue )
{
	DDX_Text ( pDX, nIDC, strValue );
}

/**********************************************************************/
/*	DDV routines											*/
/**********************************************************************/

void CSSPropertyPage::DDV_MaxChars ( CDataExchange* pDX, CString const& value, int nChars )
{
	if ( m_bValidate == TRUE )
		::DDV_MaxChars ( pDX, value, nChars );
}

void CSSPropertyPage::DDV_MinMaxInt ( CDataExchange* pDX, int value, int minVal, int maxVal )
{
	if ( m_bValidate == TRUE )
		::DDV_MinMaxInt ( pDX, value, minVal, maxVal );
}

void CSSPropertyPage::DDV_MinMaxLong ( CDataExchange* pDX, long value, long minVal, long maxVal )
{
	if ( m_bValidate == TRUE )
		::DDV_MinMaxLong ( pDX, value, minVal, maxVal );
}

void CSSPropertyPage::DDV_MinMaxDouble ( CDataExchange* pDX, double const& value, double minVal, double maxVal )
{
	if ( m_bValidate == TRUE )
		::DDV_MinMaxDouble ( pDX, value, minVal, maxVal );
}

void CSSPropertyPage::DDV_IntegerString ( CDataExchange* pDX, CString const& strValue, CString const& strLow, CString const& strHigh )
{
	if ( m_bValidate == TRUE )
		::DDV_IntegerString ( pDX, strValue, strLow, strHigh );
}

/**********************************************************************/

void CSSPropertyPage::ResizeControl( CWnd* pWnd, int nWidth, int nHeight )
{
	CRect rect ( 0, 0, nWidth, nHeight );
	MapDialogRect( &rect );
	pWnd -> SetWindowPos( &wndTop, 0, 0, rect.Width(), rect.Height(), SWP_NOMOVE | SWP_NOZORDER );
}

/**********************************************************************/

void CSSPropertyPage::MoveControl( CWnd* pWnd, int nXPos, int nYPos )
{
	CRect rect ( nXPos, nYPos, 10, 10 );
	MapDialogRect( &rect );
	pWnd -> SetWindowPos( &wndTop, rect.left, rect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
}

/**********************************************************************/

void CSSPropertyPage::MoveControl( CWnd* pWnd, int nXPos, int nYPos, int nWidth, int nHeight )
{
	CRect rect ( nXPos, nYPos, nXPos + nWidth, nYPos + nHeight );
	MapDialogRect( &rect );
	pWnd -> SetWindowPos( &wndTop, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER );
}

/**********************************************************************/

void CSSPropertyPage::ShowAndEnableWindow( CWnd* pWnd, bool bShow, bool bEnable )
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

void CSSPropertyPage::ResizeDialog( int nWidth, int nHeight )
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

bool CSSPropertyPage::IsTicked(CButton& button)
{
	return m_ControlContentHelpers.IsTicked(button);
}

/**********************************************************************/

const char* CSSPropertyPage::GetEditBoxText(CEdit& edit)
{
	return m_ControlContentHelpers.GetEditBoxText(edit);
}

/**********************************************************************/

int CSSPropertyPage::GetEditBoxInt(CEdit& edit)
{
	return m_ControlContentHelpers.GetEditBoxInt(edit);
}

/**********************************************************************/

void CSSPropertyPage::SetEditBoxInt(CEdit& edit, int n)
{
	m_ControlContentHelpers.SetEditBoxInt(edit, n);
}

/**********************************************************************/

__int64 CSSPropertyPage::GetEditBoxInt64(CEdit& edit)
{
	return m_ControlContentHelpers.GetEditBoxInt64(edit);
}

/**********************************************************************/

void CSSPropertyPage::SetEditBoxInt64(CEdit& edit, __int64 n)
{
	m_ControlContentHelpers.SetEditBoxInt64(edit, n);
}

/**********************************************************************/

double CSSPropertyPage::GetEditBoxDouble(CEdit& edit)
{
	return m_ControlContentHelpers.GetEditBoxDouble(edit);
}

/**********************************************************************/

void CSSPropertyPage::SetEditBoxDouble(CEdit& edit, double d, int nDPs)
{
	m_ControlContentHelpers.SetEditBoxDouble(edit, d, nDPs);
}

/**********************************************************************/

const char* CSSPropertyPage::GetComboBoxText(CComboBox& combo)
{
	return m_ControlContentHelpers.GetComboBoxText(combo);
}

/**********************************************************************/
