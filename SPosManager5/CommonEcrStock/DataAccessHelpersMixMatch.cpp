/**********************************************************************/
#include "CustomExportManager.h"
#include "DatabaseCSVArray.h"
#include "ListDataMixMatchDlg.h"
#include "ListDataOfferGroupDlg.h"
#include "ListDataPromotionDlg.h"
#include "ListDataTimePlanDlg.h"
#include "ListDataWalletStampRuleDlg.h"
#include "NodeTypes.h"
#include "SysInfo.h"
/**********************************************************************/
#include "DataAccessHelpersMixMatch.h"
/**********************************************************************/

CDataAccessHelpersMixMatch::CDataAccessHelpersMixMatch()
{
}

/**********************************************************************/

void CDataAccessHelpersMixMatch::EditMixMatch(int nDbNo)
{
	int nDbIdx = GetDbIdx(nDbNo);
	if (nDbIdx == -1)
	{
		return;
	}

	CWordArray arrayReadWrite;
	arrayReadWrite.Add(NODE_MIXMATCH);
	arrayReadWrite.Add(NODE_OFFER_GROUP);
	arrayReadWrite.Add(NODE_OFFER_ENTRY);
	arrayReadWrite.Add(NODE_PLU);
	arrayReadWrite.Add(NODE_PROMOTION);
	arrayReadWrite.Add(NODE_TIMEPLAN_TEMPLATE_ENTRY);

	CWordArray arrayReadOnly;
	arrayReadOnly.Add(NODE_ACAT);
	arrayReadOnly.Add(NODE_DEPARTMENT);
	arrayReadOnly.Add(NODE_DEPARTMENT_SET);
	arrayReadOnly.Add(NODE_DEPT_GROUP_EPOS);
	arrayReadOnly.Add(NODE_MODIFIER);
	arrayReadOnly.Add(NODE_PRICETEXT);
	arrayReadOnly.Add(NODE_TAX);
	arrayReadOnly.Add(NODE_TIMEPLAN);
	arrayReadOnly.Add(NODE_TIMEPLAN_ENTRY);
	
	CDataManagerInfo info;
	if (DataManager.OpenDatabaseReadWrite(nDbIdx, arrayReadWrite, arrayReadOnly, info, FALSE) == FALSE)
	{
		Prompter.ShareError(info);
		return;
	}

	CListDataMixMatchDlg dlg(m_pParentWnd);

	if (dlg.DoModal() == IDOK)
	{
		if (DataManager.WriteMixMatch(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
		else if (DataManager.WriteOfferGroup(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
		else if (DataManager.WriteOfferGroupEntry(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
		else if (DataManager.WritePromotion(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
		else if (DataManager.WriteTimePlanTemplateEntry(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
		else if (DataManager.WritePlu(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	DataManager.CloseDatabase();
}

/**********************************************************************/

void CDataAccessHelpersMixMatch::EditPromotion(int nDbNo)
{
	int nDbIdx = GetDbIdx(nDbNo);
	if (nDbIdx == -1)
	{
		return;
	}

	CWordArray arrayReadWrite;
	arrayReadWrite.Add(NODE_PROMOTION);
	arrayReadWrite.Add(NODE_OFFER_GROUP);
	arrayReadWrite.Add(NODE_OFFER_ENTRY);
	arrayReadWrite.Add(NODE_TIMEPLAN_TEMPLATE_ENTRY);

	CWordArray arrayReadOnly;
	arrayReadOnly.Add(NODE_ACAT);
	arrayReadOnly.Add(NODE_DEPARTMENT);
	arrayReadOnly.Add(NODE_DEPARTMENT_SET);
	arrayReadOnly.Add(NODE_DEPT_GROUP_EPOS);
	arrayReadOnly.Add(NODE_MIXMATCH);
	arrayReadOnly.Add(NODE_MODIFIER);
	arrayReadOnly.Add(NODE_PLU);
	arrayReadOnly.Add(NODE_PRICETEXT);
	arrayReadOnly.Add(NODE_TAX);
	arrayReadOnly.Add(NODE_TIMEPLAN);
	arrayReadOnly.Add(NODE_TIMEPLAN_ENTRY);
	

	CDataManagerInfo info;
	if (DataManager.OpenDatabaseReadWrite(nDbIdx, arrayReadWrite, arrayReadOnly, info, FALSE) == FALSE)
	{
		Prompter.ShareError(info);
		return;
	}

	CListDataPromotionDlg dlg(m_pParentWnd);

	if (dlg.DoModal() == IDOK)
	{
		if (DataManager.WritePromotion(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
		else if (DataManager.WriteOfferGroup(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
		else if (DataManager.WriteOfferGroupEntry(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
		else if (DataManager.WriteTimePlanTemplateEntry(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	DataManager.CloseDatabase();
}

/**********************************************************************/

void CDataAccessHelpersMixMatch::EditWalletStampRule(int nDbNo)
{
	int nDbIdx = GetDbIdx(nDbNo);
	if (nDbIdx == -1)
	{
		return;
	}

	CWordArray arrayReadWrite;
	arrayReadWrite.Add(NODE_WALLETSTAMPRULE);
	arrayReadWrite.Add(NODE_OFFER_GROUP);
	arrayReadWrite.Add(NODE_OFFER_ENTRY);

	CWordArray arrayReadOnly;
	arrayReadOnly.Add(NODE_ACAT);
	arrayReadOnly.Add(NODE_DEPARTMENT);
	arrayReadOnly.Add(NODE_DEPARTMENT_SET);
	arrayReadOnly.Add(NODE_DEPT_GROUP_EPOS);
	arrayReadOnly.Add(NODE_MODIFIER);
	arrayReadOnly.Add(NODE_PLU);
	arrayReadOnly.Add(NODE_PRICETEXT);
	arrayReadOnly.Add(NODE_TAX);
	
	CDataManagerInfo info;
	if (DataManager.OpenDatabaseReadWrite(nDbIdx, arrayReadWrite, arrayReadOnly, info, FALSE) == FALSE)
	{
		Prompter.ShareError(info);
		return;
	}

	CListDataWalletStampRuleDlg dlg(m_pParentWnd);

	if (dlg.DoModal() == IDOK)
	{
		bool bGotError = FALSE;

		if (DataManager.WriteWalletStampRule(info) == FALSE)
		{
			Prompter.WriteError(info);
			bGotError = TRUE;
		}
		else if (DataManager.WriteOfferGroup(info) == FALSE)
		{
			Prompter.WriteError(info);
			bGotError = TRUE;
		}
		else if (DataManager.WriteOfferGroupEntry(info) == FALSE)
		{
			Prompter.WriteError(info);
			bGotError = TRUE;
		}

		if (FALSE == bGotError)
		{
			switch (SysInfo.GetLoyaltyOrSmartPayType())
			{
			case LOYALTY_TYPE_LOY_V4:
			case LOYALTY_TYPE_SMP_V4:
				{
					CCustomExportManagerBase Export;
					Export.WritePluPointsFile();
					Export.WriteWalletStampRuleFile();
				}
				break;
			}
		}
	}

	DataManager.CloseDatabase();
}

/**********************************************************************/

void CDataAccessHelpersMixMatch::EditOfferGroup(int nDbNo)
{
	int nDbIdx = GetDbIdx(nDbNo);
	if (nDbIdx == -1)
	{
		return;
	}

	CWordArray arrayReadWrite;
	arrayReadWrite.Add(NODE_OFFER_GROUP);
	arrayReadWrite.Add(NODE_OFFER_ENTRY);

	CWordArray arrayReadOnly;
	arrayReadOnly.Add(NODE_ACAT);
	arrayReadOnly.Add(NODE_DEPARTMENT);
	arrayReadOnly.Add(NODE_DEPARTMENT_SET);
	arrayReadOnly.Add(NODE_DEPT_GROUP_EPOS);
	arrayReadOnly.Add(NODE_MIXMATCH);
	arrayReadOnly.Add(NODE_MODIFIER);
	arrayReadOnly.Add(NODE_PLU);
	arrayReadOnly.Add(NODE_PRICETEXT);
	arrayReadOnly.Add(NODE_PROMOTION);
	arrayReadOnly.Add(NODE_TAX);
	arrayReadOnly.Add(NODE_TIMEPLAN);
	arrayReadOnly.Add(NODE_TIMEPLAN_ENTRY);

	CDataManagerInfo info;
	if (DataManager.OpenDatabaseReadWrite(nDbIdx, arrayReadWrite, arrayReadOnly, info, FALSE) == FALSE)
	{
		Prompter.ShareError(info);
		return;
	}

	CListDataOfferGroupDlg dlg(m_pParentWnd);

	if (dlg.DoModal() == IDOK)
	{
		if (DataManager.WriteOfferGroup(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
		else if (DataManager.WriteOfferGroupEntry(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	DataManager.CloseDatabase();
}

/**********************************************************************/

void CDataAccessHelpersMixMatch::EditTimePlan ( int nDbNo )
{
	int nDbIdx = GetDbIdx ( nDbNo );
	if (nDbIdx == -1)
	{
		return;
	}

	CWordArray arrayReadWrite;
	arrayReadWrite.Add( NODE_TIMEPLAN );
	arrayReadWrite.Add( NODE_TIMEPLAN_ENTRY );
	arrayReadWrite.Add( NODE_TIMEPLAN_TEMPLATE_ENTRY);
	arrayReadWrite.Add( NODE_MIXMATCH );
	arrayReadWrite.Add( NODE_PROMOTION );
	arrayReadWrite.Add( NODE_OFFER_GROUP );
	arrayReadWrite.Add( NODE_OFFER_ENTRY );
	arrayReadWrite.Add( NODE_PLU );

	CWordArray arrayReadOnly;
	arrayReadOnly.Add( NODE_DEPARTMENT );
	arrayReadOnly.Add( NODE_DEPARTMENT_SET );
	arrayReadOnly.Add( NODE_DEPT_GROUP_EPOS );
	arrayReadOnly.Add( NODE_ACAT );
	arrayReadOnly.Add( NODE_MODIFIER );
	arrayReadOnly.Add( NODE_PRICETEXT );

	CDataManagerInfo info;
	if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, arrayReadOnly, info, FALSE ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}
		
	CListDataTimePlanDlg dlg ( m_pParentWnd );
		
	if (dlg.DoModal() == IDOK)
	{
		if (DataManager.WriteTimePlan(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
		else if (DataManager.WriteTimePlanEntry(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
		else if (DataManager.WriteTimePlanTemplateEntry(info) == FALSE)
		{
			Prompter.WriteError(info);
		}


		if (DataManager.WriteMixMatch(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
		else if (DataManager.WriteOfferGroup(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
		else if (DataManager.WriteOfferGroupEntry(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
		else if (DataManager.WritePromotion(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
		else if (DataManager.WritePlu(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}
	
	DataManager.CloseDatabase();
}

/**********************************************************************/
