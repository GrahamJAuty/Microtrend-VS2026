/**********************************************************************/
#include "CustomExportManager.h"
#include "DatabaseCSVArray.h"
#include "EditRatesDlg.h"
#include "ListDataBarcodeDlg.h"
#include "ListDataDepartmentDlg.h"
#include "ListDataLoyaltySchemeDlg.h"
#include "ListDataSystemDepartmentDlg.h"
#include "ListDataGroupDlg.h"
#include "NodeTypes.h"
#include "SysInfo.h"
/**********************************************************************/
#include "DataAccessHelpersEcrData.h"
/**********************************************************************/

CDataAccessHelpersEcrData::CDataAccessHelpersEcrData()
{
}

/**********************************************************************/

void CDataAccessHelpersEcrData::EditBarcode ( int nDbNo )
{
	int nDbIdx = GetDbIdx ( nDbNo );
	if ( nDbIdx == -1 ) return;

	CWordArray arrayReadWrite;
	arrayReadWrite.Add( NODE_BARCODE );

	bool bWantStockLevels = CCustomExportManagerBase::WantStockLevels();

	CWordArray arrayReadOnly;
	arrayReadOnly.Add( NODE_PLU );
	arrayReadOnly.Add( NODE_MODIFIER );
	
	if ( TRUE == bWantStockLevels )
		arrayReadOnly.Add( NODE_STOCK );

	arrayReadOnly.Add( NODE_DEPARTMENT );
	arrayReadOnly.Add( NODE_DEPARTMENT_SET );

	CDataManagerInfo info;
	if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, arrayReadOnly, info, bWantStockLevels ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}

	CListDataBarcodeDlg dlg ( m_pParentWnd );
		
	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManager.WriteBarcode( info ) == FALSE )
			Prompter.WriteError( info );
		else
			CCustomExportManagerPlu CustomExport( bWantStockLevels );
	}
	
	DataManager.CloseDatabase();
}

/**********************************************************************/

void CDataAccessHelpersEcrData::EditDept ( int nDbNo )
{
	if ( ( 0 == nDbNo ) && ( EcrmanOptions.GetGlobalDepartmentFlag() == FALSE ) )
	{
		CDataManagerInfo info;
		
		if ( DataManagerNonDb.OpenSystemDepartment( DB_READWRITE, info ) == FALSE )  
		{
			Prompter.ShareError( info );
			return;
		}
		
		CListDataSystemDepartmentDlg dlg ( m_pParentWnd );
				
		if ( dlg.DoModal() == IDOK )
		{
			if ( DataManagerNonDb.WriteSystemDepartment(info) == FALSE )
				Prompter.WriteError( info );
		}

		DataManagerNonDb.CloseSystemDepartment(info);
	}
	else
	{
		int nDbIdx = GetDbIdx ( nDbNo );
		if ( nDbIdx == -1 ) return;

		CWordArray arrayReadWrite;
		arrayReadWrite.Add( NODE_DEPARTMENT );
		arrayReadWrite.Add( NODE_DEPT_GROUP_EPOS );
		arrayReadWrite.Add( NODE_DEPT_GROUP_REPORT );
		arrayReadWrite.Add( NODE_DEPT_GROUP_CONSOL );

		CWordArray arrayReadOnly;
		arrayReadOnly.Add( NODE_PLU );
		arrayReadOnly.Add( NODE_MODIFIER );

		CDataManagerInfo info;
		if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, arrayReadOnly, info, FALSE ) == FALSE )
		{
			Prompter.ShareError( info );
			return;
		}
		
		CListDataDepartmentDlg dlg ( m_pParentWnd );
			
		if ( dlg.DoModal() == IDOK )
		{
			if ( DataManager.WriteDepartment( info ) == TRUE )
			{
				DataManager.ListKeyImageWrapperDept.SaveDatabaseChanges();
			}
			else
			{
				DataManager.ListKeyImageWrapperDept.UndoDatabaseChanges();
				Prompter.WriteError( info );
			}

			//MAKE SURE THAT UPDATED LINKS FROM REPORT GROUPS
			//TO CONSOLIDATION GROUPS ARE SAVED
			if (DataManager.WriteReportGroup(info) == FALSE)
			{
				Prompter.WriteError(info);
			}

			if (SysInfo.GetLoyaltyOrSmartPayType() == LOYALTY_TYPE_LOY_V4)
			{
				CCustomExportManagerBase Export;
				Export.WriteDepartmentFile();
			}
		}
		else
		{
			DataManager.ListKeyImageWrapperDept.UndoDatabaseChanges();
		}

		DataManager.CloseDatabase();
	}
}

/**********************************************************************/

void CDataAccessHelpersEcrData::EditGroup ( int nDbNo )
{
	int nDbIdx = GetDbIdx ( nDbNo );
	if ( nDbIdx == -1 ) return;

	CWordArray arrayReadWrite, arrayDummy;
	arrayReadWrite.Add( NODE_DEPT_GROUP_EPOS );

	CDataManagerInfo info;
	if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, arrayDummy, info, FALSE ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}
		
	CListDataGroupDlg dlg ( NODE_DEPT_GROUP_EPOS, 0, m_pParentWnd );
		
	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManager.WriteEposGroup( info ) == FALSE )
			Prompter.WriteError( info );
	}
	
	DataManager.CloseDatabase();
}

/**********************************************************************/

void CDataAccessHelpersEcrData::EditTax( int nDbNo )
{
	if ( EcrmanOptions.GetGlobalTaxFlag() == TRUE )
	{
		if ( DataManager.LockAllUsers() == FALSE )
			Prompter.LockError( "edit global tax settings" );
		else
		{
			EditGlobalTax();
			DataManager.UnlockAllUsers();
		}
	}
	else
	{
		int nDbIdx = GetDbIdx ( nDbNo );
		if ( nDbIdx == -1 ) return;

		CWordArray arrayReadWrite;
		arrayReadWrite.Add( NODE_TAX );
		arrayReadWrite.Add( NODE_PLU );

		CWordArray arrayReadOnly;
		arrayReadOnly.Add( NODE_STOCK );
		arrayReadOnly.Add( NODE_MODIFIER );

		CDataManagerInfo info;
		if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, arrayReadOnly, info, FALSE ) == FALSE )
		{
			Prompter.ShareError( info );
			return;
		}

		if ( EditTaxInternal() == TRUE )
		{
			if ( DataManager.WriteTaxRates( info ) == FALSE )
				Prompter.WriteError( info );
			else
			{
				DataManager.Plu.UpdatePrices();

				if ( DataManager.WritePlu( info ) == FALSE )
					Prompter.WriteError( info );
			}
		}

		DataManager.CloseDatabase();
	}
}

/**********************************************************************/

void CDataAccessHelpersEcrData::EditGlobalTax()
{
	CWordArray arrayReadWrite, arrayDummy;
	arrayReadWrite.Add( NODE_TAX );
	
	CDataManagerInfo info;
	if ( DataManager.OpenDatabaseReadWrite( 0, arrayReadWrite, arrayDummy, info, FALSE ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}

	bool bTaxChange = EditTaxInternal();

	if ( bTaxChange == TRUE )
	{
		if ( DataManager.WriteTaxRates( info ) == FALSE )
		{
			Prompter.WriteError( info );
			bTaxChange = FALSE;
		}
	}

	DataManager.CloseDatabase();

	if ( bTaxChange == TRUE )
	{
		CWordArray arrayReadWrite;
		arrayReadWrite.Add( NODE_PLU );

		CWordArray arrayReadOnly;
		arrayReadOnly.Add( NODE_TAX );
		arrayReadOnly.Add( NODE_STOCK );
		arrayReadOnly.Add( NODE_MODIFIER );

		for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
		{
			if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, arrayReadOnly, info, FALSE ) == TRUE )
			{
				DataManager.Plu.UpdatePrices();
				DataManager.WritePlu( info );
				DataManager.CloseDatabase();
			}
		}
	}
}

/**********************************************************************/

bool CDataAccessHelpersEcrData::EditTaxInternal()
{
	CTaxArray TaxArrayBuffer; 
	for ( int nRate = 1; nRate <= MAX_TAX_BANDS; nRate++ )
	{
		CTaxRateInfo TaxRateInfo;
		DataManager.TaxRates.GetTaxRateInfo( nRate, TaxRateInfo );
		TaxArrayBuffer.SetTaxRateInfo( nRate, TaxRateInfo );
	}
	
	bool bTaxChange = FALSE;
	
	CEditRatesDlg dlg ( m_pParentWnd );	
		
	if ( dlg.DoModal() == IDOK )
	{
		for ( int nRate = 1; nRate <= MAX_TAX_BANDS; nRate++ )
		{
			CTaxRateInfo TaxRateOld, TaxRateNew;
			TaxArrayBuffer.GetTaxRateInfo( nRate, TaxRateOld );
			DataManager.TaxRates.GetTaxRateInfo( nRate, TaxRateNew );
			
			if ( ( TaxRateOld.m_dTaxRate != TaxRateNew.m_dTaxRate ) || ( TaxRateOld.m_strReportText != TaxRateNew.m_strReportText ) )
			{
				bTaxChange = TRUE;
				break;
			}
		}
	}

	return bTaxChange;
}

/**********************************************************************/

void CDataAccessHelpersEcrData::EditLoyaltyScheme(int nDbNo)
{
	int nDbIdx = GetDbIdx(nDbNo);
	if (nDbIdx == -1)
	{
		return;
	}

	CWordArray arrayReadWrite;
	arrayReadWrite.Add(NODE_LOYALTY_SCHEME);
	arrayReadWrite.Add(NODE_PROMOTION);

	CWordArray arrayReadOnly;
	arrayReadOnly.Add(NODE_DEPARTMENT);
	arrayReadOnly.Add(NODE_ACAT);
	arrayReadOnly.Add(NODE_PRICETEXT);
	arrayReadOnly.Add(NODE_PAYMENT);

	CDataManagerInfo infoDb;
	if (DataManager.OpenDatabaseReadWrite(nDbIdx, arrayReadWrite, arrayReadOnly, infoDb, FALSE) == FALSE)
	{
		Prompter.ShareError(infoDb);
		return;
	}

	CListDataLoyaltySchemeDlg dlg(m_pParentWnd);

	if (dlg.DoModal() == IDOK)
	{
		if (DataManager.WriteLoyaltyScheme(infoDb) == FALSE)
		{
			Prompter.WriteError(infoDb);
		}

		if (DataManager.WritePromotion(infoDb) == FALSE)
		{
			Prompter.WriteError(infoDb);
		}

		if (SysInfo.GetLoyaltyOrSmartPayType() == LOYALTY_TYPE_LOY_V4)
		{
			CCustomExportManagerBase Export;
			Export.WriteLoyaltySchemeFile();
		}
	}

	DataManager.CloseDatabase();
}

/**********************************************************************/


