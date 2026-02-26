/**********************************************************************/
#include "CCSVReportLine.h"
#include "RepSpawn.h"
/**********************************************************************/
#include "ReportLoyaltySchemeDlg.h"
/**********************************************************************/

CReportLoyaltySchemeInfo::CReportLoyaltySchemeInfo()
{
	m_bLines = TRUE;
	m_bRange = TRUE ;
	m_bAction = TRUE;
	m_bLimit = TRUE;
	m_bPayment = TRUE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CReportLoyaltySchemeDlg::CReportLoyaltySchemeDlg( CReportLoyaltySchemeInfo& info, CWnd* pParent)
	: CSSDialog(CReportLoyaltySchemeDlg::IDD, pParent), m_reportInfo(info)
{
	//{{AFX_DATA_INIT(CReportLoyaltySchemeDlg)
	//}}AFX_DATA_INIT
	m_nColCount = 0;
}

/**********************************************************************/

void CReportLoyaltySchemeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportLoyaltySchemeDlg)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_CHECK_LINES, m_checkLines );
	DDX_Control( pDX, IDC_CHECK_RANGE, m_checkRange );
	DDX_Control( pDX, IDC_CHECK_ACTION, m_checkAction );
	DDX_Control( pDX, IDC_CHECK_LIMIT, m_checkLimit );
	DDX_Control( pDX, IDC_CHECK_PAYMENT, m_checkPayment );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CReportLoyaltySchemeDlg, CDialog)
	//{{AFX_MSG_MAP(CReportLoyaltySchemeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CReportLoyaltySchemeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_checkLines.SetCheck( m_reportInfo.m_bLines );
	m_checkRange.SetCheck( m_reportInfo.m_bRange );
	m_checkAction.SetCheck( m_reportInfo.m_bAction );
	m_checkLimit.SetCheck( m_reportInfo.m_bLimit );
	m_checkPayment.SetCheck( m_reportInfo.m_bPayment );
	return FALSE;  
}

/**********************************************************************/

void CReportLoyaltySchemeDlg::AddColumn( CReportFile& ReportFile, const char* sz, int nAlign, int nWidth )
{
	ReportFile.AddColumn ( sz, nAlign, nWidth );
	m_nColCount++;
}

/**********************************************************************/

void CReportLoyaltySchemeDlg::OnOK() 
{
	m_reportInfo.m_bLines = IsTicked( m_checkLines );
	m_reportInfo.m_bRange = IsTicked( m_checkRange );
	m_reportInfo.m_bAction = IsTicked( m_checkAction );
	m_reportInfo.m_bLimit = IsTicked( m_checkLimit );
	m_reportInfo.m_bPayment = IsTicked( m_checkPayment );

	CReportFile ReportFile;
	if ( ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return;

	ReportFile.SetStyle1 ( "Loyalty Schemes" );
	
	m_nColCount = 0;
	int nActionPos = 0;
	int nPaymentPos = 0;

	AddColumn( ReportFile, "No", TA_LEFT, 120 );
	AddColumn( ReportFile, "Name", TA_LEFT, 500 );
	AddColumn( ReportFile, "Active", TA_LEFT, 200 );

	if ( TRUE == m_reportInfo.m_bRange )
	{
		AddColumn ( ReportFile, "Format", TA_LEFT, 400 );
		AddColumn ( ReportFile, "Lower", TA_LEFT, 400 );
		AddColumn ( ReportFile, "Upper", TA_LEFT, 400 );
		AddColumn ( ReportFile, "ManualChkDgt", TA_LEFT, 300 );
	}

	if ( TRUE == ( m_reportInfo.m_bRange || m_reportInfo.m_bLimit ) )
	{
		AddColumn ( ReportFile, "StaffCard", TA_LEFT, 300 );
		AddColumn ( ReportFile, "StaffCredit", TA_LEFT, 300 );
	}

	if ( TRUE == m_reportInfo.m_bAction )
	{
		AddColumn ( ReportFile, "ActionType", TA_LEFT, 450 );
		nActionPos = m_nColCount;
		AddColumn ( ReportFile, "ActionDetail", TA_LEFT, 500 );
		AddColumn ( ReportFile, "Macro", TA_LEFT, 500 );
		AddColumn ( ReportFile, "TripPerPoint", TA_RIGHT, 300 );
	}

	if ( TRUE == m_reportInfo.m_bLimit )
	{
		AddColumn ( ReportFile, "StaffLimitDay",	TA_RIGHT, 300 );
		AddColumn ( ReportFile, "StaffLimitWeek",	TA_RIGHT, 300 );
		AddColumn ( ReportFile, "StaffLimitJan",	TA_RIGHT, 300 );
		AddColumn ( ReportFile, "StaffLimitFeb",	TA_RIGHT, 300 );
		AddColumn ( ReportFile, "StaffLimitMar",	TA_RIGHT, 300 );
		AddColumn ( ReportFile, "StaffLimitApr",	TA_RIGHT, 300 );
		AddColumn ( ReportFile, "StaffLimitMay",	TA_RIGHT, 300 );
		AddColumn ( ReportFile, "StaffLimitJun",	TA_RIGHT, 300 );
		AddColumn ( ReportFile, "StaffLimitJul",	TA_RIGHT, 300 );
		AddColumn ( ReportFile, "StaffLimitAug",	TA_RIGHT, 300 );
		AddColumn ( ReportFile, "StaffLimitSep",	TA_RIGHT, 300 );
		AddColumn ( ReportFile, "StaffLimitOct",	TA_RIGHT, 300 );
		AddColumn ( ReportFile, "StaffLimitNov",	TA_RIGHT, 300 );
		AddColumn ( ReportFile, "StaffLimitDec",	TA_RIGHT, 300 );
	}

	if ( TRUE == m_reportInfo.m_bPayment )
	{
		nPaymentPos = m_nColCount;
		AddColumn ( ReportFile, "DeniedPayments", TA_LEFT, 400 );
	}

	for ( int nSchemeIdx = 0; nSchemeIdx < DataManager.LoyaltyScheme.GetSize(); nSchemeIdx++ )
	{
		CLoyaltySchemeCSVRecord LoyaltyScheme;
		DataManager.LoyaltyScheme.GetAt ( nSchemeIdx, LoyaltyScheme );

		//ALWAYS INCLUDE BASIC INFO
		CCSVReportLine csvOut;
		csvOut.AppendInt( LoyaltyScheme.GetSchemeNo() );
		csvOut.AppendString( LoyaltyScheme.GetSchemeName() );
		csvOut.AppendYesOrBlank( LoyaltyScheme.GetActiveFlag() );

		//CARD RANGE
		if ( TRUE == m_reportInfo.m_bRange )
		{
			csvOut.AppendString( LoyaltyScheme.GetFormat() );
			csvOut.AppendString( LoyaltyScheme.GetNumberStartLower() );
			csvOut.AppendString( LoyaltyScheme.GetNumberStartUpper() );
			csvOut.AppendYesOrBlank( LoyaltyScheme.GetCheckDigitFlag() );
		}

		//CARD RANGE OR STAFF LIMITS
		if ( TRUE == ( m_reportInfo.m_bRange || m_reportInfo.m_bLimit ) )
		{
			csvOut.AppendYesOrBlank( LoyaltyScheme.GetStaffCardFlag() );
			csvOut.AppendYesOrBlank( LoyaltyScheme.GetStaffCreditFlag() );
		}

		//SCHEME DETAIL
		CStringArray arrayDetail;
		if ( TRUE == m_reportInfo.m_bAction )
		{
			{
				CString strAction = "Unknown";

				switch( LoyaltyScheme.GetActionType() )
				{
				case LOYALTYSCHEME_ACTION_NONE:				strAction = "None";						break;
				case LOYALTYSCHEME_ACTION_PROMO_ITEM:		strAction = "Item Promo";				break;
				case LOYALTYSCHEME_ACTION_PROMO_SUBTOTAL:	strAction = "Subtotal Promo";			break;
				case LOYALTYSCHEME_ACTION_PRICEBAND:		strAction = "Price Band";				break;
				case LOYALTYSCHEME_ACTION_PROMO_MULTIPLE:	strAction = "Multi Subtotal Promos";	break;
				}

				csvOut.AppendString( strAction );
			}
		
			switch( LoyaltyScheme.GetActionType() )
			{
			case 1:
			case 2:
				arrayDetail.Add( GetPromoName( LoyaltyScheme.GetPromoNo(0) ) );
				break;

			case 3:
				{
					CString str;
					str.Format( "Price Band %d", LoyaltyScheme.GetPromoNo(0) + 1 );
					arrayDetail.Add( str);
				}
				break;

			case 4:
				for ( int n = 0; n < 5; n++ )
				{
					int nPromoNo = LoyaltyScheme.GetPromoNo(n);

					if ( nPromoNo != 0 )
					{
						arrayDetail.Add( GetPromoName( LoyaltyScheme.GetPromoNo(n) ) );
					}
				}

				if ( arrayDetail.GetSize() == 0 )
				{
					arrayDetail.Add( "None" );
				}

				break;

			case 0:
			default:
				arrayDetail.Add( "" );
				break;
			}

			csvOut.AppendString( arrayDetail.GetAt(0) );

			{
				CString str;
				int nMacroNo = LoyaltyScheme.GetMacroNo();

				if ( 0 == nMacroNo )
				{
					str = "None";
				}
				else
				{
					str.Format( "M%4.4d %s", nMacroNo, DataManagerNonDb.CCMacroActive.GetMacroNameFromNumber( nMacroNo ) );
				}

				csvOut.AppendString( str );
			}

			int nPence = LoyaltyScheme.GetPencePerPoint();
			if ( 0 == nPence )
			{	
				csvOut.AppendString( "" );
			}
			else
			{
				csvOut.AppendInt( nPence );
			}
		}

		//STAFF LIMITS
		if ( TRUE == m_reportInfo.m_bLimit )
		{
			csvOut.AppendInt( LoyaltyScheme.GetStaffLimitDay() );
			csvOut.AppendInt( LoyaltyScheme.GetStaffLimitWeek() );

			for ( int n = 0; n < 12; n++ )
			{
				csvOut.AppendInt( LoyaltyScheme.GetStaffLimitMonth(n) );
			}
		}

		//DENIED PAYMENTS
		CStringArray arrayPayment;
		if ( TRUE == m_reportInfo.m_bPayment )
		{
			{
				CCSV csvPay ( LoyaltyScheme.GetDeniedPaymentsForVPM() );

				//IGNORE TRAILING BLANK FOR VPM
				for ( int n = 1; n < csvPay.GetSize() - 1; n++ )
				{
					arrayPayment.Add( GetPaymentName( csvPay.GetInt(n) ) );
				}

				if ( arrayPayment.GetSize() == 0 )
				{
					arrayPayment.Add( "None" );
				}
			}

			csvOut.AppendString( arrayPayment.GetAt(0) );
		}
		
		ReportFile.WriteLine ( csvOut.GetLine() );

		//APPEND EXTRA PROMO AND PAYMENT DETAIL
		{
			int nLines = max( arrayDetail.GetSize(), arrayPayment.GetSize() );

			for ( int n = 1; n < nLines; n++ )
			{
				CCSVReportLine csvOut;
				
				if ( n < arrayDetail.GetSize() )
				{
					csvOut.SetAt( nActionPos, arrayDetail.GetAt(n) );
				}

				if ( n < arrayPayment.GetSize() )
				{
					csvOut.SetAt( nPaymentPos, arrayPayment.GetAt(n) );
				}

				ReportFile.WriteLine ( csvOut.GetLine() );
			}
		}

		if ( TRUE == m_reportInfo.m_bLines )
		{
			ReportFile.WriteLine( "<LI>" );
		}
	}
	ReportFile.Close();

	CString strKey;
	strKey.Format ( "%d%d%d%d",
		m_reportInfo.m_bRange,
		m_reportInfo.m_bAction,
		m_reportInfo.m_bLimit,
		m_reportInfo.m_bPayment );
	
	CReportManager ReportManager;
	CFilenameUpdater FnUp ( SysFiles::LoyaltySchemePrm );
	ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), strKey, this );
}

/**********************************************************************/

const char* CReportLoyaltySchemeDlg::GetPromoName( int nPromoNo )
{
	int nPromoIdx;

	if ( 0 == nPromoNo )
	{
		m_strPromoName = "None";
	}
	else if ( DataManager.Promotion.FindTableByNumber( nPromoNo, nPromoIdx ) == TRUE )
	{
		CPromotionsCSVRecord Promo;
		DataManager.Promotion.GetAt( nPromoIdx, Promo );
		m_strPromoName.Format ( "P%2.2d  %s", nPromoNo, Promo.GetDisplayName() );
	}
	else
	{
		m_strPromoName.Format ( "P%2.2d  Unknown", nPromoNo );
	}

	return m_strPromoName;
}

/**********************************************************************/

const char* CReportLoyaltySchemeDlg::GetPaymentName( int nPayNo )
{
	int nPayIdx;

	if ( DataManager.Payment.FindPaymentByNumber( nPayNo, nPayIdx ) == TRUE )
	{
		CPaymentCSVRecord Payment;
		DataManager.Payment.GetAt( nPayIdx, Payment );
		m_strPaymentName.Format ( "P%2.2d  %s", nPayNo, Payment.GetDisplayName() );
	}
	else
	{
		m_strPaymentName.Format ( "P%2.2d  Unknown", nPayNo );
	}

	return m_strPaymentName;
}

/**********************************************************************/

void CReportLoyaltySchemeDlg::OnCancel() 
{
	EndDialog ( IDCANCEL );
}

/**********************************************************************/
