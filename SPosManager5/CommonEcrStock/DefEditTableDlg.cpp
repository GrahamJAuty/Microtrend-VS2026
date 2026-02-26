/**********************************************************************/
#include "DefEditTableDlg.h"
/**********************************************************************/

CDefEditTableDlg::CDefEditTableDlg( int nIndex, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent /*=NULL*/)
	: CSSDialog(CDefEditTableDlg::IDD, pParent), m_WarnCancelAfterChange( WarnCancelAfterChange )
{
	//{{AFX_DATA_INIT(CDefEditTableDlg)
	//}}AFX_DATA_INIT
	m_nIndex = nIndex;
}

/**********************************************************************/

void CDefEditTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditTableDlg)
	DDX_Control(pDX, IDC_EDIT_NAME, m_editTableName);
	DDX_Control(pDX, IDC_CHECK_FILTER, m_checkFilter);
	DDX_Control(pDX, IDC_CHECK_ACCOUNT, m_checkAccount);
	DDX_Control(pDX, IDC_CHECK_LIMIT, m_checkLimit);
	DDX_Control(pDX, IDC_CHECK_AUTOSTATUS1, m_checkAutoStatus1);
	DDX_Control(pDX, IDC_CHECK_AUTOSTATUS2, m_checkAutoStatus2);
	DDX_Control(pDX, IDC_COMBO_GROUP1, m_comboGroup[0]);
	DDX_Control(pDX, IDC_COMBO_GROUP2, m_comboGroup[1]);
	DDX_Control(pDX, IDC_COMBO_GROUP3, m_comboGroup[2]);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditTableDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditTableDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnSpinButton)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditTableDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_editTableName.LimitText( SPosTableName::TableName.Max );

	SubclassEdit( IDC_EDIT_LIMIT, SS_NUM, 4, "%d" );

	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(40);
	
	for ( int n = 0; n < 3; n++ )	
	{
		m_comboGroup[n].SetTabStops( arrayStops );
	}

	for ( int g = 1; g <= MAX_TABLEGROUP; g++ )
	{
		CTableGroupCSVRecord Group;
		DataManagerNonDb.TableGroups.GetAt( g - 1, Group );

		for ( int n = 1; n <= 3; n++ )	
		{
			m_comboGroup[n-1].AddItem( Group.GetTabbedComboText(g,n) );	
		}
	}

	GetData();
	return TRUE;  
}

/**********************************************************************/
CEdit* CDefEditTableDlg::GetEditLimit(){ return GetEdit( IDC_EDIT_LIMIT ); }
/**********************************************************************/

void CDefEditTableDlg::GetData()
{
	CTableNameCSVRecord Table;
	DataManagerNonDb.TableNames.GetAt( m_nIndex, Table );

	CString strTitle;
	strTitle.Format( "Edit Table (%d, %s)",
		Table.GetTableNo(),
		Table.GetTableName() );
	SetWindowText( strTitle );

	m_editTableName.SetWindowText( Table.GetTableName() );
	m_checkFilter.SetCheck( Table.GetShowAsFilterFlag() );
	m_checkAccount.SetCheck( Table.GetAccountTableFlag() );
	m_checkLimit.SetCheck( Table.GetEnableLimitFlag() );
	m_checkAutoStatus1.SetCheck( Table.GetAutoStatusEnableFlag() );
	m_checkAutoStatus2.SetCheck( Table.GetAutoStatusUnusedAfterPaymentFlag() );

	SetEditBoxInt( *GetEditLimit(), Table.GetLimitPounds() );

	for ( int n = 1; n <= 3; n++ )
	{
		int nGroup = Table.GetReportGroup(n);

		if ( ( nGroup >= 1 ) && ( nGroup <= m_comboGroup[n-1].GetCount() ) )
			m_comboGroup[n-1].SetCurSel( nGroup - 1 );
		else
			m_comboGroup[n-1].SetCurSel( 0 );
	}
}

/**********************************************************************/

void CDefEditTableDlg::OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if ( pNMUpDown -> iDelta > 0 )
		SpinPrevious();
	else
		SpinNext();

	*pResult = 0;
}

/**********************************************************************/

void CDefEditTableDlg::OnDefaultButton() 
{
	SpinNext();
}

/**********************************************************************/

void CDefEditTableDlg::SpinPrevious()
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

void CDefEditTableDlg::SpinNext()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_nIndex < DataManagerNonDb.TableNames.GetSize() - 1 )
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

void CDefEditTableDlg::SaveData()
{
	CTableNameCSVRecord Table;
	DataManagerNonDb.TableNames.GetAt( m_nIndex, Table );
	
	Table.SetTableName( GetEditBoxText( m_editTableName ) );
	Table.SetShowAsFilterFlag( IsTicked( m_checkFilter ) );
	Table.SetAccountTableFlag( IsTicked( m_checkAccount ) );
	Table.SetEnableLimitFlag( IsTicked( m_checkLimit ) );
	Table.SetAutoStatusEnableFlag( IsTicked( m_checkAutoStatus1 ) );
	Table.SetAutoStatusUnusedAfterPaymentFlag( IsTicked( m_checkAutoStatus2 ) );
	Table.SetLimitPounds( GetEditBoxInt( *GetEditLimit() ) );

	for ( int n = 1; n <= 3; n++ )
	{
		Table.SetReportGroup( n, m_comboGroup[n-1].GetCurSel() + 1 );
	}

	DataManagerNonDb.TableNames.SetAt( m_nIndex, Table );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CDefEditTableDlg::OnOK() 
{
	SaveData();
	EndDialog ( IDOK );
}

/**********************************************************************/
