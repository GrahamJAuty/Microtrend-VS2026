/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_PluInfo\SQLRepositoryPluInfo.h"
/**********************************************************************/
#include "ListDataStampOfferPluDlg.h"
#include "..\SPosLoyalty4Shared\ConsolidationMap.h"
#include "..\SPosLoyalty4Shared\ReportConsolidationMap.h"
#include "StampOfferDeleteDlg.h"
#include "StampOfferModifyDlg.h"
/**********************************************************************/
#include "ListDataStampOfferInfoDlg.h"
/**********************************************************************/

CSingleListContentHelpersStampOfferInfo::CSingleListContentHelpersStampOfferInfo(CSQLRowSetStampOfferInfo& RowSet)
	: CSingleListContentHelpers(), m_RowSet(RowSet)
{
}

/**********************************************************************/

int CSingleListContentHelpersStampOfferInfo::GetArraySize()
{
	return m_RowSet.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersStampOfferInfo::PrepareLine( int nIndex, CString& strLine )
{
	CSQLRowStampOfferInfo RowSO;
	m_RowSet.GetRow(nIndex, RowSO);

	int nExpiry = RowSO.GetExpiry();
	int nPluCount = RowSO.GetPluCount();

	CCSV csv('\t');
	csv.Add(RowSO.GetStampOfferID());
	csv.Add(RowSO.GetEdition());
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

void CSingleListContentHelpersStampOfferInfo::DeleteDatabaseEntry( int nIndex )
{
}

/**********************************************************************/

void CSingleListContentHelpersStampOfferInfo::FilterSelectedItemArray( CReportConsolidationArray<CSortedIntItem>& arraySelected )
{
	m_bPreventDeletion = FALSE;
}

/**********************************************************************/

void CSingleListContentHelpersStampOfferInfo::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn(0, "ID", LVCFMT_LEFT, 50);
	list.InsertColumn(1, "Edition", LVCFMT_LEFT, 50);
	list.InsertColumn(2, "Name", LVCFMT_LEFT, 220);
	list.InsertColumn(3, "Enabled", LVCFMT_LEFT, 60);
	list.InsertColumn(4, "Threshold", LVCFMT_RIGHT, 80);
	list.InsertColumn(5, "Expiry", LVCFMT_RIGHT, 80);
	list.InsertColumn(6, "Plu Items", LVCFMT_RIGHT, 60);
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataStampOfferInfoDlg::CListDataStampOfferInfoDlg(CWnd* pParent) : CListDataDlg(pParent)
{
	m_RowSet.LoadDatabaseStampOfferInfo();

	{
		CSQLRepositoryPluInfo repoInfo;
        CReportConsolidationMap<CMapKeyInt, CMapDataSortedInt> mapSQLPluCount;
		repoInfo.GetPluCountsByStampOfferID(mapSQLPluCount);

		for (int n = 0; n < m_RowSet.GetSize(); n++)
		{
			CSQLRowStampOfferInfo RowSO;
			m_RowSet.GetRow(n, RowSO);

			int nPluCount = 0;
			CMapKeyInt Key(RowSO.GetStampOfferID());
			
			auto it = mapSQLPluCount.Find(Key);
			if (it != mapSQLPluCount.end()) 
			{
				nPluCount = it->second.m_nValue;
			}
			
			RowSO.SetPluCount(nPluCount);
			m_RowSet.SetRow(n, RowSO);
		}
	}

	m_pContentHelpers = new CSingleListContentHelpersStampOfferInfo(m_RowSet);
	m_pDisplayHelpers = new CSingleListDisplayHelpers(m_listData, *m_pContentHelpers);
	
	m_pDisplayHelpers->SetTopSingleText("stamp offer");
	m_pDisplayHelpers->SetTopPluralText("stamp offers");
	m_pDisplayHelpers->SetBottomSingleText("stamp offer");
	m_pDisplayHelpers->SetBottomPluralText("stamp offers");
}

/**********************************************************************/

CListDataStampOfferInfoDlg::~CListDataStampOfferInfoDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataStampOfferInfoDlg::SetTitle()
{
	SetWindowText ( "View Stamp Offer Details" );
}

/**********************************************************************/

void CListDataStampOfferInfoDlg::SetButtonTexts()
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

void CListDataStampOfferInfoDlg::SetColumns()
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

void CListDataStampOfferInfoDlg::PrepareList()
{
	CListDataDlg::PrepareList();

	if (m_RowSet.GetSize() != 0)
	{
		SelectLine(0);
	}
}

/**********************************************************************/

void CListDataStampOfferInfoDlg::HandleButton1()
{
	int nIndex = GetSafeSelection();
	if ((nIndex >= 0) && (nIndex < m_RowSet.GetSize()))
	{
		CSQLRowStampOfferInfo RowSO;
		m_RowSet.GetRow(nIndex, RowSO);

		CListDataStampOfferPluDlg dlgPlu(RowSO, this);
		dlgPlu.DoModal();
	}
}

/**********************************************************************/

void CListDataStampOfferInfoDlg::HandleButton3()
{
	int nIndex = GetSafeSelection();
	if ((nIndex >= 0) && (nIndex < m_RowSet.GetSize()))
	{
		CSQLRowStampOfferInfo RowSO;
		m_RowSet.GetRow(nIndex, RowSO);

		CStampOfferDeleteDlg dlgAdmin(TRUE, 0, RowSO.GetStampOfferID());
		dlgAdmin.DoModal();
	}
}

/**********************************************************************/

void CListDataStampOfferInfoDlg::HandleButton4()
{
	int nIndex = GetSafeSelection();
	if ((nIndex >= 0) && (nIndex < m_RowSet.GetSize()))
	{
		CSQLRowStampOfferInfo RowSO;
		m_RowSet.GetRow(nIndex, RowSO);

		CStampOfferModifyDlg dlgAdmin(TRUE, 0, RowSO.GetStampOfferID());
		dlgAdmin.DoModal();
	}
}

/**********************************************************************/

bool CListDataStampOfferInfoDlg::HandleOK()
{
	return TRUE;
}

/**********************************************************************/

bool CListDataStampOfferInfoDlg::HandleCancel()
{
	return TRUE;
}

/**********************************************************************/

