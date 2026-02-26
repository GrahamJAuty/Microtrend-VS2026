/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "Password.h"
#include "PMSCalendarDlg.h"
#include "PMSManualSalesDatabaseDlg.h"
#include "PMSOptions.h"
#include "PMSQuickPluDlg.h"
#include "PMSReportInvoice.h"
#include "PMSRoomStatusTable.h"
#include "PriceHelpers.h"
#include "RepSpawn.h"
/**********************************************************************/
#include "PMSInvoiceDlg.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPMSInvoiceDlg, CDialog)
/**********************************************************************/

CPMSInvoiceDlg::CPMSInvoiceDlg( int nRoomGridRoomIdx, int nRoomGridAccountIdx, bool bCheckout, bool bAllowCancel, bool bPendingBooking, CWnd* pParent)
	: CSSDialog(CPMSInvoiceDlg::IDD, pParent)
{
	m_nRoomGridRoomIdx = nRoomGridRoomIdx;
	m_nRoomGridAccountIdx = nRoomGridAccountIdx;

	PMSRoomStatusTable.GetRoomInfo( nRoomGridRoomIdx, m_infoRoom );
	PMSRoomStatusTable.GetAccountInfo( nRoomGridRoomIdx, nRoomGridAccountIdx, m_infoAccount );

	m_bCheckout = bCheckout;
	m_bAllowCancel = bAllowCancel;
	m_bPendingBooking = bPendingBooking;

	m_nEditID[0] = IDC_EDIT1;
	m_nEditID[1] = IDC_EDIT2;
	m_nEditID[2] = IDC_EDIT3;
	m_nEditID[3] = IDC_EDIT4;
	m_nEditID[4] = IDC_EDIT5;
	m_nEditID[5] = IDC_EDIT6;
	m_nEditID[6] = IDC_EDIT7;
	m_nEditID[7] = IDC_EDIT8;
	m_nEditID[8] = IDC_EDIT9;
	m_nEditID[9] = IDC_EDIT10;
	
	m_nButtonID[0] = IDC_BUTTON_AUTO1;
	m_nButtonID[1] = IDC_BUTTON_AUTO2;
	m_nButtonID[2] = IDC_BUTTON_AUTO3;
	m_nButtonID[3] = IDC_BUTTON_AUTO4;
	m_nButtonID[4] = IDC_BUTTON_AUTO5;
	m_nButtonID[5] = IDC_BUTTON_AUTO6;
	m_nButtonID[6] = IDC_BUTTON_AUTO7;
	m_nButtonID[7] = IDC_BUTTON_AUTO8;
	m_nButtonID[8] = IDC_BUTTON_AUTO9;
	m_nButtonID[9] = IDC_BUTTON_AUTO10;
	
	m_nPaymentTypeCount = 0;

	for ( int n = 0; n < 10; n++ )
	{
		m_nPaymentNo[n] = 0;
		m_strPaymentName[n] = "";
		m_dPayment[n] = 0.0;
	}

	m_nInvoiceType = PMSOptions.GetInvoiceDefaultType();
	if ( ( m_nInvoiceType < 0 ) || ( m_nInvoiceType > 2 ) )
		m_nInvoiceType = 0;

	if ( ( 2 == m_nInvoiceType ) && ( TRUE == m_bPendingBooking ) )
		m_nInvoiceType = 0;

	m_dSpend = 0.0;
	m_dAllowance = 0.0;
	m_dPaid = 0.0;
	m_dBalance = 0.0;
}

/**********************************************************************/

CPMSInvoiceDlg::~CPMSInvoiceDlg()
{
}

/**********************************************************************/

void CPMSInvoiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control( pDX, IDC_STATIC_BALANCEBOX, m_staticBalanceBox );
	DDX_Control( pDX, IDC_EDIT_SPEND, m_editSpend );
	DDX_Control( pDX, IDC_BUTTON_SPEND, m_buttonSpend );
	DDX_Control( pDX, IDC_EDIT_ALLOWANCE, m_editAllowance );
	DDX_Control( pDX, IDC_EDIT_PAID, m_editPaid );
	DDX_Control( pDX, IDC_BUTTON_PAID, m_buttonPaid );
	DDX_Control( pDX, IDC_STATIC_BALANCE, m_staticBalance );
	DDX_Control( pDX, IDC_EDIT_BALANCE, m_editBalance );

	DDX_Control( pDX, IDC_STATIC_PAYMENTBOX, m_staticPaymentBox );
	DDX_Control( pDX, IDC_STATIC1, m_staticPayment[0] );
	DDX_Control( pDX, IDC_STATIC2, m_staticPayment[1] );
	DDX_Control( pDX, IDC_STATIC3, m_staticPayment[2] );
	DDX_Control( pDX, IDC_STATIC4, m_staticPayment[3] );
	DDX_Control( pDX, IDC_STATIC5, m_staticPayment[4] );
	DDX_Control( pDX, IDC_STATIC6, m_staticPayment[5] );
	DDX_Control( pDX, IDC_STATIC7, m_staticPayment[6] );
	DDX_Control( pDX, IDC_STATIC8, m_staticPayment[7] );
	DDX_Control( pDX, IDC_STATIC9, m_staticPayment[8] );
	DDX_Control( pDX, IDC_STATIC10, m_staticPayment[9] );
	DDX_Control( pDX, IDC_BUTTON_CONFIRM, m_buttonConfirm );

	DDX_Control( pDX, IDC_STATIC_INVOICEBOX, m_staticInvoiceBox );
	DDX_Control( pDX, IDC_RADIO1, m_radioInvoice1 );
	DDX_Control( pDX, IDC_RADIO2, m_radioInvoice2 );
	DDX_Control( pDX, IDC_RADIO3, m_radioInvoice3 );
	DDX_Control( pDX, IDC_BUTTON_PRINT, m_buttonPrint );
	DDX_Control( pDX, IDC_BUTTON_DISPLAY, m_buttonDisplay );
	
	DDX_Control( pDX, IDOK, m_buttonOK );
	DDX_Control( pDX, IDCANCEL, m_buttonCancel );

	for ( int n = 0; n < 10; n++ )
		DDX_Control( pDX, m_nButtonID[n], m_buttonAuto[n] );

	for ( int n = 0; n < 10; n++ )
		DDX_Text( pDX, m_nEditID[n], m_dPayment[n] );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPMSInvoiceDlg, CDialog)
	ON_BN_CLICKED( IDC_BUTTON_PRINT, OnButtonPrint)
	ON_BN_CLICKED( IDC_BUTTON_DISPLAY, OnButtonDisplay)
	ON_BN_CLICKED( IDC_BUTTON_SPEND, OnButtonSpend)
	ON_BN_CLICKED( IDC_BUTTON_PAID, OnButtonPayment)
	ON_BN_CLICKED( IDC_BUTTON_AUTO1, OnButtonAuto1)
	ON_BN_CLICKED( IDC_BUTTON_AUTO2, OnButtonAuto2)
	ON_BN_CLICKED( IDC_BUTTON_AUTO3, OnButtonAuto3)
	ON_BN_CLICKED( IDC_BUTTON_AUTO4, OnButtonAuto4)
	ON_BN_CLICKED( IDC_BUTTON_AUTO5, OnButtonAuto5)
	ON_BN_CLICKED( IDC_BUTTON_AUTO6, OnButtonAuto6)
	ON_BN_CLICKED( IDC_BUTTON_AUTO7, OnButtonAuto7)
	ON_BN_CLICKED( IDC_BUTTON_AUTO8, OnButtonAuto8)
	ON_BN_CLICKED( IDC_BUTTON_AUTO9, OnButtonAuto9)
	ON_BN_CLICKED( IDC_BUTTON_AUTO10, OnButtonAuto10)
	ON_BN_CLICKED( IDC_BUTTON_CONFIRM, OnButtonConfirm)
	ON_BN_CLICKED( IDC_RADIO1, OnSelectInvoiceType0)
	ON_BN_CLICKED( IDC_RADIO2, OnSelectInvoiceType1)
	ON_BN_CLICKED( IDC_RADIO3, OnSelectInvoiceType2)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPMSInvoiceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CPMSBookingCSVRecord BookingRecordInvoice;
	DataManagerNonDb.PMSInvoiceBookingBuffer.GetInvoiceBooking( BookingRecordInvoice );

	CString strRoomName;
	::GetPMSRoomText( m_infoRoom.GetRoomNo(), m_infoRoom.GetRoomName(), strRoomName );

	CString strRoomText;
	strRoomText.Format( "%s",
		(const char*) strRoomName );

	CString strTraining = "";
	if ( BookingRecordInvoice.GetTrainingFlag() == TRUE )
		strTraining = " (Training Mode)";
	
	if ( FALSE == m_bCheckout )
	{
		CString strAction;

		if ( TRUE == m_bPendingBooking )
			strAction = "Print Booking Info";
		else if ( BookingRecordInvoice.GetBookingStatus() != PMS_BOOKING_STATUS_COMPLETE )
			strAction = "Print Interim Invoice";
		else if ( BookingRecordInvoice.GetInvoiceNum() == 0 )
			strAction = "Print Invoice";
		else if ( 0 == m_nRoomGridAccountIdx )
			strAction.Format( "Print Invoice %8.8d", BookingRecordInvoice.GetInvoiceNum() );
		else
			strAction.Format( "Print Invoice %8.8d-%d", BookingRecordInvoice.GetInvoiceNum(), m_nRoomGridAccountIdx + 1 );

		CString str;

		if ( FALSE == m_bPendingBooking )
		{	
			str.Format ( "%s (%s, %s)",
				(const char*) strAction,
				(const char*) strRoomText,
				m_infoAccount.GetGuestName() );
		}
		else
		{
			int nCustomerIdx;
			int nCustomerId = BookingRecordInvoice.GetCustomerId();
			if ( DataManagerNonDb.PMSCustomer.FindCustomerById( nCustomerId, nCustomerIdx ) == TRUE )
			{
				CPMSCustomerCSVRecord Customer;
				DataManagerNonDb.PMSCustomer.GetAt( nCustomerIdx, Customer );

				str.Format ( "%s (%s, %s)",
					(const char*) strAction,
					(const char*) strRoomText,
					Customer.GetGuestName() );
			}
		}

		SetWindowText( str + strTraining );

		m_buttonSpend.ShowWindow( SW_HIDE );
		m_buttonSpend.EnableWindow( FALSE );
		m_buttonPaid.ShowWindow( SW_HIDE );
		m_buttonPaid.EnableWindow( FALSE );
	}
	else
	{
		CString str;
		str.Format ( "Check Out (%s, %s)", 
			(const char*) strRoomText,
			m_infoAccount.GetGuestName() );

		SetWindowText( str + strTraining );
	}

	for ( int n = 0; n < 10; n++ )
		m_dPayment[n] = 0.0;

	m_dSpend = DataManagerNonDb.PMSInvoiceBookingBuffer.GetTotalSpend( m_nRoomGridAccountIdx );
	m_dAllowance = DataManagerNonDb.PMSInvoiceBookingBuffer.GetAllowance( m_nRoomGridAccountIdx );
	m_dPaid = DataManagerNonDb.PMSInvoiceBookingBuffer.GetTotalPaid( m_nRoomGridAccountIdx );
	m_dBalance = DataManagerNonDb.PMSInvoiceBookingBuffer.GetBalance( m_nRoomGridAccountIdx );

	CDialog::OnInitDialog();

	for ( int n = 0; n < 10; n++ )
		SubclassEdit( m_nEditID[n], SS_NUM_DP, 7, "%.2f" );

	DisplayBalance();

	for ( int n = 0; n < PMSOptions.GetValidatedPaymentTypeArraySize(); n++ )
	{
		CCSV csv( PMSOptions.GetValidatedPaymentTypeString(n) );

		int nPaymentNo = csv.GetInt(0);
		CString strPaymentName = csv.GetString(1);

		m_staticPayment[ m_nPaymentTypeCount ].SetWindowText( strPaymentName );

		m_nPaymentNo[ m_nPaymentTypeCount ] = nPaymentNo;
		m_strPaymentName[ m_nPaymentTypeCount ] = strPaymentName;

		m_nPaymentTypeCount++;
	}

	for ( int n = m_nPaymentTypeCount; n < 10; n++ )
	{
		m_staticPayment[ n ].ShowWindow( SW_HIDE );
		GetEditPayment( n ) -> ShowWindow( SW_HIDE );
		m_buttonAuto[n].EnableWindow( FALSE );
		m_buttonAuto[n].ShowWindow( SW_HIDE );
	}

	if ( m_nPaymentTypeCount < 10 )
	{
		int nShrink = 20 * ( 10 - m_nPaymentTypeCount );
		if ( nShrink > 80 )
			nShrink = 80;

		ResizeControl( &m_staticBalanceBox, 195, 278 - nShrink );
		ResizeControl( &m_staticPaymentBox, 195, 278 - nShrink );
		MoveControl( &m_buttonConfirm, 219, 244 - nShrink );
		ResizeControl( &m_staticInvoiceBox, 185, 278 - nShrink );
		MoveControl( &m_buttonCancel, 7, 295 - nShrink );
		MoveControl( &m_buttonOK, 545, 295 - nShrink );
		ResizeControl( &m_staticInvoiceBox, 185, 278 - nShrink );
		ResizeDialog( 602, 330 - nShrink );
	}

	switch( m_nInvoiceType )
	{
	case 1:		OnSelectInvoiceType1();		break;
	case 2:		OnSelectInvoiceType2();		break;
	default:	OnSelectInvoiceType0();		break;
	}

	if ( ( TRUE == m_bPendingBooking ) || ( PasswordArray.GetEnable( PasswordTicks::PMSPayments ) == FALSE ) )
	{
		for ( int n = 0; n < 10; n++ )
		{
			m_staticPayment[n].ShowWindow( SW_HIDE );
			GetEditPayment(n) -> ShowWindow( SW_HIDE );
			m_buttonAuto[n].EnableWindow( FALSE );
			m_buttonAuto[n].ShowWindow( SW_HIDE );
		}

		m_buttonConfirm.ShowWindow( SW_HIDE );
		m_buttonConfirm.EnableWindow( FALSE );
		m_buttonPaid.EnableWindow( FALSE );
		m_buttonPaid.ShowWindow( SW_HIDE );
	}

	if ( TRUE == m_bPendingBooking )
	{
		m_radioInvoice1.SetWindowText( "Show deposit detail" );
		m_radioInvoice2.SetWindowText( "Show deposit summary" );
		m_radioInvoice3.ShowWindow( SW_HIDE );
		m_radioInvoice3.EnableWindow( FALSE );
		m_staticInvoiceBox.SetWindowText( "Booking && Deposit Info" );
		m_buttonDisplay.SetWindowText( "Display Info" );
		m_buttonPrint.SetWindowText( "Print Info" );
	}

	if ( PasswordArray.GetEnable( PasswordTicks::PMSInvoices ) == FALSE )
	{
		m_radioInvoice1.EnableWindow( FALSE );
		m_radioInvoice2.EnableWindow( FALSE );
		m_radioInvoice3.EnableWindow( FALSE );
		m_buttonDisplay.EnableWindow( FALSE );
		m_buttonPrint.EnableWindow( FALSE );
	}

	if ( PasswordArray.GetEnable( PasswordTicks::PMSManualSales ) == FALSE )
	{
		m_buttonSpend.EnableWindow( FALSE );
		m_buttonSpend.ShowWindow( SW_HIDE );
	}

	if ( ( TRUE == m_bCheckout ) && ( TRUE == m_bAllowCancel ) )
		m_buttonOK.SetWindowText( "Confirm\nCheckout" );
	else
	{
		m_buttonOK.SetWindowText( "Close" );
		m_buttonCancel.EnableWindow( FALSE );
		m_buttonCancel.ShowWindow( SW_HIDE );
		m_bAllowCancel = FALSE;
	}

	return TRUE;  
}

/**********************************************************************/
CEdit* CPMSInvoiceDlg::GetEditPayment( int n ){ return GetEdit( m_nEditID[n] ); }
/**********************************************************************/

void CPMSInvoiceDlg::DisplayBalance()
{
	CString strDouble;
	CPriceHelpers::FormatDouble( m_dSpend, 2, strDouble );
	m_editSpend.SetWindowText( strDouble );
	CPriceHelpers::FormatDouble( m_dAllowance, 2, strDouble );
	m_editAllowance.SetWindowText( strDouble );
	CPriceHelpers::FormatDouble( m_dPaid, 2, strDouble );
	m_editPaid.SetWindowText( strDouble );

	switch ( CPriceHelpers::CompareDoubles( m_dBalance, 0.0, 2 ) )
	{
	case 1:
		CPriceHelpers::FormatDouble( m_dBalance, 2, strDouble );
		m_editBalance.SetWindowText( strDouble );
		m_staticBalance.SetWindowText( "Balance" );
		break;

	case -1:
		CPriceHelpers::FormatDouble( -m_dBalance, 2, strDouble );
		m_editBalance.SetWindowText( strDouble );
		m_staticBalance.SetWindowText( "CREDIT Balance" );
		break;

	case 0:
	default:
		CPriceHelpers::FormatDouble( 0.0, 2, strDouble );
		m_editBalance.SetWindowText( strDouble );
		m_staticBalance.SetWindowText( "Balance" );
		break;
	}
}

/**********************************************************************/

void CPMSInvoiceDlg::OnSelectInvoiceType0(){ SelectInvoiceType(0); }
void CPMSInvoiceDlg::OnSelectInvoiceType1(){ SelectInvoiceType(1); }
void CPMSInvoiceDlg::OnSelectInvoiceType2(){ SelectInvoiceType(2); }

/**********************************************************************/

void CPMSInvoiceDlg::SelectInvoiceType( int n )
{
	if ( ( n >= 0 ) && ( n <= 2 ) )
	{
		m_nInvoiceType = n;
		m_radioInvoice1.SetCheck( 0 == m_nInvoiceType );
		m_radioInvoice2.SetCheck( 1 == m_nInvoiceType );
		m_radioInvoice3.SetCheck( 2 == m_nInvoiceType );
	}
}

/**********************************************************************/

void CPMSInvoiceDlg::OnButtonDisplay()
{
	UpdateData( TRUE, FALSE );

	CPMSReportInvoice Report( m_nRoomGridRoomIdx, m_nRoomGridAccountIdx, m_nInvoiceType, m_bPendingBooking );

	CString strKey;
	strKey.Format( "%d", m_nInvoiceType );

	if ( Report.CreateReport() == FALSE )
		Prompter.Error( "Unable to create report." );
	else
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport( Super.ReportFilename(), Report.GetParamsFilename(), strKey, this );
	}
}

/**********************************************************************/

void CPMSInvoiceDlg::OnButtonPrint()
{
	UpdateData( TRUE, FALSE );

	CPMSReportInvoice Report( m_nRoomGridRoomIdx, m_nRoomGridAccountIdx, m_nInvoiceType, m_bPendingBooking );

	CString strKey;
	strKey.Format( "%d", m_nInvoiceType );

	if ( Report.CreateReport() == FALSE )
		Prompter.Error( "Unable to create report." );
	else
	{
		CReportManager ReportManager;
		ReportManager.PrintReport( Super.ReportFilename(), Report.GetParamsFilename(), strKey, this );
	}
}

/**********************************************************************/

bool CPMSInvoiceDlg::CheckPendingPayments()
{
	double dPendingPay = 0.0;
	for ( int nPay = 0; nPay < m_nPaymentTypeCount; nPay++ )
		dPendingPay += m_dPayment[nPay];
	
	if ( CPriceHelpers::CompareDoubles( dPendingPay, 0.0, 3 ) == 1 )
	{
		CString strMsg = "";
		strMsg += "There are payments on this page that have not been confirmed.\n\n";
		strMsg += "These payments will be cancelled if you close this page now.\n\n";
		strMsg += "Are you sure you wish to cancel these payments";

		if ( Prompter.YesNo( strMsg ) != IDYES )
			return FALSE;
	}

	return TRUE;
}

/**********************************************************************/

void CPMSInvoiceDlg::OnButtonSpend()
{
	CPMSBookingCSVRecord BookingRecordInvoice;
	DataManagerNonDb.PMSInvoiceBookingBuffer.GetInvoiceBooking( BookingRecordInvoice );
	bool bTraining = BookingRecordInvoice.GetTrainingFlag();

	if ( CPMSCalendarDlg::ProcessQuickPluSales( m_nRoomGridRoomIdx, m_nRoomGridAccountIdx, bTraining, this ) == TRUE )
	{
		CPMSManualSalesDatabaseDlg dlg( m_nRoomGridRoomIdx, m_nRoomGridAccountIdx, PMS_MANUAL_SALES, bTraining, this );
		dlg.DoModal();

		DataManagerNonDb.PMSInvoiceBookingBuffer.UpdateBalance( m_nRoomGridAccountIdx );
		m_dSpend = DataManagerNonDb.PMSInvoiceBookingBuffer.GetTotalSpend( m_nRoomGridAccountIdx );
		m_dAllowance = DataManagerNonDb.PMSInvoiceBookingBuffer.GetAllowance( m_nRoomGridAccountIdx );
		m_dPaid = DataManagerNonDb.PMSInvoiceBookingBuffer.GetTotalPaid( m_nRoomGridAccountIdx );
		m_dBalance = DataManagerNonDb.PMSInvoiceBookingBuffer.GetBalance( m_nRoomGridAccountIdx );

		DisplayBalance();
	}
}

/**********************************************************************/

void CPMSInvoiceDlg::OnButtonPayment()
{
	PMSOptions.BuildValidatedPaymentTypeArray();
	if ( PMSOptions.GetValidatedPaymentTypeArraySize() == 0 )
	{
		PMSOptions.NoPaymentTypeError();
		return;
	}

	CPMSBookingCSVRecord BookingRecordInvoice;
	DataManagerNonDb.PMSInvoiceBookingBuffer.GetInvoiceBooking( BookingRecordInvoice );
	bool bTraining = BookingRecordInvoice.GetTrainingFlag();

	CPMSManualSalesDatabaseDlg dlg( m_nRoomGridRoomIdx, m_nRoomGridAccountIdx, PMS_MANUAL_PAYMENT, bTraining, this );
	dlg.DoModal();

	DataManagerNonDb.PMSInvoiceBookingBuffer.UpdateBalance( m_nRoomGridAccountIdx );
	m_dSpend = DataManagerNonDb.PMSInvoiceBookingBuffer.GetTotalSpend( m_nRoomGridAccountIdx );
	m_dAllowance = DataManagerNonDb.PMSInvoiceBookingBuffer.GetAllowance( m_nRoomGridAccountIdx );
	m_dPaid = DataManagerNonDb.PMSInvoiceBookingBuffer.GetTotalPaid( m_nRoomGridAccountIdx );
	m_dBalance = DataManagerNonDb.PMSInvoiceBookingBuffer.GetBalance( m_nRoomGridAccountIdx );

	DisplayBalance();
}

/**********************************************************************/
void CPMSInvoiceDlg::OnButtonAuto1(){ AutoBalance(0); }
void CPMSInvoiceDlg::OnButtonAuto2(){ AutoBalance(1); }
void CPMSInvoiceDlg::OnButtonAuto3(){ AutoBalance(2); }
void CPMSInvoiceDlg::OnButtonAuto4(){ AutoBalance(3); }
void CPMSInvoiceDlg::OnButtonAuto5(){ AutoBalance(4); }
void CPMSInvoiceDlg::OnButtonAuto6(){ AutoBalance(5); }
void CPMSInvoiceDlg::OnButtonAuto7(){ AutoBalance(6); }
void CPMSInvoiceDlg::OnButtonAuto8(){ AutoBalance(7); }
void CPMSInvoiceDlg::OnButtonAuto9(){ AutoBalance(8); }
void CPMSInvoiceDlg::OnButtonAuto10(){ AutoBalance(9); }
/**********************************************************************/

void CPMSInvoiceDlg::AutoBalance( int n )
{
	UpdateData( TRUE, FALSE );

	double dStartBalance = m_dBalance;

	double dPendingPay = 0.0;
	for ( int nPay = 0; nPay < m_nPaymentTypeCount; nPay++ )
	{
		if ( nPay != n )
			dPendingPay += m_dPayment[nPay];
	}

	double dAutoPay = dStartBalance - dPendingPay;
	if ( dAutoPay < 0.0 )
		dAutoPay = 0.0;

	m_dPayment[n] = dAutoPay;

	UpdateData( FALSE );
}

/**********************************************************************/

void CPMSInvoiceDlg::OnButtonConfirm()
{
	UpdateData( TRUE, FALSE );

	CPMSInvoiceArray arrayPayments;
	CreatePaymentArray( arrayPayments );

	arrayPayments.WriteInvoice( m_infoRoom.GetBookingId(), m_nRoomGridAccountIdx + 1 );

	double dPaidNow = 0.0;
	for ( int nPay = 0; nPay < m_nPaymentTypeCount; nPay++ )
		dPaidNow += m_dPayment[nPay];
	
	m_dPaid += dPaidNow;
	m_dBalance -= dPaidNow;

	for ( int nPay = 0; nPay < m_nPaymentTypeCount; nPay++ )
		m_dPayment[nPay] = 0.0;

	m_bPaymentOwed = ( CPriceHelpers::CompareDoubles( m_dBalance, 0.0, 3 ) == 1 );

	DisplayBalance();
	
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPMSInvoiceDlg::CreatePaymentArray( CPMSInvoiceArray& arrayPayments )
{
	arrayPayments.Reset();

	CPMSInvoiceLineInfo info;
	info.m_dateSale = COleDateTime::GetCurrentTime();
	info.m_nLineType = PMS_LINE_TRAN;
	info.m_nTransactionNo = 0;
	
	for ( int n = 0; n < m_nPaymentTypeCount; n++ )
	{
		if ( CPriceHelpers::CompareDoubles( m_dPayment[n], 0.0, 3 ) == 1 )
		{
			if ( arrayPayments.GetInvoiceItemCount() == 0 )
				arrayPayments.AddInvoiceItem( info );

			info.m_nLineType = PMS_LINE_PAYMENT;
			info.m_nBasePluNo = m_nPaymentNo[n];
			info.m_strDescription = m_strPaymentName[n];
			info.m_dValue = m_dPayment[n];
			arrayPayments.AddInvoiceItem( info );
		}
	}
}

/**********************************************************************/

void CPMSInvoiceDlg::OnOK() 
{
	UpdateData( TRUE, FALSE );

	if ( CheckPendingPayments() == TRUE )
		EndDialog(IDOK);
}

/**********************************************************************/

void CPMSInvoiceDlg::OnCancel() 
{
	UpdateData( TRUE, FALSE );

	if ( CheckPendingPayments() == TRUE )
	{
		if ( TRUE == m_bAllowCancel )
			EndDialog(IDCANCEL);
		else
			EndDialog(IDOK);
	}
}

/**********************************************************************/
#endif
/**********************************************************************/
