/******************************************************************************/
#include "SmartPay4Manager.h"
/******************************************************************************/
#include "..\SmartPay4Shared\AuditReportTexts.h"
/******************************************************************************/
#include "AuditReportActionDlg.h"
/******************************************************************************/

CAuditReportActionDlg::CAuditReportActionDlg(CAuditReportConfig& reportconfig, CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CAuditReportActionDlg::IDD, pParent), m_reportconfig(reportconfig)
{
	m_bAdjustedRowHeight = FALSE;
	m_arrayActions.Add(APPNO_INSERT);
	m_arrayActions.Add(APPNO_EDIT);
	m_arrayActions.Add(APPNO_DELETE);
	m_arrayActions.Add(APPNO_BALANCE);
	m_arrayActions.Add(APPNO_SALE);
	m_arrayActions.Add(APPNO_REFUNDSALE);
	m_arrayActions.Add(APPNO_MEAL);
	m_arrayActions.Add(APPNO_ADDCASH);
	m_arrayActions.Add(APPNO_CREDIT);
	m_arrayActions.Add(APPNO_REFRESH);
	m_arrayActions.Add(APPNO_ADDPOINTS);
	m_arrayActions.Add(APPNO_REVALUEBONUSCASH);
	m_arrayActions.Add(APPNO_REVALUEBONUSPOINTS);
	m_arrayActions.Add(APPNO_REWARDBONUSCASH);	
	m_arrayActions.Add(APPNO_REWARDBONUSPOINTS);
	m_arrayActions.Add(APPNO_REFUNDREWARDBONUSCASH);
	m_arrayActions.Add(APPNO_REFUNDREWARDBONUSPOINTS);
	m_arrayActions.Add(APPNO_REDEEM);	
	m_arrayActions.Add(APPNO_BATCHUPDATE);
	m_arrayActions.Add(APPNO_BATCHDELETE);
	m_arrayActions.Add(APPNO_BATCHREDEEM);
	m_arrayActions.Add(APPNO_IMPORTINSERT);
	m_arrayActions.Add(APPNO_IMPORTUPDATE);
	m_arrayActions.Add(APPNO_IMPORTTOPUP);
	m_arrayActions.Add(APPNO_IMPORTHOTLIST);
	m_arrayActions.Add(APPNO_IMPORTREPLACE);
	m_arrayActions.Add(APPNO_IMPORTCUSTOM);
	m_arrayActions.Add(APPNO_IMPORTDELETE);
	m_arrayActions.Add(APPNO_HOTLIST);
	m_arrayActions.Add(APPNO_HOTLISTREPLACE);
	m_arrayActions.Add(APPNO_UNHOTLIST);
	m_arrayActions.Add(APPNO_XFEROUT);
	m_arrayActions.Add(APPNO_XFERIN);
	m_arrayActions.Add(APPNO_HOTXFEROUT);
	m_arrayActions.Add(APPNO_HOTXFERIN);
	m_arrayActions.Add(APPNO_PEBBLE_TOPUP);
	m_arrayActions.Add(APPNO_PEBBLE_TOPUP_REFUND);
	m_arrayActions.Add(APPNO_PEBBLE_ORDER);
	m_arrayActions.Add(APPNO_PEBBLE_ORDER_REFUND);
	m_arrayActions.Add(APPNO_BROMCOM);
	m_arrayActions.Add(APPNO_PARENTPAY);
	m_arrayActions.Add(APPNO_WISEPAY);
	m_arrayActions.Add(APPNO_TUCASI);
	m_arrayActions.Add(APPNO_INSERTRECORDWONDE);
	m_arrayActions.Add(APPNO_INSERTRECORDSIMS);
	m_arrayActions.Add(APPNO_INSERTRECORDPPAY);
	m_arrayActions.Add(APPNO_INSERTRECORDWISEPAY);
	m_arrayActions.Add(APPNO_SCHOOLCOMMS);
	m_arrayActions.Add(APPNO_TUCASIv2);
	m_arrayActions.Add(APPNO_SQUID);
	m_arrayActions.Add(APPNO_PARENTMAIL);
	m_arrayActions.Add(APPNO_WPOPENINGBALANCE);	  
	m_arrayActions.Add(APPNO_DELETELEAVER);
	m_arrayActions.Add(APPNO_BATCHLEAVER);
	m_arrayActions.Add(APPNO_IMPORTLEAVER);
	m_arrayActions.Add(APPNO_BIOREG);
	m_arrayActions.Add(APPNO_BIOUNREG);
	m_arrayActions.Add(APPNO_SO_AWARD);
	m_arrayActions.Add(APPNO_SO_MADD);
	m_arrayActions.Add(APPNO_SO_MEDIT);
	m_arrayActions.Add(APPNO_SO_MDEL);
	m_arrayActions.Add(APPNO_SO_XFER);
	m_arrayActions.Add(APPNO_SO_HOTL);
	m_arrayActions.Add(APPNO_SO_ADEL);
	m_arrayActions.Add(APPNO_SO_MDEL_EXPIRE);
	m_arrayActions.Add(APPNO_SO_MFIX_EXPIRE);
	m_arrayActions.Add(APPNO_SO_XDEL_EXPIRE);
}

/******************************************************************************/

CAuditReportActionDlg::~CAuditReportActionDlg()
{
}

/******************************************************************************/

void CAuditReportActionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

/******************************************************************************/

BEGIN_MESSAGE_MAP(CAuditReportActionDlg, CSSAutoShutdownDialog)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_NONE, OnButtonNone)
END_MESSAGE_MAP()

/******************************************************************************/

BOOL CAuditReportActionDlg::CSSAutoShutdownPostInitDialog()
{
	m_listAction.ClearRowHeightFlag();
	m_listAction.SetMultiColumnsFlag(FALSE);
	m_listAction.SubclassDlgItem(IDC_LIST_ACTION, this);
	m_listAction.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listAction.InsertColumn(0, "Action", LVCFMT_LEFT, 240);
	m_listAction.SetLockItemZeroFlag(FALSE);

	FillList();

	return TRUE;
}

/******************************************************************************/

void CAuditReportActionDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (nIDCtl == IDC_LIST_ACTION)
	{
		if (FALSE == m_bAdjustedRowHeight)
		{
			m_listAction.AdjustRowHeight(lpMeasureItemStruct, 10);
			m_bAdjustedRowHeight = TRUE;
		}
	}
}

//$$******************************************************************

void CAuditReportActionDlg::FillList()
{
	for (int n = 0; n < m_arrayActions.GetSize(); n++)
	{
		CSSListTaggedSelectItem itemPlu;
		int nAppNo = m_arrayActions[n];	
		itemPlu.m_strText = CAuditReportTexts::GetDefaultComment(nAppNo);
		itemPlu.SetTagState(m_reportconfig.GetShowActionFlag(nAppNo));
		m_listAction.AddItemToList(itemPlu);
	}
}

//$$******************************************************************

void CAuditReportActionDlg::OnOK()
{
	for (int n = 0; (n < m_listAction.GetItemCount()) && ( n < m_arrayActions.GetCount()); n++)
	{
		int nAppNo = m_arrayActions[n];
		m_reportconfig.SetShowActionFlag(nAppNo, m_listAction.GetItemTagState(n));
	}
	
	EndDialog(IDOK);
}

/******************************************************************************/
