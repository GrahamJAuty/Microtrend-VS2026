/**********************************************************************/
#include "DateRangeHelpers.h"
/**********************************************************************/
#include "CustomerSyncDlg.h"
/**********************************************************************/

CCustomerSyncDlg::CCustomerSyncDlg( bool bOneOff, CArray<int,int>& arrayIDs, CWnd* pParent)
	: CSSDialog(CCustomerSyncDlg::IDD, pParent), m_arrayIDs( arrayIDs )
{
	//{{AFX_DATA_INIT(CCustomerSyncDlg)
	//}}AFX_DATA_INIT
	m_bOneOff = bOneOff;
	m_arrayIDs.RemoveAll();
	m_nAction = 0;
	m_strIDs = "";
	m_bImport = FALSE;
	m_bRemove = FALSE;
}

/**********************************************************************/

void CCustomerSyncDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustomerSyncDlg)
	DDX_Control(pDX, IDC_EDIT_IDS, m_editIDs);
	DDX_Control(pDX, IDC_CHECK_IMPORT, m_checkImport);
	DDX_Control(pDX, IDC_CHECK_REMOVE, m_checkRemove);
	DDX_Control(pDX, IDC_RADIO2, m_radio2);
	DDX_Control(pDX, IDC_RADIO3, m_radio3);
	DDX_Radio(pDX, IDC_RADIO1, m_nAction);
	DDX_Check(pDX, IDC_CHECK_IMPORT, m_bImport);
	DDX_Check(pDX, IDC_CHECK_REMOVE, m_bRemove);
	DDX_Text(pDX, IDC_EDIT_IDS, m_strIDs);
	DDV_MaxChars(pDX, m_strIDs, 300);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CCustomerSyncDlg, CDialog)
	//{{AFX_MSG_MAP(CCustomerSyncDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED( IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED( IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED( IDC_RADIO3, OnRadio3)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CCustomerSyncDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString str;
	str.Format( "Import customers that do not exist in %s database", AfxGetAppName() );
	m_checkImport.SetWindowText( str );

	OnRadio1();

	if ( TRUE == m_bOneOff )
	{
		ShowAndEnableWindow( &m_radio3, FALSE );
		ShowAndEnableWindow( &m_checkImport, FALSE );
		ShowAndEnableWindow( &m_checkRemove, FALSE );
	}
	
	return TRUE;  
}

/**********************************************************************/

void CCustomerSyncDlg::OnRadio1()
{
	m_editIDs.EnableWindow( TRUE );
	m_checkImport.EnableWindow( FALSE );
	m_checkRemove.EnableWindow( FALSE );
}

/**********************************************************************/

void CCustomerSyncDlg::OnRadio2()
{
	m_editIDs.EnableWindow( FALSE );
	m_checkImport.EnableWindow( FALSE );
	m_checkRemove.EnableWindow( FALSE );
}

/**********************************************************************/

void CCustomerSyncDlg::OnRadio3()
{
	m_editIDs.EnableWindow( FALSE );
	m_checkImport.EnableWindow( TRUE );
	m_checkRemove.EnableWindow( TRUE );
}

/**********************************************************************/

void CCustomerSyncDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( 0 == m_nAction )
		{
			m_arrayIDs.RemoveAll();
			
			CString strID = "";

			int nLength = m_strIDs.GetLength();
			
			for ( int nPos = 0; nPos < nLength; nPos++ )
			{
				if ( nPos == nLength - 1 )
					strID += m_strIDs.GetAt(nPos);
				else
				{
					if ( m_strIDs.Mid( nPos, 2 ) == "\r\n" )
					{
						if ( ( strID != "" ) && ( strID.GetLength() <= 8 ) && ( ::TestNumeric( strID ) == TRUE ) )
							m_arrayIDs.Add( atoi( strID ) );

						strID = "";
						
						nPos++;
					}
					else
						strID += m_strIDs.GetAt(nPos);
				}
			}

			if ( ( strID != "" ) && ( strID.GetLength() <= 8 ) && ( ::TestNumeric( strID ) == TRUE ) )
				m_arrayIDs.Add( atoi( strID ) );
		}

		EndDialog ( IDOK );
	}
}

/**********************************************************************/

