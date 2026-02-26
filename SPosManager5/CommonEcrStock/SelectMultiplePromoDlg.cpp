/**********************************************************************/
#include "ReportConsolidationArray.h"
#include "ReportHelpers.h"
/**********************************************************************/
#include "SelectMultiplePromoDlg.h"
/**********************************************************************/

CSelectMultiplePromoDlg::CSelectMultiplePromoDlg( CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo>& arrayEntityInfo, CWnd* pParent )
	: CSelectMultipleEntityDlg( "", arrayEntityInfo, FALSE, FALSE, pParent )
{
	m_strTitle = "Select Loyalty Scheme Promotions (Click Lines to Select, Max 5)";
	m_bSorted = FALSE;
}

/**********************************************************************/

void CSelectMultiplePromoDlg::SetListColumnns()
{
	m_listEntities.InsertColumn ( 0, "No", LVCFMT_LEFT, 40 );
	m_listEntities.InsertColumn ( 1, "Name", LVCFMT_LEFT, 150 );
	m_listEntities.InsertColumn ( 2, "Type", LVCFMT_LEFT, 80 );
	m_listEntities.InsertColumn ( 3, "Usage", LVCFMT_LEFT, 100 );
	m_listEntities.InsertColumn ( 4, "Amount", LVCFMT_RIGHT, 70 );
	
	m_buttonAll.SetWindowText( "Sort" );
	ShowAndEnableWindow( &m_buttonAll, TRUE );
}

/**********************************************************************/

void CSelectMultiplePromoDlg::GetListData() 
{
	m_nColumnCount = 5;
	
	for ( int nIndex = 0; nIndex < m_arrayEntityInfo.GetSize(); nIndex++ )
	{
		CSelectMultipleEntityInfo infoNode = m_arrayEntityInfo[nIndex];

		infoNode.m_strText[0].Format ("%d", infoNode.m_nPromoNo );
		
		int nPromoIdx;
		if ( DataManager.Promotion.FindTableByNumber( infoNode.m_nPromoNo, nPromoIdx ) == FALSE )
		{
			infoNode.m_strText[1] = "Unknown";
			infoNode.m_strText[2] = "";
			infoNode.m_strText[3] = "";
			infoNode.m_strText[4] = "";
		}
		else
		{
			CPromotionsCSVRecord Promo;
			DataManager.Promotion.GetAt ( nPromoIdx, Promo );

			infoNode.m_strText[1] = Promo.GetDisplayName();
			infoNode.m_strText[2] = Promo.GetPromoTypeName();

			{
				CString strUsage = "";

				switch( Promo.GetUsage() )
				{
				case 1:		strUsage = "Item";					break;
				case 2:		strUsage = "Subtotal";				break;
				case 3:		strUsage = "Item or Subtotal";		break;
				case 6:		strUsage = "Deferred Subtotal";		break;
				default:	strUsage.Format( "Unknown (%d)", Promo.GetUsage() ); break;			
				}			
				
				infoNode.m_strText[3] = strUsage;
			}

			{
				CString strAmount = "";

				if ( Promo.GetFlexType() == 1 )
				{
					strAmount = "Flex (Value)";
				}
				else if ( Promo.GetFlexType() == 2 )
				{
					strAmount =  "Flex (Covers)";
				}
				else if ( Promo.GetPromoType() != PROMOTION_TYPE_MARKDOWN )
				{
					strAmount.Format( "%.2f%%", Promo.GetPercent() );
				}
				else
				{
					strAmount.Format( "£%.2f", Promo.GetMarkdown() );
				}
				
				infoNode.m_strText[4] = strAmount;
			}
		}

		m_arrayEntityInfo[nIndex] = infoNode;
	}
}

/**********************************************************************/

void CSelectMultiplePromoDlg::HandleAll()
{
	int nOriginalSize = m_arrayEntityInfo.GetSize();

	if ( nOriginalSize > 1 )
	{
		if ( FALSE == m_bSorted )
		{
			CReportConsolidationArray<CSortedIntByInt> arraySorted;

			for ( int n = 0; n < nOriginalSize; n++ )
			{
				CSortedIntByInt item;
				item.m_nKey = m_arrayEntityInfo[n].m_nPromoNo;
				item.m_nVal = n;
				arraySorted.Consolidate(item);
			}

			for ( int n = 0; n < arraySorted.GetSize(); n++ )
			{
				CSortedIntByInt item;
				arraySorted.GetAt( n, item );
				
				CSelectMultipleEntityInfo info = m_arrayEntityInfo.GetAt( item.m_nVal );
				m_arrayEntityInfo.Add( info );
			}

			m_bSorted = TRUE;
		}
		else
		{
			int nInsertSelected = nOriginalSize;
			int nInsertExcluded = nOriginalSize;
			
			for ( int nPos = 0; nPos < nOriginalSize; nPos++ )
			{
				CSelectMultipleEntityInfo info = m_arrayEntityInfo.GetAt( nPos );

				if ( info.m_bSelected == TRUE )
				{
					m_arrayEntityInfo.InsertAt( nInsertSelected, info );
					nInsertSelected++;
					nInsertExcluded++;
				}	
				else
				{
					m_arrayEntityInfo.InsertAt( nInsertExcluded, info );
					nInsertExcluded++;
				}
			}

			m_bSorted = FALSE;
		}

		m_arrayEntityInfo.RemoveAt( 0, nOriginalSize );

		RedrawList();
		m_listEntities.Invalidate();
	}
}

/**********************************************************************/

bool CSelectMultiplePromoDlg::HandleOK() 
{
	CSelectMultipleEntityDlg::HandleOK();

	int nCount = 0;
	for ( int n = 0; n < m_arrayEntityInfo.GetSize(); n++ )
	{
		if ( TRUE == m_arrayEntityInfo[n].m_bSelected )
		{
			nCount++;

			if ( nCount > 5 )
			{
				Prompter.Error ( "Please select 5 or fewer promotions" );
				return FALSE;
			}
		}
	}

	return TRUE;
}

/**********************************************************************/
