/**********************************************************************/

CProgressIndexSort::CProgressIndexSort()
{
	m_bStopSort = FALSE;
	m_pProgress = NULL;
}

/**********************************************************************/

bool CProgressIndexSort::Progress ( int nTextIndex )
{
	MSG msg;

	if ( ::PeekMessage ( &msg, NULL, 0, 0, PM_REMOVE ) != 0 )
	{
		::TranslateMessage ( &msg );
		::DispatchMessage ( &msg );
	}

	m_pProgress->SetPos ( nTextIndex );

	return m_bStopSort;
}

/**********************************************************************/

bool CProgressIndexSort::Sort ( CProgressCtrl* pProgress, CStringArray* pTextArray, CString (CALLBACK EXPORT* lpGetLabel)(const char*), int nOffset, bool bAscending, bool bIgnoreCase )
{
	m_pProgress = pProgress;
	pProgress->SetRange32 ( 0, pTextArray->GetSize() );
	m_bStopSort = FALSE;

	return CIndexSort::Sort ( pTextArray, lpGetLabel, nOffset, bAscending, bIgnoreCase );
}

/**********************************************************************/

void CProgressIndexSort::Stop()
{
	m_bStopSort = TRUE;
}

/**********************************************************************/
