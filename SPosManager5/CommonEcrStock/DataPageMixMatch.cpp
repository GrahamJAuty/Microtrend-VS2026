/**********************************************************************/
#include "..\CommonEcrStockTray\SysInfo.h"
/**********************************************************************/
#include "CloneDatabaseDlg.h"
#include "Password.h"
/**********************************************************************/
#include "PropPageSystemDataList.h"
/**********************************************************************/

void CPropPageSystemDataList::PrepareMixMatchPage()
{	
	m_Filter.PrepareMixMatchFilter();
}

/**********************************************************************/

void CPropPageSystemDataList::BuildMixMatchList()
{
	m_arrayLocation.RemoveAll();
	m_arrayAction.RemoveAll();

	if ( GotLocalData() == TRUE )
	{
		if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
		{
			for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
				if ( PasswordArray.CheckLocationAccessDbIdx( nDbIdx ) == TRUE )
					AddDatabase( nDbIdx );	
		}
		else
		{
			CWordArray arrayDbIdx;
			dbDatabase.SortByName( arrayDbIdx );

			for ( int n = 0; n < arrayDbIdx.GetSize(); n++ )
			{
				int nDbIdx = arrayDbIdx.GetAt(n);
				if ( PasswordArray.CheckLocationAccessDbIdx( nDbIdx ) == TRUE )
					AddDatabase( nDbIdx );
			}
		}
	}
	else if ( GotGlobalData() == TRUE )
	{
		AddGlobal();
	}

	m_buttonAction1.SetWindowText ( "Edit" );
	m_buttonAction2.SetWindowText ( "Clone" );
	m_buttonAction3.ShowWindow ( SW_HIDE );
	m_buttonAction4.ShowWindow ( SW_HIDE );
	m_buttonAction5.ShowWindow ( SW_HIDE );
	m_buttonAction6.ShowWindow ( SW_HIDE );

	m_Filter.SetPluFlag ( m_Filter.GetMixMatchFlag() );
	
	if ( CheckCloneDatabase() == FALSE )
	{
		m_buttonAction2.ShowWindow ( SW_HIDE );
	}

	m_Filter.SetPluFlag( SCOPE_HIDE );
}

/**********************************************************************/

void CPropPageSystemDataList::SelectLocationMixMatch()
{
	int nDbNo, nAction, nConType;
	GetEcrDataAction( nDbNo, nAction, nConType );

	m_arrayAction.RemoveAll();
	m_listAction.DeleteAllItems();

	if (m_Filter.GetMixMatchFlag() != SCOPE_HIDE)
	{
		AddAction(NODE_MIXMATCH, "Mix Match");
	}

	if (m_Filter.GetWalletStampRuleFlag() != SCOPE_HIDE)
	{
		switch (SysInfo.GetLoyaltyOrSmartPayType())
		{
		case LOYALTY_TYPE_SMP_V4:
			AddAction(NODE_WALLETSTAMPRULE, "Purchase Controls");
			break;

		case LOYALTY_TYPE_LOY_V4:
		default:
			AddAction(NODE_WALLETSTAMPRULE, "Stamp Offers");
			break;
		}
	}
	
	if (((m_Filter.GetMixMatchFlag() != SCOPE_HIDE)) || (m_Filter.GetWalletStampRuleFlag() != SCOPE_HIDE))
	{
		AddAction(NODE_OFFER_GROUP, "Offer Groups");
	}

	if (m_Filter.GetMixMatchFlag() != SCOPE_HIDE)
	{
		if (m_Filter.GetPromotionFlag() != SCOPE_HIDE)
		{
			AddAction(NODE_PROMOTION, "Promotions");
		}

		if (m_Filter.GetTimePlanFlag() != SCOPE_HIDE)
		{
			AddAction(NODE_TIMEPLAN, "Time Plans");
		}
	}

	m_buttonAction2.EnableWindow( ( m_bCanCloneDatabase ) && ( DealerFlags.GetGlobalPromotionFlag() == FALSE ) );
	ForceSelectAction( nAction );
}

/**********************************************************************/

void CPropPageSystemDataList::GetMixMatchAction(int& nDbNo, int& nAction)
{
	nDbNo = 0;
	nAction = 0;

	{
		int nSel = m_listLocation.GetCurSel();
		if ((nSel >= 0) && (nSel < m_arrayLocation.GetSize()))
		{
			CLocationSelectorEntity LocSelEntity = m_arrayLocation.GetAt(nSel);

			if (LocSelEntity.GetConType() == NODE_SYSTEM)
			{
				nDbNo = 0;
			}
			else
			{
				nDbNo = LocSelEntity.GetEntityNo();
			}
		}

	}

	{
		int nSel = m_listAction.GetCurSel();
		if ((nSel >= 0) && (nSel < m_arrayAction.GetSize()))
		{
			nAction = m_arrayAction.GetAt(nSel);
		}
	}
}

/**********************************************************************/

void CPropPageSystemDataList::Button1MixMatchPage()
{
	int nDbNo = 0, nNodeType = 0;
	GetMixMatchAction(nDbNo, nNodeType);

	switch (nNodeType)
	{
	case NODE_MIXMATCH:
		m_pHelpers->m_MixMatch.EditMixMatch(nDbNo);
		break;

	case NODE_PROMOTION:
		m_pHelpers->m_MixMatch.EditPromotion(nDbNo);
		break;

	case NODE_OFFER_GROUP:
		m_pHelpers->m_MixMatch.EditOfferGroup(nDbNo);
		break;

	case NODE_TIMEPLAN:
		m_pHelpers->m_MixMatch.EditTimePlan(nDbNo);
		break;

	case NODE_WALLETSTAMPRULE:
		m_pHelpers->m_MixMatch.EditWalletStampRule(nDbNo);
		break;

	default:
		break;
	}
}

/**********************************************************************/

void CPropPageSystemDataList::Button2MixMatchPage() 
{
	int nDbNo, nNodeType;
	GetMixMatchAction( nDbNo, nNodeType );
	
	m_Filter.SetPluFlag( m_Filter.GetMixMatchFlag() );
	CloneDatabase( nDbNo );	
	m_Filter.SetPluFlag( SCOPE_HIDE );
}

/**********************************************************************/

