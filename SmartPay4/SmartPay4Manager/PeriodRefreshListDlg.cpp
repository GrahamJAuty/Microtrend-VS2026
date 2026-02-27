//******************************************************************************
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
#include "..\SmartPay4Shared\SQLTable_PeriodRefresh\SQLRepositoryPeriodRefresh.h"
//******************************************************************************
#include "PeriodRefreshListDlg.h"
//******************************************************************************

CPeriodRefreshListDlg::CPeriodRefreshListDlg(CSQLRowAccountFull& SQLRowAccount, CSQLRowGroupFull& SQLRowGroup, CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CPeriodRefreshListDlg::IDD, pParent)
	, m_purse2(SQLRowAccount, SQLRowGroup)
{
	m_strBalancePeriod1 = "";
	m_strBalancePeriod2 = "";
	m_strBalancePeriod3 = "";
	m_strBalancePeriod4 = "";
	m_strBalancePeriod5 = "";
	m_strBalancePeriod6 = "";

	m_strLastRefreshPeriod1 = "";
	m_strLastRefreshPeriod2 = "";
	m_strLastRefreshPeriod3 = "";
	m_strLastRefreshPeriod4 = "";
	m_strLastRefreshPeriod5 = "";
	m_strLastRefreshPeriod6 = "";

	SQLRowSetAuditPeriod.LoadPeriods();

	int nCurrentIndex = SQLRowSetAuditPeriod.GetPeriodIndex();					// get current period index
	m_strPeriod1 = GetDisplayPeriodNo(0, nCurrentIndex);
	m_strPeriod2 = GetDisplayPeriodNo(1, nCurrentIndex);
	m_strPeriod3 = GetDisplayPeriodNo(2, nCurrentIndex);
	m_strPeriod4 = GetDisplayPeriodNo(3, nCurrentIndex);
	m_strPeriod5 = GetDisplayPeriodNo(4, nCurrentIndex);
	m_strPeriod6 = GetDisplayPeriodNo(5, nCurrentIndex);

	m_strPeriodText1 = SQLRowSetAuditPeriod.GetPeriodName(0);
	m_strPeriodText2 = SQLRowSetAuditPeriod.GetPeriodName(1);
	m_strPeriodText3 = SQLRowSetAuditPeriod.GetPeriodName(2);
	m_strPeriodText4 = SQLRowSetAuditPeriod.GetPeriodName(3);
	m_strPeriodText5 = SQLRowSetAuditPeriod.GetPeriodName(4);
	m_strPeriodText6 = SQLRowSetAuditPeriod.GetPeriodName(5);

	m_strRefreshTypePeriod1 = SQLRowGroup.GetRefreshPeriodTypeText(0);
	m_strRefreshTypePeriod2 = SQLRowGroup.GetRefreshPeriodTypeText(1);
	m_strRefreshTypePeriod3 = SQLRowGroup.GetRefreshPeriodTypeText(2);
	m_strRefreshTypePeriod4 = SQLRowGroup.GetRefreshPeriodTypeText(3);
	m_strRefreshTypePeriod5 = SQLRowGroup.GetRefreshPeriodTypeText(4);
	m_strRefreshTypePeriod6 = SQLRowGroup.GetRefreshPeriodTypeText(5);

	m_strStartPeriod1 = SQLRowSetAuditPeriod.FormatStartTime(0);
	m_strStartPeriod2 = SQLRowSetAuditPeriod.FormatStartTime(1);
	m_strStartPeriod3 = SQLRowSetAuditPeriod.FormatStartTime(2);
	m_strStartPeriod4 = SQLRowSetAuditPeriod.FormatStartTime(3);
	m_strStartPeriod5 = SQLRowSetAuditPeriod.FormatStartTime(4);
	m_strStartPeriod6 = SQLRowSetAuditPeriod.FormatStartTime(5);

	m_dRefreshPeriod1 = SQLRowGroup.GetRefreshValueByPeriod(0);
	m_dRefreshPeriod2 = SQLRowGroup.GetRefreshValueByPeriod(1);
	m_dRefreshPeriod3 = SQLRowGroup.GetRefreshValueByPeriod(2);
	m_dRefreshPeriod4 = SQLRowGroup.GetRefreshValueByPeriod(3);
	m_dRefreshPeriod5 = SQLRowGroup.GetRefreshValueByPeriod(4);
	m_dRefreshPeriod6 = SQLRowGroup.GetRefreshValueByPeriod(5);

	CString strDate = "";
	m_purse2.GetBalance(FALSE);										// initalise purse2 dateToday etc

	CSQLRowPeriodRefresh RowPeriodRefresh;
	RowPeriodRefresh.SetUserID(SQLRowAccount.GetUserID());

	CSQLRepositoryPeriodRefresh RepoPeriod;
	bool bExists = (RepoPeriod.SelectRow(RowPeriodRefresh, NULL).GetSQLError() == SQLCRUD_ERR_NONE);

	strDate = "";
	if (TRUE == bExists)
	{
		strDate = RowPeriodRefresh.GetDate(0);
	}

	m_strLastRefreshPeriod1 = strDate;
	if (m_purse2.CheckRefreshReqd(SQLRowGroup.GetRefreshTypeByPeriod(0), strDate, TRUE) == TRUE)
	{
		m_strBalancePeriod1 = ::FormatDoubleValue(m_dRefreshPeriod1);
	}
	else
	{
		m_strBalancePeriod1 = GetDisplayBalance(strDate, RowPeriodRefresh.GetBalance(0));
	}

	strDate = "";
	if ( TRUE == bExists )
	{
		strDate = RowPeriodRefresh.GetDate(1);
	}

	m_strLastRefreshPeriod2 = strDate;
	if (m_purse2.CheckRefreshReqd(SQLRowGroup.GetRefreshTypeByPeriod(1), strDate, TRUE) == TRUE)
	{
		m_strBalancePeriod2 = ::FormatDoubleValue(m_dRefreshPeriod2);
	}
	else
	{
		m_strBalancePeriod2 = GetDisplayBalance(strDate, RowPeriodRefresh.GetBalance(1));
	}

	strDate = "";
	if (TRUE == bExists)
	{
		strDate = RowPeriodRefresh.GetDate(2);
	}

	m_strLastRefreshPeriod3 = strDate;
	if (m_purse2.CheckRefreshReqd(SQLRowGroup.GetRefreshTypeByPeriod(2), strDate, TRUE) == TRUE)
	{
		m_strBalancePeriod3 = ::FormatDoubleValue(m_dRefreshPeriod3);
	}
	else
	{
		m_strBalancePeriod3 = GetDisplayBalance(strDate, RowPeriodRefresh.GetBalance(2));
	}

	strDate = "";
	if (TRUE == bExists)
	{
		strDate = RowPeriodRefresh.GetDate(3);
	}

	m_strLastRefreshPeriod4 = strDate;
	if (m_purse2.CheckRefreshReqd(SQLRowGroup.GetRefreshTypeByPeriod(3), strDate, TRUE) == TRUE)
	{
		m_strBalancePeriod4 = ::FormatDoubleValue(m_dRefreshPeriod4);
	}
	else
	{
		m_strBalancePeriod4 = GetDisplayBalance(strDate, RowPeriodRefresh.GetBalance(3));
	}

	strDate = "";
	if (TRUE == bExists)
	{
		strDate = RowPeriodRefresh.GetDate(4);
	}

	m_strLastRefreshPeriod5 = strDate;
	if (m_purse2.CheckRefreshReqd(SQLRowGroup.GetRefreshTypeByPeriod(4), strDate, TRUE) == TRUE)
	{
		m_strBalancePeriod5 = ::FormatDoubleValue(m_dRefreshPeriod5);
	}
	else
	{
		m_strBalancePeriod5 = GetDisplayBalance(strDate, RowPeriodRefresh.GetBalance(4));
	}

	strDate = "";
	if (TRUE == bExists)
	{
		strDate = RowPeriodRefresh.GetDate(5);
	}

	m_strLastRefreshPeriod6 = strDate;
	if (m_purse2.CheckRefreshReqd(SQLRowGroup.GetRefreshTypeByPeriod(5), strDate, TRUE) == TRUE)
	{
		m_strBalancePeriod6 = ::FormatDoubleValue(m_dRefreshPeriod6);
	}
	else
	{
		m_strBalancePeriod6 = GetDisplayBalance(strDate, RowPeriodRefresh.GetBalance(5));
	}
}

//******************************************************************************

CPeriodRefreshListDlg::~CPeriodRefreshListDlg()
{
}

//******************************************************************************

void CPeriodRefreshListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PERIODTEXT1, m_strPeriodText1);
	DDX_Text(pDX, IDC_EDIT_PERIODTEXT2, m_strPeriodText2);
	DDX_Text(pDX, IDC_EDIT_PERIODTEXT3, m_strPeriodText3);
	DDX_Text(pDX, IDC_EDIT_PERIODTEXT4, m_strPeriodText4);
	DDX_Text(pDX, IDC_EDIT_PERIODTEXT5, m_strPeriodText5);
	DDX_Text(pDX, IDC_EDIT_PERIODTEXT6, m_strPeriodText6);
	DDX_Text(pDX, IDC_EDIT_REFRESH1, m_dRefreshPeriod1);
	DDX_Text(pDX, IDC_EDIT_REFRESH2, m_dRefreshPeriod2);
	DDX_Text(pDX, IDC_EDIT_REFRESH3, m_dRefreshPeriod3);
	DDX_Text(pDX, IDC_EDIT_REFRESH4, m_dRefreshPeriod4);
	DDX_Text(pDX, IDC_EDIT_REFRESH5, m_dRefreshPeriod5);
	DDX_Text(pDX, IDC_EDIT_REFRESH6, m_dRefreshPeriod6);
	DDX_Text(pDX, IDC_STATIC_PERIODTYPE1, m_strRefreshTypePeriod1);
	DDX_Text(pDX, IDC_STATIC_PERIODTYPE2, m_strRefreshTypePeriod2);
	DDX_Text(pDX, IDC_STATIC_PERIODTYPE3, m_strRefreshTypePeriod3);
	DDX_Text(pDX, IDC_STATIC_PERIODTYPE4, m_strRefreshTypePeriod4);
	DDX_Text(pDX, IDC_STATIC_PERIODTYPE5, m_strRefreshTypePeriod5);
	DDX_Text(pDX, IDC_STATIC_PERIODTYPE6, m_strRefreshTypePeriod6);
	DDX_Text(pDX, IDC_EDIT_BALANCE1, m_strBalancePeriod1);
	DDX_Text(pDX, IDC_EDIT_BALANCE2, m_strBalancePeriod2);
	DDX_Text(pDX, IDC_EDIT_BALANCE3, m_strBalancePeriod3);
	DDX_Text(pDX, IDC_EDIT_BALANCE4, m_strBalancePeriod4);
	DDX_Text(pDX, IDC_EDIT_BALANCE5, m_strBalancePeriod5);
	DDX_Text(pDX, IDC_EDIT_BALANCE6, m_strBalancePeriod6);
	DDX_Text(pDX, IDC_STATIC_REFRESHED1, m_strLastRefreshPeriod1);
	DDX_Text(pDX, IDC_STATIC_REFRESHED2, m_strLastRefreshPeriod2);
	DDX_Text(pDX, IDC_STATIC_REFRESHED3, m_strLastRefreshPeriod3);
	DDX_Text(pDX, IDC_STATIC_REFRESHED4, m_strLastRefreshPeriod4);
	DDX_Text(pDX, IDC_STATIC_REFRESHED5, m_strLastRefreshPeriod5);
	DDX_Text(pDX, IDC_STATIC_REFRESHED6, m_strLastRefreshPeriod6);
	DDX_Text(pDX, IDC_STATIC_STARTPERIOD1, m_strStartPeriod1);
	DDX_Text(pDX, IDC_STATIC_STARTPERIOD2, m_strStartPeriod2);
	DDX_Text(pDX, IDC_STATIC_STARTPERIOD3, m_strStartPeriod3);
	DDX_Text(pDX, IDC_STATIC_STARTPERIOD4, m_strStartPeriod4);
	DDX_Text(pDX, IDC_STATIC_STARTPERIOD5, m_strStartPeriod5);
	DDX_Text(pDX, IDC_STATIC_STARTPERIOD6, m_strStartPeriod6);
	DDX_Text(pDX, IDC_STATIC_PERIOD1, m_strPeriod1);
	DDX_Text(pDX, IDC_STATIC_PERIOD2, m_strPeriod2);
	DDX_Text(pDX, IDC_STATIC_PERIOD3, m_strPeriod3);
	DDX_Text(pDX, IDC_STATIC_PERIOD4, m_strPeriod4);
	DDX_Text(pDX, IDC_STATIC_PERIOD5, m_strPeriod5);
	DDX_Text(pDX, IDC_STATIC_PERIOD6, m_strPeriod6);
}

//******************************************************************************

BEGIN_MESSAGE_MAP(CPeriodRefreshListDlg, CSSAutoShutdownDialog)
END_MESSAGE_MAP()

//******************************************************************************
// CPeriodRefreshListDlg message handlers

BOOL CPeriodRefreshListDlg::CSSAutoShutdownPostInitDialog()
{
	SubclassEdit ( IDC_EDIT_REFRESH1,	SS_NUM_SDP, 11, "%.2f" ); 
	SubclassEdit ( IDC_EDIT_REFRESH2,	SS_NUM_SDP, 11, "%.2f" ); 
	SubclassEdit ( IDC_EDIT_REFRESH3,	SS_NUM_SDP, 11, "%.2f" ); 
	SubclassEdit ( IDC_EDIT_REFRESH4,	SS_NUM_SDP, 11, "%.2f" ); 
	SubclassEdit ( IDC_EDIT_REFRESH5,	SS_NUM_SDP, 11, "%.2f" ); 
	SubclassEdit ( IDC_EDIT_REFRESH6,	SS_NUM_SDP, 11, "%.2f" ); 

	return TRUE;
}

//******************************************************************************

CString CPeriodRefreshListDlg::GetDisplayBalance ( const char* szDate, double dValue )
{
	CString strReply = "";

	if (lstrlen(szDate) != 0)
	{
		strReply.Format("%.2lf", dValue);
	}

	return strReply;
}

//******************************************************************************

CString CPeriodRefreshListDlg::GetDisplayPeriodNo ( int nIndex, int nCurrentIndex )
{
	CString strReply = "";
	strReply.Format ( "%d", nIndex+1 );
	
	if (nIndex == nCurrentIndex)
	{
		strReply += "  ->";
	}

	return strReply;
}

//******************************************************************************
