/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "KeyboardHelpers.h"
#include "ListDataAnalysisCategoryDlg.h"
#include "ListDataGroupDlg.h"
#include "ListDataPaymentDlg.h"
#include "..\CommonEcrStock\ListDataUnitDlg.h"
#include "ModifierEditDlg.h"
#include "PaidInOutTextEditDlg.h"
#include "PriceTextEditDlg.h"
#include "..\CommonEcrStock\SmartEntertainmentPaymentMapDlg.h"
#include "SportsBookerPaymentMapDlg.h"
#include "SportsBookerPriceBandDlg.h"
/**********************************************************************/
#include "DataAccessHelpersEcrTexts.h"
/**********************************************************************/

CDataAccessHelpersEcrTexts::CDataAccessHelpersEcrTexts()
{
}

/**********************************************************************/

void CDataAccessHelpersEcrTexts::EditPluUnit()
{
	CDataManagerInfo info;
	if ( DataManagerNonDb.OpenPluUnit( DB_READWRITE, info ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}
		
	CListDataUnitDlg dlg ( DataManagerNonDb.PluUnit, "", m_pParentWnd );
		
	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManagerNonDb.WritePluUnit( info ) == FALSE )
			Prompter.WriteError( info );
	}
	
	DataManagerNonDb.ClosePluUnit( info );
}

/**********************************************************************/

void CDataAccessHelpersEcrTexts::EditPriceText()
{
	CWordArray arrayReadWrite, arrayDummy;
	arrayReadWrite.Add( NODE_PRICETEXT );

	CDataManagerInfo info;
	if ( DataManager.OpenDatabaseReadWrite( 0, arrayReadWrite, arrayDummy, info, FALSE ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}
		
	CPriceTextEditDlg dlg ( m_pParentWnd );
		
	if ( dlg.DoModal() == IDOK )
		if ( DataManager.WritePriceText( info ) == FALSE )
			Prompter.WriteError( info );
		
	DataManager.CloseDatabase();
}

/**********************************************************************/

void CDataAccessHelpersEcrTexts::EditSBPriceText()
{
	CWordArray arrayReadWrite;
	arrayReadWrite.Add( NODE_SBPRICETEXT );

	CWordArray arrayReadOnly;
	arrayReadWrite.Add( NODE_PRICETEXT );

	CDataManagerInfo info;
	if ( DataManager.OpenDatabaseReadWrite( 0, arrayReadWrite, arrayReadOnly, info, FALSE ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}
	
	CSportsBookerPriceBandDlg dlg ( m_pParentWnd );
		
	if ( dlg.DoModal() == IDOK )
		if ( DataManager.WriteSBPriceText( info ) == FALSE )
			Prompter.WriteError( info );
		
	DataManager.CloseDatabase();
}

/**********************************************************************/

void CDataAccessHelpersEcrTexts::EditModifier( int nDbNo )
{
	int nDbIdx = GetDbIdx ( nDbNo );
	if ( nDbIdx == -1 ) return;

	CWordArray arrayReadWrite, arrayDummy;
	arrayReadWrite.Add( NODE_MODIFIER );

	CDataManagerInfo info;
	if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, arrayDummy, info, FALSE ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}
		
	CModifierEditDlg dlg ( m_pParentWnd );
		
	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManager.WriteModifier( info ) == FALSE )
			Prompter.WriteError( info );
		else
		{
			int nStartDbIdx = 0;
			int nEndDbIdx = dbDatabase.GetSize() - 1;

			if ( FALSE == EcrmanOptions.GetGlobalModifierFlag() )
			{
				nStartDbIdx = nDbIdx;
				nEndDbIdx = nDbIdx;
			}

			for ( int nDbIdx = nStartDbIdx; nDbIdx <= nEndDbIdx; nDbIdx++ )
				CKeyboardHelpers::RemovePluFile( nDbIdx );
		}
	}
	
	DataManager.CloseDatabase();
}

/**********************************************************************/

void CDataAccessHelpersEcrTexts::EditPayment( int nDbNo )
{
	int nDbIdx = GetDbIdx ( nDbNo );
	if ( nDbIdx == -1 ) return;

	CWordArray arrayReadWrite, arrayDummy;
	arrayReadWrite.Add( NODE_PAYMENT );
	arrayReadWrite.Add( NODE_PAYMENT_GROUP );

	CDataManagerInfo info;
	if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, arrayDummy, info, FALSE ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}
		
	CListDataPaymentDlg dlg ( m_pParentWnd );
		
	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManager.WritePayment( info ) == TRUE )
		{
			DataManager.Payment.WriteSMPaymentFile();
			DataManager.Payment.WriteSMPaymentGroupFile();
		}
		else
			Prompter.WriteError( info );		
	}
	
	DataManager.CloseDatabase();
}

/**********************************************************************/

void CDataAccessHelpersEcrTexts::EditSBPaymentMap( int nDbNo )
{
	int nDbIdx = GetDbIdx ( nDbNo );
	if ( nDbIdx == -1 ) return;

	CWordArray arrayReadOnly;
	arrayReadOnly.Add( NODE_PAYMENT );

	CWordArray arrayReadWrite;
	arrayReadWrite.Add( NODE_SBPAYMENTMAP );

	CDataManagerInfo info;
	if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, arrayReadOnly, info, FALSE ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}
		
	CSportsBookerPaymentMapDlg dlg ( m_pParentWnd );
		
	if ( dlg.DoModal() == IDOK )
	{
		DataManager.SBPaymentMap.PrepareForWrite();
		if ( DataManager.WriteSBPaymentMap( info ) == FALSE )
			Prompter.WriteError( info );
	}
	
	DataManager.CloseDatabase();
}

/**********************************************************************/

void CDataAccessHelpersEcrTexts::EditSEPaymentMap( int nDbNo )
{
	int nDbIdx = GetDbIdx ( nDbNo );
	if ( nDbIdx == -1 ) return;

	CWordArray arrayReadOnly;
	arrayReadOnly.Add( NODE_PAYMENT );

	CWordArray arrayReadWrite;
	arrayReadWrite.Add( NODE_SEPAYMENTMAP );

	CDataManagerInfo info;
	if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, arrayReadOnly, info, FALSE ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}
		
	CSmartEntertainmentPaymentMapDlg dlg ( m_pParentWnd );
		
	if ( dlg.DoModal() == IDOK )
	{
		DataManager.SEPaymentMap.PrepareForWrite();
		if ( DataManager.WriteSEPaymentMap( info ) == FALSE )
			Prompter.WriteError( info );
	}
	
	DataManager.CloseDatabase();
}

/**********************************************************************/

void CDataAccessHelpersEcrTexts::EditPaidInOutText( int nDbNo )
{
	int nDbIdx = GetDbIdx ( nDbNo );
	if ( nDbIdx == -1 ) return;

	CWordArray arrayReadWrite, arrayDummy;
	arrayReadWrite.Add( NODE_PAIDIN_TEXT );
	arrayReadWrite.Add( NODE_PAIDOUT_TEXT );

	CDataManagerInfo info;
	if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, arrayDummy, info, FALSE ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}
		
	CPaidInOutTextEditDlg dlg ( m_pParentWnd );
		
	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManager.WritePaidInText( info ) == FALSE )
			Prompter.WriteError( info );
		else if ( DataManager.WritePaidOutText( info ) == FALSE )
			Prompter.WriteError( info );
	}
	
	DataManager.CloseDatabase();
}

/**********************************************************************/

void CDataAccessHelpersEcrTexts::EditAnalysisCategory( int nDbNo )
{
	int nDbIdx = GetDbIdx ( nDbNo );
	if ( nDbIdx == -1 ) return;

	CWordArray arrayReadWrite, arrayDummy;
	arrayReadWrite.Add( NODE_ACAT );

	CDataManagerInfo info;
	if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, arrayDummy, info, FALSE ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}
		
	CListDataAnalysisCategoryDlg dlg ( m_pParentWnd );
		
	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManager.WriteAnalysisCategory( info ) == FALSE )
			Prompter.WriteError( info );
	}
	
	DataManager.CloseDatabase();
}

/**********************************************************************/

void CDataAccessHelpersEcrTexts::EditAllergen()
{
	CDataManagerInfo info;
	if ( DataManagerNonDb.OpenAllergen( DB_READWRITE, info ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}
		
	CListDataGroupDlg dlg ( NODE_ALLERGEN, 0, m_pParentWnd );
		
	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManagerNonDb.WriteAllergen( info ) == FALSE )
			Prompter.WriteError( info );
		else
			DataManagerNonDb.Allergen.WriteSMAllergenFile();
	}
	
	DataManagerNonDb.CloseAllergen( info );
}

/**********************************************************************/
