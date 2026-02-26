/**********************************************************************/
#include "DefEditTableAreaDlg.h"
/**********************************************************************/

CDefEditTableAreaDlg::CDefEditTableAreaDlg( int nIndex, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent /*=NULL*/)
	: CSSDialog(CDefEditTableAreaDlg::IDD, pParent),
	m_MacroSelector( m_editMacro, m_comboMacro ),
	m_WarnCancelAfterChange( WarnCancelAfterChange )
{
	//{{AFX_DATA_INIT(CDefEditTableAreaDlg)
	//}}AFX_DATA_INIT
	m_nIndex = nIndex;
	m_wIDKP[0] = IDC_CHECK_KP1;
	m_wIDKP[1] = IDC_CHECK_KP2;
	m_wIDKP[2] = IDC_CHECK_KP3;
	m_wIDKP[3] = IDC_CHECK_KP4;
	m_wIDKP[4] = IDC_CHECK_KP5;
	m_wIDKP[5] = IDC_CHECK_KP6;
	m_wIDKP[6] = IDC_CHECK_KP7;
	m_wIDKP[7] = IDC_CHECK_KP8;
	m_wIDKP[8] = IDC_CHECK_KP9;
	m_wIDKP[9] = IDC_CHECK_KP10;
	m_wIDKP[10] = IDC_CHECK_KP11;
	m_wIDKP[11] = IDC_CHECK_KP12;
	m_wIDKP[12] = IDC_CHECK_KP13;
	m_wIDKP[13] = IDC_CHECK_KP14;
	m_wIDKP[14] = IDC_CHECK_KP15;
	m_wIDKP[15] = IDC_CHECK_KP16;
}

/**********************************************************************/

void CDefEditTableAreaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditTableAreaDlg)
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_CHECK_AUTOTAB, m_checkAutoTab);
	DDX_Control(pDX, IDC_CHECK_SUPPRESSCOVERS, m_checkSuppressCovers);
	DDX_Control(pDX, IDC_EDIT_MACRO, m_editMacro);
	DDX_Control(pDX, IDC_COMBO_MACRO, m_comboMacro);

	for ( int n = 0; n < 16; n++ )
	{
		DDX_Control( pDX, m_wIDKP[n], m_checkKP[n] );
	}

	if ( TRUE == pDX -> m_bSaveAndValidate )
	{
		int nStart = GetEditBoxInt( *GetEditTableFrom() );
		if ( nStart <= 0 )
		{
			Prompter.Error( "Start table must be between 1 and 9999" );
			GetEditTableFrom() -> SetSel(0,-1);
			GetEditTableFrom() -> SetFocus();
			pDX -> Fail();
		}

		int nEnd = GetEditBoxInt( *GetEditTableTo() );
		if ( nEnd <= 0 )
		{
			Prompter.Error( "End table must be between 1 and 9999" );
			GetEditTableTo() -> SetSel(0,-1);
			GetEditTableTo() -> SetFocus();
			pDX -> Fail();
		}

		if ( nEnd < nStart )
		{
			Prompter.Error( "End table must be greater than or equal to start table" );
			GetEditTableTo() -> SetSel(0,-1);
			GetEditTableTo() -> SetFocus();
			pDX -> Fail();
		}
	}

	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditTableAreaDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditTableAreaDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnSpinButton)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	ON_EN_KILLFOCUS(IDC_EDIT_MACRO, OnKillFocusMacro)
	ON_CBN_SELCHANGE(IDC_COMBO_MACRO, OnSelectMacro)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditTableAreaDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SubclassEdit( IDC_EDIT_TABLEFROM, SS_NUM, 4, "%d" );
	SubclassEdit( IDC_EDIT_TABLETO, SS_NUM, 4, "%d" );

	m_editMacro.LimitText( 4 );
	m_MacroSelector.FillMacroCombo(0);

	GetData();
	return TRUE;  
}

/**********************************************************************/
CEdit* CDefEditTableAreaDlg::GetEditTableFrom(){ return GetEdit( IDC_EDIT_TABLEFROM ); }
CEdit* CDefEditTableAreaDlg::GetEditTableTo(){ return GetEdit( IDC_EDIT_TABLETO ); }
/**********************************************************************/

void CDefEditTableAreaDlg::GetData()
{
	CTableAreaCSVRecord TableArea;
	DataManagerNonDb.TableAreas.GetAt( m_nIndex, TableArea );

	CString strName = TableArea.GetAreaName();

	CString strTitle;
	strTitle.Format( "Edit Table Area (%s)",
		(const char*) strName );
	SetWindowText( strTitle );

	m_editName.SetWindowText( strName );
	m_checkAutoTab.SetCheck( TableArea.GetAutoTabsOnlyFlag() );
	m_checkSuppressCovers.SetCheck( TableArea.GetSuppressCoversFlag() );
	SetEditBoxInt( *GetEditTableFrom(), TableArea.GetTableNoLo() );
	SetEditBoxInt( *GetEditTableTo(), TableArea.GetTableNoHi() );

	m_nMacroNo = TableArea.GetTableOpenMacro();
	m_MacroSelector.UpdateMacroCombo( m_nMacroNo );
	m_MacroSelector.UpdateMacroEditBox();

	for ( int n = 0; n < 16; n++ )
	{
		m_checkKP[n].SetCheck( TableArea.GetKPFlag(n+1) );
	}
}

/**********************************************************************/

void CDefEditTableAreaDlg::OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if ( pNMUpDown -> iDelta > 0 )
		SpinPrevious();
	else
		SpinNext();

	*pResult = 0;
}

/**********************************************************************/

void CDefEditTableAreaDlg::OnDefaultButton() 
{
	SpinNext();
}

/**********************************************************************/

void CDefEditTableAreaDlg::SpinPrevious()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_nIndex > 0 )
		{
			SaveData();
			m_nIndex--;
			GetData();

			if ( m_pListDataDlg != NULL )
				m_pListDataDlg -> SelectLine ( m_nIndex );
		}
	}
}

/**********************************************************************/

void CDefEditTableAreaDlg::SpinNext()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_nIndex < DataManagerNonDb.TableAreas.GetSize() - 1 )
		{
			SaveData();
			m_nIndex++;
			GetData();

			if ( m_pListDataDlg != NULL )
				m_pListDataDlg -> SelectLine( m_nIndex );
		}
	}
}

/**********************************************************************/

void CDefEditTableAreaDlg::OnSelectMacro()
{
	m_MacroSelector.SelectMacroFromCombo();
	m_nMacroNo = m_MacroSelector.GetMacroNo();
}

/**********************************************************************/

void CDefEditTableAreaDlg::OnKillFocusMacro()
{
	m_MacroSelector.SelectMacroFromEditBox();
	m_nMacroNo = m_MacroSelector.GetMacroNo();
}

/**********************************************************************/

void CDefEditTableAreaDlg::SaveData()
{
	CTableAreaCSVRecord TableArea;
	DataManagerNonDb.TableAreas.GetAt( m_nIndex, TableArea );
	
	TableArea.SetAreaName( GetEditBoxText( m_editName ) );
	TableArea.SetAutoTabsOnlyFlag( IsTicked( m_checkAutoTab ) );
	TableArea.SetSuppressCoversFlag( IsTicked( m_checkSuppressCovers ) );
	TableArea.SetTableNoLo( GetEditBoxInt( *GetEditTableFrom() ) );
	TableArea.SetTableNoHi( GetEditBoxInt( *GetEditTableTo() ) );
	TableArea.SetTableOpenMacro( m_nMacroNo );

	for ( int n = 0; n < 16; n++ )
	{
		TableArea.SetKPFlag(n+1, IsTicked( m_checkKP[n] ) );
	}

	DataManagerNonDb.TableAreas.SetAt( m_nIndex, TableArea );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CDefEditTableAreaDlg::OnOK() 
{
	if ( UpdateData(TRUE) == TRUE )
	{
		SaveData();
		EndDialog ( IDOK );
	}
}

/**********************************************************************/
