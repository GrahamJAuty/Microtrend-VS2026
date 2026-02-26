/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PMSBookingCSVArray.h"
#include "PMSDepositEditDlg.h"
/**********************************************************************/
#include "ListDataPMSDepositDlg.h"
/**********************************************************************/

CSingleListContentHelpersPMSDeposit::CSingleListContentHelpersPMSDeposit( CPMSBookingCSVRecord& BookingRecordBuffer ) : m_BookingRecordBuffer( BookingRecordBuffer )
{
}

/**********************************************************************/

int CSingleListContentHelpersPMSDeposit::GetArraySize()
{
	return m_BookingRecordBuffer.GetDepositCount();
}

/**********************************************************************/

bool CSingleListContentHelpersPMSDeposit::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CPMSDepositInfo infoDeposit;
		m_BookingRecordBuffer.GetDepositInfo( nIndex, infoDeposit );

		COleDateTime oleDate;
		GetOleDateSince2011( infoDeposit.GetPMSDay(), oleDate );

		CString strDate;
		strDate.Format( "%2.2d/%2.2d/%4.4d",
			oleDate.GetDay(),
			oleDate.GetMonth(),
			oleDate.GetYear() );

		CString strType = ( infoDeposit.GetRefundFlag() ) ? "REFUND" : "PAYMENT";
		CString strMethod = DataManager.Payment.GetDisplayName( infoDeposit.GetPaymentType() );
		double dValue = infoDeposit.GetValue();

		strLine.Format ( "%s\t%s\t%s\t%.2f",
			(const char*) strDate,
			(const char*) strType,
			(const char*) strMethod,
			dValue );

		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersPMSDeposit::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < m_BookingRecordBuffer.GetDepositCount() ) )
	{
		m_BookingRecordBuffer.RemoveDepositInfo( nIndex );
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersPMSDeposit::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "Date", LVCFMT_LEFT, 80 );
	list.InsertColumn ( 1, "Type", LVCFMT_LEFT, 140 );
	list.InsertColumn ( 2, "Method", LVCFMT_LEFT, 140 );
	list.InsertColumn ( 3, "Amount", LVCFMT_RIGHT, 120 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataPMSDepositDlg::CListDataPMSDepositDlg( CPMSBookingCSVRecord& BookingRecord, CWnd* pParent) : CListDataDlg ( pParent ), m_BookingRecord( BookingRecord )
{
	CPMSBookingCSVArray arrayTemp;
	arrayTemp.InsertAt( 0, m_BookingRecord );
	arrayTemp.GetAt( 0, m_BookingRecordBuffer );
	m_BookingRecordBuffer.SortDepositTransactions(0);

	m_pContentHelpers = new CSingleListContentHelpersPMSDeposit( m_BookingRecordBuffer );
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "deposit entry" );
	m_pDisplayHelpers -> SetTopPluralText( "deposit entries" );
	m_pDisplayHelpers -> SetBottomSingleText( "entry" );
	m_pDisplayHelpers -> SetBottomPluralText( "entries" );
}

/**********************************************************************/

CListDataPMSDepositDlg::~CListDataPMSDepositDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataPMSDepositDlg::SetTitle()
{
	SetWindowText( "Edit Deposit" );
}

/**********************************************************************/

void CListDataPMSDepositDlg::SetButtonTexts()
{
	m_button1.SetWindowText( "Payment" );
	m_button2.SetWindowText( "Refund" );
	m_button3.SetWindowText( "Edit" );
	m_button4.SetWindowText( "Delete" );
	m_button5.ShowWindow( SW_HIDE );
	m_button6.ShowWindow( SW_HIDE );

	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button2, 368, 47 );
	MoveControl( &m_button3, 368, 87 );
	MoveControl( &m_button4, 368, 147 );
}

/**********************************************************************/

void CListDataPMSDepositDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataPMSDepositDlg::HandleButton1()
{
	COleDateTime timeNow = COleDateTime::GetCurrentTime();
	int nPMSDay = GetDayNumberSince2011( timeNow );

	CPMSDepositInfo infoDeposit;
	infoDeposit.SetPMSDay( nPMSDay );
	infoDeposit.SetHour( timeNow.GetHour() );
	infoDeposit.SetMinute( timeNow.GetMinute() );
	infoDeposit.SetRefundFlag( FALSE );
	infoDeposit.SetPaymentType(0);
	infoDeposit.SetValue(0);

	CPMSDepositEditDlg dlgEdit( infoDeposit, TRUE, this );
	if ( dlgEdit.DoModal() == IDOK )
	{
		m_BookingRecordBuffer.AddDepositInfo( infoDeposit );
		m_listData.SetItemCountEx ( GetArraySize() );
		SelectLine ( m_BookingRecordBuffer.SortDepositTransactions( m_BookingRecordBuffer.GetDepositCount() - 1 ) );
		m_WarnCancelAfterChange.SetFlag();
	}
}

/**********************************************************************/

void CListDataPMSDepositDlg::HandleButton2()
{
	COleDateTime timeNow = COleDateTime::GetCurrentTime();
	int nPMSDay = GetDayNumberSince2011( timeNow );

	CPMSDepositInfo infoDeposit;
	infoDeposit.SetPMSDay( nPMSDay );
	infoDeposit.SetHour( timeNow.GetHour() );
	infoDeposit.SetMinute( timeNow.GetMinute() );
	infoDeposit.SetRefundFlag( TRUE );
	infoDeposit.SetPaymentType(0);
	infoDeposit.SetValue(0);
	
	//IF A PREVIOUS PAYMENT IS SELECTED THEN ASSUME WE ARE TRYING
	//TO REFUND IT TO THE SAME PAYMENT METHOD AND AMOUNT
	int nIndex = GetSafeSelection();
	if ( nIndex != -1 )
	{
		CPMSDepositInfo infoSelected;
		m_BookingRecordBuffer.GetDepositInfo( nIndex, infoSelected );

		if ( infoSelected.GetRefundFlag() == FALSE )
		{
			infoDeposit.SetPaymentType( infoSelected.GetPaymentType() );
			infoDeposit.SetValue( infoSelected.GetValue() );
		}
	}

	infoDeposit.SetRefundFlag( TRUE );

	CPMSDepositEditDlg dlgEdit( infoDeposit, TRUE, this );
	if ( dlgEdit.DoModal() == IDOK )
	{
		m_BookingRecordBuffer.AddDepositInfo( infoDeposit );
		m_listData.SetItemCountEx ( GetArraySize() );
		SelectLine ( m_BookingRecordBuffer.SortDepositTransactions( m_BookingRecordBuffer.GetDepositCount() - 1 ) );
		m_WarnCancelAfterChange.SetFlag();
	}
}

/**********************************************************************/

void CListDataPMSDepositDlg::HandleButton3()
{
	int nIndex = GetSafeSelection();
	if ( nIndex == -1 )
		return;

	CPMSDepositInfo infoDeposit;
	m_BookingRecordBuffer.GetDepositInfo( nIndex, infoDeposit );

	CPMSDepositEditDlg dlgEdit( infoDeposit, FALSE, this );
	if ( dlgEdit.DoModal() == IDOK )
	{
		m_BookingRecordBuffer.SetDepositInfo( nIndex, infoDeposit );
		SelectLine ( m_BookingRecordBuffer.SortDepositTransactions( nIndex ) );
		m_WarnCancelAfterChange.SetFlag();
	}
}

/**********************************************************************/

void CListDataPMSDepositDlg::HandleButton4()
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );
}

/**********************************************************************/

bool CListDataPMSDepositDlg::HandleOK()
{
	CPMSBookingCSVArray arrayTemp;
	arrayTemp.InsertAt( 0, m_BookingRecordBuffer );
	arrayTemp.GetAt( 0, m_BookingRecord );
	return TRUE;
}

/**********************************************************************/
#endif
/**********************************************************************/
