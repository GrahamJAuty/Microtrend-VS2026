/**********************************************************************/
#include "PluCSVArray.h"
/**********************************************************************/
#include "PluDatabaseHeader.h"
/**********************************************************************/

CPluDatabaseHeader::CPluDatabaseHeader(void)
{
	CString strLabel = "";

	//DESCRIPTION AND TYPE
	AddField(Plu::PluNo.Label, "", 1);
	AddField("", "");	//protected "VersionNo"
	AddField(Plu::EposText.Label, "", 1);
	AddField(Plu::KeyText.Label, "", 1);
	AddField(Plu::RepText.Label, "");
	AddField(Plu::SortCode.Label, "");
	AddField(Plu::AgressoGLCode.Label, "");
	AddField(Plu::AgressoCostCentre.Label, "");
	AddField(Plu::AgressoProject.Label, "");
	AddField(Plu::DeptNo.Label, "");
	AddField(Plu::AnalysisCategory.Label, "PluCategory");
	AddField(Plu::MicrotrendPluType.Label, "");
	AddField(Plu::ExternalProcess.Label, "");
	AddField(Plu::TaxBand.Label, "TaxCode", 1);

	//PRICES AND POINTS
	for (int n = 1; n <= MAX_PLU_PRICES; n++)
	{
		CPluCSVRecord::GetPriceLabel(n, strLabel);
		AddField(strLabel, "", 1);
	}

	AddField(Plu::Points.Label, "");
	AddField(Plu::StampWeight.Label, "");

	//COST AND PROFIT
	AddField(Plu::PurchaseCost.Label, "");
	AddField(Plu::ReportCostEnable.Label, "");
	AddField(Plu::ReportCost.Label, "", 2);
	AddField(Plu::GPType.Label, "");
	AddField(Plu::ProfitMargin.Label, "");

	//ALLERGENS
	for (int a = 1; a <= MAX_PLU_ALLERGENS; a++)
	{
		CPluCSVRecord::GetAllergenLabel(a, strLabel);
		AddField(strLabel, "");
	}

	//KEYBOARD LISTS
	AddField(Plu::FirstMenu.Label, "", 1);
	AddField(Plu::FirstQualifier.Label, "", 1);
	AddField(Plu::FirstExtra.Label, "", 1);

	//ORDER PRINTERS
	AddField(Plu::OrderPrinterGroup.Label, "", 1);

	for (int p = 1; p <= PLU_ORDER_PRINTERS_NOW; p++)
	{
		CPluCSVRecord::GetOrderPrinterLabel(p, strLabel);
		AddField(strLabel, "", 1);
	}

	AddField(Plu::Priority.Label, "", 1);

	//MODIFIERS
	AddField(Plu::AllowModifier.Label, "");
	for (int m = 1; m <= PLU_MODIFIERS; m++)
	{
		CString strLabelOld;
		CString strLabelNew;

		CPluCSVRecord::GetModifierLabelEnable(m, FALSE, strLabelNew);
		CPluCSVRecord::GetModifierLabelEnable(m, TRUE, strLabelOld);
		AddField(strLabelNew, strLabelOld, 1);

		CPluCSVRecord::GetModifierLabelEposTextFlag(m, FALSE, strLabelNew);
		CPluCSVRecord::GetModifierLabelEposTextFlag(m, TRUE, strLabelOld);
		AddField(strLabelNew, strLabelOld, 1);

		CPluCSVRecord::GetModifierLabelEposText(m, FALSE, strLabelNew);
		CPluCSVRecord::GetModifierLabelEposText(m, TRUE, strLabelOld);
		AddField(strLabelNew, strLabelOld, 1);

		CPluCSVRecord::GetModifierLabelKeyTextFlag(m, FALSE, strLabelNew);
		CPluCSVRecord::GetModifierLabelKeyTextFlag(m, TRUE, strLabelOld);
		AddField(strLabelNew, strLabelOld, 1);

		CPluCSVRecord::GetModifierLabelKeyText(m, FALSE, strLabelNew);
		CPluCSVRecord::GetModifierLabelKeyText(m, TRUE, strLabelOld);
		AddField(strLabelNew, strLabelOld, 1);

		CPluCSVRecord::GetModifierLabelDeptNo(m, strLabelNew);
		AddField(strLabelNew, "");

		CPluCSVRecord::GetModifierLabelAnalysisCategory(m, strLabelNew);
		AddField(strLabelNew, "");

		CPluCSVRecord::GetModifierLabelMicrotrendPluType(m, strLabelNew);
		AddField(strLabelNew, "");

		CPluCSVRecord::GetModifierLabelTaxBand(m, FALSE, strLabelNew);
		CPluCSVRecord::GetModifierLabelTaxBand(m, TRUE, strLabelOld);
		AddField(strLabelNew, strLabelOld, 1);

		for (int p = 1; p <= MAX_PLU_PRICES; p++)
		{
			CPluCSVRecord::GetModifierLabelPrice(m, p, FALSE, strLabelNew);
			CPluCSVRecord::GetModifierLabelPrice(m, p, TRUE, strLabelOld);
			AddField(strLabelNew, strLabelOld, 1);
		}

		CPluCSVRecord::GetModifierLabelFirstMenu(m, strLabelNew);
		AddField(strLabelNew, "", 1);

		CPluCSVRecord::GetModifierLabelFirstQualifier(m, strLabelNew);
		AddField(strLabelNew, "", 1);

		CPluCSVRecord::GetModifierLabelFirstExtra(m, strLabelNew);
		AddField(strLabelNew, "", 1);

		CPluCSVRecord::GetModifierLabelOrderPrinterGroup(m, strLabelNew);
		AddField(strLabelNew, "", 1);
	}

	//MISC PROGRAMMING
	AddField(Plu::PriceType.Label, "");
	AddField(Plu::AgeVerificationType.Label, "");
	AddField(Plu::KPNotAlone.Label, "");
	AddField(Plu::KPConsolidateTopItem.Label, "");
	AddField(Plu::KPImagePrint.Label, "");
	AddField(Plu::KPImageModifier.Label, "");
	AddField(Plu::AutoCoversQty.Label, "");
	AddField(Plu::EANOnReceipt.Label, "");
	AddField(Plu::SalesTracking.Label, "");
	AddField(Plu::SoldOut.Label, "");
	AddField(Plu::Discountable.Label, "");
	AddField(Plu::StaffDiscount.Label, "");
	AddField(Plu::FreeText.Label, "");
	AddField(Plu::FreeTextDescription.Label, "");
	AddField(Plu::AccountRequired.Label, "");
	
	//LABELS
	AddField(Plu::PrintShelf.Label, "");
	AddField(Plu::PrintProduct.Label, "");
	AddField(Plu::UnitPriceText.Label, "");
	AddField(Plu::UnitPriceSize.Label, "");

	//ECRMAN STOCK
	AddField(Plu::ApparentLevel.Label, "");
	AddField(Plu::MinimumLevel.Label, "");
	AddField(Plu::SupplierRef.Label, "");
	AddField(Plu::SupplierNo.Label, "");
	AddField(Plu::NonStockItem.Label, "");

	//MISC
	AddField("", "");	//protected "PluInfo Modifier Flag"	
	AddField(Plu::LinkType.Label, "", 1);
	AddField(Plu::LinkSellTiedItem.Label, "", 1);
	AddField(Plu::LinkTable.Label, "", 1);
	AddField(Plu::EcrmanRecipeTable.Label, "");
	AddField(Plu::ImageFilename.Label, "");
	AddField(Plu::ActiveItem.Label, "");

	//INTERNAL FIELDS
	AddField("", "");	//protected "ModifiedDateTime(Plu)"
	AddField("", "");	//protected "ModifiedDateTime(Info)"
	AddField("", "");	//protected "DownloadDate(Plu)"
	AddField("", "");	//protected "Export"

	//NON DATABASE FIELDS
	AddField("DiscountType", "");

	for (int n = 0; n < m_mapLegacyFields.GetSize(); n++)
	{
		CSortedIntByString item;
		m_mapLegacyFields.GetAt(n, item);
		m_arrayCurrentFields[item.m_nItem].m_nLegacyMapIdx = n;
	}
}

/**********************************************************************/

void CPluDatabaseHeader::AddField( const char* szCurrent, const char* szLegacy, int nPriceSetStatus )
{
	CString strLegacy = szLegacy;
	if ( strLegacy != "" )
	{
		CSortedIntByString item;
		item.m_nItem = m_arrayCurrentFields.GetSize();
		
		item.m_strItem = strLegacy;
		m_mapLegacyFields.ConsolidateWithHint( item, 1 );

		item.m_strItem = szCurrent;
		m_mapCurrentFields.ConsolidateWithHint( item, 1 );
	}

	CPluDatabaseHeaderField item;
	item.m_strCurrent = szCurrent;
	item.m_nPriceSetStatus = nPriceSetStatus;
	item.m_nLegacyMapIdx = -1;
	item.m_bHeaderTag = FALSE;
	m_arrayCurrentFields.Add( item );
}

/**********************************************************************/

void CPluDatabaseHeader::TagLegacyFields( const char* szFilename )
{
	for (int n = 0; n < m_arrayCurrentFields.GetSize(); n++)
	{
		m_arrayCurrentFields[n].m_bHeaderTag = FALSE;
	}
	
	CSSFile file;
	if ( file.Open( szFilename, "rb" ) == TRUE )
	{
		CString strBuffer;
		file.ReadString( strBuffer );
		file.Close();

		CCSV csv( strBuffer );

		int nPos = 0;
		CSortedIntByString item;

		for ( int n = 0; n < csv.GetSize(); n++ )
		{
			item.m_strItem = csv.GetString(n);

			if (m_mapLegacyFields.FindWithHint(item, 1, nPos) == TRUE)
			{
				m_mapLegacyFields.GetAt(nPos, item);
				if ((item.m_nItem >= 0) && (item.m_nItem < m_arrayCurrentFields.GetSize()))
				{
					m_arrayCurrentFields[item.m_nItem].m_bHeaderTag = TRUE;
				}
			}
		}
	}
}

/**********************************************************************/

const char* CPluDatabaseHeader::ReplaceLegacyFields( const char* szHeader )
{
	CCSV csvIn( szHeader );
	CCSV csvOut;

	for( int n = 0; n < csvIn.GetSize(); n++ )
	{
		CString strField = csvIn.GetString(n);

		CSortedIntByString item;
		item.m_strItem = strField;

		int nPos = 0;
		if ( m_mapCurrentFields.FindWithHint( item, 1, nPos ) == TRUE )
		{
			m_mapCurrentFields.GetAt( nPos, item );

			if ( ( item.m_nItem >= 0 ) && ( item.m_nItem < m_arrayCurrentFields.GetSize() ) )
			{
				if ( TRUE == m_arrayCurrentFields[ item.m_nItem ].m_bHeaderTag )
				{
					int nLegacyIdx = m_arrayCurrentFields[ item.m_nItem ].m_nLegacyMapIdx;

					if ((nLegacyIdx >= 0) && (nLegacyIdx < m_mapLegacyFields.GetSize()))
					{
						m_mapLegacyFields.GetAt(nLegacyIdx, item);

						if (item.m_strItem != "")
						{
							strField = item.m_strItem;
						}
					}
				}
			}
		}

		csvOut.Add( strField );
	}

	m_strHeader = csvOut.GetLine();
	return m_strHeader;
}

/**********************************************************************/

const char* CPluDatabaseHeader::GetDatabaseHeader()
{
	CCSV csv;
	for (int n = 0; n < m_arrayCurrentFields.GetSize(); n++)
	{
		csv.Add(m_arrayCurrentFields[n].m_strCurrent);
	}

	m_strHeader = csv.GetLine();
	return m_strHeader;
}

/**********************************************************************/

const char* CPluDatabaseHeader::GetPriceSetHeader()
{
	CCSV csv;
	for ( int n = 0; n < m_arrayCurrentFields.GetSize(); n++ )
	{
		switch( m_arrayCurrentFields[n].m_nPriceSetStatus )
		{
		case 1:
			csv.Add( m_arrayCurrentFields[n].m_strCurrent );
			break;

		case 2:
			if ( DealerFlags.GetReportCostPriceFlag() == TRUE )
				csv.Add( m_arrayCurrentFields[n].m_strCurrent );
			break;
		}
	}

	m_strHeader = csv.GetLine();
	return m_strHeader;
}

/**********************************************************************/

const char* CPluDatabaseHeader::GetLegacyHeader()
{
	CCSV csv;
	for (int n = 0; n < m_arrayCurrentFields.GetSize(); n++)
	{
		CString strField = "";

		int nLegacyIdx = m_arrayCurrentFields[n].m_nLegacyMapIdx;
		if ((nLegacyIdx >= 0) && (nLegacyIdx < m_mapLegacyFields.GetSize()))
		{
			CSortedIntByString item;
			m_mapLegacyFields.GetAt(nLegacyIdx, item);
			strField = item.m_strItem;
		}

		if (strField != "")
		{
			csv.Add(strField);
		}
		else
		{
			csv.Add(m_arrayCurrentFields[n].m_strCurrent);
		}
	}

	m_strHeader = csv.GetLine();
	return m_strHeader;
}

/**********************************************************************/

const char* CPluDatabaseHeader::GetLegacyHeader( const char* szHeader )
{
	for (int n = 0; n < m_arrayCurrentFields.GetSize(); n++)
	{
		m_arrayCurrentFields[n].m_bHeaderTag = TRUE;
	}

	return ReplaceLegacyFields( szHeader );
}

/**********************************************************************/

const char* CPluDatabaseHeader::GetCompatibilityHeader( const char* szFilename )
{
	TagLegacyFields( szFilename );

	CCSV csv;
	for (int n = 0; n < m_arrayCurrentFields.GetSize(); n++)
	{
		CString strField = "";

		if (TRUE == m_arrayCurrentFields[n].m_bHeaderTag)
		{
			int nLegacyIdx = m_arrayCurrentFields[n].m_nLegacyMapIdx;
			if ((nLegacyIdx >= 0) && (nLegacyIdx < m_mapLegacyFields.GetSize()))
			{
				CSortedIntByString item;
				m_mapLegacyFields.GetAt(nLegacyIdx, item);
				strField = item.m_strItem;
			}
		}

		if (strField != "")
		{
			csv.Add(strField);
		}
		else
		{
			csv.Add(m_arrayCurrentFields[n].m_strCurrent);
		}
	}

	m_strHeader = csv.GetLine();
	return m_strHeader;
}

/**********************************************************************/

const char* CPluDatabaseHeader::GetCompatibilityHeader( const char* szFilename, const char* szHeader )
{
	TagLegacyFields( szFilename );
	return ReplaceLegacyFields( szHeader );
}

/**********************************************************************/

const char* CPluDatabaseHeader::GetUpdatedHeader( const char* szHeader )
{
	CCSV csvIn( szHeader );
	CCSV csvOut;

	for( int n = 0; n < csvIn.GetSize(); n++ )
	{
		CString strField = csvIn.GetString(n);

		CSortedIntByString item;
		item.m_strItem = strField;

		int nPos;
		if (m_mapLegacyFields.FindWithHint(item, 1, nPos) == TRUE)
		{
			m_mapLegacyFields.GetAt(nPos, item);

			if ((item.m_nItem >= 0) && (item.m_nItem <= m_arrayCurrentFields.GetSize()))
			{
				if (m_arrayCurrentFields[item.m_nItem].m_strCurrent != "")
				{
					strField = m_arrayCurrentFields[item.m_nItem].m_strCurrent;
				}
			}
		}
	
		csvOut.Add( strField );
	}

	m_strHeader = csvOut.GetLine();
	return m_strHeader;
}

/**********************************************************************/

