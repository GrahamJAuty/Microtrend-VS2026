/**********************************************************************/
#include "ModifierCSVArray.h"
/**********************************************************************/
#include "ModifierSelectDlg.h"
/**********************************************************************/

CModifierSelectDlg::CModifierSelectDlg( int nNodeType, int nCommonBits, int nMask, const char* szPluNo, CWnd* pParent)
	: CDialog(CModifierSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModifierSelectDlg)
	//}}AFX_DATA_INIT
	m_nNodeType = nNodeType;
	m_nInitialCommonBits = nCommonBits;
	m_nInitialMask = nMask;
	m_strPluNo = szPluNo;
	m_bNoDescription = FALSE;
}

/**********************************************************************/

void CModifierSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModifierSelectDlg)
	DDX_Control(pDX, IDC_CHECK_CHANGE0, m_checkChange[0]);
	DDX_Control(pDX, IDC_CHECK_CHANGE1, m_checkChange[1]);
	DDX_Control(pDX, IDC_CHECK_CHANGE2, m_checkChange[2]);
	DDX_Control(pDX, IDC_CHECK_CHANGE3, m_checkChange[3]);
	DDX_Control(pDX, IDC_CHECK_CHANGE4, m_checkChange[4]);
	DDX_Control(pDX, IDC_CHECK_CHANGE5, m_checkChange[5]);
	DDX_Control(pDX, IDC_CHECK_CHANGE6, m_checkChange[6]);
	DDX_Control(pDX, IDC_CHECK_CHANGE7, m_checkChange[7]);
	DDX_Control(pDX, IDC_CHECK_CHANGE8, m_checkChange[8]);
	DDX_Control(pDX, IDC_CHECK_CHANGE9, m_checkChange[9]);
	DDX_Control(pDX, IDC_CHECK_MODIFIER0, m_checkModifier[0]);
	DDX_Control(pDX, IDC_CHECK_MODIFIER1, m_checkModifier[1]);
	DDX_Control(pDX, IDC_CHECK_MODIFIER2, m_checkModifier[2]);
	DDX_Control(pDX, IDC_CHECK_MODIFIER3, m_checkModifier[3]);
	DDX_Control(pDX, IDC_CHECK_MODIFIER4, m_checkModifier[4]);
	DDX_Control(pDX, IDC_CHECK_MODIFIER5, m_checkModifier[5]);
	DDX_Control(pDX, IDC_CHECK_MODIFIER6, m_checkModifier[6]);
	DDX_Control(pDX, IDC_CHECK_MODIFIER7, m_checkModifier[7]);
	DDX_Control(pDX, IDC_CHECK_MODIFIER8, m_checkModifier[8]);
	DDX_Control(pDX, IDC_CHECK_MODIFIER9, m_checkModifier[9]);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CModifierSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CModifierSelectDlg)
	ON_BN_CLICKED( IDC_CHECK_CHANGE0, OnToggleChange)
	ON_BN_CLICKED( IDC_CHECK_CHANGE1, OnToggleChange)
	ON_BN_CLICKED( IDC_CHECK_CHANGE2, OnToggleChange)
	ON_BN_CLICKED( IDC_CHECK_CHANGE3, OnToggleChange)
	ON_BN_CLICKED( IDC_CHECK_CHANGE4, OnToggleChange)
	ON_BN_CLICKED( IDC_CHECK_CHANGE5, OnToggleChange)
	ON_BN_CLICKED( IDC_CHECK_CHANGE6, OnToggleChange)
	ON_BN_CLICKED( IDC_CHECK_CHANGE7, OnToggleChange)
	ON_BN_CLICKED( IDC_CHECK_CHANGE8, OnToggleChange)
	ON_BN_CLICKED( IDC_CHECK_CHANGE9, OnToggleChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CModifierSelectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	switch( m_nNodeType )
	{
	case NODE_PLUFILTER_NAME:
		SetWindowText( "Select Modifiers for Plu Filter" );
		break;

	case NODE_OFFER_GROUP:
	default:
		SetWindowText( "Select Modifiers for Offer Group" );
		break;
	}

	if ( m_strPluNo != "" )
	{
		CString strBasePlu = "PLU ";
		strBasePlu += m_strPluNo + " ";

		if ( FALSE == m_bNoDescription )
		{
			int nPluIdx;
			if ( DataManager.Plu.FindPluByNumber( _atoi64(m_strPluNo), nPluIdx ) == TRUE )
			{
				CPluCSVRecord PluRecord;
				DataManager.Plu.GetAt( nPluIdx, PluRecord );
				strBasePlu += PluRecord.GetReportText();
			}
			else
				strBasePlu += " Unknown";
		}
		
		m_checkModifier[0].SetWindowText( strBasePlu );
	}
	
	for ( int nMod = 1; nMod <= 9; nMod++ )
	{
		if ( FALSE == m_bNoDescription )
		{
			m_checkModifier[nMod].SetWindowText( DataManager.Modifier.GetDisplayName(nMod) );
		}
		else
		{
			CString strModifier;
			strModifier.Format( "Modifier %d", nMod );
			m_checkModifier[nMod].SetWindowText( strModifier );
		}
	}
	
	int nBitMask = 1;
	for ( int nMod = 0; nMod <= 9; nMod++ )
	{
		if ( m_strPluNo == "" )
		{
			int nEnableMask = m_nInitialMask & nBitMask;
			m_checkChange[nMod].SetCheck( nEnableMask != 0 );
			m_checkModifier[nMod].EnableWindow( nEnableMask != 0 );
			m_checkModifier[nMod].SetCheck( ( nEnableMask & m_nInitialCommonBits ) != 0 );
		}
		else
		{
			m_checkChange[nMod].SetCheck( TRUE );
			m_checkChange[nMod].EnableWindow( FALSE );
			m_checkModifier[nMod].SetCheck( ( nBitMask & m_nInitialCommonBits ) != 0 );
			m_checkModifier[nMod].EnableWindow( TRUE );
		}

		nBitMask *= 2;
	}
	
	return TRUE;  
}

/**********************************************************************/

void CModifierSelectDlg::OnToggleChange()
{
	for ( int nMod = 0; nMod <= 9; nMod++ )
		m_checkModifier[nMod].EnableWindow( m_checkChange[nMod].GetCheck() != 0 );
}

/**********************************************************************/

void CModifierSelectDlg::OnOK() 
{
	m_nFinalCommonBits = 0;
	m_nFinalMask = 0;

	int nBitMask = 1;
	for ( int nMod = 0; nMod <= 9; nMod++ )
	{
		if ( m_checkChange[nMod].GetCheck() != 0 )
		{
			m_nFinalMask |= nBitMask;
			
			if ( m_checkModifier[nMod].GetCheck() != 0 )
				m_nFinalCommonBits |= nBitMask;
		}

		nBitMask *= 2;
	}

	EndDialog( IDOK );
}

/**********************************************************************/
