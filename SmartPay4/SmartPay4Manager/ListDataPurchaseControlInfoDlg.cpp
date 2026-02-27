/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PluInfo\SQLRepositoryPluInfo.h"
/**********************************************************************/
#include "AutoShutdownStacker.h"
#include "ListDataPurchaseControlPluDlg.h"
#include "PurchaseControlStampDeleteDlg.h"
#include "PurchaseControlStampModifyDlg.h"
/**********************************************************************/
#include "ListDataPurchaseControlInfoDlg.h"
/**********************************************************************/

CSingleListContentHelpersPurchaseControlInfo::CSingleListContentHelpersPurchaseControlInfo(CSQLRowSetPurchaseControlInfo& RowSet)
	: CSingleListContentHelpers(), m_RowSet(RowSet)
{
}

/**********************************************************************/

int CSingleListContentHelpersPurchaseControlInfo::GetArraySize()
{
	return m_RowSet.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersPurchaseControlInfo::PrepareLine( int nIndex, CString& strLine )
{
	CSQLRowPurchaseControlRule RowSO;
	m_RowSet.GetRow(nIndex, RowSO);

	int nExpiry = RowSO.GetExpiryDays();
	int nPluCount = RowSO.GetPluCount();

	CCSV csv('\t');
	csv.Add(RowSO.GetRuleID());
	csv.Add(RowSO.GetDisplayDescription());
	csv.Add(RowSO.GetEnableFlag() ? "Yes" : "No");
	csv.Add(RowSO.GetThreshold());
	
	if (0 == nExpiry)
	{
		csv.Add("----");
	}
	else
	{
		csv.Add(nExpiry);
	}

	if (0 == nPluCount)
	{
		csv.Add("----");
	}
	else
	{
		csv.Add(nPluCount);
	}
	
	strLine = csv.GetLine();

	return TRUE;
}

/**********************************************************************/

void CSingleListContentHelpersPurchaseControlInfo::DeleteDatabaseEntry( int nIndex )
{
}

/**********************************************************************/

void CSingleListContentHelpersPurchaseControlInfo::FilterSelectedItemArray( CReportConsolidationArray<CSortedIntItem>& arraySelected )
{
	m_bPreventDeletion = FALSE;
}

/**********************************************************************/

void CSingleListContentHelpersPurchaseControlInfo::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn(0, "Control ID", LVCFMT_LEFT, 70);
	list.InsertColumn(1, "Control Name", LVCFMT_LEFT, 240);
	list.InsertColumn(2, "Enabled", LVCFMT_LEFT, 60);
	list.InsertColumn(3, "Threshold", LVCFMT_RIGHT, 80);
	list.InsertColumn(4, "Expiry", LVCFMT_RIGHT, 80);
	list.InsertColumn(5, "Plu Items", LVCFMT_RIGHT, 70);
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataPurchaseControlInfoDlg::CListDataPurchaseControlInfoDlg(CWnd* pParent) : CListDataDlg(pParent)
{
	m_RowSet.LoadDatabasePurchaseControlInfo();

	{
		CSQLRepositoryPluInfo RepoPlu;
		CReportConsolidationArray<CSortedIntByInt> arraySQLPluCount;
		RepoPlu.GetPluCountsByPCtrlRuleID(arraySQLPluCount);

		for (int n = 0; n < m_RowSet.GetSize(); n++)
		{
			CSQLRowPurchaseControlRule RowSO;
			m_RowSet.GetRow(n, RowSO);

			CSortedIntByInt item;
			item.m_nKey = RowSO.GetRuleID();

			int nPos = 0;
			int nPluCount = 0;
			if (arraySQLPluCount.Find(item, nPos) == TRUE)
			{
				arraySQLPluCount.GetAt(nPos, item);
				nPluCount = item.m_nVal;
			}

			RowSO.SetPluCount(nPluCount);
			m_RowSet.SetRow(n, RowSO);
		}
	}

	m_pContentHelpers = new CSingleListContentHelpersPurchaseControlInfo(m_RowSet);
	m_pDisplayHelpers = new CSingleListDisplayHelpers(m_listData, *m_pContentHelpers);
	
	m_pDisplayHelpers->SetTopSingleText("purchase control");
	m_pDisplayHelpers->SetTopPluralText("purchase controls");
	m_pDisplayHelpers->SetBottomSingleText("purchase control");
	m_pDisplayHelpers->SetBottomPluralText("purchase controls");
}

/**********************************************************************/

CListDataPurchaseControlInfoDlg::~CListDataPurchaseControlInfoDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataPurchaseControlInfoDlg::SetTitle()
{
	SetWindowText ( "View Purchase Control Details" );
}

/**********************************************************************/

void CListDataPurchaseControlInfoDlg::SetButtonTexts()
{
	m_button1.SetWindowText("&Plu\nItems");
	m_button2.ShowWindow(SW_HIDE);
	m_button3.SetWindowText("Delete\nExpired");
	m_button4.SetWindowText("Modify\nExpiry");
	m_button5.ShowWindow(SW_HIDE);
	m_button6.ShowWindow(SW_HIDE);

	ShowAndEnableWindow(&m_buttonOK, FALSE);
	m_buttonCancel.SetWindowText("Close");
}

/**********************************************************************/

void CListDataPurchaseControlInfoDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );

	ResizeDialog(500, 348);
	ResizeControl(&m_listData, 420, 334);
	MoveControl(&m_button1, 443, 7);
	MoveControl(&m_button2, 443, 42);
	MoveControl(&m_button3, 443, 140);
	MoveControl(&m_button4, 443, 185);
	MoveControl(&m_button5, 443, 172);
	MoveControl(&m_button6, 443, 207);
	MoveControl(&m_buttonOK, 443, 311);
	MoveControl(&m_buttonCancel, 443, 327);
}

/**********************************************************************/

void CListDataPurchaseControlInfoDlg::PrepareList()
{
	CListDataDlg::PrepareList();

	if (m_RowSet.GetSize() != 0)
	{
		SelectLine(0);
	}
}

/**********************************************************************/

void CListDataPurchaseControlInfoDlg::HandleButton1()
{
	int nIndex = GetSafeSelection();
	if ((nIndex >= 0) && (nIndex < m_RowSet.GetSize()))
	{
		CSQLRowPurchaseControlRule RowSO;
		m_RowSet.GetRow(nIndex, RowSO);

		CListDataPurchaseControlPluDlg dlgPlu(RowSO, this);
		dlgPlu.DoModal();
	}
}

/**********************************************************************/

void CListDataPurchaseControlInfoDlg::HandleButton3()
{
	int nIndex = GetSafeSelection();
	if ((nIndex >= 0) && (nIndex < m_RowSet.GetSize()))
	{
		CAutoShutdownDlgStacker AutoShutdownStacker(this);

		CSQLRowPurchaseControlRule RowSO;
		m_RowSet.GetRow(nIndex, RowSO);

		CPurchaseControlStampDeleteDlg dlgAdmin(TRUE, 0, RowSO.GetRuleID());
		dlgAdmin.DoModal();
	}
}

/**********************************************************************/

void CListDataPurchaseControlInfoDlg::HandleButton4()
{
	int nIndex = GetSafeSelection();
	if ((nIndex >= 0) && (nIndex < m_RowSet.GetSize()))
	{
		CAutoShutdownDlgStacker AutoShutdownStacker(this);

		CSQLRowPurchaseControlRule RowSO;
		m_RowSet.GetRow(nIndex, RowSO);

		CPurchaseControlStampModifyDlg dlgAdmin(TRUE, 0, RowSO.GetRuleID());
		dlgAdmin.DoModal();
	}
}

/**********************************************************************/

bool CListDataPurchaseControlInfoDlg::HandleOK()
{
	return TRUE;
}

/**********************************************************************/

bool CListDataPurchaseControlInfoDlg::HandleCancel()
{
	return TRUE;
}

/**********************************************************************/

