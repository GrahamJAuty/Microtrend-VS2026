/**********************************************************************/
#include "AccumulatorPropertySheet.h"
#include "ColumnHandler.h"
#include "ReportDefaultHandler.h"
#include "SmartPay4Manager.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLDb.h"
#include "..\SmartPay4Shared\SQLTable_PluInfo\SQLRecordSetPluInfo.h"
/**********************************************************************/
#include "AccumulatorPropertyPage2.h"
/**********************************************************************/

CAccumulatorPropertyPage2::CAccumulatorPropertyPage2()
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CAccumulatorPropertyPage2::IDD)
{
	m_strFindString = "";
	m_pReportInfo = NULL;
}

/**********************************************************************/

CAccumulatorPropertyPage2::~CAccumulatorPropertyPage2()
{
}

/**********************************************************************/

void CAccumulatorPropertyPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FIND, m_strFindString);
	DDX_Control(pDX, IDC_SPIN_FIND, m_spinFind);
	DDX_Control(pDX, IDC_STATIC_COUNT, m_staticCount);
	DDX_Control(pDX, IDC_CHECK_NEWPAGE, m_checkNewPage);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CAccumulatorPropertyPage2, CSSAutoShutdownPropertyPage)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_FIND, OnSpinFind)
	ON_STN_DBLCLK(IDC_STATIC_COUNT, OnDoubleClickStaticCount)
	ON_MESSAGE(WM_APP + 2, OnUpdateCount)
END_MESSAGE_MAP()

//*******************************************************************

void CAccumulatorPropertyPage2::SetReportLabel(CString strLabel)
{
	m_strReportLabel = strLabel;
}

//*******************************************************************
static const int nColumnCount = 2;
//*******************************************************************

BOOL CAccumulatorPropertyPage2::OnInitDialog() 
{
	bool bNewPage = m_pReportInfo -> GetNewPageFlag();

	CString strLine = "";
	CReportDefaultHandler repdefs;
	if ( repdefs.Read ( m_strReportLabel, strLine ) == TRUE )
	{
		CCSV csv ( strLine );
		
		if (csv.IsEmpty(0) == FALSE)
		{
			bNewPage = csv.GetBool(0);
		}

		m_strFindString = csv.GetString(1);
	}

	CPropertyPage::OnInitDialog();
	CWaitCursor cursor;
	
	//CColumnHandler column;
	//if ( column.Read (  m_strReportLabel, nColumnCount ) == FALSE )
	//{
	//	column.SetWidth ( 0, 100 );						// default values
	//	column.SetWidth ( 1, 300 );
	//}

	m_checkNewPage.SetCheck(bNewPage);

	m_listPlu.SubclassDlgItem(IDC_LIST_PLU, this);
	m_listPlu.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listPlu.InsertColumn(0, "", LVCFMT_LEFT, 390);
	m_listPlu.SetLockItemZeroFlag(FALSE);
	m_listPlu.SetTagStateNotifyMessage(this, WM_APP + 2);

	{
		CCSV csvDiscount;
		csvDiscount.Add(szDISCOUNT_PLUNO);
		csvDiscount.Add(szDISCOUNT_ECRTEXT);

		CSSListTaggedSelectItem itemDiscount;
		itemDiscount.m_strText = csvDiscount.GetLine();
		itemDiscount.SetTagState(0);
		m_listPlu.AddItemToList(itemDiscount);
	}

	{
		CSQLRecordSetPluInfo RecordSet(NULL);
	
		CSQLRowPluInfo RowPluInfo;
		CString strSpace = "     ";
		while (RecordSet.StepSelectAll(RowPluInfo) == TRUE)
		{
			CCSV csvPlu;
			csvPlu.Add(RowPluInfo.GetPluNoString());
			csvPlu.Add(strSpace + RowPluInfo.GetDescription());

			CSSListTaggedSelectItem itemPlu;
			itemPlu.m_strText = csvPlu.GetLine();
			itemPlu.SetTagState(0);
			m_listPlu.AddItemToList(itemPlu);
		}
	}

	CSSFile file;
	int nCurSel = 0;

	if ( file.Open ( Filenames.GetPluRankingListFilename(), "rb") == TRUE)
	{
		CString strBuf;
		while ( file.ReadString ( strBuf ) == TRUE )
		{
			for ( int i = 0 ; i < m_listPlu.GetItemCount() ; i++ )
			{
				CSSListTaggedSelectItem itemPlu;
				m_listPlu.GetListItem(i, itemPlu);
				CCSV csv(itemPlu.m_strText);

				if ( strBuf == csv.GetString(0) )
				{
					m_listPlu.SetItemTagState(i, 1);

					if ( m_listPlu.GetTaggedItemCount() == 1)
					{
						nCurSel = i;
					}

					break;
				}
			}
		}
		file.Close();
	}

	m_listPlu.SetCurSel(nCurSel);
	
	DisplayCount();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/**********************************************************************/

void CAccumulatorPropertyPage2::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (nIDCtl == IDC_LIST_PLU)
	{
		m_listPlu.AdjustRowHeight(lpMeasureItemStruct, 10);
	}
}

/**********************************************************************/

void CAccumulatorPropertyPage2::DisplayCount()
{
	CString strCount = "";
	strCount.Format("Selected: %d", m_listPlu.GetTaggedItemCount());
	m_staticCount.SetWindowText(strCount);
}

//******************************************************************

BOOL CAccumulatorPropertyPage2::OnKillActive()
{
	if (UpdateData() == TRUE)
	{
		m_pReportInfo -> SetNewPageFlag(IsTicked(m_checkNewPage));

		CSSFile file;
		if (file.Open(Filenames.GetPluRankingListFilename(), "wb") == TRUE)
		{
			for (int nIndex = 0; nIndex < m_listPlu.GetItemCount(); nIndex++)
			{
				CSSListTaggedSelectItem itemPlu;
				m_listPlu.GetListItem(nIndex, itemPlu);

				if (itemPlu.GetTagState() == 1)
				{
					CCSV csv(itemPlu.m_strText);
					file.WriteLine(csv.GetString(0));
				}
			}
			file.Close();
		}

		CCSV csv;
		csv.Add(m_pReportInfo -> GetNewPageFlag());
		csv.Add(m_strFindString);

		CReportDefaultHandler repdefs;
		repdefs.Save(m_strReportLabel, &csv);

		//CColumnHandler column;
		//column.Save ( m_strReportLabel, 0, nColumnCount, m_listPlu );
	}

	return CPropertyPage::OnKillActive();

}

//*******************************************************************

void CAccumulatorPropertyPage2::OnSpinFind(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	int nIndex = -1;

	int nCurSel = m_listPlu.GetCurSel();

	if (UpdateData() == TRUE)
	{
		if (m_strFindString == "")
		{
			nIndex = nCurSel;
			bool bFound = FALSE;

			if (pNMUpDown->iDelta > 0)			// Down
			{
				for (++nIndex; nIndex < m_listPlu.GetItemCount(); nIndex++)
				{
					CSSListTaggedSelectItem itemPlu;
					m_listPlu.GetListItem(nIndex, itemPlu);

					if (itemPlu.GetTagState() == 1 )
					{
						bFound = TRUE;
						break;
					}
				}
			}
			else									// up
			{
				for (--nIndex; nIndex >= 0; nIndex--)
				{
					CSSListTaggedSelectItem itemPlu;
					m_listPlu.GetListItem(nIndex, itemPlu);

					if (itemPlu.GetTagState() == 1)
					{
						bFound = TRUE;
						break;
					}
				}
			}

			if (bFound == FALSE)
			{
				nIndex = -1;
			}
		}
		else
		{
			if (pNMUpDown->iDelta > 0)
			{
				nIndex = FindNext(nCurSel, TRUE);	// previous / Down
			}
			else
			{
				nIndex = FindNext(nCurSel, FALSE);		// next / Up
			}
		}

		if (nIndex != -1)
		{
			m_listPlu.SetCurSel(nIndex);
		}
	}

	*pResult = 0;
}

//*******************************************************************

int CAccumulatorPropertyPage2::FindNext ( int nStartPos, bool bAscending )
{
	int nIndex = -1;
	
	CString strFindString = m_strFindString;
	strFindString.MakeUpper();

	if ( bAscending == TRUE )
	{
		for ( int i = nStartPos + 1 ; i < m_listPlu.GetItemCount() ; i++ )
		{
			CSSListTaggedSelectItem itemPlu;
			m_listPlu.GetListItem(i, itemPlu);
			CString strText = itemPlu.m_strText;

			strText.MakeUpper();

			if ( strText.Find ( strFindString ) > -1 )
			{
				nIndex = i;
				break;
			}
		}
	}
	else
	{
		for ( int i = nStartPos - 1 ; i >= 0 ; i-- )
		{
			CSSListTaggedSelectItem itemPlu;
			m_listPlu.GetListItem(i, itemPlu);
			CString strText = itemPlu.m_strText;

			strText.MakeUpper();

			if ( strText.Find ( strFindString ) > -1 )
			{
				nIndex = i;
				break;
			}
		}
	}

	return nIndex;
}

//*******************************************************************

long CAccumulatorPropertyPage2::OnUpdateCount(WPARAM wIndex, LPARAM lParam)
{
	DisplayCount();
	return 0l;
}

//*******************************************************************

void CAccumulatorPropertyPage2::OnDoubleClickStaticCount()
{
	if (UpdateData() == TRUE)
	{
		for (int nIndex = 0; nIndex < m_listPlu.GetItemCount(); nIndex++)
		{
			m_listPlu.SetItemTagState(nIndex, 0);
		}

		m_listPlu.Invalidate();
		m_listPlu.RedrawWindow();
		m_listPlu.SetCurSel(0);
		DisplayCount();
	}
}

//*******************************************************************
