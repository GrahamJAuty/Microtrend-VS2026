/**********************************************************************/
#include "..\CommonEcrStockTray\SysInfo.h"
/**********************************************************************/
#include "..\SPosStockManager5\UnitechBatchJobDlg.h"
#include "..\SPosStockManager5\UnitechOptions.h"
/**********************************************************************/
#include "CloneDatabaseDlg.h"
#include "Password.h"
#include "PluAutoImporterCSV.h"
#include "PluAutoImporterEOD.h"
/**********************************************************************/
#include "PropPageSystemDataList.h"
/**********************************************************************/

void CPropPageSystemDataList::PrepareEcrDataPage()
{	
	m_Filter.PrepareEcrDataFilter();
}

/**********************************************************************/

void CPropPageSystemDataList::BuildEcrDataList()
{
	m_arrayLocation.RemoveAll();
	m_arrayAction.RemoveAll();

	if (GotLocalData() == TRUE)
	{
		if (EcrmanOptions.GetReportsLocationSortType() == 0)
		{
			for (int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++)
				if (PasswordArray.CheckLocationAccessDbIdx(nDbIdx) == TRUE)
					AddDatabase(nDbIdx);
		}
		else
		{
			CWordArray arrayDbIdx;
			dbDatabase.SortByName(arrayDbIdx);

			for (int n = 0; n < arrayDbIdx.GetSize(); n++)
			{
				int nDbIdx = arrayDbIdx.GetAt(n);
				if (PasswordArray.CheckLocationAccessDbIdx(nDbIdx) == TRUE)
				{
					AddDatabase(nDbIdx);
				}
			}
		}
	}

	if (PasswordArray.CheckLocationAccessSystem() == TRUE)
	{
		if (GotSystemFilterData() == TRUE)
		{
			AddSystemFilter();
		}

		if (GotGlobalData() == TRUE)
		{
			AddGlobal();
		}
	}

	m_buttonAction1.SetWindowText("Edit");
	m_buttonAction2.SetWindowText("Clone");
	m_buttonAction3.ShowWindow(SW_HIDE);
	m_buttonAction4.SetWindowText("Shelf Labels");
#ifdef STOCKMAN_SYSTEM
	m_buttonAction5.SetWindowText("Item Labels");
#else
	m_buttonAction5.ShowWindow(SW_HIDE);
#endif
	m_buttonAction6.ShowWindow(SW_HIDE);

	if (CheckCloneDatabase() == FALSE)
	{
		m_buttonAction2.ShowWindow(SW_HIDE);
	}

	if ((Sysset.IsBarmanSystem() == FALSE) || (EcrmanOptions.GetLabelsShelfAutoBatchFlag() == FALSE))
	{
		m_buttonAction4.ShowWindow(SW_HIDE);
	}

	if ((Sysset.IsBarmanSystem() == FALSE) || (EcrmanOptions.GetLabelsProductAutoBatchFlag() == FALSE))
	{
		m_buttonAction5.ShowWindow(SW_HIDE);
	}

	SelectLine(m_listLocation, 0);
}

/**********************************************************************/

void CPropPageSystemDataList::SelectLocationEcrData()
{
	int nDbNo, nAction, nConType;
	GetEcrDataAction(nDbNo, nAction, nConType);

	m_arrayAction.RemoveAll();
	m_listAction.DeleteAllItems();

	if (NODE_SYSTEM_FILTER == nConType)
	{
		if (m_Filter.GetDeptFlag() == SCOPE_DBASE)
		{
			AddAction(NODE_DEPARTMENT, "System Department");
		}
		m_buttonAction2.EnableWindow(FALSE);
	}
	else if (0 == nDbNo)
	{
		if (m_Filter.GetDeptFlag() == SCOPE_GLOBAL)
		{
			AddAction(NODE_DEPARTMENT, "Department");
		}

		if (m_Filter.GetEposGroupFlag() == SCOPE_GLOBAL)
		{
			AddAction(NODE_DEPT_GROUP_EPOS, "Dept Group");
		}

		if (m_Filter.GetTaxFlag() == SCOPE_GLOBAL)
		{
			AddAction(NODE_TAX, EcrmanOptions.GetTaxString("%T Rates"));
		}

		m_buttonAction2.EnableWindow(FALSE);

		if (m_Filter.GetMixMatchFlag() == SCOPE_GLOBAL)
		{
			AddAction(NODE_MIXMATCH, "Mix Match");
		}

		if (m_Filter.GetWalletStampRuleFlag() == SCOPE_GLOBAL)
		{
			switch (SysInfo.GetLoyaltyOrSmartPayType())
			{
			case LOYALTY_TYPE_SMP_V4:
				AddAction(NODE_WALLETSTAMPRULE, "Purchase Control");
				break;

			case LOYALTY_TYPE_LOY_V4:
			default:
				AddAction(NODE_WALLETSTAMPRULE, "Stamp Offers");
				break;
			}
		}

		if ((m_Filter.GetMixMatchFlag() == SCOPE_GLOBAL) || (m_Filter.GetWalletStampRuleFlag() == SCOPE_GLOBAL))
		{
			AddAction(NODE_OFFER_GROUP, "Offer Groups");
		}

		if (m_Filter.GetWalletStampRuleFlag() == SCOPE_GLOBAL)
		{
			if (m_Filter.GetPromotionFlag() == SCOPE_GLOBAL)
			{
				AddAction(NODE_PROMOTION, "Promotions");
			}

			if (m_Filter.GetTimePlanFlag() == SCOPE_GLOBAL)
			{
				AddAction(NODE_TIMEPLAN, "Time Plans");
			}
		}
	}
	else
	{
		if (m_Filter.GetPluFlag() == SCOPE_DBASE)
		{
			AddAction(NODE_PLU, "Plu");
		}

		if (m_Filter.GetBarcodeFlag() == SCOPE_DBASE)
		{
			AddAction(NODE_BARCODE, "Barcodes");
		}

		if (m_Filter.GetUnitechFlag() == SCOPE_DBASE)
		{
			AddAction(NODE_UNITECH_PLU, "Unitech Download");
		}

		if (m_Filter.GetMixMatchFlag() == SCOPE_DBASE)
		{
			AddAction(NODE_MIXMATCH, "Mix Match");
		}

		if (m_Filter.GetWalletStampRuleFlag() == SCOPE_DBASE)
		{
			switch (SysInfo.GetLoyaltyOrSmartPayType())
			{
			case LOYALTY_TYPE_SMP_V4:
				AddAction(NODE_WALLETSTAMPRULE, "Purchase Control");
				break;

			case LOYALTY_TYPE_LOY_V4:
			default:
				AddAction(NODE_WALLETSTAMPRULE, "Stamp offers");
				break;
			}
		}

		if ((m_Filter.GetMixMatchFlag() == SCOPE_DBASE) || (m_Filter.GetWalletStampRuleFlag() == SCOPE_DBASE))
		{
			AddAction(NODE_OFFER_GROUP, "Offer Groups");
		}

		if (m_Filter.GetMixMatchFlag() == SCOPE_DBASE)
		{
			if (m_Filter.GetPromotionFlag() == SCOPE_DBASE)
			{
				AddAction(NODE_PROMOTION, "Promotions");
			}

			if (m_Filter.GetTimePlanFlag() == SCOPE_DBASE)
			{
				AddAction(NODE_TIMEPLAN, "Time Plans");
			}
		}

		if (m_Filter.GetDeptFlag() == SCOPE_DBASE)
		{
			AddAction(NODE_DEPARTMENT, "Department");
		}

		if (m_Filter.GetEposGroupFlag() == SCOPE_DBASE)
		{
			AddAction(NODE_DEPT_GROUP_EPOS, "Dept Group");
		}

		if (m_Filter.GetTaxFlag() == SCOPE_DBASE)
		{
			AddAction(NODE_TAX, EcrmanOptions.GetTaxString("%T Rates"));
		}

		if (m_Filter.GetLoyaltySchemeFlag() == SCOPE_DBASE)
		{
			AddAction(NODE_LOYALTY_SCHEME, "Loyalty Schemes");
		}

		m_buttonAction2.EnableWindow(m_bCanCloneDatabase);
	}

	ForceSelectAction(nAction);
}

/**********************************************************************/

void CPropPageSystemDataList::GetEcrDataAction(int& nDbNo, int& nAction, int& nConType)
{
	nDbNo = 0;
	nAction = 0;
	nConType = 0;

	{
		int nSel = m_listLocation.GetCurSel();
		if ((nSel >= 0) && (nSel < m_arrayLocation.GetSize()))
		{
			CLocationSelectorEntity LocSelEntity = m_arrayLocation.GetAt(nSel);

			nConType = LocSelEntity.GetConType();

			switch (nConType)
			{
			case NODE_SYSTEM:
			case NODE_SYSTEM_FILTER:
				nDbNo = 0;
				break;

			default:
				nDbNo = LocSelEntity.GetEntityNo();
				break;
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

void CPropPageSystemDataList::SelectActionEcrData()
{
	int nDbNo, nAction, nConType;
	GetEcrDataAction( nDbNo, nAction, nConType );

	bool bEnable = FALSE;

	if ( nDbNo != 0 )
	{
		switch ( nAction )
		{
		case NODE_PLU:
		case NODE_PRICESET1:
		case NODE_PRICESET2:
		case NODE_PRICESET3:
			bEnable = TRUE;
			break;
		}
	}
		
	m_buttonAction4.EnableWindow(bEnable);	
	m_buttonAction5.EnableWindow(bEnable);	
}

/**********************************************************************/

void CPropPageSystemDataList::Button1EcrDataPage() 
{
	int nDbNo, nNodeType, nConType;
	GetEcrDataAction( nDbNo, nNodeType, nConType );

	switch ( nNodeType )
	{
	case NODE_PLU:			
		m_pHelpers -> m_Plu.EditPlu ( nDbNo, FALSE );		
		break;

#ifdef STOCKMAN_SYSTEM
	case NODE_UNITECH_PLU:
		{
			int nDbIdx;
			dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx );
			CUnitechBatchJobDlg dlgBatch( NODE_UNITECH_PLU, nDbIdx );
			dlgBatch.DoModal();
			PluAutoImporterCSV.ImportPluExternal( nDbIdx );
			PluAutoImporterEOD.ImportPluExternal( nDbIdx );
		}
		break;
#endif

	case NODE_DEPARTMENT:			
		m_pHelpers -> m_EcrData.EditDept( nDbNo );
		break;

	case NODE_DEPT_GROUP_EPOS:	
		m_pHelpers -> m_EcrData.EditGroup( nDbNo );	
		break;

	case NODE_TAX:		
		m_pHelpers -> m_EcrData.EditTax( nDbNo );
		break;

	case NODE_BARCODE:			
		m_pHelpers -> m_EcrData.EditBarcode( nDbNo );	
		break;

	case NODE_LOYALTY_SCHEME:
		m_pHelpers->m_EcrData.EditLoyaltyScheme(nDbNo);
		break;

	case NODE_PROMOTION:	
		m_pHelpers -> m_MixMatch.EditPromotion( nDbNo );
		break;

	case NODE_OFFER_GROUP:		
		m_pHelpers -> m_MixMatch.EditOfferGroup( nDbNo );
		break;

	case NODE_MIXMATCH:	
		m_pHelpers -> m_MixMatch.EditMixMatch( nDbNo );
		break;

	case NODE_TIMEPLAN:				
		m_pHelpers -> m_MixMatch.EditTimePlan( nDbNo );
		break;

	case NODE_WALLETSTAMPRULE:			
		m_pHelpers -> m_MixMatch.EditWalletStampRule(nDbNo);
		break;

	default:				
		break;
	}
}

/**********************************************************************/

void CPropPageSystemDataList::Button2EcrDataPage()
{
	int nDbNo, nAction, nConType;
	GetEcrDataAction(nDbNo, nAction, nConType);

	if (nDbNo != 0)
	{
		CloneDatabase(nDbNo);
	}
}

/**********************************************************************/

void CPropPageSystemDataList::Button4EcrDataPage()
{
	int nDbNo, nNodeType, nConType;
	GetEcrDataAction(nDbNo, nNodeType, nConType);

	switch (nNodeType)
	{
	case NODE_PLU:
		m_pHelpers->m_Plu.PrintShelfLabels(nDbNo);
		break;
	}
}

/**********************************************************************/

void CPropPageSystemDataList::Button5EcrDataPage() 
{
#ifdef STOCKMAN_SYSTEM
	int nDbNo, nNodeType, nConType;
	GetEcrDataAction( nDbNo, nNodeType, nConType );

	switch ( nNodeType )
	{
	case NODE_PLU:	m_pHelpers -> m_Plu.PrintBarcodeLabels( nDbNo );	break;
	}
#endif
}

/**********************************************************************/

