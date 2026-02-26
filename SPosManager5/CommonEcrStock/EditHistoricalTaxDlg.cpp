/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "NodeTypes.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EditHistoricalTaxDlg.h"
/**********************************************************************/

CEditHistoricalTaxDlg::CEditHistoricalTaxDlg(CTaxDateInfo& info, bool bAdd, CWnd* pParent)
	: CSSDialog(CEditHistoricalTaxDlg::IDD, pParent), m_TaxDateInfo(info)
{
	m_bAdd = bAdd;

	//{{AFX_DATA_INIT(CEditHistoricalTaxDlg)
	//}}AFX_DATA_INIT
	m_IDEditRate[0] = IDC_EDIT_RATE1;
	m_IDEditRate[1] = IDC_EDIT_RATE2;
	m_IDEditRate[2] = IDC_EDIT_RATE3;
	m_IDEditRate[3] = IDC_EDIT_RATE4;
	m_IDEditRate[4] = IDC_EDIT_RATE5;
	m_IDEditRate[5] = IDC_EDIT_RATE6;
	m_IDEditRate[6] = IDC_EDIT_RATE7;
	m_IDEditRate[7] = IDC_EDIT_RATE8;
	m_IDEditRate[8] = IDC_EDIT_RATE9;
	m_IDEditRate[9] = IDC_EDIT_RATE10;
	m_IDEditRate[10] = IDC_EDIT_RATE11;
	m_IDEditRate[11] = IDC_EDIT_RATE12;

	for (int nIndex = 0; nIndex < MAX_TAX_BANDS; nIndex++)
	{
		m_dRate[nIndex] = m_TaxDateInfo.m_dRate[nIndex];
	}

	m_nMaxLen = 8;
	m_nDPQty = 4;
}

/**********************************************************************/

void CEditHistoricalTaxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditHistoricalTaxDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DATEPICKER, m_DatePicker);

	for ( int nIndex = 0; nIndex < MAX_TAX_BANDS; nIndex++ )
	{
		DDX_Text(pDX, m_IDEditRate[nIndex], m_dRate[nIndex] );
	}
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CEditHistoricalTaxDlg, CDialog)
	//{{AFX_MSG_MAP(CEditHistoricalTaxDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CEditHistoricalTaxDlg::GetEditRate( int n ) { return GetEdit ( m_IDEditRate[n] ); }
/**********************************************************************/

BOOL CEditHistoricalTaxDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	for ( int nIndex = 0; nIndex < MAX_TAX_BANDS; nIndex++ )
	{
		SubclassEdit ( m_IDEditRate[nIndex], SS_NUM_DP, m_nMaxLen, "%.*f", m_nDPQty );
	}

	CString strType;
	if (TRUE == m_bAdd)
	{
		strType = EcrmanOptions.GetTaxString("Add Historical %T Rates");
	}
	else
	{
		strType = EcrmanOptions.GetTaxString("Edit Historical %T Rates");
	}

	CString strTitle;
	strTitle.Format( "%s (%s)",
		(const char*) strType,
		dbDatabase.GetDatabaseTitle( DataManager.GetActiveDbIdx(), EcrmanOptions.GetGlobalTaxFlag(), FALSE ) );

	SetWindowText ( strTitle );

	CString strInfoDate = m_TaxDateInfo.m_strDate;

	COleDateTime date (		atoi( strInfoDate.Left(4) ),
							atoi( strInfoDate.Mid(4,2) ),
							atoi( strInfoDate.Right(2) ),
							0, 0, 0 );
								
	CTime start ( 1980, 1, 1, 0, 0, 0, -1 );
	CTime end ( 2035, 1, 1, 0, 0, 0, -1 ); 
	m_DatePicker.SetRange ( &start, &end );
	
	CTime time = start + CTimeSpan ( 1, 0, 0, 0 );
	m_DatePicker.SetTime ( date );
	m_DatePicker.SetFormat( "ddd dd MMM yyy" );

	if ( FALSE == m_bAdd )
	{
		GetEditRate(0) -> SetFocus();
		GetEditRate(0) -> SetSel( 0, -1 );
	}
	
	return m_bAdd;  
}

/**********************************************************************/

void CEditHistoricalTaxDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	COleDateTime date;
	m_DatePicker.GetTime( date );

	CString strDate;
	strDate.Format( "%4.4d%2.2d%2.2d",
		date.GetYear(),
		date.GetMonth(),
		date.GetDay() );

	m_TaxDateInfo.m_strDate = strDate;

	for (int nIndex = 0; nIndex < MAX_TAX_BANDS; nIndex++)
	{
		m_TaxDateInfo.m_dRate[nIndex] = m_dRate[nIndex];
	}

	EndDialog ( IDOK );
}

/**********************************************************************/
