/**********************************************************************/
#include "..\SPosStockManager5\StockOptionsIni.h"
/**********************************************************************/
#include "PropPageOptionsExpire.h"
/**********************************************************************/

CPropPageOptionsExpire::CPropPageOptionsExpire() : CSSPropertyPage(CPropPageOptionsExpire::IDD)
{
	//{{AFX_DATA_INIT(CPropPageOptionsExpire)
	m_bExpire1 = FALSE;
	m_bExpire2 = FALSE;
	m_bExpire3 = FALSE;
	m_bExpire4 = FALSE;
	m_bExpire5 = FALSE;
	m_bExpire6 = FALSE;
	m_strMonths1 = _T("");
	m_strMonths2 = _T("");
	m_strMonths3 = _T("");
	m_strMonths4 = _T("");
	m_strMonths5 = _T("");
	m_strMonths6 = _T("");
	//}}AFX_DATA_INIT
}

/**********************************************************************/

CPropPageOptionsExpire::~CPropPageOptionsExpire()
{
}

/**********************************************************************/

void CPropPageOptionsExpire::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageOptionsExpire)
	DDX_Control(pDX, IDC_STATIC_MONTHS5, m_staticMonth5);
	DDX_Control(pDX, IDC_STATIC_MONTHS6, m_staticMonth6);
	DDX_Control(pDX, IDC_CHECK_EXPIRE2, m_checkExpire2);
	DDX_Control(pDX, IDC_CHECK_EXPIRE3, m_checkExpire3);
	DDX_Control(pDX, IDC_CHECK_EXPIRE4, m_checkExpire4);
	DDX_Control(pDX, IDC_CHECK_EXPIRE5, m_checkExpire5);
	DDX_Control(pDX, IDC_CHECK_EXPIRE6, m_checkExpire6);
	DDX_Check(pDX, IDC_CHECK_EXPIRE1, m_bExpire1);
	DDX_Check(pDX, IDC_CHECK_EXPIRE2, m_bExpire2);
	DDX_Check(pDX, IDC_CHECK_EXPIRE3, m_bExpire3);
	DDX_Check(pDX, IDC_CHECK_EXPIRE4, m_bExpire4);
	DDX_Check(pDX, IDC_CHECK_EXPIRE5, m_bExpire5);
	DDX_Check(pDX, IDC_CHECK_EXPIRE6, m_bExpire6);
	DDX_Text(pDX, IDC_EDIT_MONTHS1, m_strMonths1);
	DDX_Text(pDX, IDC_EDIT_MONTHS2, m_strMonths2);
	DDX_Text(pDX, IDC_EDIT_MONTHS3, m_strMonths3);
	DDX_Text(pDX, IDC_EDIT_MONTHS4, m_strMonths4);
	DDX_Text(pDX, IDC_EDIT_MONTHS5, m_strMonths5);
	DDX_Text(pDX, IDC_EDIT_MONTHS6, m_strMonths6);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageOptionsExpire, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageOptionsExpire)
	ON_BN_CLICKED(IDC_CHECK_EXPIRE1, OnToggleAutoExpire)
	ON_BN_CLICKED(IDC_CHECK_EXPIRE2, OnToggleAutoExpire)
	ON_BN_CLICKED(IDC_CHECK_EXPIRE3, OnToggleAutoExpire)
	ON_BN_CLICKED(IDC_CHECK_EXPIRE4, OnToggleAutoExpire)
	ON_BN_CLICKED(IDC_CHECK_EXPIRE5, OnToggleAutoExpire)
	ON_BN_CLICKED(IDC_CHECK_EXPIRE6, OnToggleAutoExpire)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPropPageOptionsExpire::GetExpireEdit1() { return GetEdit( IDC_EDIT_MONTHS1 ); }
CEdit* CPropPageOptionsExpire::GetExpireEdit2() { return GetEdit( IDC_EDIT_MONTHS2 ); }
CEdit* CPropPageOptionsExpire::GetExpireEdit3() { return GetEdit( IDC_EDIT_MONTHS3 ); }
CEdit* CPropPageOptionsExpire::GetExpireEdit4() { return GetEdit( IDC_EDIT_MONTHS4 ); }
CEdit* CPropPageOptionsExpire::GetExpireEdit5() { return GetEdit( IDC_EDIT_MONTHS5 ); }
CEdit* CPropPageOptionsExpire::GetExpireEdit6() { return GetEdit( IDC_EDIT_MONTHS6 ); }
/**********************************************************************/

BOOL CPropPageOptionsExpire::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();
	SubclassEdit ( IDC_EDIT_MONTHS1, SS_NUM, 3, "%s" );
	SubclassEdit ( IDC_EDIT_MONTHS2, SS_NUM, 3, "%s" );
	SubclassEdit ( IDC_EDIT_MONTHS3, SS_NUM, 3, "%s" );
	SubclassEdit ( IDC_EDIT_MONTHS4, SS_NUM, 3, "%s" );
	SubclassEdit ( IDC_EDIT_MONTHS5, SS_NUM, 3, "%s" );
	SubclassEdit ( IDC_EDIT_MONTHS6, SS_NUM, 3, "%s" );
	
#ifdef STOCKMAN_SYSTEM
	m_checkExpire4.SetWindowText ( "Delete Stock sales history after" );
	m_checkExpire5.SetWindowText ( "Delete Stock audits after" );
	m_checkExpire6.SetWindowText ( "Delete Sales Exceptions retry files after" );
#else
	m_checkExpire4.SetWindowText ( "Delete Plu delivery audit after" );
	m_checkExpire5.ShowWindow( SW_HIDE );
	m_staticMonth5.ShowWindow( SW_HIDE );
	GetExpireEdit5() -> ShowWindow( SW_HIDE );
	m_checkExpire6.ShowWindow( SW_HIDE );
	m_staticMonth6.ShowWindow( SW_HIDE );
	GetExpireEdit6() -> ShowWindow( SW_HIDE );
#endif

	OnToggleAutoExpire();
	return TRUE;  
}

/**********************************************************************/

BOOL CPropPageOptionsExpire::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageOptionsExpire::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	OnToggleAutoExpire();
	return bResult;
}

/**********************************************************************/

void CPropPageOptionsExpire::GetRecordData()
{
	int nMonths1 = EcrmanOptions.GetAutoExpireTimePluReports();
	int nMonths2 = EcrmanOptions.GetAutoExpireTimeSummaryReports();
	int nMonths3 = EcrmanOptions.GetAutoExpireTimeDetailReports();
#ifdef STOCKMAN_SYSTEM
	int nMonths4 = StockOptions.GetAutoExpireTimeSalesHistory();
	int nMonths5 = StockOptions.GetAutoExpireTimeStockAudit();
	int nMonths6 = StockOptions.GetAutoExpireTimeSalesRetry();
#else
	int nMonths4 = EcrmanOptions.GetAutoExpireTimePluDeliveryAudit();
	int nMonths5 = 0;
	int nMonths6 = 0;
#endif

	m_bExpire1 = ( nMonths1 >= 1 && nMonths1 <= 999 );
	m_bExpire2 = ( nMonths2 >= 1 && nMonths2 <= 999 );
	m_bExpire3 = ( nMonths3 >= 1 && nMonths3 <= 999 );
	m_bExpire4 = ( nMonths4 >= 1 && nMonths4 <= 999 );
	m_bExpire5 = ( nMonths5 >= 1 && nMonths5 <= 999 );
	m_bExpire6 = ( nMonths6 >= 1 && nMonths6 <= 999 );

	m_strMonths1 = "";
	m_strMonths2 = "";
	m_strMonths3 = "";
	m_strMonths4 = "";
	m_strMonths5 = "";
	m_strMonths6 = "";

	if ( m_bExpire1 ) m_strMonths1.Format ( "%d", nMonths1 );
	if ( m_bExpire2 ) m_strMonths2.Format ( "%d", nMonths2 );
	if ( m_bExpire3 ) m_strMonths3.Format ( "%d", nMonths3 );
	if ( m_bExpire4 ) m_strMonths4.Format ( "%d", nMonths4 );
	if ( m_bExpire5 ) m_strMonths5.Format ( "%d", nMonths5 );
	if ( m_bExpire6 ) m_strMonths6.Format ( "%d", nMonths6 );
}

/**********************************************************************/

void CPropPageOptionsExpire::SaveRecord()
{
	int nMonths1 = 0;
	int nMonths2 = 0;
	int nMonths3 = 0;
	int nMonths4 = 0;
	int nMonths5 = 0;
	int nMonths6 = 0;

	if ( m_bExpire1 ) nMonths1 = atoi ( m_strMonths1 );
	if ( m_bExpire2 ) nMonths2 = atoi ( m_strMonths2 );
	if ( m_bExpire3 ) nMonths3 = atoi ( m_strMonths3 );
	if ( m_bExpire4 ) nMonths4 = atoi ( m_strMonths4 );
	if ( m_bExpire5 ) nMonths5 = atoi ( m_strMonths5 );
	if ( m_bExpire6 ) nMonths6 = atoi ( m_strMonths6 );

	if ( nMonths1 < 1 || nMonths1 > 999 ) nMonths1 = 0;
	if ( nMonths2 < 1 || nMonths2 > 999 ) nMonths2 = 0;
	if ( nMonths3 < 1 || nMonths3 > 999 ) nMonths3 = 0;
	if ( nMonths4 < 1 || nMonths4 > 999 ) nMonths4 = 0;
	if ( nMonths5 < 1 || nMonths5 > 999 ) nMonths5 = 0;
	if ( nMonths6 < 1 || nMonths6 > 999 ) nMonths6 = 0;

	EcrmanOptions.SetAutoExpireTimePluReports( nMonths1 );
	EcrmanOptions.SetAutoExpireTimeSummaryReports( nMonths2 );
	EcrmanOptions.SetAutoExpireTimeDetailReports( nMonths3 );

#ifdef STOCKMAN_SYSTEM
	StockOptions.SetAutoExpireTimeSalesHistory( nMonths4 );
	StockOptions.SetAutoExpireTimeStockAudit( nMonths5 );
	StockOptions.SetAutoExpireTimeSalesRetry( nMonths6 );
#else
	EcrmanOptions.SetAutoExpireTimePluDeliveryAudit( nMonths4 );
#endif
}

/**********************************************************************/

void CPropPageOptionsExpire::OnToggleAutoExpire() 
{
	UpdateData( TRUE, FALSE );
	GetExpireEdit1() -> EnableWindow( m_bExpire1 );
	GetExpireEdit2() -> EnableWindow( m_bExpire2 );
	GetExpireEdit3() -> EnableWindow( m_bExpire3 );
	GetExpireEdit4() -> EnableWindow( m_bExpire4 );
	GetExpireEdit5() -> EnableWindow( m_bExpire5 );
	GetExpireEdit6() -> EnableWindow( m_bExpire6 );
	if ( m_bExpire1 == FALSE ) m_strMonths1 = "";
	if ( m_bExpire2 == FALSE ) m_strMonths2 = "";
	if ( m_bExpire3 == FALSE ) m_strMonths3 = "";
	if ( m_bExpire4 == FALSE ) m_strMonths4 = "";
	if ( m_bExpire5 == FALSE ) m_strMonths5 = "";
	if ( m_bExpire6 == FALSE ) m_strMonths6 = "";
	UpdateData( FALSE );
}

/**********************************************************************/
