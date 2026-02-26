/**********************************************************************/
#include "CCSVReportLine.h"
#include "DatabaseCSVArray.h"
#include "..\SPosStockManager5\PluPrtStockLinkDlg.h"
#include "PriceHelpers.h"
#include "PriceSetItemManager.h"
#include "ReportHelpers.h"
#include "Repspawn.h"
#include "StatusProgress.h"
#include "SysInfo.h"
#include "TaxArray.h"
/**********************************************************************/
#include "ReportPluCustomList.h"
/**********************************************************************/
static const int REPORT_OKAY = 0;
static const int REPORT_ERROR_RANGE = 1;
static const int REPORT_ERROR_FILE = 2;
/**********************************************************************/

CReportPluCustomList::CReportPluCustomList( int nType, CPluFilterArray& FilterArray, CKeyboardFilterPluArray& arrayPluKbd, const char* szKey ) : m_FilterArray ( FilterArray ), m_PluKbdArray( arrayPluKbd )
{
	m_ChangesDate = COleDateTime::GetCurrentTime();
	m_strCustomName = "";

	m_nType = nType;

	switch ( m_nType )
	{
	case PLUCUSTOMLIST_PRICE:
		{
			CFilenameUpdater FnUp( SysFiles::PluPriceList );
			m_strParamsFilename = FnUp.GetFilenameToUse();
		}
		m_strTitle = "Plu Information";
		break;

	case PLUCUSTOMLIST_PROGRAM:
		{
			CFilenameUpdater FnUp( SysFiles::PluProgramming );
			m_strParamsFilename = FnUp.GetFilenameToUse();
		}
		m_strTitle = "Plu Programming";
		break;

	case PLUCUSTOMLIST_FULL:
	default:
		{
			CFilenameUpdater FnUp( SysFiles::PluCustomList );
			m_strParamsFilename = FnUp.GetFilenameToUse();m_strTitle = "Plu Custom List";
		}
		break;
	}
	m_strKey = szKey;
	m_bSortByDept = FALSE;
	m_bZeroPriced = FALSE;
	m_bSeparate = FALSE;
	m_bModifiers = FALSE;
}

/**********************************************************************/

void CReportPluCustomList::CreateReport()
{
	StatusProgress.Lock( TRUE, "Creating report" );
	int nResult = CreateReportInternal();
	StatusProgress.Unlock();

	switch ( nResult )
	{
	case 0:
		{
			CReportManager ReportManager;
			ReportManager.DisplayReport ( Super.ReportFilename(), m_strParamsFilename, m_strKey, NULL );
		}
		break;

	case 1:
		Prompter.Error ( "There are no Plus in the selected range" );
		break;

	case 2:
		Prompter.Error ( "Unable to create Plu report file" );
		break;
	}
}

/**********************************************************************/

int CReportPluCustomList::CreateReportInternal()
{
	__int64 nPluFrom = _atoi64( m_strPluFrom );
	__int64 nPluTo = _atoi64( m_strPluTo ); 

	int nStartIdx, nRange;
	m_FilterArray.GetArrayIdxRange( nPluFrom, nPluTo, nStartIdx, nRange );
		
	if ( nRange == 0 )
		return REPORT_ERROR_RANGE;				//no plus in range

	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return REPORT_ERROR_FILE;				//unable to create file
	
	CString strLocation;
	int nDbIdx = DataManager.GetActiveDbIdx();
	
	if ( PriceSetItemManager.IsPluMaster() == TRUE )
	{
		strLocation.Format ( " (%s, changes since %2.2d/%2.2d/%4.4d)", 
			dbDatabase.GetName( nDbIdx ),
			m_ChangesDate.GetDay(),
			m_ChangesDate.GetMonth(),
			m_ChangesDate.GetYear() );
	}
	else
		strLocation.Format ( " (%s, %s)", 
			dbDatabase.GetName( nDbIdx ), 
			PriceSetItemManager.GetPriceSetNameAndNumber() );

	CString strTitle = m_strTitle;

	if ( m_nType == PLUCUSTOMLIST_FULL )
	{
		CString strExtra;
		strExtra.Format ( " (%s)", (const char*) m_strCustomName );
		strTitle += strExtra;
	}

	strTitle += strLocation;

	m_ReportFile.SetStyle1 ( strTitle );
	CreateHeader();
	
	CDepartmentSetCSVRecord DepartmentSet;
	DepartmentSet.CreateDepartmentSet ( m_nDepartmentFilter );

	CString strChangesDate;
	strChangesDate.Format ( "%4.4d%2.2d%2.2d",
		m_ChangesDate.GetYear(),
		m_ChangesDate.GetMonth(),
		m_ChangesDate.GetDay() );

	CReportConsolidationArray<CSortedIntAndInt> ConsolArray;

	for ( int nStep = 0; nStep < nRange; nStep++ )	
	{ 
		int nMultiplyBy = ( m_bSortByDept == TRUE ) ? 50 : 100;
		StatusProgress.SetPos ( nStep, nRange, 0, nMultiplyBy );
	
		CPluCSVRecord PluRecord;
		int nPluIdx = m_FilterArray.GetPluItemIndexAt ( nStartIdx + nStep );
		DataManager.Plu.GetAt ( nPluIdx, PluRecord );

		__int64 nPluNo = PluRecord.GetPluNoInt();
		if ( ( nPluNo < nPluFrom ) || ( nPluNo > nPluTo ) )
			continue;

		if ( m_PluKbdArray.IsFiltered() == TRUE )
			if ( m_PluKbdArray.CheckPluNo( nPluNo ) == FALSE )
				continue;
						
		if ( DepartmentSet.GetDepartmentStatus ( PluRecord.GetBaseDeptNo() ) == FALSE )
			continue;

		if ( ( PluRecord.GetPrice ( 0 ) == 0.0 ) && ( m_bZeroPriced == FALSE ) )
			continue;
	
		//FIDDLE - WIZARD DATA IMPORTS BEFORE V1.14 DID NOT SET THE DOWNLOAD
		//DATE FIELD IN THE PLU RECORD, SO MODIFIED DATE MAY BE BLANK
		if ( m_bNoDateFilter == FALSE )
		{
			CString strDateTime = PluRecord.GetModifiedDateTimePlu();
	
			if ( strDateTime.GetLength() < 8 )
				continue;

			if ( strChangesDate > strDateTime.Left(8) )
				continue;
		}

		if ( m_bSortByDept == TRUE )
		{
			CSortedIntAndInt item;
			item.m_nInt1 = PluRecord.GetBaseDeptNo();
			item.m_nInt2 = nStartIdx + nStep;
			ConsolArray.Consolidate( item );
		}
		else
			CreateReportLine ( PluRecord );
	}
	
	nRange = ConsolArray.GetSize();
	int nCurrentDept = -1;

	for ( int nStep = 0; nStep < nRange; nStep++ )
	{
		StatusProgress.SetPos ( nStep, nRange, 50, 50 );

		CSortedIntAndInt item;
		ConsolArray.GetAt( nStep, item );

		int nPluIdx = m_FilterArray.GetPluItemIndexAt ( item.m_nInt2 );
		
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt ( nPluIdx, PluRecord );
		
		if ( item.m_nInt1 != nCurrentDept )		//Check for new department
		{
			if ( nCurrentDept != -1 )
				m_ReportFile.WriteLine ( "" );

			nCurrentDept = item.m_nInt1;
			CString strTemp;
			strTemp.Format ( "<..>D%2.2d %s", nCurrentDept, DataManager.Department.GetReportTextByDeptNo ( nCurrentDept ) );
			m_ReportFile.WriteLine ( strTemp );
			if ( m_bSeparate ) m_ReportFile.WriteLine ( "<LI>" );
		}
		CreateReportLine ( PluRecord );
	}

	m_ReportFile.WriteLine ( "" );	
	m_ReportFile.Close();
	return 0;			//function completed properly
}

/**********************************************************************/

void CReportPluCustomList::AddFieldToHeader ( CustomPluField Field )
{
	CString strLabel = Field.ReportLabel;
	
	if ( strLabel == "" )
	{
		if ( Field.Type == CustomPluList::SortCode.Type )
			strLabel = DealerFlags.GetSortCodeLabel();
		else if ( Field.Type == CustomPluList::TaxBand.Type )
			strLabel = EcrmanOptions.GetSalesTaxName();
		else
			strLabel = Field.ListLabel;
	}
	
	m_ReportFile.AddColumn ( strLabel, Field.Orientation, Field.DefSize );
}

/**********************************************************************/

void CReportPluCustomList::CreateHeader()
{
	CCSV csvKey(m_strKey);
	for (int nPos = 0; nPos < csvKey.GetSize(); nPos++)
	{
		int nField = csvKey.GetInt(nPos);
		if (nField == CustomPluList::PluNo.Type)						AddFieldToHeader(CustomPluList::PluNo);
		else if (nField == CustomPluList::RepText.Type)				AddFieldToHeader(CustomPluList::RepText);
		else if (nField == CustomPluList::EposText.Type)				AddFieldToHeader(CustomPluList::EposText);
		else if (nField == CustomPluList::KeyText.Type)				AddFieldToHeader(CustomPluList::KeyText);
		else if (nField == CustomPluList::SortCode.Type)				AddFieldToHeader(CustomPluList::SortCode);
		else if (nField == CustomPluList::ImageFilename.Type)			AddFieldToHeader(CustomPluList::ImageFilename);
		else if (nField == CustomPluList::DeptNo.Type)				AddFieldToHeader(CustomPluList::DeptNo);
		else if (nField == CustomPluList::TaxBand.Type)				AddFieldToHeader(CustomPluList::TaxBand);
		else if (nField == CustomPluList::PurchaseCost.Type)			AddFieldToHeader(CustomPluList::PurchaseCost);
		else if (nField == CustomPluList::ReportCost.Type)			AddFieldToHeader(CustomPluList::ReportCost);
		else if (nField == CustomPluList::UnitPrice.Type)				AddFieldToHeader(CustomPluList::UnitPrice);
		else if (nField == CustomPluList::GP.Type)					AddFieldToHeader(CustomPluList::GP);
		else if (nField == CustomPluList::GPPercent.Type)				AddFieldToHeader(CustomPluList::GPPercent);
		else if (nField == CustomPluList::Price1.Type)				AddFieldToHeader(CustomPluList::Price1);
		else if (nField == CustomPluList::Price2.Type)				AddFieldToHeader(CustomPluList::Price2);
		else if (nField == CustomPluList::Price3.Type)				AddFieldToHeader(CustomPluList::Price3);
		else if (nField == CustomPluList::Price4.Type)				AddFieldToHeader(CustomPluList::Price4);
		else if (nField == CustomPluList::Price5.Type)				AddFieldToHeader(CustomPluList::Price5);
		else if (nField == CustomPluList::Price6.Type)				AddFieldToHeader(CustomPluList::Price6);
		else if (nField == CustomPluList::Price7.Type)				AddFieldToHeader(CustomPluList::Price7);
		else if (nField == CustomPluList::Price8.Type)				AddFieldToHeader(CustomPluList::Price8);
		else if (nField == CustomPluList::Price9.Type)				AddFieldToHeader(CustomPluList::Price9);
		else if (nField == CustomPluList::Price10.Type)				AddFieldToHeader(CustomPluList::Price10);
		else if (nField == CustomPluList::AllowModifier.Type)			AddFieldToHeader(CustomPluList::AllowModifier);
		else if (nField == CustomPluList::Points.Type)				AddFieldToHeader(CustomPluList::Points);
		else if (nField == CustomPluList::StampWeight.Type)			AddFieldToHeader(CustomPluList::StampWeight);
		else if (nField == CustomPluList::PluType.Type)				AddFieldToHeader(CustomPluList::PluType);
		else if (nField == CustomPluList::ExternalProcess.Type)		AddFieldToHeader(CustomPluList::ExternalProcess);
		else if (nField == CustomPluList::Category.Type)				AddFieldToHeader(CustomPluList::Category);
		else if (nField == CustomPluList::Allergen1.Type)				AddFieldToHeader(CustomPluList::Allergen1);
		else if (nField == CustomPluList::Allergen2.Type)				AddFieldToHeader(CustomPluList::Allergen2);
		else if (nField == CustomPluList::Allergen3.Type)				AddFieldToHeader(CustomPluList::Allergen3);
		else if (nField == CustomPluList::Allergen4.Type)				AddFieldToHeader(CustomPluList::Allergen4);
		else if (nField == CustomPluList::Allergen5.Type)				AddFieldToHeader(CustomPluList::Allergen5);
		else if (nField == CustomPluList::Allergen6.Type)				AddFieldToHeader(CustomPluList::Allergen6);
		else if (nField == CustomPluList::PriceType.Type)				AddFieldToHeader(CustomPluList::PriceType);
		else if (nField == CustomPluList::AgeVerificationType.Type)	AddFieldToHeader(CustomPluList::AgeVerificationType);
		else if (nField == CustomPluList::KPNotAlone.Type)			AddFieldToHeader(CustomPluList::KPNotAlone);
		else if (nField == CustomPluList::KPConsolidateTopItem.Type)	AddFieldToHeader(CustomPluList::KPConsolidateTopItem);
		else if (nField == CustomPluList::AutoCoversQty.Type)			AddFieldToHeader(CustomPluList::AutoCoversQty);
		else if (nField == CustomPluList::Menu.Type)					AddFieldToHeader(CustomPluList::Menu);
		else if (nField == CustomPluList::Extra.Type)					AddFieldToHeader(CustomPluList::Extra);
		else if (nField == CustomPluList::Qualifier.Type)				AddFieldToHeader(CustomPluList::Qualifier);
		else if (nField == CustomPluList::OrderPrinterGroup.Type)		AddFieldToHeader(CustomPluList::OrderPrinterGroup);
		else if (nField == CustomPluList::OrderPrinters.Type)			AddFieldToHeader(CustomPluList::OrderPrinters);
		else if (nField == CustomPluList::Priority.Type)				AddFieldToHeader(CustomPluList::Priority);
		else if (nField == CustomPluList::Discountable.Type)			AddFieldToHeader(CustomPluList::Discountable);
		else if (nField == CustomPluList::StaffDiscount.Type)			AddFieldToHeader(CustomPluList::StaffDiscount);
		else if (nField == CustomPluList::FreeText.Type)				AddFieldToHeader(CustomPluList::FreeText);
		else if (nField == CustomPluList::FreeTextDescription.Type)	AddFieldToHeader(CustomPluList::FreeTextDescription);
		else if (nField == CustomPluList::AccountRequired.Type)	AddFieldToHeader(CustomPluList::AccountRequired);
		else if (nField == CustomPluList::SalesTracking.Type)	AddFieldToHeader(CustomPluList::SalesTracking);
		else if (nField == CustomPluList::SoldOut.Type)				AddFieldToHeader(CustomPluList::SoldOut);
		else if (nField == CustomPluList::EANOnReceipt.Type)			AddFieldToHeader(CustomPluList::EANOnReceipt);
		else if (nField == CustomPluList::KPImagePrint.Type)			AddFieldToHeader(CustomPluList::KPImagePrint);
		else if (nField == CustomPluList::PluInfoModifier.Type)		AddFieldToHeader(CustomPluList::PluInfoModifier);
		else if (nField == CustomPluList::PrintShelf.Type)			AddFieldToHeader(CustomPluList::PrintShelf);
		else if (nField == CustomPluList::PrintProduct.Type)			AddFieldToHeader(CustomPluList::PrintProduct);
		else if (nField == CustomPluList::ModifiedDatePlu.Type)		AddFieldToHeader(CustomPluList::ModifiedDatePlu);
		else if (nField == CustomPluList::ModifiedDateInfo.Type)		AddFieldToHeader(CustomPluList::ModifiedDateInfo);
		else if (nField == CustomPluList::MinimumSPOSVersion.Type)	AddFieldToHeader(CustomPluList::MinimumSPOSVersion);
		else if (nField == CustomPluList::MinimumEposReason.Type)		AddFieldToHeader(CustomPluList::MinimumEposReason);
		else if (nField == CustomPluList::AgressoGLCode.Type)			AddFieldToHeader(CustomPluList::AgressoGLCode);
		else if (nField == CustomPluList::AgressoCostCentre.Type)		AddFieldToHeader(CustomPluList::AgressoCostCentre);
		else if (nField == CustomPluList::AgressoProject.Type)		AddFieldToHeader(CustomPluList::AgressoProject);
		else if (nField == CustomPluList::ActiveItem.Type)			AddFieldToHeader(CustomPluList::ActiveItem);
		else if (nField == CustomPluList::EmptyColumn.Type)			AddFieldToHeader(CustomPluList::EmptyColumn);
#ifdef STOCKMAN_SYSTEM
		else if (nField == CustomPluList::StockCode.Type)				AddFieldToHeader(CustomPluList::StockCode);
#else
		else if (nField == CustomPluList::MinimumLevel.Type)			AddFieldToHeader(CustomPluList::MinimumLevel);
		else if (nField == CustomPluList::ApparentLevel.Type)			AddFieldToHeader(CustomPluList::ApparentLevel);
		else if (nField == CustomPluList::SupplierRef.Type)			AddFieldToHeader(CustomPluList::SupplierRef);
		else if (nField == CustomPluList::SupplierNo.Type)			AddFieldToHeader(CustomPluList::SupplierNo);
		else if (nField == CustomPluList::NonStockItem.Type)			AddFieldToHeader(CustomPluList::NonStockItem);
#endif
	}
}

/**********************************************************************/

void CReportPluCustomList::CreateReportLine ( CPluCSVRecord& PluRecord )
{
	CCSV csvKey ( m_strKey );

	__int64 nPluNo = PluRecord.GetPluNoInt();
	
	__int64 nSharpPluNo = 0;
	bool bModifiable = FALSE;
	CMinimumSPOSVersion Version;
	PluRecord.CheckMinimumSPOSVersion( Version, nSharpPluNo, bModifiable );

	int nLastMod = 0;
	if ((TRUE == bModifiable) && (TRUE == m_bModifiers))
	{
		nLastMod = 9;
	}
		
	for ( int nMod = 0; nMod <= nLastMod; nMod ++ )
	{
		if ( ( nMod > 0 ) && ( PluRecord.GetModifierEnable( nMod ) == FALSE ) )
		{		
			continue;
		}

		CCSVReportLine csvOut( TRUE, FALSE, FALSE );

		double dCostPrice = DataManager.Plu.GetPluPurchaseCost( PluRecord ) * DataManager.Modifier.GetMultiplierForQty( nMod );
		double dPrice1 = PluRecord.GetModifierPrice( nMod, 0 );

		CTaxRateInfo TaxRateInfo;
		DataManager.TaxRates.GetTaxRateInfo( PluRecord.GetModifierTaxBandIntForReport(nMod), TaxRateInfo );

		for ( int nPos = 0; nPos < csvKey.GetSize(); nPos++ )
		{
			int nField = csvKey.GetInt(nPos);
			
			if ( nField == CustomPluList::PluNo.Type )				
			{
				if (nMod == 0)
				{
					csvOut.AppendString(ReportHelpers.GetDisplayPluNo(PluRecord.GetPluNoInt()));
				}
				else
				{
					CString strMod = "--";
					strMod += DataManager.Modifier.GetDisplayName( nMod );
					csvOut.AppendString( strMod );
				}
			}
					
			else if ( nField == CustomPluList::RepText.Type )				csvOut.AppendString( PluRecord.GetReportText() );
			else if ( nField == CustomPluList::EposText.Type )				csvOut.AppendString( PluRecord.GetModifierEposText( nMod ) );
			else if ( nField == CustomPluList::KeyText.Type )				csvOut.AppendString( PluRecord.GetModifierKeyText( nMod ) );
			else if ( nField == CustomPluList::SortCode.Type )				csvOut.AppendString( PluRecord.GetSortCode() );
			else if ( nField == CustomPluList::ImageFilename.Type )			csvOut.AppendString( PluRecord.GetImageFilename() );
			else if ( nField == CustomPluList::DeptNo.Type )				
			{
				int nDeptNo = PluRecord.GetModifierDeptNoRaw(nMod);

				if ( nDeptNo == -1 )
				{
					CString strDeptNo;
					strDeptNo.Format( "[%d]", PluRecord.GetBaseDeptNo() );
					csvOut.AppendString( strDeptNo );
				}
				else
				{
					csvOut.AppendInt( nDeptNo );
				}
			}
			else if ( nField == CustomPluList::TaxBand.Type )				
			{
				CString strTaxBand = PluRecord.GetModifierTaxBandStringRaw(nMod);
				
				if ( strTaxBand == "*" )
				{
					strTaxBand.Format( "[%s]", PluRecord.GetBaseTaxBandString() );
				}
				
				csvOut.AppendString( strTaxBand );
			}
			else if (nField == CustomPluList::PurchaseCost.Type)
			{
				csvOut.AppendVal(dCostPrice);
			}
			else if (nField == CustomPluList::ReportCost.Type)
			{
				if (PluRecord.GetReportCostEnable() == FALSE)
				{
					csvOut.AppendString("--");
				}
				else
				{
					csvOut.AppendVal(PluRecord.GetReportCost());
				}
			}
			else if ( nField == CustomPluList::GP.Type )					csvOut.AppendVal( ReportHelpers.CalcGP ( dPrice1, TaxRateInfo.m_dTaxRate, dCostPrice ) );
			else if ( nField == CustomPluList::GPPercent.Type )				csvOut.AppendLineQty( ReportHelpers.CalcGPPercent ( dPrice1, TaxRateInfo.m_dTaxRate, dCostPrice ), 2 );
			else if ( nField == CustomPluList::Price1.Type )				csvOut.AppendVal( dPrice1 );
			else if ( nField == CustomPluList::Price2.Type )				csvOut.AppendVal( PluRecord.GetModifierPrice( nMod, 1 ) );
			else if ( nField == CustomPluList::Price3.Type )				csvOut.AppendVal( PluRecord.GetModifierPrice( nMod, 2 ) );
			else if ( nField == CustomPluList::Price4.Type )				csvOut.AppendVal( PluRecord.GetModifierPrice( nMod, 3 ) );
			else if ( nField == CustomPluList::Price5.Type )				csvOut.AppendVal( PluRecord.GetModifierPrice( nMod, 4 ) );
			else if ( nField == CustomPluList::Price6.Type )				csvOut.AppendVal( PluRecord.GetModifierPrice( nMod, 5 ) );
			else if ( nField == CustomPluList::Price7.Type )				csvOut.AppendVal( PluRecord.GetModifierPrice( nMod, 6 ) );
			else if ( nField == CustomPluList::Price8.Type )				csvOut.AppendVal( PluRecord.GetModifierPrice( nMod, 7 ) );
			else if ( nField == CustomPluList::Price9.Type )				csvOut.AppendVal( PluRecord.GetModifierPrice( nMod, 8 ) );
			else if ( nField == CustomPluList::Price10.Type )				csvOut.AppendVal( PluRecord.GetModifierPrice( nMod, 9 ) );
			else if ( nField == CustomPluList::AllowModifier.Type )			csvOut.AppendYesOrBlank(PluRecord.GetAllowModifierFlag() && ( 0 == nMod ) && bModifiable);
			else if ( nField == CustomPluList::Points.Type )				csvOut.AppendInt( PluRecord.GetPoints() );
			else if ( nField == CustomPluList::StampWeight.Type)			csvOut.AppendInt( PluRecord.GetStampWeight());
			else if ( nField == CustomPluList::EmptyColumn.Type)			csvOut.AppendString( "" );
			else if ( nField == CustomPluList::PluType.Type )				csvOut.AppendString( PluRecord.GetPluTypeText( nMod ) );
			else if ( nField == CustomPluList::ExternalProcess.Type )		csvOut.AppendString( PluRecord.GetExternalProcessText() );
			else if ( nField == CustomPluList::Category.Type )				csvOut.AppendString( PluRecord.GetCategoryText(nMod) );
			else if ( nField == CustomPluList::Allergen1.Type )				csvOut.AppendString( PluRecord.GetAllergenText(0) );
			else if ( nField == CustomPluList::Allergen2.Type )				csvOut.AppendString( PluRecord.GetAllergenText(1) );
			else if ( nField == CustomPluList::Allergen3.Type )				csvOut.AppendString( PluRecord.GetAllergenText(2) );
			else if ( nField == CustomPluList::Allergen4.Type )				csvOut.AppendString( PluRecord.GetAllergenText(3) );
			else if ( nField == CustomPluList::Allergen5.Type )				csvOut.AppendString( PluRecord.GetAllergenText(4) );
			else if ( nField == CustomPluList::Allergen6.Type )				csvOut.AppendString( PluRecord.GetAllergenText(5) );
			else if ( nField == CustomPluList::PriceType.Type )				csvOut.AppendString( PluRecord.GetPriceTypeText() );
			else if ( nField == CustomPluList::Discountable.Type )			csvOut.AppendYesOrBlank( PluRecord.GetDiscountableFlag() );
			else if ( nField == CustomPluList::StaffDiscount.Type )			csvOut.AppendYesOrBlank( PluRecord.GetStaffDiscountFlag() );
			else if ( nField == CustomPluList::AgeVerificationType.Type )	csvOut.AppendString( PluRecord.GetAgeVerificationTypeText() );
			else if ( nField == CustomPluList::KPNotAlone.Type )			csvOut.AppendYesOrBlank( PluRecord.GetKPNotAloneFlag() );
			else if ( nField == CustomPluList::KPConsolidateTopItem.Type )	csvOut.AppendYesOrBlank( PluRecord.GetKPConsolidateTopItemFlag() );
			else if ( nField == CustomPluList::AutoCoversQty.Type )			csvOut.AppendYesOrBlank( PluRecord.GetAutoCoversQtyFlag() );
			else if ( nField == CustomPluList::Menu.Type )					
			{
				int nNum = PluRecord.GetModifierFirstMenu(nMod);

				if ( nNum < 0 )
				{
					CString strNum;
					strNum.Format( "[%d]", PluRecord.GetFirstMenu() );
					csvOut.AppendString( strNum );
				}
				else
				{
					csvOut.AppendInt(nNum);
				}

			}
			else if ( nField == CustomPluList::Extra.Type )					
			{
				int nNum = PluRecord.GetModifierFirstExtra(nMod);

				if ( nNum < 0 )
				{
					CString strNum = "";
					strNum.Format( "[%d]", PluRecord.GetFirstExtra() );
					csvOut.AppendString( strNum );
				}
				else
				{
					csvOut.AppendInt(nNum);
				}
			}
			else if ( nField == CustomPluList::Qualifier.Type )				
			{
				int nNum = PluRecord.GetModifierFirstQualifier(nMod);

				if ( nNum < 0 )
				{
					CString strNum;
					strNum.Format( "[%d]", PluRecord.GetFirstQualifier() );
					csvOut.AppendString( strNum );
				}
				else
				{
					csvOut.AppendInt(nNum);
				}
			}
			else if ( nField == CustomPluList::Priority.Type )				csvOut.AppendInt( PluRecord.GetPriority() );
			else if ( nField == CustomPluList::FreeText.Type )				csvOut.AppendYesOrBlank( PluRecord.GetFreeTextFlag() );
			else if ( nField == CustomPluList::SalesTracking.Type )			csvOut.AppendYesOrBlank( PluRecord.GetSalesTrackingFlag() );
			else if ( nField == CustomPluList::SoldOut.Type )				csvOut.AppendYesOrBlank( PluRecord.GetSoldOutFlag() );
			else if ( nField == CustomPluList::EANOnReceipt.Type )			csvOut.AppendYesOrBlank( PluRecord.GetEANOnReceiptFlag() );
			else if (nField == CustomPluList::AgressoGLCode.Type)
			{
				int nGLCode = PluRecord.GetAgressoGLCode();

				if (nGLCode < 1000)
				{
					csvOut.AppendString("");
				}
				else
				{
					csvOut.AppendInt(nGLCode);
				}
			}
			else if ( nField == CustomPluList::AgressoCostCentre.Type )		csvOut.AppendString( PluRecord.GetAgressoCostCentre() );
			else if ( nField == CustomPluList::AgressoProject.Type )		csvOut.AppendString( PluRecord.GetAgressoProject() );
			else if ( nField == CustomPluList::ActiveItem.Type )			csvOut.AppendYesOrBlank( PluRecord.GetActiveItemFlag() );
			else if ( nField == CustomPluList::MinimumSPOSVersion.Type )	csvOut.AppendString( Version.GetMinVerName() );
			else if ( nField == CustomPluList::MinimumEposReason.Type )		csvOut.AppendString( Version.m_strReason );
			else if ( nField == CustomPluList::KPImagePrint.Type )	
			{
				if (PluRecord.GetKPImagePrintFlag() == TRUE)
				{
					if (PluRecord.GetKPImageModifierFlag() == TRUE)
					{
						csvOut.AppendString("Yes");
					}
					else
					{
						csvOut.AppendString("Standard item only");
					}
				}
				else
				{
					if (PluRecord.GetKPImageModifierFlag() == TRUE)
					{
						csvOut.AppendString("Modifiers only");
					}
					else
					{
						csvOut.AppendString("");
					}
				}
			}
			else if ( nField == CustomPluList::PluInfoModifier.Type )		csvOut.AppendYesOrBlank( PluRecord.GetPluInfoModifierFlag() );
			else if ( nField == CustomPluList::PrintShelf.Type )			csvOut.AppendYesOrBlank( PluRecord.GetPrintShelfFlag() );
			else if ( nField == CustomPluList::PrintProduct.Type )			csvOut.AppendYesOrBlank( PluRecord.GetPrintProductFlag() );
			else if ( nField == CustomPluList::UnitPrice.Type )
			{
				double dRatio = CPriceHelpers::GetUnitPriceRatio ( PluRecord );		
				double dUnitPrice = ( dRatio == 0.0 ) ? 0.0 : dPrice1 / dRatio;
				CString strPluUnit = PluRecord.GetUnitPriceText();
				CString strLabel = strPluUnit;

				if (dUnitPrice >= 1.00)
				{
					if (dRatio != 0.0)
					{
						strLabel.Format("£%.2f %s", dUnitPrice, (const char*)strPluUnit);
					}
				}
				else
				{
					if (dRatio != 0.0)
					{
						strLabel.Format("%.1fp %s", dUnitPrice * 100.0, (const char*)strPluUnit);
					}
				}
				csvOut.AppendString( strLabel );
			}
			else if ( nField == CustomPluList::OrderPrinterGroup.Type )
			{
				CString strGroup = ( 0 == nMod ) ? "None" : "As Base Plu";

				int nGroup = PluRecord.GetModifierOrderPrinterGroup( nMod );
				if ( ( nGroup >= 1 ) && ( nGroup <= DataManagerNonDb.OPGroupSystem.GetOPGroupCount() ) )
				{
					COPGroupMiniInfo infoGroup;
					DataManagerNonDb.OPGroupSystem.GetOPGroup( nGroup - 1, infoGroup );
					strGroup = infoGroup.m_strName;
				}
		
				csvOut.AppendString( strGroup );
			}	
			else if ( nField == CustomPluList::OrderPrinters.Type )
			{
				COPGroupInfo infoGroup;
				bool bPrinterGroup = FALSE;

				{
					int nOPGroup = PluRecord.GetModifierOrderPrinterGroup(nMod);
					if ((0 == nOPGroup) && (nMod != 0))
					{
						nOPGroup = PluRecord.GetOrderPrinterGroup();
					}

					if ( nOPGroup != 0 )
					{
						COPGroupMiniInfo infoMini;
						infoMini.m_bDefer = TRUE;
	
						if (PriceSetItemManager.IsPluMaster() == FALSE)
						{
							DataManagerNonDb.OPGroupLocation.GetOPGroup(nOPGroup - 1, infoMini);
						}
						if (TRUE == infoMini.m_bDefer)
						{
							DataManagerNonDb.OPGroupDatabase.GetOPGroup(nOPGroup - 1, infoMini);
						}

						if (TRUE == infoMini.m_bDefer)
						{
							DataManagerNonDb.OPGroupSystem.GetOPGroup(nOPGroup - 1, infoMini);
						}

						infoGroup.SetEnableFlagsSystem( infoMini.m_nFlags );
						bPrinterGroup = TRUE;
					}
				}
	
				CString strPrinters = "";
				for ( int nIdx = 0; nIdx < PLU_ORDER_PRINTERS_NOW; nIdx++ )
				{
					bool bInclude = FALSE;
					if (TRUE == bPrinterGroup)
					{
						bInclude = infoGroup.GetEnableSystem(nIdx);
					}
					else
					{
						bInclude = PluRecord.GetOrderPrinterFlag(nIdx);
					}

					if ( TRUE == bInclude )
					{	
						CString strNum = "";
						strNum.Format ( "%d", nIdx + 1 );

						if (strPrinters != "")
						{
							strPrinters += ",";
						}

						strPrinters += strNum;
					}
				}
				csvOut.AppendString( strPrinters );
			}	
			else if ( nField == CustomPluList::ModifiedDatePlu.Type )
			{
				CString strPluDate = PluRecord.GetModifiedDateTimePlu();
				strPluDate += "00000000000000";
				
				CString strReportDate;
				strReportDate.Format ( "%s/%s/%s at %s:%s",
					(const char*) strPluDate.Mid(6,2),
					(const char*) strPluDate.Mid(4,2),
					(const char*) strPluDate.Left(4),
					(const char*) strPluDate.Mid(8,2),
					(const char*) strPluDate.Mid(10,2) );
				
				csvOut.AppendString( strReportDate );
			}
			else if ( nField == CustomPluList::ModifiedDateInfo.Type )
			{
				CString strInfoDate = PluRecord.GetModifiedDateTimeInfo();

				if (strInfoDate == "")
				{
					csvOut.AppendString("N/A");
				}
				else
				{
					strInfoDate += "00000000000000";
					
					CString strReportDate;
					strReportDate.Format ( "%s/%s/%s at %s:%s",
						(const char*) strInfoDate.Mid(6,2),
						(const char*) strInfoDate.Mid(4,2),
						(const char*) strInfoDate.Left(4),
						(const char*) strInfoDate.Mid(8,2),
						(const char*) strInfoDate.Mid(10,2) );
					
					csvOut.AppendString( strReportDate );
				}
			}
			else if (nField == CustomPluList::FreeTextDescription.Type)
			{
				if (PluRecord.GetFreeTextFlag() == FALSE)
				{
					csvOut.AppendString("N/A");
				}
				else if (PluRecord.GetFreeTextDescriptionFlag() == FALSE)
				{
					csvOut.AppendString("");
				}
				else
				{
					csvOut.AppendString("Yes");
				}
			}

			else if (nField == CustomPluList::AccountRequired.Type)	csvOut.AppendYesOrBlank(PluRecord.GetAccountRequiredFlag());

#ifdef STOCKMAN_SYSTEM
			if ( nField == CustomPluList::StockCode.Type )
			{
				CString strStockCode;
				DataManager.Plu.GetPluStockCode( PluRecord.GetPluNoInt(), strStockCode, TRUE );
				csvOut.AppendString( ReportHelpers.GetDisplayStockCode ( strStockCode ) );
			}
#else
			if ( nField == CustomPluList::MinimumLevel.Type )			csvOut.AppendLineQty( PluRecord.GetMinimumLevel() );
			else if ( nField == CustomPluList::ApparentLevel.Type )		csvOut.AppendLineQty( PluRecord.GetApparentLevel() );
			else if ( nField == CustomPluList::SupplierRef.Type )		csvOut.AppendString( PluRecord.GetSupplierRef() );
			else if ( nField == CustomPluList::SupplierNo.Type )		csvOut.AppendInt( PluRecord.GetSupplierNo() );
			else if ( nField == CustomPluList::NonStockItem.Type )		csvOut.AppendYesOrBlank( ( PluRecord.GetNonStockExternalFlag() == TRUE ) );
#endif
		}	
		m_ReportFile.WriteLine( csvOut.GetLine() );
	}
	if ( m_bSeparate ) m_ReportFile.WriteLine ( "<LI>" );
}

/**********************************************************************/

void CReportPluCustomList::GetCSVReportName( int n, CString& strName )
{
	strName = "";

	if ( n >= 1 && n <= 5 )
	{	
		CSSFile file;
		CFilenameUpdater FnUp( SysFiles::PluCustomDef, -1, n + 2 );
		if ( file.Open( FnUp.GetFilenameToUse(), "rb" ) == TRUE )
		{
			CString strBuffer;
			file.ReadString( strBuffer );
			file.Close();
		
			CCSV csv( strBuffer );
			for ( int nPos = 0; nPos < csv.GetSize(); nPos++ )
			{
				if ( csv.GetInt(nPos) == 9999 )
				{
					strName = csv.GetString( nPos + 1 );
					break;
				}
			}
		}
	}

	::TrimSpaces( strName, FALSE );

	if ( strName == "" )
		strName.Format( "Custom List %d", n );
}

/**********************************************************************/
