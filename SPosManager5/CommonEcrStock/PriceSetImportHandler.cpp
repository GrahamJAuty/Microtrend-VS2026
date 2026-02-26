/**********************************************************************/
#include "ImportDefines.h"
#include "PluDatabaseHeader.h"
#include "PriceSetItemManager.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "PriceSetImportHandler.h"
/**********************************************************************/

CPriceSetImportHandler::CPriceSetImportHandler()
{
	ResetCounters();
}

/**********************************************************************/

void CPriceSetImportHandler::ResetCounters()
{
	m_nImportTotalLines = 0;
	m_nImportIgnoredLines = 0;
	m_nImportPluModified = 0;
}

/**********************************************************************/

int CPriceSetImportHandler::ImportFile( const char* szFilename, bool bCanSetStatus )
{
	ResetCounters();
	
	CString strFilename = szFilename;
	CSSImportFile fileImport ( PluDatabaseHeader.GetCompatibilityHeader( strFilename ) );
	if (fileImport.Open(strFilename, "rb") == FALSE)
	{
		return IMPORTERROR_PLU_OPENFILE;
	}

	StatusProgress.Lock( TRUE, "Importing price set data" );
	
	//CREATE A TEMPORARY BUFFER TO HOLD THE IMPORT
	CPluCSVArray arrayPluImport;
	int nSize = DataManager.Plu.GetSize();
	for ( int nPluIdx = 0; nPluIdx < nSize; nPluIdx++ )
	{
		StatusProgress.SetPos( nPluIdx, nSize, 0, 20 );

		bool bAllowItem = FALSE;
		switch (PriceSetItemManager.GetPluStatus(nPluIdx))
		{
		case LOCALPLU_LOCAL:
			bAllowItem = TRUE;
			break;

		case LOCALPLU_MASTER:
			bAllowItem = bCanSetStatus;
			break;

		case LOCALPLU_EXCLUDE:
			bAllowItem = FALSE;
			break;
		}

		if ( TRUE == bAllowItem )
		{
			CPluCSVRecord PluRecord;
			DataManager.Plu.GetAt( nPluIdx, PluRecord );
			arrayPluImport.Add( PluRecord );
		}
	}

	//IMPORT INTO THE TEMPORARY BUFFER
	CString strBuffer = "";
	while ( fileImport.ReadData ( strBuffer ) == TRUE )
	{
		StatusProgress.SetPos( fileImport.GetPercentPosition(), 100, 20, 40 );

		int nImportPos = 0;
		int nResult = arrayPluImport.ImportLine ( IMPORT_METHOD_EXIST, nImportPos, fileImport, strBuffer );

		switch ( nResult )
		{
		case IMPORT_LINE_MODIFIED:
			m_nImportTotalLines++;
			m_nImportPluModified++;

			if (TRUE == bCanSetStatus)
			{
				int nPluIdx = 0;
				CCSV csvImport(strBuffer, ',', '"', 1);
				__int64 nPluNo = csvImport.GetInt64(0);

				if (DataManager.Plu.FindPluByNumber(nPluNo, nPluIdx) == TRUE)
				{
					PriceSetItemManager.SetPluStatus(nPluIdx, LOCALPLU_LOCAL);
				}
			}

			break;
				
		case IMPORT_LINE_IGNORED:
			m_nImportTotalLines++;
			m_nImportIgnoredLines++;
			break;
		}
	}

	//WORK OUT WHICH FIELDS WE NEED TO IMPORT
	bool bImportPrice[MAX_PLU_PRICES];
	bool bImportModifierEnable[10];
	bool bImportModifierEposTextFlag[10];
	bool bImportModifierEposText[10];
	bool bImportModifierKeyTextFlag[10];	
	bool bImportModifierKeyText[10];
	bool bImportModifierFirstMenu[10];
	bool bImportModifierFirstQualifier[10];
	bool bImportModifierFirstExtra[10];
	bool bImportModifierPrice[10][MAX_PLU_PRICES];
	bool bImportModifierOrderPrinterGroup[10];
	bool bImportModifierTaxBand[10];
	bool bImportOrderPrinter[PLU_ORDER_PRINTERS_NOW];
	bool bImportPriority = FALSE;
	bool bImportEPOSText = FALSE;
	bool bImportRepText = FALSE;
	bool bImportFirstMenu = FALSE;
	bool bImportFirstExtra = FALSE;
	bool bImportFirstQualifier = FALSE;
	bool bImportLinkType = FALSE;
	bool bImportLinkSellTiedItems = FALSE;
	bool bImportLinkTable = FALSE;
	bool bImportReportCost = FALSE;
	bool bImportTaxBand = FALSE;
	
	for ( int nLevel = 0; nLevel < MAX_PLU_PRICES; nLevel++ )
	{
		CString strLabelPrice;
		CPluCSVRecord::GetPriceLabel( nLevel + 1, strLabelPrice );
		bImportPrice[ nLevel ] = PriceSetItemManager.CheckField( strLabelPrice );
	}

	for ( int nMod = 1; nMod < 10; nMod++ )
	{
		CString strLabel;
		CPluCSVRecord::GetModifierLabelEnable( nMod, FALSE, strLabel );
		bImportModifierEnable[nMod] = PriceSetItemManager.CheckField( strLabel );
		
		CPluCSVRecord::GetModifierLabelEposTextFlag( nMod, FALSE, strLabel );
		bImportModifierEposTextFlag[nMod] = PriceSetItemManager.CheckField( strLabel );
		CPluCSVRecord::GetModifierLabelEposText( nMod, FALSE, strLabel );
		bImportModifierEposText[nMod] = PriceSetItemManager.CheckField( strLabel );
		
		CPluCSVRecord::GetModifierLabelKeyTextFlag( nMod, FALSE, strLabel );
		bImportModifierKeyTextFlag[nMod] = PriceSetItemManager.CheckField( strLabel );
		CPluCSVRecord::GetModifierLabelKeyText( nMod, FALSE, strLabel );
		bImportModifierKeyText[nMod] = PriceSetItemManager.CheckField( strLabel );

		CPluCSVRecord::GetModifierLabelFirstMenu( nMod, strLabel );
		bImportModifierFirstMenu[nMod] = PriceSetItemManager.CheckField( strLabel );

		CPluCSVRecord::GetModifierLabelFirstQualifier( nMod, strLabel );
		bImportModifierFirstQualifier[nMod] = PriceSetItemManager.CheckField( strLabel );

		CPluCSVRecord::GetModifierLabelFirstExtra( nMod, strLabel );
		bImportModifierFirstExtra[nMod] = PriceSetItemManager.CheckField( strLabel );

		CPluCSVRecord::GetModifierLabelOrderPrinterGroup(nMod, strLabel);
		bImportModifierOrderPrinterGroup[nMod] = PriceSetItemManager.CheckField(strLabel);

		CPluCSVRecord::GetModifierLabelTaxBand(nMod, FALSE, strLabel);
		bImportModifierTaxBand[nMod] = PriceSetItemManager.CheckField(strLabel);

		for ( int nLevel = 0; nLevel < MAX_PLU_PRICES; nLevel++ )
		{
			CPluCSVRecord::GetModifierLabelPrice( nMod, nLevel + 1, FALSE, strLabel );
			bImportModifierPrice[nMod][nLevel] = PriceSetItemManager.CheckField( strLabel );
		}
	}

	for ( int nOp = 0; nOp < PLU_ORDER_PRINTERS_NOW; nOp++ )
	{
		CString strLabel;
		CPluCSVRecord::GetOrderPrinterLabel( nOp + 1, strLabel );
		bImportOrderPrinter[nOp] = PriceSetItemManager.CheckField( strLabel );
	}

	bImportPriority = PriceSetItemManager.CheckField( Plu::Priority.Label );
	bImportEPOSText = PriceSetItemManager.CheckField( Plu::EposText.Label );
	bImportRepText = PriceSetItemManager.CheckField( Plu::RepText.Label );
	bImportFirstMenu = PriceSetItemManager.CheckField( Plu::FirstMenu.Label );
	bImportFirstExtra = PriceSetItemManager.CheckField( Plu::FirstExtra.Label );
	bImportFirstQualifier = PriceSetItemManager.CheckField( Plu::FirstQualifier.Label );
	bImportLinkType = PriceSetItemManager.CheckField( Plu::LinkType.Label );
	bImportLinkSellTiedItems = PriceSetItemManager.CheckField(Plu::LinkSellTiedItem.Label);
	bImportLinkTable = PriceSetItemManager.CheckField( Plu::LinkTable.Label );
	bImportReportCost = PriceSetItemManager.CheckField( Plu::ReportCost.Label );
	bImportTaxBand = PriceSetItemManager.CheckField(Plu::TaxBand.Label);
	
	//NOW COPY THE PRICE SET FIELDS INTO THE ACTUAL DATABASE
	nSize = arrayPluImport.GetSize();
	for ( int nImportIdx = 0; nImportIdx < nSize; nImportIdx++ )
	{
		StatusProgress.SetPos( nImportIdx, nSize, 60, 40 );

		CPluCSVRecord PluRecordImport;
		arrayPluImport.GetAt( nImportIdx, PluRecordImport );
		CString strPluNo = arrayPluImport.GetPluNoString( nImportIdx );
		
		int nPluIdx;
		if (DataManager.Plu.FindPluByNumber(_atoi64(strPluNo), nPluIdx) == FALSE)
		{
			continue;
		}

		CPluCSVRecord PluRecordPriceSet;
		DataManager.Plu.GetAt( nPluIdx, PluRecordPriceSet );

		for (int nLevel = 0; nLevel < MAX_PLU_PRICES; nLevel++)
		{
			if (TRUE == bImportPrice[nLevel])
			{
				PluRecordPriceSet.SetPrice(nLevel, PluRecordImport.GetPrice(nLevel));
			}
		}
		
		for ( int nMod = 1; nMod < 10; nMod++ )
		{
			if ( TRUE == bImportModifierEnable[nMod] )
			{
				PluRecordPriceSet.SetModifierEnable( nMod, PluRecordImport.GetModifierEnable( nMod ) );
			}

			if ( TRUE == bImportModifierEposTextFlag[nMod] )
			{
				CString strText = PluRecordPriceSet.GetModifierEposText( nMod );
				PluRecordPriceSet.SetModifierEposText( nMod, PluRecordImport.GetModifierEposTextFlag(nMod), strText );
			}

			if ( ( TRUE == bImportModifierEposText[nMod] ) && ( PluRecordPriceSet.GetModifierEposTextFlag( nMod ) == TRUE ) )
			{
				PluRecordPriceSet.SetModifierEposText( nMod, TRUE, PluRecordImport.GetModifierEposText( nMod ) );
			}

			if ( TRUE == bImportModifierKeyTextFlag[nMod] )
			{
				CString strText = PluRecordPriceSet.GetModifierKeyText( nMod );
				PluRecordPriceSet.SetModifierKeyText( nMod, PluRecordImport.GetModifierKeyTextFlag(nMod), strText );
			}

			if ( ( TRUE == bImportModifierKeyText[nMod] ) && ( PluRecordPriceSet.GetModifierKeyTextFlag( nMod ) == TRUE ) )	
			{
				PluRecordPriceSet.SetModifierKeyText( nMod, TRUE, PluRecordImport.GetModifierKeyText( nMod ) );
			}

			if ( TRUE == bImportModifierFirstMenu[nMod] )
			{
				PluRecordPriceSet.SetModifierFirstMenu( nMod, PluRecordImport.GetModifierFirstMenu(nMod) );
			}

			if ( TRUE == bImportModifierFirstQualifier[nMod] )
			{
				PluRecordPriceSet.SetModifierFirstQualifier( nMod, PluRecordImport.GetModifierFirstQualifier(nMod) );
			}

			if ( TRUE == bImportModifierFirstExtra[nMod] )
			{
				PluRecordPriceSet.SetModifierFirstExtra( nMod, PluRecordImport.GetModifierFirstExtra(nMod) );
			}

			if (TRUE == bImportModifierOrderPrinterGroup[nMod])
			{
				PluRecordPriceSet.SetModifierOrderPrinterGroup(nMod, PluRecordImport.GetModifierOrderPrinterGroup(nMod));
			}

			if (TRUE == bImportModifierTaxBand[nMod])
			{
				PluRecordPriceSet.SetModifierTaxBandStringRaw(nMod, PluRecordImport.GetModifierTaxBandStringRaw(nMod));
			}

			for ( int nLevel = 0; nLevel < MAX_PLU_PRICES; nLevel++ )
			{
				if ( TRUE == bImportModifierPrice[nMod][nLevel] )
					PluRecordPriceSet.SetModifierPrice( nMod, nLevel, PluRecordImport.GetModifierPrice( nMod, nLevel ) );
			}
		}

		for (int nOp = 0; nOp < PLU_ORDER_PRINTERS_NOW; nOp++)
		{
			if (TRUE == bImportOrderPrinter[nOp])
				PluRecordPriceSet.SetOrderPrinterFlag(nOp, PluRecordImport.GetOrderPrinterFlag(nOp));
		}

		if (TRUE == bImportPriority)
		{
			PluRecordPriceSet.SetPriority(PluRecordImport.GetPriority());
		}

		if (TRUE == bImportEPOSText)
		{
			PluRecordPriceSet.SetEposText(PluRecordImport.GetEposText());
		}

		if (TRUE == bImportRepText)
		{
			PluRecordPriceSet.SetRepText(PluRecordImport.GetRepText());
		}

		if (TRUE == bImportFirstMenu)
		{
			PluRecordPriceSet.SetFirstMenu(PluRecordImport.GetFirstMenu());
		}

		if (TRUE == bImportFirstExtra)
		{
			PluRecordPriceSet.SetFirstExtra(PluRecordImport.GetFirstExtra());
		}

		if (TRUE == bImportFirstQualifier)
		{
			PluRecordPriceSet.SetFirstQualifier(PluRecordImport.GetFirstQualifier());
		}

		if (TRUE == bImportLinkType)
		{
			PluRecordPriceSet.SetLinkType(PluRecordImport.GetLinkType());
		}

		if (TRUE == bImportLinkSellTiedItems)
		{
			PluRecordPriceSet.SetLinkSellTiedItemFlag(PluRecordImport.GetLinkSellTiedItemFlag());
		}

		if (TRUE == bImportLinkTable)
		{
			PluRecordPriceSet.SetLinkTableString(PluRecordImport.GetLinkTableString());
		}

		if (TRUE == bImportReportCost)
		{
			PluRecordPriceSet.SetReportCost(PluRecordImport.GetReportCost());
		}

		if (TRUE == bImportTaxBand)
		{
			PluRecordPriceSet.SetBaseTaxBandString(PluRecordImport.GetBaseTaxBandString());
		}

		DataManager.Plu.SetAt( nPluIdx, PluRecordPriceSet );
	}

	StatusProgress.Unlock();	
	return IMPORTERROR_NONE;
}

/**********************************************************************/

