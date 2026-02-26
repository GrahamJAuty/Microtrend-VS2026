/**********************************************************************/
 
/**********************************************************************/
#include "CommsBatchDlg.h"
//#include "DataManager.h"
#include "DownloadDateDlg.h"
//#include "EcrmanOptionsIni.h"
#include "KeyboardMap.h"
#include "ListDataDlg.h"
#include "MaxLengths.h"
#include "ModifierCSVArray.h"
#include "NetworkCSVArray.h"
#include "Password.h"
 
#include "SelectMultipleLocationDlg.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ModifierEditDlg.h"
/**********************************************************************/

CModifierEditDlg::CModifierEditDlg(CWnd* pParent)
	: CSSDialog(CModifierEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModifierEditDlg)
	//}}AFX_DATA_INIT
	m_editNameID[0] = IDC_EDIT_NAME1;
	m_editNameID[1] = IDC_EDIT_NAME2;
	m_editNameID[2] = IDC_EDIT_NAME3;
	m_editNameID[3] = IDC_EDIT_NAME4;
	m_editNameID[4] = IDC_EDIT_NAME5;
	m_editNameID[5] = IDC_EDIT_NAME6;
	m_editNameID[6] = IDC_EDIT_NAME7;
	m_editNameID[7] = IDC_EDIT_NAME8;
	m_editNameID[8] = IDC_EDIT_NAME9;

	m_editMultiplierQtyID[0] = IDC_EDIT_MULTIPLIER1;
	m_editMultiplierQtyID[1] = IDC_EDIT_MULTIPLIER2;
	m_editMultiplierQtyID[2] = IDC_EDIT_MULTIPLIER3;
	m_editMultiplierQtyID[3] = IDC_EDIT_MULTIPLIER4;
	m_editMultiplierQtyID[4] = IDC_EDIT_MULTIPLIER5;
	m_editMultiplierQtyID[5] = IDC_EDIT_MULTIPLIER6;
	m_editMultiplierQtyID[6] = IDC_EDIT_MULTIPLIER7;
	m_editMultiplierQtyID[7] = IDC_EDIT_MULTIPLIER8;
	m_editMultiplierQtyID[8] = IDC_EDIT_MULTIPLIER9;

	m_editMultiplierPointsID[0] = IDC_EDIT_POINTS1;
	m_editMultiplierPointsID[1] = IDC_EDIT_POINTS2;
	m_editMultiplierPointsID[2] = IDC_EDIT_POINTS3;
	m_editMultiplierPointsID[3] = IDC_EDIT_POINTS4;
	m_editMultiplierPointsID[4] = IDC_EDIT_POINTS5;
	m_editMultiplierPointsID[5] = IDC_EDIT_POINTS6;
	m_editMultiplierPointsID[6] = IDC_EDIT_POINTS7;
	m_editMultiplierPointsID[7] = IDC_EDIT_POINTS8;
	m_editMultiplierPointsID[8] = IDC_EDIT_POINTS9;

	for ( int i = 0; i < 9; i++ )
	{
		CModifierCSVRecord Modifier;
		DataManager.Modifier.GetAt ( i, Modifier );
		m_strName[i] = Modifier.GetName();
		m_dMultiplierQty[i] = Modifier.GetMultiplierForQty();
		m_dMultiplierPoints[i] = Modifier.GetMultiplierForPoints();
	}

	m_bWarnCancelAfterDownload = FALSE;
}

/**********************************************************************/

void CModifierEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModifierEditDlg)
	//}}AFX_DATA_MAP

	for ( int i = 0; i < 9; i++ )
	{
		DDX_Text(pDX, m_editNameID[i], m_strName[i]);
		DDV_MaxChars(pDX, m_strName[i], MAX_LENGTH_MODIFIER);
		DDX_Text(pDX, m_editMultiplierQtyID[i], m_dMultiplierQty[i]);
		DDX_Text(pDX, m_editMultiplierPointsID[i], m_dMultiplierPoints[i]);
	}
	DDX_Control(pDX, IDC_BUTTON_DOWNLOAD, m_buttonDownload);
	DDX_Control(pDX, IDC_STATIC_POINTS, m_staticPoints);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CModifierEditDlg, CDialog)
	//{{AFX_MSG_MAP(CModifierEditDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD, OnButtonDownload)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CModifierEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	

	for ( int i = 0; i < 9; i++ )
	{
		SubclassEdit ( m_editMultiplierQtyID[i], SS_NUM_DP, 8, "%.6f" );
		SubclassEdit ( m_editMultiplierPointsID[i], SS_NUM_DP, 5, "%.2f" );
	}

	if ( PasswordArray.GetEnable( PasswordTicks::CommsDownloads ) == FALSE ) 
	{
		m_buttonDownload.EnableWindow( FALSE );
		m_buttonDownload.ShowWindow( SW_HIDE );
	}

	if ( SysInfo.GotLoyaltyOrSmartPayLink() == FALSE )
	{
		m_staticPoints.ShowWindow( SW_HIDE );

		for ( int i = 0; i < 9; i++ )
			GetEditPoints(i) -> ShowWindow( SW_HIDE );

	}

	CString strTitle = "Setup Modifiers ";
	strTitle += dbDatabase.GetDatabaseTitle( DataManager.GetActiveDbIdx(), EcrmanOptions.GetGlobalModifierFlag() ); 
	SetWindowText ( strTitle );

	return TRUE;  
}

/**********************************************************************/
CEdit* CModifierEditDlg::GetEditPoints( int i ){ return GetEdit( m_editMultiplierPointsID[i] ); }
/**********************************************************************/

void CModifierEditDlg::OnButtonDownload() 
{
	if ( UpdateData( TRUE ) == FALSE )
		return;

	int nDbIdx;
	if ( EcrmanOptions.GetGlobalModifierFlag() )
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

	CString strTempName[9];
	double dTempMultiplierQty[9];
	double dTempMultiplierPoints[9];

	for ( int n = 0; n < 9; n++ )
	{
		CModifierCSVRecord Modifier;
		DataManager.Modifier.GetAt( n, Modifier );
		
		strTempName[n] = Modifier.GetName();
		dTempMultiplierQty[n] = Modifier.GetMultiplierForQty();
		dTempMultiplierPoints[n] = Modifier.GetMultiplierForPoints();

		Modifier.SetName( m_strName[n] );
		Modifier.SetMultiplierForQty( m_dMultiplierQty[n] );
		Modifier.SetMultiplierForPoints( m_dMultiplierPoints[n] );
		DataManager.Modifier.SetAt( n, Modifier );
	}
		
	HandleDownload( arrayLocIdx );

	for ( int n = 0; n < 9; n++ )
	{
		CModifierCSVRecord Modifier;
		Modifier.SetName( strTempName[n] );
		Modifier.SetMultiplierForQty( dTempMultiplierQty[n] );
		Modifier.SetMultiplierForPoints( dTempMultiplierPoints[n] );
		DataManager.Modifier.SetAt( n, Modifier );
	}
}

/**********************************************************************/

void CModifierEditDlg::HandleDownload( CArray<int,int>& arrayLocIdx )
{
	if ( CSelectMultipleLocationDlg::FilterLocListForDownload( arrayLocIdx, "Download Modifiers", this ) == TRUE )
	{
		CEposTaskDatabase TaskDatabase;
		TaskDatabase.CreateSimpleTask( DN_MOD, arrayLocIdx );

		CString strDate = "";
		{
			CEposTaskHeader TaskHeader;
			TaskDatabase.GetEditableTaskHeader( TaskHeader );
			
			CString strTitle;
			TaskHeader.SetName( "Download Modifiers" );
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

void CModifierEditDlg::OnCancel() 
{
	if ( CListDataDlg::CheckBeforeCancel( m_bWarnCancelAfterDownload, TRUE ) == TRUE )
		EndDialog( IDCANCEL );
}

/**********************************************************************/

void CModifierEditDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		for ( int i = 0; i < 9; i++ )
		{
			CModifierCSVRecord Modifier;
			Modifier.SetName( m_strName[i] );
			Modifier.SetMultiplierForQty( m_dMultiplierQty[i] );
			Modifier.SetMultiplierForPoints( m_dMultiplierPoints[i] );
			DataManager.Modifier.SetAt ( i, Modifier );
		}
		EndDialog ( IDOK );
	}
}

/**********************************************************************/
