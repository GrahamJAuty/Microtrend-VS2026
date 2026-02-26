/**********************************************************************/
#include "CloneDatabaseDlg.h"
#include "Password.h"
#include "SysInfo.h"
#include "..\SPosStockManager5\UnitechBatchJobDlg.h"
#include "..\SPosStockManager5\UnitechOptions.h"
/**********************************************************************/
#include "PropPageSystemDataList.h"
/**********************************************************************/

void CPropPageSystemDataList::PrepareEcrTextPage()
{	
	m_Filter.PrepareEcrTextFilter();
}

/**********************************************************************/

void CPropPageSystemDataList::BuildEcrTextList()
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

	if ( PasswordArray.CheckLocationAccessSystem() == TRUE )
		if ( GotGlobalData() == TRUE )
			AddGlobal();
			
	m_buttonAction1.SetWindowText ( "Edit" );
	m_buttonAction2.SetWindowText ( "Clone" );
	m_buttonAction3.ShowWindow ( SW_HIDE );
	m_buttonAction4.ShowWindow ( SW_HIDE );
	m_buttonAction5.ShowWindow ( SW_HIDE );
	m_buttonAction6.ShowWindow ( SW_HIDE );

	if ( CheckCloneDatabase() == FALSE )
		m_buttonAction2.ShowWindow ( SW_HIDE );

	SelectLine( m_listLocation, 0 );
}

/**********************************************************************/

void CPropPageSystemDataList::SelectLocationEcrText()
{
	int nDbNo, nAction, nConType;
	GetEcrDataAction( nDbNo, nAction, nConType );

	m_arrayAction.RemoveAll();
	m_listAction.DeleteAllItems();

	if ( 0 == nDbNo )
	{
		if ( m_Filter.GetPaymentFlag() == SCOPE_GLOBAL )		
		{
			AddAction ( NODE_PAYMENT, "Payment Methods" );

			if ( SysInfo.IsSportsBookerSystem() == TRUE )
				AddAction( NODE_SBPAYMENTMAP, "Sports Booker Payments" );

			if ( SysInfo.IsSmartEntertainmentSystem() == TRUE )
				AddAction( NODE_SEPAYMENTMAP, "Smart Entertainment Payments" );
		}
		if ( m_Filter.GetModifierFlag() == SCOPE_GLOBAL )			AddAction ( NODE_MODIFIER, "Modifiers" );
		if ( m_Filter.GetAnalysisCategoryFlag() == SCOPE_GLOBAL )	AddAction ( NODE_ACAT, "Analysis Categories" );
		if ( m_Filter.GetPaidInOutTextFlag() == SCOPE_GLOBAL )		AddAction ( NODE_PAIDIN_TEXT, "Paid In / Out Texts" );
		if ( m_Filter.GetPluUnitFlag() == SCOPE_GLOBAL )			AddAction ( NODE_PLUUNIT, "Plu Units" );
		if ( m_Filter.GetPriceTextFlag() == SCOPE_GLOBAL )		
		{
			AddAction ( NODE_PRICETEXT, "Price Level Texts" );

			if ( SysInfo.IsSportsBookerSystem() == TRUE )
				AddAction( NODE_SBPRICETEXT, "Sports Booker Price Bands" );
		}
		if ( m_Filter.GetAllergenFlag() == SCOPE_GLOBAL )			AddAction ( NODE_ALLERGEN, "Allergens" );
		m_buttonAction2.EnableWindow( FALSE );
	}
	else
	{
		if ( m_Filter.GetPaymentFlag() == SCOPE_DBASE )			
		{
			AddAction( NODE_PAYMENT, "Payment Methods" );

			if ( SysInfo.IsSportsBookerSystem() == TRUE )
				AddAction( NODE_SBPAYMENTMAP, "Sports Booker Payments" );

			if ( SysInfo.IsSmartEntertainmentSystem() == TRUE )
				AddAction( NODE_SEPAYMENTMAP, "Smart Entertainment Payments" );
		}
		if ( m_Filter.GetModifierFlag() == SCOPE_DBASE )			AddAction( NODE_MODIFIER, "Modifiers" );
		if ( m_Filter.GetAnalysisCategoryFlag() == SCOPE_DBASE )	AddAction( NODE_ACAT, "Analysis Categories" );
		if ( m_Filter.GetPaidInOutTextFlag() == SCOPE_DBASE )		AddAction( NODE_PAIDIN_TEXT, "Paid In / Out Texts" );
		m_buttonAction2.EnableWindow( m_bCanCloneDatabase );
	}

	ForceSelectAction( nAction );
}

/**********************************************************************/

void CPropPageSystemDataList::GetEcrTextAction( int& nDbNo, int& nAction )
{
	nDbNo = 0;
	nAction = 0;

	{
		int nSel = m_listLocation.GetCurSel();
		if ( ( nSel >= 0 ) && ( nSel < m_arrayLocation.GetSize() ) )
		{
			CLocationSelectorEntity LocSelEntity = m_arrayLocation.GetAt( nSel );

			if ( LocSelEntity.GetConType() == NODE_SYSTEM )
				nDbNo = 0;
			else
				nDbNo = LocSelEntity.GetEntityNo();
		}
	}

	{
		int nSel = m_listAction.GetCurSel();
		if ( ( nSel >= 0 ) && ( nSel < m_arrayAction.GetSize() ) )
			nAction = m_arrayAction.GetAt(nSel);
	}
}

/**********************************************************************/

void CPropPageSystemDataList::SelectActionEcrText()
{
}

/**********************************************************************/

void CPropPageSystemDataList::Button1EcrTextPage() 
{
	int nDbNo, nNodeType, nConType;
	GetEcrDataAction( nDbNo, nNodeType, nConType );

	switch ( nNodeType )
	{
	case NODE_PLUUNIT:		m_pHelpers -> m_EcrText.EditPluUnit();					break;
	case NODE_PRICETEXT:	m_pHelpers -> m_EcrText.EditPriceText();				break;
	case NODE_SBPRICETEXT:	m_pHelpers -> m_EcrText.EditSBPriceText();				break;
	case NODE_MODIFIER:		m_pHelpers -> m_EcrText.EditModifier( nDbNo );			break;
	case NODE_PAYMENT:		m_pHelpers -> m_EcrText.EditPayment( nDbNo );			break;
	case NODE_SBPAYMENTMAP:	m_pHelpers -> m_EcrText.EditSBPaymentMap( nDbNo );		break;
	case NODE_SEPAYMENTMAP:	m_pHelpers -> m_EcrText.EditSEPaymentMap( nDbNo );		break;
	case NODE_ACAT:			m_pHelpers -> m_EcrText.EditAnalysisCategory( nDbNo );	break;
	case NODE_ALLERGEN:		m_pHelpers -> m_EcrText.EditAllergen();					break;
	case NODE_PAIDIN_TEXT:	m_pHelpers -> m_EcrText.EditPaidInOutText( nDbNo );		break;
	}
}

/**********************************************************************/

void CPropPageSystemDataList::Button2EcrTextPage() 
{
	int nDbNo, nAction, nConType;
	GetEcrDataAction( nDbNo, nAction, nConType );
	
	if ( nDbNo != 0 )
		CloneDatabase( nDbNo );
}

/**********************************************************************/
