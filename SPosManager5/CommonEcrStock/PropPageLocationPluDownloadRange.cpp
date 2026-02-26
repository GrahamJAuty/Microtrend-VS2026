 /**********************************************************************/
#include "PluCSVArray.h"
/**********************************************************************/
#include "PropPageLocationPluDownloadRange.h"
/**********************************************************************/

CPropPageLocationPluDownloadRange::CPropPageLocationPluDownloadRange() : CSSPropertyPage(CPropPageLocationPluDownloadRange::IDD)
{
	//{{AFX_DATA_INIT(CPropPageLocationPluDownloadRange)
	//}}AFX_DATA_INIT
	m_pLocationBuffer = NULL;
	m_nPluFromID[0] = IDC_EDIT_PLUNO_FROM1;
	m_nPluFromID[1] = IDC_EDIT_PLUNO_FROM2;
	m_nPluFromID[2] = IDC_EDIT_PLUNO_FROM3;
	m_nPluFromID[3] = IDC_EDIT_PLUNO_FROM4;
	m_nPluFromID[4] = IDC_EDIT_PLUNO_FROM5;
	m_nPluToID[0] = IDC_EDIT_PLUNO_TO1;
	m_nPluToID[1] = IDC_EDIT_PLUNO_TO2;
	m_nPluToID[2] = IDC_EDIT_PLUNO_TO3;
	m_nPluToID[3] = IDC_EDIT_PLUNO_TO4;
	m_nPluToID[4] = IDC_EDIT_PLUNO_TO5;
}

/**********************************************************************/

CPropPageLocationPluDownloadRange::~CPropPageLocationPluDownloadRange()
{
}

/**********************************************************************/

void CPropPageLocationPluDownloadRange::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageLocationPluDownloadRange)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_COMBO_TYPE, m_comboType );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageLocationPluDownloadRange, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageLocationPluDownloadRange)
	ON_CBN_SELCHANGE( IDC_COMBO_TYPE, OnSelectType )
	ON_EN_KILLFOCUS( IDC_EDIT_PLUNO_FROM1, OnKillFocusPluFrom1)
	ON_EN_KILLFOCUS( IDC_EDIT_PLUNO_FROM2, OnKillFocusPluFrom2)
	ON_EN_KILLFOCUS( IDC_EDIT_PLUNO_FROM3, OnKillFocusPluFrom3)
	ON_EN_KILLFOCUS( IDC_EDIT_PLUNO_FROM4, OnKillFocusPluFrom4)
	ON_EN_KILLFOCUS( IDC_EDIT_PLUNO_FROM5, OnKillFocusPluFrom5)
	ON_EN_KILLFOCUS( IDC_EDIT_PLUNO_TO1, OnKillFocusPluTo1)
	ON_EN_KILLFOCUS( IDC_EDIT_PLUNO_TO2, OnKillFocusPluTo2)
	ON_EN_KILLFOCUS( IDC_EDIT_PLUNO_TO3, OnKillFocusPluTo3)
	ON_EN_KILLFOCUS( IDC_EDIT_PLUNO_TO4, OnKillFocusPluTo4)
	ON_EN_KILLFOCUS( IDC_EDIT_PLUNO_TO5, OnKillFocusPluTo5)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageLocationPluDownloadRange::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	m_comboType.AddString( "None" );
	m_comboType.AddString( "Include" );
	m_comboType.AddString( "Exclude" );

	int nType = m_pLocationBuffer -> GetPluDownloadRangeType(0);

	if ( ( nType < 0 ) || ( nType > 2 ) )
		nType = 0;

	m_comboType.SetCurSel( nType );
	OnSelectType();

	m_pLocationBuffer -> TidyPluDownloadRanges(0);

	for ( int n = 0; n < 5; n++ )
	{
		SubclassEdit( m_nPluFromID[n], SS_NUM, Super.MaxPluLen(), "%s" );
		SubclassEdit( m_nPluToID[n], SS_NUM, Super.MaxPluLen(), "%s" );

		{
			CString strPluNo = "";
			__int64 nPluNo = m_pLocationBuffer -> GetPluDownloadRangeStart(0,n);
			
			if ( 0 != nPluNo )
				strPluNo.Format( "%14.14I64d", nPluNo );
			
			GetEditPluFrom(n) -> SetWindowText( strPluNo );
		}

		{
			CString strPluNo = "";
			__int64 nPluNo = m_pLocationBuffer -> GetPluDownloadRangeEnd(0,n);
			
			if ( 0 != nPluNo )
				strPluNo.Format( "%14.14I64d", nPluNo );
			
			GetEditPluTo(n) -> SetWindowText( strPluNo );
		}
		
	}

	return TRUE;  
}

/**********************************************************************/

BOOL CPropPageLocationPluDownloadRange::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageLocationPluDownloadRange::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

bool CPropPageLocationPluDownloadRange::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPageLocationPluDownloadRange::Refresh()
{
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPropPageLocationPluDownloadRange::GetRecordData()
{
}

/**********************************************************************/

void CPropPageLocationPluDownloadRange::SaveRecord()
{
	m_pLocationBuffer -> SetPluDownloadRangeType( 0, m_comboType.GetCurSel() );

	for ( int n = 0; n < 5; n++ )
	{
		CString str;
		GetEditPluFrom(n) -> GetWindowText( str );
		m_pLocationBuffer -> SetPluDownloadRangeStart( 0, n, _atoi64(str) );
		GetEditPluTo(n) -> GetWindowText( str );
		m_pLocationBuffer -> SetPluDownloadRangeEnd( 0, n, _atoi64(str) );
	}
	m_pLocationBuffer -> TidyPluDownloadRanges(0);
}

/**********************************************************************/

void CPropPageLocationPluDownloadRange::OnSelectType()
{
	bool bEnable = ( m_comboType.GetCurSel() != 0 );
	for ( int n = 0; n < 5; n++ )
	{
		GetEditPluFrom(n) -> EnableWindow( bEnable );
		GetEditPluTo(n) -> EnableWindow( bEnable );
	}
}

/**********************************************************************/
void CPropPageLocationPluDownloadRange::OnKillFocusPluFrom1(){ KillFocusPlu( GetEditPluFrom(0) ); }
void CPropPageLocationPluDownloadRange::OnKillFocusPluFrom2(){ KillFocusPlu( GetEditPluFrom(1) ); }
void CPropPageLocationPluDownloadRange::OnKillFocusPluFrom3(){ KillFocusPlu( GetEditPluFrom(2) ); }
void CPropPageLocationPluDownloadRange::OnKillFocusPluFrom4(){ KillFocusPlu( GetEditPluFrom(3) ); }
void CPropPageLocationPluDownloadRange::OnKillFocusPluFrom5(){ KillFocusPlu( GetEditPluFrom(4) ); }
void CPropPageLocationPluDownloadRange::OnKillFocusPluTo1(){ KillFocusPlu( GetEditPluTo(0) ); }
void CPropPageLocationPluDownloadRange::OnKillFocusPluTo2(){ KillFocusPlu( GetEditPluTo(1) ); }
void CPropPageLocationPluDownloadRange::OnKillFocusPluTo3(){ KillFocusPlu( GetEditPluTo(2) ); }
void CPropPageLocationPluDownloadRange::OnKillFocusPluTo4(){ KillFocusPlu( GetEditPluTo(3) ); }
void CPropPageLocationPluDownloadRange::OnKillFocusPluTo5(){ KillFocusPlu( GetEditPluTo(4) ); }
/**********************************************************************/

void CPropPageLocationPluDownloadRange::KillFocusPlu( CEdit* pEdit )
{
	CString strPluNo;
	pEdit -> GetWindowText( strPluNo );

	__int64 nPluNo = _atoi64(strPluNo);

	if ( 0 != nPluNo )
		strPluNo.Format( "%14.14I64d", nPluNo );
	else
		strPluNo = "";
			
	pEdit -> SetWindowText( strPluNo );

}

/**********************************************************************/

