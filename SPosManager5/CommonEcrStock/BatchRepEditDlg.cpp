/**********************************************************************/
#include "EposReports.h"
#include "MaxLengths.h"
#include "ReportTypes.h"
#include "SysInfo.h"
/**********************************************************************/
#include "BatchRepEditDlg.h"
/**********************************************************************/

CBatchRepEditDlg::CBatchRepEditDlg(CPresetReportInfo& BatchRecord, CWnd* pParent)
	: CDialog(CBatchRepEditDlg::IDD, pParent), m_Base(BatchRecord)
{
	//{{AFX_DATA_INIT(CBatchRepEditDlg)
	m_strName = _T("");
	//}}AFX_DATA_INIT

	{
		CString str = m_Base.GetOtherFields();
		m_Preset.FromString(str);
	}

	bool bModifier = (SysInfo.GetMaxBasePluLen() != 0);
	bool bReason = (EcrmanOptions.GetReportsEODReasonsFlag() == TRUE);
	bool bVIP = (EcrmanOptions.GetReportsVIPType() != 0);
	m_nActionType = m_Preset.GetActionType();

	if (SysInfo.IsEcrLiteSystem() == TRUE)
	{
		m_arrayReportTypes.Add(REPORT_TYPE_PLUSALES);
		m_arrayReportTypes.Add(REPORT_TYPE_PLUSALES_DEPT_ITEM);

		if (TRUE == bModifier)
		{
			m_arrayReportTypes.Add(REPORT_TYPE_PLUSALES_MODIFIER);
		}

		m_arrayReportTypes.Add(REPORT_TYPE_PLUSALES_DEPT_SUMMARY);
		m_arrayReportTypes.Add(REPORT_TYPE_HOURLY);
		m_arrayReportTypes.Add(REPORT_TYPE_COVERS_DAILY);
		m_arrayReportTypes.Add(REPORT_TYPE_PAYMENT_SUMMARY);
		m_arrayReportTypes.Add(REPORT_TYPE_TRANSACTION_NORMAL);
		m_arrayReportTypes.Add(REPORT_TYPE_TRANSACTION_DISCREPANCY);
	}
	else
	{
		m_arrayReportTypes.Add(REPORT_TYPE_PLUSALES);
		m_arrayReportTypes.Add(REPORT_TYPE_PLUSALES_NOZEROSKIP);
		m_arrayReportTypes.Add(REPORT_TYPE_PLUSALES_DEPT_ITEM);
		m_arrayReportTypes.Add(REPORT_TYPE_PLUSALES_TAX_ITEM);
		m_arrayReportTypes.Add(REPORT_TYPE_PLUSALES_TRANSERVER);
		m_arrayReportTypes.Add(REPORT_TYPE_COVERS_PLU_TRANSERVER);
		m_arrayReportTypes.Add(REPORT_TYPE_PLUSALES_CUSTOMER);

		if (TRUE == bModifier)
		{
			m_arrayReportTypes.Add(REPORT_TYPE_PLUSALES_MODIFIER);
		}

		m_arrayReportTypes.Add(REPORT_TYPE_PLUSALES_DEPT_SUMMARY);
		m_arrayReportTypes.Add(REPORT_TYPE_TAX_SUMMARY_TAX);
		m_arrayReportTypes.Add(REPORT_TYPE_TAX_SUMMARY_PRICE);

		if (DealerFlags.GetCascadeReportFlag() == TRUE)
		{
			m_arrayReportTypes.Add(REPORT_TYPE_SPTBOOK_CASCADE);
		}

		if (TRUE == bVIP)
		{
			m_arrayReportTypes.Add(REPORT_TYPE_VIPSALES);
			m_arrayReportTypes.Add(REPORT_TYPE_VIPSALES_DEPT_ITEM);
			m_arrayReportTypes.Add(REPORT_TYPE_VIPSALES_TRANSERVER);

			if (TRUE == bModifier)
			{
				m_arrayReportTypes.Add(REPORT_TYPE_VIPSALES_MODIFIER);
			}

			m_arrayReportTypes.Add(REPORT_TYPE_VIPSALES_DEPT_SUMMARY);
		}

		m_arrayReportTypes.Add(REPORT_TYPE_PLUWASTE);
		m_arrayReportTypes.Add(REPORT_TYPE_PLUWASTE_DEPT_ITEM);
		m_arrayReportTypes.Add(REPORT_TYPE_PLUWASTE_TRANSERVER);

		if (TRUE == bModifier)
		{
			m_arrayReportTypes.Add(REPORT_TYPE_PLUWASTE_MODIFIER);
		}

		m_arrayReportTypes.Add(REPORT_TYPE_PLUWASTE_DEPT_SUMMARY);

		m_arrayReportTypes.Add(REPORT_TYPE_HOURLY);
		m_arrayReportTypes.Add(REPORT_TYPE_HOURLY_GROUP);
		m_arrayReportTypes.Add(REPORT_TYPE_HOURLY_DEPT);
		m_arrayReportTypes.Add(REPORT_TYPE_DAILY);
		m_arrayReportTypes.Add(REPORT_TYPE_DAILY_GROUP);
		m_arrayReportTypes.Add(REPORT_TYPE_DAILY_DEPT);
		m_arrayReportTypes.Add(REPORT_TYPE_COVERS_GROUP);
		m_arrayReportTypes.Add(REPORT_TYPE_COVERS_DEPT);
		m_arrayReportTypes.Add(REPORT_TYPE_COVERS_HOURLY);
		m_arrayReportTypes.Add(REPORT_TYPE_COVERS_HOURLY_GROUP);
		m_arrayReportTypes.Add(REPORT_TYPE_COVERS_HOURLY_DEPT);
		m_arrayReportTypes.Add(REPORT_TYPE_COVERS_DAILY);
		m_arrayReportTypes.Add(REPORT_TYPE_COVERS_DAILY_GROUP);
		m_arrayReportTypes.Add(REPORT_TYPE_COVERS_DAILY_DEPT);
		m_arrayReportTypes.Add(REPORT_TYPE_COVERS_MONTHLY);
		m_arrayReportTypes.Add(REPORT_TYPE_COVERS_PRICELEVEL);
		m_arrayReportTypes.Add(REPORT_TYPE_COVERS_PAYMENT);
		m_arrayReportTypes.Add(REPORT_TYPE_TRANSERVER_SUMMARY);
		m_arrayReportTypes.Add(REPORT_TYPE_GENERAL_GROUP);
		m_arrayReportTypes.Add(REPORT_TYPE_GENERAL_DEPT);
		m_arrayReportTypes.Add(REPORT_TYPE_GENERAL_GROUP_QTY);
		m_arrayReportTypes.Add(REPORT_TYPE_GENERAL_DEPT_QTY);
		m_arrayReportTypes.Add(REPORT_TYPE_WEEKLY_GROUP);
		m_arrayReportTypes.Add(REPORT_TYPE_WEEKLY_DEPT);
		m_arrayReportTypes.Add(REPORT_TYPE_WEEKLY_GROUP_QTY);
		m_arrayReportTypes.Add(REPORT_TYPE_WEEKLY_DEPT_QTY);
		m_arrayReportTypes.Add(REPORT_TYPE_PAYMENT_SUMMARY);
		m_arrayReportTypes.Add(REPORT_TYPE_PAYMENT_SERVER);
		m_arrayReportTypes.Add(REPORT_TYPE_PAYMENT_GROUP_SUMMARY);
		m_arrayReportTypes.Add(REPORT_TYPE_PAYMENT_ESTIMATE_GROUP);
		m_arrayReportTypes.Add(REPORT_TYPE_PAYMENT_ESTIMATE_DEPT);
		m_arrayReportTypes.Add(REPORT_TYPE_PAYMENT_ESTIMATE_ITEM);
		m_arrayReportTypes.Add(REPORT_TYPE_PAYMENT_ESTIMATE_TAX);
		m_arrayReportTypes.Add(REPORT_TYPE_PAYMENT_DETAIL);
		m_arrayReportTypes.Add(REPORT_TYPE_DEPOSIT_PURCHASED);
		m_arrayReportTypes.Add(REPORT_TYPE_DEPOSIT_REDEEMED);
		m_arrayReportTypes.Add(REPORT_TYPE_DEPOSIT_UNREDEEMED);
		m_arrayReportTypes.Add(REPORT_TYPE_DEPOSIT_BOOKING_PURCHASE);
		m_arrayReportTypes.Add(REPORT_TYPE_DEPOSIT_BOOKING_EVENT);
		m_arrayReportTypes.Add(REPORT_TYPE_CUSTOMER_RA);
		m_arrayReportTypes.Add(REPORT_TYPE_CUSTOMER_SPEND);
		m_arrayReportTypes.Add(REPORT_TYPE_CUSTOMER_ACTIVITY);
		m_arrayReportTypes.Add(REPORT_TYPE_CUSTOMER_BALANCE);
		m_arrayReportTypes.Add(REPORT_TYPE_PROMOTION_SUMMARY);
		m_arrayReportTypes.Add(REPORT_TYPE_MIXMATCH_GROUP);
		m_arrayReportTypes.Add(REPORT_TYPE_MIXMATCH_DEPT);
		m_arrayReportTypes.Add(REPORT_TYPE_MIXMATCH_ITEM);
		m_arrayReportTypes.Add(REPORT_TYPE_MIXMATCH_GROUP_COVERS);
		m_arrayReportTypes.Add(REPORT_TYPE_MIXMATCH_DEPT_COVERS);
		m_arrayReportTypes.Add(REPORT_TYPE_MIXMATCH_ITEM_COVERS);
		m_arrayReportTypes.Add(REPORT_TYPE_CHEAPESTONEFREE_SUMMARY);
		m_arrayReportTypes.Add(REPORT_TYPE_CHEAPESTONEFREE_DETAIL);
		m_arrayReportTypes.Add(REPORT_TYPE_TRANSERVER_VOID);
		m_arrayReportTypes.Add(REPORT_TYPE_TRANSERVER_VOID_REASON);
		m_arrayReportTypes.Add(REPORT_TYPE_TRANSERVER_REFUND);
		m_arrayReportTypes.Add(REPORT_TYPE_TRANSERVER_REFUND_REASON);
		m_arrayReportTypes.Add(REPORT_TYPE_TRANSERVER_DISCOUNT);
		m_arrayReportTypes.Add(REPORT_TYPE_TRANSERVER_DISCOUNT_REASON);
		m_arrayReportTypes.Add(REPORT_TYPE_NOSALE);
		m_arrayReportTypes.Add(REPORT_TYPE_AGEVERIFY);
		m_arrayReportTypes.Add(REPORT_TYPE_EODSTATUS);

		if (TRUE == bReason)
		{
			m_arrayReportTypes.Add(REPORT_TYPE_REASON_VOID);
			m_arrayReportTypes.Add(REPORT_TYPE_REASON_REFUND);
			m_arrayReportTypes.Add(REPORT_TYPE_REASON_PROMOTION);
			m_arrayReportTypes.Add(REPORT_TYPE_REASON_WASTAGE);

			if (TRUE == bVIP)
			{
				m_arrayReportTypes.Add(REPORT_TYPE_REASON_VIP);
			}
		}

		m_arrayReportTypes.Add(REPORT_TYPE_TRANSACTION_NORMAL);
		m_arrayReportTypes.Add(REPORT_TYPE_TRANSACTION_DISCREPANCY);
	}

	m_EposReportSelect.LoadReportSelection();

	for (int nIndex = 0; nIndex < m_EposReportSelect.GetSelectedReportCount(); nIndex++)
	{
		int nReportType = m_EposReportSelect.GetSelectedReportType(nIndex);
		if (nReportType >= EPOS_CUSTOM_FIRST_REPORT)
		{
			bool bAcceptType = FALSE;

			CEposReportSelectInfo infoReport;
			infoReport.SetReportType(nReportType);
			m_EposReportSelect.GetReportInfo(infoReport);

			switch (infoReport.GetCustomReportFamily())
			{
			case EPOS_CUSTOM_FAMILY_HOURLY:
			case EPOS_CUSTOM_FAMILY_TRANSACTION:
				bAcceptType = TRUE;
				break;

			case EPOS_CUSTOM_FAMILY_GENERAL:
			case EPOS_CUSTOM_FAMILY_DEPOSIT:
			case EPOS_CUSTOM_FAMILY_MIXMATCH:
			case EPOS_CUSTOM_FAMILY_DISCOUNT:
			case EPOS_CUSTOM_FAMILY_HOURLYGRP:
			case EPOS_CUSTOM_FAMILY_HOURLYDPT:
			case EPOS_CUSTOM_FAMILY_COVERS_HOURLY:
			case EPOS_CUSTOM_FAMILY_COVERS_HOURLYGRP:
			case EPOS_CUSTOM_FAMILY_COVERS_HOURLYDPT:
			case EPOS_CUSTOM_FAMILY_LOYALTYRECON:
			case EPOS_CUSTOM_FAMILY_PAYMENTDETAIL:
			case EPOS_CUSTOM_FAMILY_PLU_PRICEBAND:
				bAcceptType = (SysInfo.IsEcrLiteSystem() == FALSE);
				break;
			}

			if (TRUE == bAcceptType)
			{
				m_arrayReportTypes.Add(nReportType);
			}
		}
	}
}

/**********************************************************************/

void CBatchRepEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBatchRepEditDlg)
	DDX_Control(pDX, IDC_CHECK_LOCATIONS, m_checkLocations);
	DDX_Control(pDX, IDC_CHECK_TERMINALS, m_checkTerminals);
	DDX_Control(pDX, IDC_CHECK_ALL, m_checkAll);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, MAX_LENGTH_GENERAL_NAME);
	DDX_Radio(pDX, IDC_RADIO_ACTION, m_nActionType);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CBatchRepEditDlg, CDialog)
	//{{AFX_MSG_MAP(CBatchRepEditDlg)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_ALLREPORT, OnButtonAllReport)
	ON_BN_CLICKED(IDC_BUTTON_NONEREPORT, OnButtonNoneReport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CBatchRepEditDlg::OnInitDialog() 
{
	m_strName = m_Base.GetName();
	CDialog::OnInitDialog();

	m_listReports.SubclassDlgItem ( IDC_LIST, this );
	m_listReports.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listReports.InsertColumn ( 0, "Reports", LVCFMT_LEFT, 310 );
	m_listReports.SetLockItemZeroFlag( FALSE );

	GetRecordData();
	AddList();
	return TRUE;  
}

/**********************************************************************/

void CBatchRepEditDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )
   {
	   m_listReports.AdjustRowHeight( lpMeasureItemStruct, 10 );
   }
}

/**********************************************************************/

void CBatchRepEditDlg::GetRecordData()
{
	m_checkAll.SetCheck ( m_Preset.GetReportAllLocationsFlag() );
	m_checkTerminals.SetCheck ( m_Preset.GetReportByTerminalFlag() );
	m_checkLocations.SetCheck ( m_Preset.GetReportByLocationFlag() );
}

/**********************************************************************/

void CBatchRepEditDlg::OnButtonAllReport() 
{	
	m_listReports.SelectAll();
}

/**********************************************************************/

void CBatchRepEditDlg::OnButtonNoneReport() 
{	
	m_listReports.SelectNone();
}

/**********************************************************************/

void CBatchRepEditDlg::AddList()
{
	for ( int n = 0; n < m_arrayReportTypes.GetSize(); n++ )
	{
		int nReportType = m_arrayReportTypes.GetAt(n);

		CSSListTaggedSelectItem item;
		
		if (nReportType < EPOS_CUSTOM_FIRST_REPORT)
		{
			item.m_strText = CEposReport::GetReportNameExternal(nReportType);
		}
		else
		{
			item.m_strText = m_EposReportSelect.GetCustomReportName(nReportType);
		}

		item.m_nData = nReportType;
		item.SetTagState( m_Preset.GetReportFlag ( nReportType ) ? 1 : 0 );

		m_listReports.AddItemToList( item );
	}
}

/**********************************************************************/

void CBatchRepEditDlg::OnOK()
{
	if (UpdateData(TRUE) == TRUE)
	{
		::TrimSpaces(m_strName, FALSE);

		if (m_strName == "")
		{
			m_strName = "BATCH REPORTS";
		}

		m_Base.SetName(m_strName);
		m_Preset.SetReportAllLocationsFlag(m_checkAll.GetCheck() != 0);
		m_Preset.SetReportByTerminalFlag(m_checkTerminals.GetCheck() != 0);
		m_Preset.SetReportByLocationFlag(m_checkLocations.GetCheck() != 0);
		m_Preset.SetActionType(m_nActionType);

		for (int nPos = 0; nPos < m_listReports.GetItemCount(); nPos++)
		{
			CSSListTaggedSelectItem ListItem;
			m_listReports.GetListItem(nPos, ListItem);
			m_Preset.SetReportFlag(ListItem.m_nData, (ListItem.GetTagState() == 1));
		}

		CString str;
		m_Preset.ToString(str);
		m_Base.SetOtherFields(str);

		EndDialog(IDOK);
	}
}

/**********************************************************************/
