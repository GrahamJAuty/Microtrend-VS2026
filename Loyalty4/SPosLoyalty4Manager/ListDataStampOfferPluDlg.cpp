/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_PluInfo\SQLRepositoryPluInfo.h"
/**********************************************************************/
#include "ListDataStampOfferPluDlg.h"
/**********************************************************************/

CSingleListContentHelpersStampOfferPlu::CSingleListContentHelpersStampOfferPlu(CReportConsolidationArray<CSortedStringByIntAndInt64>& arrayPlu)
	: CSingleListContentHelpers(), m_arrayPlu(arrayPlu)
{
}

/**********************************************************************/

int CSingleListContentHelpersStampOfferPlu::GetArraySize()
{
	return m_arrayPlu.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersStampOfferPlu::PrepareLine( int nIndex, CString& strLine )
{
	CSortedStringByIntAndInt64 item;
	m_arrayPlu.GetAt(nIndex, item);

	CString strPluNo = "";
	strPluNo.Format("%14.14I64d", item.m_nInt64Key);

	CCSV csv('\t');
	csv.Add(item.m_nIntKey);
	csv.Add(strPluNo);
	csv.Add(item.m_strItem);
	
	strLine = csv.GetLine();

	return TRUE;
}

/**********************************************************************/

void CSingleListContentHelpersStampOfferPlu::DeleteDatabaseEntry( int nIndex )
{
}

/**********************************************************************/

void CSingleListContentHelpersStampOfferPlu::FilterSelectedItemArray( CReportConsolidationArray<CSortedIntItem>& arraySelected )
{
	m_bPreventDeletion = FALSE;
}

/**********************************************************************/

void CSingleListContentHelpersStampOfferPlu::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn(0, "DbNo", LVCFMT_LEFT, 60);
	list.InsertColumn(1, "Plu No", LVCFMT_LEFT, 120);
	list.InsertColumn(2, "Description", LVCFMT_LEFT, 300);
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataStampOfferPluDlg::CListDataStampOfferPluDlg( CSQLRowStampOfferInfo& RowSO, CWnd* pParent) : CListDataDlg(pParent)
{
	m_strTitle.Format("Plu Items for Stamp Offer %d: %s",
		RowSO.GetStampOfferID(),
		(const char*)RowSO.GetDisplayDescription());

	CSQLRepositoryPluInfo repoInfo;
	repoInfo.GetPluListByStampOfferID(RowSO.GetStampOfferID(), m_arrayPlu);

	m_pContentHelpers = new CSingleListContentHelpersStampOfferPlu(m_arrayPlu);
	m_pDisplayHelpers = new CSingleListDisplayHelpers(m_listData, *m_pContentHelpers);
	
	m_pDisplayHelpers->SetTopSingleText("plu item");
	m_pDisplayHelpers->SetTopPluralText("plu items");
	m_pDisplayHelpers->SetBottomSingleText("plu item");
	m_pDisplayHelpers->SetBottomPluralText("plu items");
}

/**********************************************************************/

CListDataStampOfferPluDlg::~CListDataStampOfferPluDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataStampOfferPluDlg::SetTitle()
{
	SetWindowText ( m_strTitle );
}

/**********************************************************************/

void CListDataStampOfferPluDlg::SetButtonTexts()
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

void CListDataStampOfferPluDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataStampOfferPluDlg::PrepareList()
{
	CListDataDlg::PrepareList();

	if (m_arrayPlu.GetSize() != 0)
	{
		SelectLine(0);
	}
}

/**********************************************************************/

bool CListDataStampOfferPluDlg::HandleOK()
{
	return TRUE;
}

/**********************************************************************/

bool CListDataStampOfferPluDlg::HandleCancel()
{
	return TRUE;
}

/**********************************************************************/

