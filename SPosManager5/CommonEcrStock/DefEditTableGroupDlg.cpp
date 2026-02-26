/**********************************************************************/
#include "DefEditTableGroupDlg.h"
/**********************************************************************/

CDefEditTableGroupDlg::CDefEditTableGroupDlg( int nIndex, int nLevel, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent /*=NULL*/)
	: CSSDialog(CDefEditTableGroupDlg::IDD, pParent), m_WarnCancelAfterChange( WarnCancelAfterChange )
{
	//{{AFX_DATA_INIT(CDefEditTableGroupDlg)
	//}}AFX_DATA_INIT
	m_nIndex = nIndex;
	m_nLevel = nLevel;
}

/**********************************************************************/

void CDefEditTableGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditTableGroupDlg)
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_CHECK_FILTER, m_checkFilter);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditTableGroupDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditTableGroupDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnSpinButton)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditTableGroupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_editName.LimitText( SPosTableGroup::GroupName.Max );
	GetData();
	return TRUE;  
}

/**********************************************************************/

void CDefEditTableGroupDlg::GetData()
{
	CTableGroupCSVRecord TableGroup;
	DataManagerNonDb.TableGroups.GetAt( m_nIndex, TableGroup );

	CString strName = TableGroup.GetGroupName(m_nLevel);

	CString strTitle;
	strTitle.Format( "Edit Table Group (%s)",
		(const char*) strName );
	SetWindowText( strTitle );

	m_editName.SetWindowText( strName );
	m_checkFilter.SetCheck( TableGroup.GetShowAsFilterFlag(m_nLevel) );
}

/**********************************************************************/

void CDefEditTableGroupDlg::OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if ( pNMUpDown -> iDelta > 0 )
		SpinPrevious();
	else
		SpinNext();

	*pResult = 0;
}

/**********************************************************************/

void CDefEditTableGroupDlg::OnDefaultButton() 
{
	SpinNext();
}

/**********************************************************************/

void CDefEditTableGroupDlg::SpinPrevious()
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

void CDefEditTableGroupDlg::SpinNext()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_nIndex < DataManagerNonDb.TableGroups.GetSize() - 1 )
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

void CDefEditTableGroupDlg::SaveData()
{
	CTableGroupCSVRecord TableGroup;
	DataManagerNonDb.TableGroups.GetAt( m_nIndex, TableGroup );
	
	TableGroup.SetGroupName( m_nLevel, GetEditBoxText( m_editName ) );
	TableGroup.SetShowAsFilterFlag( m_nLevel, IsTicked( m_checkFilter ) );
	
	DataManagerNonDb.TableGroups.SetAt( m_nIndex, TableGroup );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CDefEditTableGroupDlg::OnOK() 
{
	SaveData();
	EndDialog ( IDOK );
}

/**********************************************************************/
