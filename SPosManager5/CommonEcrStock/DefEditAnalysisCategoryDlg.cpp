/**********************************************************************/
#include "AnalysisCategoryCSVArray.h"
/**********************************************************************/
#include "DefEditAnalysisCategoryDlg.h"
/**********************************************************************/

CDefEditAnalysisCategoryDlg::CDefEditAnalysisCategoryDlg( int nCatNo, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent)
	: CDialog(CDefEditAnalysisCategoryDlg::IDD, pParent), m_WarnCancelAfterChange( WarnCancelAfterChange )
{
	m_pListDataDlg = NULL;

	//{{AFX_DATA_INIT(CDefEditAnalysisCategoryDlg)
	m_strEposText = _T("");
	//}}AFX_DATA_INIT
	m_nCatNo = nCatNo;
}

/**********************************************************************/

void CDefEditAnalysisCategoryDlg::DoDataExchange(CDataExchange* pDX)
{
	int nMaxLen = AnalysisCategory::EposText.Max;
	
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditAnalysisCategoryDlg)
	DDX_Control(pDX, IDC_COMBO_TYPE, m_comboType);
	DDX_Control(pDX, IDC_EDIT_EPOSTEXT, m_editEposText);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_EPOSTEXT, m_strEposText);
	DDV_MaxChars(pDX, m_strEposText, nMaxLen);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditAnalysisCategoryDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditAnalysisCategoryDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnSpinButton)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditAnalysisCategoryDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_comboType.AddString( "Food" );
	m_comboType.AddString( "Drinks" );
	m_comboType.AddString( "Other" );
	GetData();
	return TRUE;  
}

/**********************************************************************/

void CDefEditAnalysisCategoryDlg::OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if ( pNMUpDown -> iDelta > 0 )
		SpinPrevious();
	else
		SpinNext();
	
	*pResult = 0;
}

/**********************************************************************/

void CDefEditAnalysisCategoryDlg::OnDefaultButton() 
{
	SpinNext();
}

/**********************************************************************/

void CDefEditAnalysisCategoryDlg::SpinPrevious()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_nCatNo > 1 )
		{
			SaveData();
			m_nCatNo--;
			GetData();

			if ( m_pListDataDlg != NULL )
				m_pListDataDlg -> SelectLine( m_nCatNo - 1 );
		}
	}
}

/**********************************************************************/

void CDefEditAnalysisCategoryDlg::SpinNext()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_nCatNo < DataManager.AnalysisCategory.GetSize() )
		{
			SaveData();
			m_nCatNo++;
			GetData();

			if ( m_pListDataDlg != NULL )
				m_pListDataDlg -> SelectLine( m_nCatNo - 1 );
		}
	}
}

/**********************************************************************/

void CDefEditAnalysisCategoryDlg::GetData()
{
	CString strTitle;
	strTitle.Format ( "Analysis Category %d", m_nCatNo );
	SetWindowText( strTitle );

	CAnalysisCategoryCSVRecord Record;
	DataManager.AnalysisCategory.GetDisplayCategory( m_nCatNo, Record );
	
	switch( Record.GetType() )
	{
	case ACAT_FOOD:		m_comboType.SetCurSel(0);	break;
	case ACAT_DRINKS:	m_comboType.SetCurSel(1);	break;
	case ACAT_OTHER:
	default:			m_comboType.SetCurSel(2);	break;
	}
		
	m_editEposText.SetWindowText ( Record.GetEposText() );
	m_editEposText.SetFocus();
	m_editEposText.SetSel ( 0, -1 );
}

/**********************************************************************/

void CDefEditAnalysisCategoryDlg::SaveData()
{
	m_WarnCancelAfterChange.SetFlag();

	::TrimSpaces ( m_strEposText, FALSE );

	CAnalysisCategoryCSVRecord Record;
	DataManager.AnalysisCategory.GetCategory( m_nCatNo, Record );
	Record.SetEposText( m_strEposText );

	switch ( m_comboType.GetCurSel() )
	{
	case 0:		Record.SetType( ACAT_FOOD );	break;
	case 1:		Record.SetType( ACAT_DRINKS );	break;
	case 2:
	default:	Record.SetType( ACAT_OTHER );	break;
	}
	
	DataManager.AnalysisCategory.SetCategory( m_nCatNo, Record );
}

/**********************************************************************/

void CDefEditAnalysisCategoryDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveData();
		EndDialog ( IDOK );
	}
}

/**********************************************************************/
