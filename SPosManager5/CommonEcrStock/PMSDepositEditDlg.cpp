/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PMSOptions.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PMSDepositEditDlg.h"
/**********************************************************************/

CPMSDepositEditDlg::CPMSDepositEditDlg( CPMSDepositInfo& infoDeposit, bool bAdd, CWnd* pParent)
	: CSSDialog(CPMSDepositEditDlg::IDD, pParent), m_infoDeposit( infoDeposit )
{
	m_bAdd = bAdd;
	DataInit();
}

/**********************************************************************/

void CPMSDepositEditDlg::DataInit()
{
	//{{AFX_DATA_INIT(CPMSDepositEditDlg)
	//}}AFX_DATA_INIT
	m_bModified = FALSE;
	m_dPayment = 0.0;
	m_nSearchDay = 0;
	m_nSearchDayBase = 0;
	m_strInputLine = "";
	m_strOutputLine = "";
}

/**********************************************************************/

void CPMSDepositEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPMSDepositEditDlg)
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Control(pDX, IDOK, m_buttonOK);
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_TIMEPICKER1, m_TimePicker1);
	DDX_Control(pDX, IDC_COMBO_DATE, m_comboDate);
	DDX_Control(pDX, IDC_DATEPICKER, m_datePicker);
	DDX_Control(pDX, IDC_COMBO_PAYMENT, m_comboPayment);
	DDX_Text(pDX, IDC_EDIT_PAYMENT, m_dPayment );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPMSDepositEditDlg, CDialog)
	//{{AFX_MSG_MAP(CPMSDepositEditDlg)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO_DATE, OnChangeDateCombo)
	ON_CBN_DROPDOWN(IDC_COMBO_DATE, OnOpenDateCombo)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER, OnChangeDatePicker)
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPMSDepositEditDlg::GetEditPayment() { return GetEdit( IDC_EDIT_PAYMENT ); }
/**********************************************************************/

BOOL CPMSDepositEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString strTitle = "";
	strTitle += ( m_bAdd ) ? "Add" : "Edit";
	strTitle += " Deposit ";
	strTitle += ( m_infoDeposit.GetRefundFlag() ) ? "Refund" : "Payment";
	SetWindowText( strTitle );

	SubclassEdit ( IDC_EDIT_PAYMENT, SS_NUM_SDP, 8, "%.*f", SysInfo.GetDPValue() );
		
	m_dPayment = m_infoDeposit.GetValue();
	UpdateData( FALSE );
		
	GetEditPayment() -> SetFocus();
	GetEditPayment() -> SetSel( 0, -1 );

	for ( int n = 0; n < PMSOptions.GetValidatedPaymentTypeArraySize(); n++ )
	{
		CCSV csv( PMSOptions.GetValidatedPaymentTypeString(n) );
		int nPos = m_comboPayment.AddString( csv.GetString(1) );
		m_comboPayment.SetItemData( nPos, csv.GetInt(0) );
	}

	if ( m_infoDeposit.GetPaymentType() == 0 )
		m_comboPayment.SetCurSel(0);
	else
	{
		bool bGotMatch = FALSE;

		for ( int n = 0; n < m_comboPayment.GetCount(); n++ )
		{
			if ( m_comboPayment.GetItemData(n) == m_infoDeposit.GetPaymentType() )
			{
				m_comboPayment.SetCurSel(n);
				bGotMatch = TRUE;
				break;
			}
		}

		if ( FALSE == bGotMatch )
		{
			CString strPayment;
			strPayment.Format( "Payment %d", m_infoDeposit.GetPaymentType() );
			m_comboPayment.InsertString( 0, strPayment );
			m_comboPayment.SetItemData( 0, (int) m_infoDeposit.GetPaymentType() );
		}
	}

	if ( m_comboPayment.GetCount() == 1 )
	{
		m_comboPayment.SetCurSel(0);
		m_comboPayment.EnableWindow(FALSE);
	}

	CenterWindow();

	if ( m_infoDeposit.GetPaymentType() != 0 )
		m_nSearchDay = m_infoDeposit.GetPMSDay();
	else
		m_nSearchDay = DataManagerNonDb.PMSBooking.GetCurrentDayNumber();

	m_datePicker.SetFormat( "'calendar'" );

	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(35);
	arrayStops.Add(55);
	arrayStops.Add(80);
	m_comboDate.SetTabStops( arrayStops );

	OnOpenDateCombo();

	COleDateTime dateMin;
	GetOleDateSince2011( PMSBooking::DepositDay.Min, dateMin );
	
	COleDateTime dateMax;
	GetOleDateSince2011( PMSBooking::DepositDay.Max, dateMax );
	
	m_datePicker.SetRange( &dateMin, &dateMax );	
	UpdateDatePickerDate();

	m_TimePicker1.SetFormat ( "HH:mm" );

	{
		COleDateTime date = COleDateTime ( 2010, 1, 1, m_infoDeposit.GetHour(), m_infoDeposit.GetMinute(), 0 );
		
		if ( date.m_status != COleDateTime::valid )
			date = COleDateTime ( 2010, 1, 1, 0, 0, 0 );
			
		m_TimePicker1.SetTime( date );
	}

	//WE HAVE SET FOCUS TO A CONTROL
	return FALSE;  
}

/**********************************************************************/

void CPMSDepositEditDlg::OnOpenDateCombo()
{
	int nMinDay = PMSBooking::DepositDay.Min;
	int nMaxDay = PMSBooking::DepositDay.Max;

	int nSelDay = m_nSearchDay;

	m_nSearchDayBase = nSelDay - 7;
	
	if ( m_nSearchDayBase < nMinDay )
		m_nSearchDayBase = nMinDay;

	int nEndDay = nSelDay + 7;
	if ( nEndDay > nMaxDay )
		nEndDay = nMaxDay;

	if ( m_nSearchDayBase > nEndDay )
		m_nSearchDayBase = nEndDay;

	m_comboDate.ResetContent();

	for ( int nDay = m_nSearchDayBase; nDay <= nEndDay; nDay++ )
	{
		COleDateTime date;
		GetOleDateSince2011( nDay, date );

		CString strDayName = date.Format( "%a" );
		CString strDayNum = date.Format( "%d" );
		CString strMonth = date.Format( "%b" );
		CString strYear = date.Format( "%Y" );
		
		CCSV csv;
		csv.Add( strDayName );
		csv.Add( strDayNum );
		csv.Add( strMonth );
		csv.Add( strYear );
		m_comboDate.AddItem( csv.GetLine() );
	}

	m_comboDate.SetCurSel( nSelDay - m_nSearchDayBase );
}

/**********************************************************************/

void CPMSDepositEditDlg::UpdateDatePickerDate()
{
	int nDay = m_nSearchDay;
		
	if ( ( nDay < PMSBooking::DepositDay.Min ) || ( nDay > PMSBooking::DepositDay.Max ) )
		nDay = PMSBooking::DepositDay.Min;

	COleDateTime date;
	GetOleDateSince2011( nDay, date );
	m_datePicker.SetTime( date );
}

/**********************************************************************/

void CPMSDepositEditDlg::OnChangeDateCombo() 
{
	m_nSearchDay = m_nSearchDayBase + m_comboDate.GetCurSel();
	UpdateDatePickerDate();
}

/**********************************************************************/

void CPMSDepositEditDlg::OnChangeDatePicker(NMHDR* pNMHDR, LRESULT* pResult) 
{
	COleDateTime date;
	m_datePicker.GetTime( date );

	int nDate = GetDayNumberSince2011( date );

	if ( ( nDate >= PMSBooking::DepositDay.Min ) && ( nDate <= PMSBooking::DepositDay.Max ) )
	{
		m_nSearchDay = nDate;
		OnOpenDateCombo();
	}
	
	UpdateDatePickerDate();
	*pResult = 0;
}

/**********************************************************************/

void CPMSDepositEditDlg::OnOK() 
{
	m_buttonOK.SetFocus();

	if ( UpdateData ( TRUE ) == FALSE )
		return;

	HandleAdd();
	EndDialog ( IDOK );
}

/**********************************************************************/

void CPMSDepositEditDlg::HandleAdd() 
{
	m_buttonOK.SetFocus();

	COleDateTime time1;
	m_TimePicker1.GetTime ( time1 );

	int nSel = m_comboPayment.GetCurSel();
	m_infoDeposit.SetPMSDay( m_nSearchDay );
	m_infoDeposit.SetHour( time1.GetHour() );
	m_infoDeposit.SetMinute( time1.GetMinute() );
	m_infoDeposit.SetPaymentType( m_comboPayment.GetItemData(nSel) );
	m_infoDeposit.SetValue( m_dPayment );
}

/**********************************************************************/

void CPMSDepositEditDlg::OnCancel() 
{
	EndDialog( IDCANCEL );
}

/**********************************************************************/
#endif
/**********************************************************************/
