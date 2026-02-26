/**********************************************************************/
 
/**********************************************************************/
#include "CommsBatchDlg.h"
#include "DownloadDateDlg.h"
//#include "EcrmanOptionsIni.h"
#include "ListDataDlg.h"
#include "NetworkCSVArray.h"
#include "Password.h"
 
#include "SelectMultipleLocationDlg.h"
/**********************************************************************/
#include "PaidInOutTextEditDlg.h"
/**********************************************************************/

CPaidInOutTextEditDlg::CPaidInOutTextEditDlg(CWnd* pParent)
	: CSSDialog(CPaidInOutTextEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPaidInOutTextEditDlg)
	//}}AFX_DATA_INIT
	m_nTabPage = 0;

	m_editTextID[0] = IDC_EDIT_TEXT1;
	m_editTextID[1] = IDC_EDIT_TEXT2;
	m_editTextID[2] = IDC_EDIT_TEXT3;
	m_editTextID[3] = IDC_EDIT_TEXT4;
	m_editTextID[4] = IDC_EDIT_TEXT5;
	m_editTextID[5] = IDC_EDIT_TEXT6;
	m_editTextID[6] = IDC_EDIT_TEXT7;
	m_editTextID[7] = IDC_EDIT_TEXT8;
	m_editTextID[8] = IDC_EDIT_TEXT9;
	m_editTextID[9] = IDC_EDIT_TEXT10;
	m_editTextID[10] = IDC_EDIT_TEXT11;
	m_editTextID[11] = IDC_EDIT_TEXT12;
	m_editTextID[12] = IDC_EDIT_TEXT13;
	m_editTextID[13] = IDC_EDIT_TEXT14;
	m_editTextID[14] = IDC_EDIT_TEXT15;
	m_editTextID[15] = IDC_EDIT_TEXT16;
	m_editTextID[16] = IDC_EDIT_TEXT17;
	m_editTextID[17] = IDC_EDIT_TEXT18;
	m_editTextID[18] = IDC_EDIT_TEXT19;
	m_editTextID[19] = IDC_EDIT_TEXT20;
	
	for ( int i = 0; i < PAIDIO_TEXT_COUNT; i++ )
	{
		CPaidInOutTextCSVRecord Text;
		DataManager.PaidInText.GetAt ( i, Text );
		m_strPaidInBuffer[i] = Text.GetEposText();
		DataManager.PaidOutText.GetAt ( i, Text );
		m_strPaidOutBuffer[i] = Text.GetEposText();
	}

	m_bWarnCancelAfterDownload = FALSE;
}

/**********************************************************************/

void CPaidInOutTextEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPaidInOutTextEditDlg)
	//}}AFX_DATA_MAP

	DDX_Control( pDX, IDC_TAB_PAGE, m_TabPage );

	for ( int i = 0; i < PAIDIO_TEXT_COUNT; i++ )
		DDX_Control(pDX, m_editTextID[i], m_editText[i]);
	
	DDX_Control(pDX, IDC_BUTTON_DOWNLOAD, m_buttonDownload);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPaidInOutTextEditDlg, CDialog)
	//{{AFX_MSG_MAP(CPaidInOutTextEditDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD, OnButtonDownload)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PAGE, OnTcnSelchangeTabs)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPaidInOutTextEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	

	m_TabPage.InsertItem( 0, "Paid In" );
	m_TabPage.InsertItem( 1, "Paid Out" );
	m_TabPage.SetCurSel(0);

	for ( int n = 0; n < PAIDIO_TEXT_COUNT; n++ )
		m_editText[n].LimitText( PaidInOutText::EposText.Max );
	
	if ( PasswordArray.GetEnable( PasswordTicks::CommsDownloads ) == FALSE ) 
	{
		m_buttonDownload.EnableWindow( FALSE );
		m_buttonDownload.ShowWindow( SW_HIDE );
	}

	CString strTitle = "Setup Paid In / Out Texts ";
	strTitle += dbDatabase.GetDatabaseTitle( DataManager.GetActiveDbIdx(), EcrmanOptions.GetGlobalPaidInOutFlag() ); 
	SetWindowText ( strTitle );

	SelectTab(0);

	return TRUE;  
}

/**********************************************************************/

void CPaidInOutTextEditDlg::BufferCurrentTabChanges()
{
	for ( int n = 0; n < PAIDIO_TEXT_COUNT; n++ )
	{
		if ( 1 == m_nTabPage )
			m_editText[n].GetWindowText( m_strPaidOutBuffer[n] );
		else
			m_editText[n].GetWindowText( m_strPaidInBuffer[n] );
	}
}

/**********************************************************************/

void CPaidInOutTextEditDlg::OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	BufferCurrentTabChanges();
	
	m_nTabPage = m_TabPage.GetCurSel();
	if ( ( m_nTabPage < 0 ) || ( m_nTabPage >= m_TabPage.GetItemCount() ) )
		m_nTabPage = 0;

	SelectTab( m_nTabPage );
}

/**********************************************************************/

void CPaidInOutTextEditDlg::SelectTab( int nPage )
{
	for ( int n = 0; n < PAIDIO_TEXT_COUNT; n++ )
	{
		if ( 1 == m_nTabPage )
			m_editText[n].SetWindowText( m_strPaidOutBuffer[n] );
		else
			m_editText[n].SetWindowText( m_strPaidInBuffer[n] );
	}
}

/**********************************************************************/

void CPaidInOutTextEditDlg::OnButtonDownload() 
{
	if ( UpdateData( TRUE ) == FALSE )
		return;

	int nDbIdx;
	if ( EcrmanOptions.GetGlobalPaidInOutFlag() )
		nDbIdx = -1;
	else
		nDbIdx = DataManager.GetActiveDbIdx();

	bool bBlocked = FALSE;
	CArray<int,int> arrayLocIdx;
	dbNetwork.GetDatabaseCommsLocArray( nDbIdx, arrayLocIdx, bBlocked );

	if ( arrayLocIdx.GetSize() == 0 )
	{
		if ( FALSE == bBlocked )
			Prompter.Error ( "There are no locations available for\ndownload on this database." );
		else
			Prompter.Error ( "You are not authorised to download files\nfrom this database." );

		return;
	}

	BufferCurrentTabChanges();

	CString strPaidIn[PAIDIO_TEXT_COUNT];
	CString strPaidOut[PAIDIO_TEXT_COUNT];

	for ( int n = 0; n < PAIDIO_TEXT_COUNT; n++ )
	{
		CPaidInOutTextCSVRecord Text;
		
		DataManager.PaidInText.GetPaidInOutText( n + 1, Text );
		strPaidIn[n] = Text.GetEposText();
		DataManager.PaidOutText.GetPaidInOutText( n + 1, Text );
		strPaidOut[n] = Text.GetEposText();
		
		Text.SetEposText( m_strPaidInBuffer[n] );
		DataManager.PaidInText.SetAt( n, Text );
		Text.SetEposText( m_strPaidOutBuffer[n] );
		DataManager.PaidOutText.SetAt( n, Text );
	}
		
	HandleDownload( arrayLocIdx );

	for ( int n = 0; n < PAIDIO_TEXT_COUNT; n++ )
	{
		CPaidInOutTextCSVRecord Text;
		Text.SetEposText( strPaidIn[n] );
		DataManager.PaidInText.SetPaidInOutText( n + 1, Text );
		Text.SetEposText( strPaidOut[n] );
		DataManager.PaidOutText.SetPaidInOutText( n + 1, Text );
	}
}

/**********************************************************************/

void CPaidInOutTextEditDlg::HandleDownload( CArray<int,int>& arrayLocIdx )
{
	if ( CSelectMultipleLocationDlg::FilterLocListForDownload( arrayLocIdx, "Download Paid In / Out Text", this ) == TRUE )
	{
		CEposTaskDatabase TaskDatabase;
		TaskDatabase.CreateSimpleTask( DN_PAIDIO, arrayLocIdx );

		CString strDate = "";
		{
			CEposTaskHeader TaskHeader;
			TaskDatabase.GetEditableTaskHeader( TaskHeader );

			CString strTitle;
			TaskHeader.SetName( "Download Paid In / Out Text" );
			strTitle.Format ( "Epos Import Date (%s)", TaskHeader.GetName() );

			CDownloadDateDlg dlgDate( strTitle, this );
			if ( dlgDate.RunIfNeeded() == FALSE )
				return;

			if ( dlgDate.m_bImmediate == TRUE )
				TaskHeader.SetImportType( IMPORT_IMMEDIATE );
			else
			{
				TaskHeader.SetImportType( IMPORT_SPECIFIED );
				strDate = dlgDate.m_strDate;
			}

			TaskDatabase.SetEditableTaskHeader( TaskHeader );
		}

		CKeyboardMap KeyboardMap;
		CCommsBatchDlg dlgComms ( TaskDatabase, strDate, TRUE, KeyboardMap, -1, 0, 0, this );
		dlgComms.DoModal();
		m_bWarnCancelAfterDownload = TRUE;
	}
}

/**********************************************************************/

void CPaidInOutTextEditDlg::OnCancel() 
{
	if ( CListDataDlg::CheckBeforeCancel( m_bWarnCancelAfterDownload, TRUE ) == TRUE )
		EndDialog( IDCANCEL );
}

/**********************************************************************/

void CPaidInOutTextEditDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		BufferCurrentTabChanges();

		for ( int i = 0; i < PAIDIO_TEXT_COUNT; i++ )
		{
			CPaidInOutTextCSVRecord Text;

			Text.SetEposText( m_strPaidInBuffer[i] );
			DataManager.PaidInText.SetAt ( i, Text );

			Text.SetEposText( m_strPaidOutBuffer[i] );
			DataManager.PaidOutText.SetAt ( i, Text );
		}
		EndDialog ( IDOK );
	}
}

/**********************************************************************/
