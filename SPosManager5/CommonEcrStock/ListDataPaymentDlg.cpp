/**********************************************************************/
#include "CommsBatchDlg.h"
#include "..\CommonEcrStock\DefEditDlg.h"
#include "NewRecordNumberDlg.h"
#include "DownloadDateDlg.h"
#include "NetworkCSVArray.h"
#include "Password.h"
#include "PropertySheetPayment.h"
#include "ReportDataTablePaymentDlg.h"
#include "SelectMultipleLocationDlg.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ListDataPaymentDlg.h"
/**********************************************************************/

CSingleListContentHelpersPayment::CSingleListContentHelpersPayment() 
{
}

/**********************************************************************/

int CSingleListContentHelpersPayment::GetArraySize()
{
	return DataManager.Payment.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersPayment::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CPaymentCSVRecord PaymentRecord;
		DataManager.Payment.GetAt ( nIndex, PaymentRecord );
	
		strLine.Format ( "%d\t%s",
			PaymentRecord.GetPaymentNo(),
			PaymentRecord.GetDisplayName() );
		
		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersPayment::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < DataManager.Payment.GetSize() ) )
	{
		DataManager.Payment.RemoveAt( nIndex );
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersPayment::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "No", LVCFMT_LEFT, 50 );
	list.InsertColumn ( 1, "Name", LVCFMT_LEFT, 430 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataPaymentDlg::CListDataPaymentDlg( CWnd* pParent) : CListDataDlg ( pParent )
{
	m_pContentHelpers = new CSingleListContentHelpersPayment();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "payment" );
	m_pDisplayHelpers -> SetTopPluralText( "payments" );
	m_pDisplayHelpers -> SetBottomSingleText( "payment" );
	m_pDisplayHelpers -> SetBottomPluralText( "payments" );
}

/**********************************************************************/

CListDataPaymentDlg::~CListDataPaymentDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataPaymentDlg::SetTitle()
{
	CString strTitle = "Payment Methods ";
	strTitle += dbDatabase.GetDatabaseTitle( DataManager.GetActiveDbIdx(), EcrmanOptions.GetGlobalPaymentFlag() ); 
	SetWindowText ( strTitle );
}

/**********************************************************************/

void CListDataPaymentDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Add" );
	m_button2.SetWindowText ( "&Edit" );
	m_button3.SetWindowText ( "&Delete" );
	m_button4.SetWindowText ( "&Print" );
	m_button5.SetWindowText ( "Do&wnload" );
	m_button6.ShowWindow ( SW_HIDE );

	if ( PasswordArray.GetEnable( PasswordTicks::CommsDownloads ) == FALSE ) 
	{
		m_button5.EnableWindow( FALSE );
		m_button5.ShowWindow( SW_HIDE );
	}

	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button2, 368, 47 );
	MoveControl( &m_button3, 368, 87 );
	MoveControl( &m_button4, 368, 147 );
	MoveControl( &m_button5, 368, 187 );
}

/**********************************************************************/

void CListDataPaymentDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataPaymentDlg::HandleButton1()
{
	int nPaymentNo = DataManager.Payment.FindFirstFreeNumber();

	if ( nPaymentNo == 0 )
	{
		Prompter.Error ( "All available payment methods are already in use." );
		return;
	}

	CNewRecordNumberDlg dlg ( NODE_PAYMENT, this );
	dlg.m_nNumber = nPaymentNo;
	if ( dlg.DoModal() != IDOK )
		return;

	int nPaymentIdx = 0;
	nPaymentNo = dlg.m_nNumber;
	if ( DataManager.Payment.FindPaymentByNumber ( nPaymentNo, nPaymentIdx ) )
	{
		Prompter.Error ( "The selected payment number is already in use." );
		return;
	}

	CPaymentCSVRecord Payment;
	Payment.SetPaymentNo( nPaymentNo );
	Payment.SetEPOSName ( Payment.GetDisplayName() );
	DataManager.Payment.InsertAt ( nPaymentIdx, Payment );
	m_listData.SetItemCountEx ( GetArraySize() );
	SelectLine ( nPaymentIdx );

	CPropertySheetPayment propsheet ( IDC_SPIN, nPaymentIdx, m_WarnCancelAfterChange, this );
	propsheet.SetListDataDlg( this );

	if ( propsheet.DoModal() == IDOK )
	{
		propsheet.SaveRecord();
	}

	SelectLine ( propsheet.GetIndex() );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataPaymentDlg::HandleButton2() 
{
	int nIndex = GetSafeSelection();
	if ( nIndex == -1 )
		return;
	
	CPropertySheetPayment propsheet ( IDC_SPIN, nIndex, m_WarnCancelAfterChange, this );
	propsheet.SetListDataDlg( this );

	if ( propsheet.DoModal() == IDOK )
		propsheet.SaveRecord();

	SelectLine ( propsheet.GetIndex() );
}

/**********************************************************************/

void CListDataPaymentDlg::HandleButton3()
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );
}

/**********************************************************************/

void CListDataPaymentDlg::HandleButton4() 
{
	if ( DataManager.Payment.GetSize() == 0 )
	{
		Prompter.Error ( "You must create some payment methods\nbefore you can print this report" );
		return;
	}
	else
	{
		CReportDataTablePaymentDlg dlg( this );
		dlg.DoModal();
	}
}
		
/**********************************************************************/
	
void CListDataPaymentDlg::HandleButton5() 
{
	int nDbIdx;
	if ( EcrmanOptions.GetGlobalPaymentFlag() )
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

	HandleDownload( arrayLocIdx );
}

/**********************************************************************/

void CListDataPaymentDlg::HandleDownload( CArray<int,int>& arrayLocIdx )
{
	if ( CSelectMultipleLocationDlg::FilterLocListForDownload( arrayLocIdx, "Download Payments", this ) == TRUE )
	{
		CEposTaskDatabase TaskDatabase;
		TaskDatabase.CreateSimpleTask( DN_PAY, arrayLocIdx );

		CString strDate = "";
		{
			CEposTaskHeader TaskHeader;
			TaskDatabase.GetEditableTaskHeader( TaskHeader );

			CString strTitle;
			TaskHeader.SetName( "Download Payments" );
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
