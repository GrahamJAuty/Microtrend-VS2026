/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "SageOptions.h"
#include "SageExportManager.h"
/**********************************************************************/
#include "SageExportOptionsDlg.h"
/**********************************************************************/

CSageExportOptionsDlg::CSageExportOptionsDlg( CWnd* pParent )
	: CSSDialog(CSageExportOptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKeyboardNumberDlg)
	//}}AFX_DATA_INIT
	m_nDelay = SageOptions.GetExportDelayTime();
	m_strReference = SageOptions.GetAccountReference();
	m_bInitialEnable = SageOptions.GetExportEnableFlag();
}

/**********************************************************************/

void CSageExportOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSageExportOptionsDlg)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_CHECK_ENABLE, m_checkEnable );
	DDX_Control( pDX, IDC_COMBO_NOMINAL, m_comboNominal );
	DDX_Control( pDX, IDC_EDIT_REFERENCE, m_editReference );
	DDX_Control( pDX, IDC_DATEPICKER_DATE, m_DatePicker );
	DDX_Control( pDX, IDC_STATIC_DATE, m_staticDate );
	DDX_Text( pDX, IDC_EDIT_REFERENCE, m_strReference );
	DDV_MaxChars( pDX, m_strReference, 8 );
	DDX_Text( pDX, IDC_EDIT_PAUSE, m_nDelay );
	DDV_MinMaxInt( pDX, m_nDelay, 1, 6 );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSageExportOptionsDlg, CDialog)
	//{{AFX_MSG_MAP(CSageExportOptionsDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED( IDC_CHECK_ENABLE, OnCheckEnable)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSageExportOptionsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if ( TRUE == m_bInitialEnable )
	{
		m_staticDate.ShowWindow( SW_HIDE );
		m_DatePicker.ShowWindow( SW_HIDE );
	}

	COleDateTime dateStart = COleDateTime( 2013, 1, 1, 0, 0, 0 );
	COleDateTime dateEnd = COleDateTime::GetCurrentTime();
	m_DatePicker.SetRange ( &dateStart, &dateEnd );
	m_DatePicker.SetFormat( "ddd dd MMM yyy" );
	m_DatePicker.SetTime( COleDateTime::GetCurrentTime() );

	m_comboNominal.AddString( "4 digits" );
	m_comboNominal.AddString( "5 digits" );
	m_comboNominal.AddString( "6 digits" );
	m_comboNominal.AddString( "7 digits" );
	m_comboNominal.AddString( "8 digits" );

	int nLen = SageOptions.GetNominalCodeLength();

	if ( ( nLen >= 4 ) && ( nLen <= 8 ) )
		m_comboNominal.SetCurSel( nLen - 4 );
	else
		m_comboNominal.SetCurSel(0);

	m_checkEnable.SetCheck( SageOptions.GetExportEnableFlag() );
	OnCheckEnable();

	return TRUE;  
}

/**********************************************************************/
CEdit* CSageExportOptionsDlg::GetEditDelay(){ return GetEdit( IDC_EDIT_PAUSE ); }
/**********************************************************************/

void CSageExportOptionsDlg::OnCheckEnable()
{
	bool bEnable = ( m_checkEnable.GetCheck() != 0 );
	m_comboNominal.EnableWindow( bEnable );
	m_editReference.EnableWindow( bEnable );
	GetEditDelay() -> EnableWindow( bEnable );

	if ( FALSE == m_bInitialEnable )
		m_DatePicker.EnableWindow( bEnable );
}

/**********************************************************************/

void CSageExportOptionsDlg::OnOK() 
{
	if ( UpdateData( TRUE ) == TRUE )
	{
		bool bEnable = ( m_checkEnable.GetCheck() != 0 );

		if ( ( TRUE == bEnable ) && ( FALSE == m_bInitialEnable ) )
		{
			COleDateTime dateTemp;
			m_DatePicker.GetTime( dateTemp );

			COleDateTime dateExport = COleDateTime( dateTemp.GetYear(), dateTemp.GetMonth(), dateTemp.GetDay(), 0, 0, 0 );

			if ( dateExport.m_status == COleDateTime::valid )
			{
				dateExport -= COleDateTimeSpan( 1, 0, 0, 0 );

				for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
				{
					CFilenameUpdater FnUp( SysFiles::SageDate, nDbIdx  );
					CString strDateFile = FnUp.GetFilenameToUse();
			
					CSSFile fileDate;
					if ( fileDate.Open( strDateFile, "wb" ) == TRUE )
					{
						CString strDate;
						strDate.Format( "%4.4d%2.2d%2.2d",
							dateExport.GetYear(),
							dateExport.GetMonth(),
							dateExport.GetDay() );

						fileDate.WriteLine( strDate );
					}
				}
			}
		}

		SageOptions.SetExportEnableFlag( m_checkEnable.GetCheck() != 0 );
		SageOptions.SetExportDelayTime( m_nDelay );
		SageOptions.SetAccountReference( m_strReference );
		SageOptions.SetNominalCodeLength( m_comboNominal.GetCurSel() + 4 );
		SageExportManager.ResetExportRetryTime();

		EndDialog( IDOK );
	}
}

/**********************************************************************/

