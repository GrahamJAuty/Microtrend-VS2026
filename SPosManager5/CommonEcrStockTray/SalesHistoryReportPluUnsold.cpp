/**********************************************************************/
#include "DepartmentSorter.h"
#include "ReportHelpers.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "SalesHistoryReportPlu.h"
/**********************************************************************/

void CSalesHistoryReportPlu::CreateDummySales()
{
	CSortedDateTimeItem DateFrom = DataManagerNonDb.SessionDateTimeFilter.GetDateFrom();
	CSortedDateTimeItem DateTo = DataManagerNonDb.SessionDateTimeFilter.GetDateTo();

	COleDateTime dateFirstMonth = COleDateTime ( DateFrom.m_oleDateTime.GetYear(), DateFrom.m_oleDateTime.GetMonth(), 1, 0, 0, 0 );
	COleDateTime dateLastMonth = COleDateTime ( DateTo.m_oleDateTime.GetYear(), DateTo.m_oleDateTime.GetMonth(), 1, 0, 0, 0 );

	while ( dateFirstMonth <= dateLastMonth )
	{
		m_QtyValueTable.SetBufferDate( 0, dateFirstMonth );

		CEntityTreePluSalesHistoryNormalNode DummyNode;
		m_QtyValueTable.Consolidate( DummyNode );

		if ( dateFirstMonth.GetMonth() < 12 )
			dateFirstMonth = COleDateTime ( dateFirstMonth.GetYear(), dateFirstMonth.GetMonth() + 1, 1, 0, 0, 0 );
		else
			dateFirstMonth = COleDateTime ( dateFirstMonth.GetYear() + 1, 1, 1, 0, 0, 0 );
	}
}

/**********************************************************************/
	
void CSalesHistoryReportPlu::WriteUnsoldItems()
{
	switch ( m_nReportType )
	{
	case SH_PLU_UNSOLD:
	case SH_PLU_UNSOLD_MONTH:
		{
			for ( int nPluIdx = 0; nPluIdx < m_arrayUnsoldItems.GetSize(); nPluIdx++ )
			{
				if ( m_arrayUnsoldItems[ nPluIdx ] )
				{
					__int64 nPluNo = DataManager.Plu.GetPluNoInt( nPluIdx );

					//FILTER BY PLU RANGE
					if ( m_bItemRange == TRUE )
					{
						if ((m_nItemFrom != 0) && (nPluNo < m_nItemFrom))
						{
							continue;
						}

						if ((m_nItemTo != 0) && (nPluNo > m_nItemTo))
						{
							continue;
						}
					}

					CSQLPluNoInfo infoPluNo;
					infoPluNo.m_nBasePluNo = nPluNo;
					infoPluNo.m_nModifier = 0;
					
					if ( m_ReportFilters.CheckPlu( infoPluNo ) == TRUE )
					{
						m_PluInfoFinder.SetPluNo( nPluNo );
						
						int nDeptNo = m_PluInfoFinder.GetModifierDeptNo(0);
						if ( m_ReportFilters.CheckDepartment( nDeptNo )  )
						{
							CCSV csvOut ( '\t' );
							csvOut.Add( ReportHelpers.GetDisplayPluNo(nPluNo) );
							csvOut.Add( m_PluInfoFinder.GetPluInfoRepText(TRUE) );
							csvOut.Add( nDeptNo );
							csvOut.Add( m_PluInfoFinder.GetPrice(0), SysInfo.GetDPValue() );
							m_ReportFile.WriteReportDataLine( csvOut.GetLine() );
						}
					}
				}
			}
		}
		break;

	case SH_PLU_UNSOLD_DEPT:
	case SH_PLU_UNSOLD_MONTH_DEPT:
		{
			for ( int n = Dept::DeptNo.Min + 1; n <= Dept::DeptNo.Max + 1; n++ )
			{
				int nDeptNo = 0;
				if (Dept::DeptNo.Max + 1 == n)
				{
					nDeptNo = 0;
				}
				else
				{
					nDeptNo = n;
				}

				if (m_ReportFilters.CheckDepartment(nDeptNo) == FALSE)
				{
					continue;
				}

				bool bGotDeptLine = FALSE;
				for ( int nPos = 0; nPos < m_DepartmentSorter.GetItemCount( nDeptNo ); nPos++ )
				{
					int nPluIdx = m_DepartmentSorter.GetItemIndex( nDeptNo, nPos );

					if ( m_arrayUnsoldItems[ nPluIdx ] )
					{
						__int64 nPluNo = DataManager.Plu.GetPluNoInt( nPluIdx );
						
						//FILTER BY PLU RANGE
						if (m_bItemRange == TRUE)
						{
							if ((m_nItemFrom != 0) && (nPluNo < m_nItemFrom))
							{
								continue;
							}

							if ((m_nItemTo != 0) && (nPluNo > m_nItemTo))
							{
								continue;
							}
						}

						CSQLPluNoInfo infoPluNo;
						infoPluNo.m_nBasePluNo = nPluNo;
						infoPluNo.m_nModifier = 0;
					
						if ( m_ReportFilters.CheckPlu( infoPluNo ) == TRUE )
						{
							m_PluInfoFinder.SetPluNo( nPluNo );

							CCSV csvOut ( '\t' );
							csvOut.Add( ReportHelpers.GetDisplayPluNo(nPluNo) );
							csvOut.Add( m_PluInfoFinder.GetPluInfoRepText(TRUE) );
							csvOut.Add( m_PluInfoFinder.GetModifierDeptNo(0) );
							csvOut.Add( m_PluInfoFinder.GetPrice(0), SysInfo.GetDPValue() );
							m_ReportFile.WriteReportDataLine( csvOut.GetLine() );
							bGotDeptLine = TRUE;
						}
					}
				}
				if (bGotDeptLine)
				{
					m_ReportFile.RequestBlankLines(FALSE);
				}
			}
		}
		break;
	}
}

/**********************************************************************/

bool CSalesHistoryReportPlu::CreateUnsoldMonthlyReportInternal()
{
	if (m_ReportFile.Open(Super.ReportFilename()) == FALSE)
	{
		return FALSE;
	}

	m_DepartmentSorter.Build();

	CreateDummySales();
	
	m_arrayUnsoldItems.SetSize( DataManager.Plu.GetSize() );
	
	m_ReportFile.SetStyle1 ( GetReportTitle( "Unsold Plu Items" ) );
	m_ReportFile.AddColumn ( "Plu Number", TA_LEFT, 350 );
	m_ReportFile.AddColumn ( "Description", TA_LEFT, 350 );
	m_ReportFile.AddColumn ( "Dept", TA_RIGHT, 200 );
	m_ReportFile.AddColumn ( "Price", TA_RIGHT, 250 );
	
	StatusProgress.Lock( TRUE, "Creating report" );
	
	__int64 nProgressTarget = m_QtyValueTable.GetNodeCount();
	__int64 nProgressSoFar = 0;

	int nNodeCount1 = m_QtyValueTable.GetBranchSize(0);	
	for ( int nNodeIdx1 = 0; nNodeIdx1 < nNodeCount1; nNodeIdx1++ )
	{
		for (int n = 0; n < DataManager.Plu.GetSize(); n++)
		{
			m_arrayUnsoldItems[n] = TRUE;
		}
	
		m_ReportFile.WriteReportMiscLine( GetRepMonth ( m_QtyValueTable.GetLabelDate( 0, nNodeIdx1 ), FALSE, TRUE ) );
		m_ReportFile.WriteReportMiscLine( "<LI>" );
		
		int nArrayIdx2 = m_QtyValueTable.GetNextBranchIdx( 0, nNodeIdx1 );
		
		if ( nArrayIdx2 > 0 )
		{
			int nNodeCount2 = m_QtyValueTable.GetBranchSize(nArrayIdx2);
			for (int nNodeIdx2 = 0; nNodeIdx2 < nNodeCount2; nNodeIdx2++)
			{
				StatusProgress.SetPos(nProgressSoFar++, nProgressTarget);

				CEntityTreePluSalesHistoryNormalNode SalesNode;
				m_QtyValueTable.GetNode(nArrayIdx2, nNodeIdx2, SalesNode);
				if ((SalesNode.m_SalesBlock.GetBaseQty() == 0.0) && (SalesNode.m_SalesBlock.GetTotalVal() == 0.0))
				{
					continue;
				}

				int nPluIdx = 0;
				__int64 nPluNo = m_QtyValueTable.GetLabelInt64(nArrayIdx2, nNodeIdx2);
				if (DataManager.Plu.FindPluByNumber(nPluNo, nPluIdx))
				{
					if (nPluIdx < m_arrayUnsoldItems.GetSize())
					{
						m_arrayUnsoldItems[nPluIdx] = FALSE;
					}
				}
			}
			WriteUnsoldItems();
			m_ReportFile.RequestBlankLines(1);
		}
		else
		{
			m_ReportFile.WriteReportMiscLine( "No sales on file" );
			m_ReportFile.RequestBlankLines(1);
		}
	}
	StatusProgress.Unlock();
	
	m_ReportFile.RequestBlankLines(1);
	m_ReportFile.Close();
	return TRUE;
}

/**********************************************************************/

bool CSalesHistoryReportPlu::CreateUnsoldReportInternal()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_DepartmentSorter.Build();
	
	m_arrayUnsoldItems.SetSize( DataManager.Plu.GetSize() );
	for ( int n = 0; n < DataManager.Plu.GetSize(); n++ )
		m_arrayUnsoldItems[n] = TRUE;
		
	m_ReportFile.SetStyle1 ( GetReportTitle( "Unsold Plu Items" ) );
	m_ReportFile.AddColumn ( "Plu Number", TA_LEFT, 350 );
	m_ReportFile.AddColumn ( "Description", TA_LEFT, 350 );
	m_ReportFile.AddColumn ( "Dept", TA_RIGHT, 200 );
	m_ReportFile.AddColumn ( "Price", TA_RIGHT, 250 );
		
	StatusProgress.Lock( TRUE, "Creating report" );
	int nNodeCount1 = m_QtyValueTable.GetBranchSize(0);
	for ( int nNodeIdx1 = 0; nNodeIdx1 < nNodeCount1; nNodeIdx1++ )
	{
		StatusProgress.SetPos ( nNodeIdx1, nNodeCount1 );
		
		CEntityTreePluSalesHistoryNormalNode SalesNode;
		m_QtyValueTable.GetNode( 0, nNodeIdx1, SalesNode );
		if ((SalesNode.m_SalesBlock.GetBaseQty() == 0.0) && (SalesNode.m_SalesBlock.GetTotalVal() == 0.0))
		{
			continue;
		}

		int nPluIdx = 0;
		__int64 nPluNo = m_QtyValueTable.GetLabelInt64( 0, nNodeIdx1 );
		if (DataManager.Plu.FindPluByNumber(nPluNo, nPluIdx))
		{
			if (nPluIdx < m_arrayUnsoldItems.GetSize())
			{
				m_arrayUnsoldItems[nPluIdx] = FALSE;
			}
		}
	}
	StatusProgress.Unlock();
	WriteUnsoldItems();
	m_ReportFile.RequestBlankLines(1);
	m_ReportFile.Close();
	
	return TRUE;
}

/**********************************************************************/

