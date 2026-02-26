/**********************************************************************/
#include "GroupCSVArrayAllergen.h"
#include "SysInfo.h"
/**********************************************************************/
#include "DefEditGroupDlg.h"
/**********************************************************************/

CDefEditGroupDlg::CDefEditGroupDlg( CGroupCSVArrayBase* pArray, int nGroupType, int nIndex, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent)
	: CSSDialog(CDefEditGroupDlg::IDD, pParent), m_WarnCancelAfterChange( WarnCancelAfterChange )
{
	m_pGroupArray = pArray;
	m_nGroupType = nGroupType;
	m_nIndex = nIndex;

	m_pListDataDlg = NULL;
	//{{AFX_DATA_INIT(CDefEditGroupDlg)
	m_strRepText = _T("");
	//}}AFX_DATA_INIT
	
}

/**********************************************************************/

void CDefEditGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	int nMaxLen = 0;

	switch( m_nGroupType )
	{
	case NODE_DEPT_GROUP_CONSOL:	
		nMaxLen = ConsolGroup::RepText.Max;
		break;
	
	case NODE_PAYMENT_GROUP:		
		nMaxLen = PaymentGroup::RepText.Max;
		break;

	case NODE_ALLERGEN:				
	default:
		nMaxLen = Allergen::EposText.Max;
		break;
	}
	
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditGroupDlg)
	DDX_Control(pDX, IDC_STATIC_LABEL, m_staticLabel);
	DDX_Control(pDX, IDC_EDIT_REPTEXT, m_editRepText);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_REPTEXT, m_strRepText);
	DDV_MaxChars(pDX, m_strRepText, nMaxLen);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditGroupDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditGroupDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnSpinButton)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditGroupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if ( NODE_ALLERGEN == m_nGroupType )
		m_staticLabel.SetWindowText( "Description" );
	else
		m_staticLabel.SetWindowText( "Report text" );

	GetData();
	return TRUE;  
}

/**********************************************************************/

void CDefEditGroupDlg::OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if ( pNMUpDown -> iDelta > 0 )
		SpinPrevious();
	else
		SpinNext();
	
	*pResult = 0;
}

/**********************************************************************/

void CDefEditGroupDlg::OnDefaultButton() 
{
	SpinNext();
}

/**********************************************************************/

void CDefEditGroupDlg::SpinPrevious()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_nIndex <= 1 )
			return;

		SaveData();
		m_nIndex--;
		GetData();

		if ( m_pListDataDlg != NULL )
			m_pListDataDlg -> SelectLine( m_nIndex );
	}
}

/**********************************************************************/

void CDefEditGroupDlg::SpinNext()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_nIndex < m_pGroupArray -> GetSize() - 1 )
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

void CDefEditGroupDlg::GetData()
{
	CString strTitle;

	CBaseGroupInfo infoGroup;
	m_pGroupArray -> GetGroupInfo( m_nIndex, infoGroup );

	m_editRepText.SetWindowText ( infoGroup.m_strReportText );
	strTitle.Format ( "G%2.2d %s", infoGroup.m_nGroupNo, (const char*) infoGroup.m_strReportText );
	SetWindowText ( strTitle );

	m_editRepText.SetSel ( 0, -1 );
	m_editRepText.SetFocus();
}

/**********************************************************************/

void CDefEditGroupDlg::SaveData()
{
	switch( m_nGroupType )
	{
	case NODE_DEPT_GROUP_CONSOL:
		{
			CGroupCSVRecordConsol GroupRecord;
			DataManager.ConsolGroup.GetAt ( m_nIndex, GroupRecord );
			GroupRecord.SetRepText ( m_strRepText );
			DataManager.ConsolGroup.SetAt ( m_nIndex, GroupRecord );
		}
		break;

	case NODE_PAYMENT_GROUP:
		{
			CGroupCSVRecordPayment GroupRecord;
			DataManager.PaymentGroup.GetAt ( m_nIndex, GroupRecord );
			GroupRecord.SetRepText ( m_strRepText );
			DataManager.PaymentGroup.SetAt ( m_nIndex, GroupRecord );
		}
		break;

	case NODE_ALLERGEN:
	default:
		{
			CGroupCSVRecordAllergen AllergenRecord;
			DataManagerNonDb.Allergen.GetAt ( m_nIndex, AllergenRecord );
			AllergenRecord.SetEposText ( m_strRepText );
			DataManagerNonDb.Allergen.SetAt ( m_nIndex, AllergenRecord );
		}
		break;
	}

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CDefEditGroupDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveData();
		EndDialog ( IDOK );
	}
}

/**********************************************************************/
