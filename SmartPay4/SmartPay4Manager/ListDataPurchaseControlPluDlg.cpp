/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PluInfo\SQLRepositoryPluInfo.h"
/**********************************************************************/
#include "ListDataPurchaseControlPluDlg.h"
/**********************************************************************/

CSingleListContentHelpersPurchaseControlPlu::CSingleListContentHelpersPurchaseControlPlu(CReportConsolidationArray<CSortedStringAndIntByInt64>& arrayPlu)
	: CSingleListContentHelpers(), m_arrayPlu(arrayPlu)
{
}

/**********************************************************************/

int CSingleListContentHelpersPurchaseControlPlu::GetArraySize()
{
	return m_arrayPlu.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersPurchaseControlPlu::PrepareLine( int nIndex, CString& strLine )
{
	CSortedStringAndIntByInt64 item;
	m_arrayPlu.GetAt(nIndex, item);

	CString strPluNo = "";
	strPluNo.Format("%14.14I64d", item.m_nKey);

	CCSV csv('\t');
	csv.Add(strPluNo);
	csv.Add(item.m_strVal);
	csv.Add(item.m_nVal);
	
	strLine = csv.GetLine();

	return TRUE;
}

/**********************************************************************/

void CSingleListContentHelpersPurchaseControlPlu::DeleteDatabaseEntry( int nIndex )
{
}

/**********************************************************************/

void CSingleListContentHelpersPurchaseControlPlu::FilterSelectedItemArray( CReportConsolidationArray<CSortedIntItem>& arraySelected )
{
	m_bPreventDeletion = FALSE;
}

/**********************************************************************/

void CSingleListContentHelpersPurchaseControlPlu::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn(0, "Plu No", LVCFMT_LEFT, 120);
	list.InsertColumn(1, "Description", LVCFMT_LEFT, 300);
	list.InsertColumn(2, "Weight", LVCFMT_RIGHT, 60);
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataPurchaseControlPluDlg::CListDataPurchaseControlPluDlg( CSQLRowPurchaseControlRule& RowSO, CWnd* pParent) : CListDataDlg(pParent)
{
	m_strTitle.Format("Plu Items for Purchase Control %d: %s",
		RowSO.GetRuleID(),
		(const char*)RowSO.GetDisplayDescription());

	CSQLRepositoryPluInfo RepoPlu;
	RepoPlu.GetPluListByPCtrlRuleID(RowSO.GetRuleID(), m_arrayPlu);

	m_pContentHelpers = new CSingleListContentHelpersPurchaseControlPlu(m_arrayPlu);
	m_pDisplayHelpers = new CSingleListDisplayHelpers(m_listData, *m_pContentHelpers);
	
	m_pDisplayHelpers->SetTopSingleText("plu item");
	m_pDisplayHelpers->SetTopPluralText("plu items");
	m_pDisplayHelpers->SetBottomSingleText("plu item");
	m_pDisplayHelpers->SetBottomPluralText("plu items");
}

/**********************************************************************/

CListDataPurchaseControlPluDlg::~CListDataPurchaseControlPluDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataPurchaseControlPluDlg::SetTitle()
{
	SetWindowText ( m_strTitle );
}

/**********************************************************************/

void CListDataPurchaseControlPluDlg::SetButtonTexts()
{
	m_button1.ShowWindow(SW_HIDE);
	m_button2.ShowWindow(SW_HIDE);
	m_button3.ShowWindow(SW_HIDE);
	m_button4.ShowWindow(SW_HIDE);
	m_button5.ShowWindow(SW_HIDE);
	m_button6.ShowWindow(SW_HIDE);

	MoveControl(&m_button1, 368, 7);
	MoveControl(&m_button2, 368, 47);
	MoveControl(&m_button3, 368, 87);
	MoveControl(&m_button4, 368, 147);
	MoveControl(&m_button5, 368, 187);

	ShowAndEnableWindow(&m_buttonOK, FALSE);
	m_buttonCancel.SetWindowText("Close");
}

/**********************************************************************/

void CListDataPurchaseControlPluDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataPurchaseControlPluDlg::PrepareList()
{
	CListDataDlg::PrepareList();

	if (m_arrayPlu.GetSize() != 0)
	{
		SelectLine(0);
	}
}

/**********************************************************************/

bool CListDataPurchaseControlPluDlg::HandleOK()
{
	return TRUE;
}

/**********************************************************************/

bool CListDataPurchaseControlPluDlg::HandleCancel()
{
	return TRUE;
}

/**********************************************************************/

