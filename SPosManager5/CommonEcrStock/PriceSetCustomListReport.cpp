/**********************************************************************/
#include "CCSVReportLine.h"
#include "DatabaseCSVArray.h"
#include "LocationCSVArray.h"
#include "PluCSVArray.h"
#include "ReportHelpers.h"
#include "RepSpawn.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PriceSetCustomListReport.h"
/**********************************************************************/
static const int REPORT_OKAY = 0;
static const int REPORT_ERROR_RANGE = 1;
static const int REPORT_ERROR_FILE = 2;
/**********************************************************************/

CPriceSetCustomListReport::CPriceSetCustomListReport( CKeyboardFilterPluArray& arrayPluKbd, CPriceSetCustomListReportHelpers& Helpers ) : m_PluKbdArray( arrayPluKbd ), m_Helpers( Helpers )
{
	CFilenameUpdater FnUp( SysFiles::PluPriceSetList );
	m_strParamsFilename = FnUp.GetFilenameToUse();
	m_strTitle = "Price Set Custom List Report";
	m_strKey = m_Helpers.GetFieldKey();
	m_bSortByDept = FALSE;
	m_bZeroPriced = FALSE;
	m_bSeparate = FALSE;
	m_bModifiers = FALSE;
}

/**********************************************************************/

void CPriceSetCustomListReport::CreateReport()
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

int CPriceSetCustomListReport::CreateReportInternal()
{
	__int64 nPluFrom = _atoi64( m_strPluFrom );
	__int64 nPluTo = _atoi64( m_strPluTo ); 

	int nStartIdx, nEndIdx;
	DataManager.Plu.FindPluByNumber( nPluFrom, nStartIdx );
	if ( DataManager.Plu.FindPluByNumber( nPluTo, nEndIdx ) == FALSE )
		nEndIdx--;

	int nRange = ( nEndIdx - nStartIdx ) + 1;
	
	if ( nRange <= 0 )
		return REPORT_ERROR_RANGE;				//no plus in range

	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return REPORT_ERROR_FILE;				//unable to create file
	
	CString strLocation;
	int nDbIdx = DataManager.GetActiveDbIdx();
	strLocation.Format( " (%s, Price Set %d)", dbDatabase.GetName(nDbIdx), m_Helpers.GetPriceSetNo() );

	m_ReportFile.SetStyle1 ( m_strTitle + strLocation );
	CreateHeader();
	m_ReportFile.WriteLine( "" );

	CDepartmentSetCSVRecord DepartmentSet;
	DepartmentSet.CreateDepartmentSet ( m_nDepartmentFilter );

	CReportConsolidationArray<CSortedIntAndInt> ConsolArray;

	for ( int nStep = 0; nStep < nRange; nStep++ )	
	{ 
		int nMultiplyBy = ( m_bSortByDept == TRUE ) ? 50 : 100;
		StatusProgress.SetPos ( nStep, nRange, 0, nMultiplyBy );
	
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt ( nStartIdx + nStep, PluRecord );

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

		int nPluIdx = item.m_nInt2;
		
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
	return 0;
}

/**********************************************************************/

void CPriceSetCustomListReport::AddFieldToHeader ( CustomPluField Field, bool bPriceSet, int& nColumn )
{
	CString strLabel = Field.ReportLabel;
	
	if ( strLabel == "" )
	{
		if ( Field.Type == CustomPluList::SortCode.Type )
			strLabel = DealerFlags.GetSortCodeLabel();
		else
			strLabel = Field.ListLabel;
	}
	
	if ( ( TRUE == bPriceSet ) && ( m_Helpers.GetLocationCount() != 0 ) )
	{
		int nSubColumns = m_Helpers.GetLocationCount() + 1;
		double dPercentWidth = 100.0 / nSubColumns;

		m_ReportFile.AddColumn ( strLabel, TA_CENTER, 300 * nSubColumns );
	
		m_ReportFile.AddSubColumn( nColumn, "Master", Field.Orientation, dPercentWidth );

		for ( int n = 0; n < m_Helpers.GetLocationCount(); n++ )
		{
			int nLocIdx = m_Helpers.GetLocIdx( n );
			m_ReportFile.AddSubColumn ( nColumn, dbLocation.GetName( nLocIdx ), Field.Orientation, dPercentWidth );
		}
	}
	else
	{
		m_ReportFile.AddColumn ( strLabel, Field.Orientation, Field.DefSize );
	}

	nColumn++;
}

/**********************************************************************/

void CPriceSetCustomListReport::CreateHeader()
{
	int nColumn = 0;

	for (int nPos = 0; nPos < m_Helpers.GetFieldCount(); nPos++)
	{
		CPriceSetCustomListFieldInfo info;
		m_Helpers.GetFieldInfo(nPos, info);

		int nField = info.m_nFieldType;
		bool bPriceSet = info.m_bPriceSet;

		//MASTER DATABASE ONLY FIELDS
		if (nField == CustomPluList::PluNo.Type)						AddFieldToHeader(CustomPluList::PluNo, FALSE, nColumn);
		else if (nField == CustomPluList::RepText.Type)				AddFieldToHeader(CustomPluList::RepText, FALSE, nColumn);
		else if (nField == CustomPluList::SortCode.Type)				AddFieldToHeader(CustomPluList::SortCode, FALSE, nColumn);
		else if (nField == CustomPluList::AgressoGLCode.Type)			AddFieldToHeader(CustomPluList::AgressoGLCode, FALSE, nColumn);
		else if (nField == CustomPluList::AgressoCostCentre.Type)		AddFieldToHeader(CustomPluList::AgressoCostCentre, FALSE, nColumn);
		else if (nField == CustomPluList::AgressoProject.Type)		AddFieldToHeader(CustomPluList::AgressoProject, FALSE, nColumn);

#ifdef STOCKMAN_SYSTEM
		else if (nField == CustomPluList::StockCode.Type)				AddFieldToHeader(CustomPluList::StockCode, FALSE, nColumn);
#endif

		else if (nField == CustomPluList::DeptNo.Type)				AddFieldToHeader(CustomPluList::DeptNo, FALSE, nColumn);
		else if (nField == CustomPluList::PurchaseCost.Type)			AddFieldToHeader(CustomPluList::PurchaseCost, FALSE, nColumn);
		else if (nField == CustomPluList::Points.Type)				AddFieldToHeader(CustomPluList::Points, FALSE, nColumn);
		else if (nField == CustomPluList::StampWeight.Type)			AddFieldToHeader(CustomPluList::StampWeight, FALSE, nColumn);
		else if (nField == CustomPluList::PluType.Type)				AddFieldToHeader(CustomPluList::PluType, FALSE, nColumn);
		else if (nField == CustomPluList::ExternalProcess.Type)		AddFieldToHeader(CustomPluList::ExternalProcess, FALSE, nColumn);
		else if (nField == CustomPluList::Category.Type)				AddFieldToHeader(CustomPluList::Category, FALSE, nColumn);
		else if (nField == CustomPluList::Allergen1.Type)				AddFieldToHeader(CustomPluList::Allergen1, FALSE, nColumn);
		else if (nField == CustomPluList::Allergen2.Type)				AddFieldToHeader(CustomPluList::Allergen2, FALSE, nColumn);
		else if (nField == CustomPluList::Allergen3.Type)				AddFieldToHeader(CustomPluList::Allergen3, FALSE, nColumn);
		else if (nField == CustomPluList::Allergen4.Type)				AddFieldToHeader(CustomPluList::Allergen4, FALSE, nColumn);
		else if (nField == CustomPluList::Allergen5.Type)				AddFieldToHeader(CustomPluList::Allergen5, FALSE, nColumn);
		else if (nField == CustomPluList::Allergen6.Type)				AddFieldToHeader(CustomPluList::Allergen6, FALSE, nColumn);
		else if (nField == CustomPluList::PriceType.Type)				AddFieldToHeader(CustomPluList::PriceType, FALSE, nColumn);
		else if (nField == CustomPluList::AgeVerificationType.Type)	AddFieldToHeader(CustomPluList::AgeVerificationType, FALSE, nColumn);
		else if (nField == CustomPluList::KPNotAlone.Type)			AddFieldToHeader(CustomPluList::KPNotAlone, FALSE, nColumn);
		else if (nField == CustomPluList::KPConsolidateTopItem.Type)	AddFieldToHeader(CustomPluList::KPConsolidateTopItem, FALSE, nColumn);
		else if (nField == CustomPluList::AutoCoversQty.Type)			AddFieldToHeader(CustomPluList::AutoCoversQty, FALSE, nColumn);
		else if (nField == CustomPluList::Discountable.Type)			AddFieldToHeader(CustomPluList::Discountable, FALSE, nColumn);
		else if (nField == CustomPluList::StaffDiscount.Type)			AddFieldToHeader(CustomPluList::StaffDiscount, FALSE, nColumn);
		else if (nField == CustomPluList::FreeText.Type)				AddFieldToHeader(CustomPluList::FreeText, FALSE, nColumn);
		else if (nField == CustomPluList::FreeTextDescription.Type)	AddFieldToHeader(CustomPluList::FreeTextDescription, FALSE, nColumn);
		else if (nField == CustomPluList::AccountRequired.Type)	AddFieldToHeader(CustomPluList::AccountRequired, FALSE, nColumn);
		else if (nField == CustomPluList::SalesTracking.Type)	AddFieldToHeader(CustomPluList::SalesTracking, FALSE, nColumn);
		else if (nField == CustomPluList::EANOnReceipt.Type)			AddFieldToHeader(CustomPluList::EANOnReceipt, FALSE, nColumn);
		else if (nField == CustomPluList::KPImagePrint.Type)			AddFieldToHeader(CustomPluList::KPImagePrint, FALSE, nColumn);
		else if (nField == CustomPluList::PluInfoModifier.Type)		AddFieldToHeader(CustomPluList::PluInfoModifier, FALSE, nColumn);
		else if (nField == CustomPluList::PrintShelf.Type)			AddFieldToHeader(CustomPluList::PrintShelf, FALSE, nColumn);
		else if (nField == CustomPluList::PrintProduct.Type)			AddFieldToHeader(CustomPluList::PrintProduct, FALSE, nColumn);
		else if (nField == CustomPluList::AllowModifier.Type)			AddFieldToHeader(CustomPluList::AllowModifier, FALSE, nColumn);

		//PRICE SET OVERRIDEABLE FIELDS
		else if (nField == CustomPluList::EposText.Type)				AddFieldToHeader(CustomPluList::EposText, bPriceSet, nColumn);
		else if (nField == CustomPluList::KeyText.Type)				AddFieldToHeader(CustomPluList::KeyText, bPriceSet, nColumn);
		else if (nField == CustomPluList::ReportCost.Type)			AddFieldToHeader(CustomPluList::ReportCost, bPriceSet, nColumn);
		else if (nField == CustomPluList::Price1.Type)				AddFieldToHeader(CustomPluList::Price1, bPriceSet, nColumn);
		else if (nField == CustomPluList::Price2.Type)				AddFieldToHeader(CustomPluList::Price2, bPriceSet, nColumn);
		else if (nField == CustomPluList::Price3.Type)				AddFieldToHeader(CustomPluList::Price3, bPriceSet, nColumn);
		else if (nField == CustomPluList::Price4.Type)				AddFieldToHeader(CustomPluList::Price4, bPriceSet, nColumn);
		else if (nField == CustomPluList::Price5.Type)				AddFieldToHeader(CustomPluList::Price5, bPriceSet, nColumn);
		else if (nField == CustomPluList::Price6.Type)				AddFieldToHeader(CustomPluList::Price6, bPriceSet, nColumn);
		else if (nField == CustomPluList::Price7.Type)				AddFieldToHeader(CustomPluList::Price7, bPriceSet, nColumn);
		else if (nField == CustomPluList::Price8.Type)				AddFieldToHeader(CustomPluList::Price8, bPriceSet, nColumn);
		else if (nField == CustomPluList::Price9.Type)				AddFieldToHeader(CustomPluList::Price9, bPriceSet, nColumn);
		else if (nField == CustomPluList::Price10.Type)				AddFieldToHeader(CustomPluList::Price10, bPriceSet, nColumn);
		else if (nField == CustomPluList::TaxBand.Type)				AddFieldToHeader(CustomPluList::TaxBand, bPriceSet, nColumn);
		else if (nField == CustomPluList::Menu.Type)					AddFieldToHeader(CustomPluList::Menu, bPriceSet, nColumn);
		else if (nField == CustomPluList::Qualifier.Type)				AddFieldToHeader(CustomPluList::Qualifier, bPriceSet, nColumn);
		else if (nField == CustomPluList::Extra.Type)					AddFieldToHeader(CustomPluList::Extra, bPriceSet, nColumn);
		else if (nField == CustomPluList::OrderPrinterGroup.Type)		AddFieldToHeader(CustomPluList::OrderPrinterGroup, bPriceSet, nColumn);
		else if (nField == CustomPluList::Priority.Type)				AddFieldToHeader(CustomPluList::Priority, bPriceSet, nColumn);
	}
}

/**********************************************************************/

void CPriceSetCustomListReport::CreateReportLine ( CPluCSVRecord& PluRecord )
{
	__int64 nPluNo = PluRecord.GetPluNoInt();
	
	__int64 nSharpPluNo;
	bool bModifiable;
	CMinimumSPOSVersion Version;
	PluRecord.CheckMinimumSPOSVersion( Version, nSharpPluNo, bModifiable );

	int nLastMod = 0;
	if ((TRUE == bModifiable) && (TRUE == m_bModifiers))
	{
		nLastMod = 9;
	}

	for ( int nMod = 0; nMod <= nLastMod; nMod ++ )
	{
		if ((nMod > 0) && (PluRecord.GetModifierEnable(nMod) == FALSE))
		{
			continue;
		}

		CCSVReportLine csvOut( TRUE, FALSE, FALSE );

		double dCostPrice = DataManager.Plu.GetPluPurchaseCost( PluRecord ) * DataManager.Modifier.GetMultiplierForQty( nMod );

		CTaxRateInfo TaxRateInfo;
		DataManager.TaxRates.GetTaxRateInfo( PluRecord.GetModifierTaxBandIntForReport(nMod), TaxRateInfo );

		for ( int nPos = 0; nPos < m_Helpers.GetFieldCount(); nPos++ )
		{
			CPriceSetCustomListFieldInfo info;
			m_Helpers.GetFieldInfo( nPos, info );

			int nField = info.m_nFieldType;
			bool bPriceSet = info.m_bPriceSet;
			
			int nPriceLevel = -1;
	
			//MASTER DATABASE ONLY FIELDS
			if (nField == CustomPluList::PluNo.Type)
			{
				if (nMod == 0)
				{
					csvOut.AppendString(ReportHelpers.GetDisplayPluNo(PluRecord.GetPluNoInt()));
				}
				else
				{
					CString strMod = "--";
					strMod += DataManager.Modifier.GetDisplayName(nMod);
					csvOut.AppendString(strMod);
				}
			}
					
			else if ( nField == CustomPluList::RepText.Type )				csvOut.AppendString ( PluRecord.GetReportText() );
			else if ( nField == CustomPluList::SortCode.Type )				csvOut.AppendString ( PluRecord.GetSortCode() );
			
#ifdef STOCKMAN_SYSTEM
			else if ( nField == CustomPluList::StockCode.Type )
			{
				CString strStockCode;
				DataManager.Plu.GetPluStockCode( PluRecord.GetPluNoInt(), strStockCode, TRUE );
				csvOut.AppendString( ReportHelpers.GetDisplayStockCode ( strStockCode ) );
			}
#endif

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
			else if ( nField == CustomPluList::PurchaseCost.Type )			csvOut.AppendVal( dCostPrice );
			else if ( nField == CustomPluList::Points.Type )				csvOut.AppendInt( PluRecord.GetPoints() );
			else if ( nField == CustomPluList::StampWeight.Type)			csvOut.AppendInt( PluRecord.GetStampWeight());
			else if ( nField == CustomPluList::PluType.Type )				csvOut.AppendString( PluRecord.GetPluTypeText(nMod) );
			else if ( nField == CustomPluList::ExternalProcess.Type )		csvOut.AppendString( PluRecord.GetExternalProcessText() );	
			else if ( nField == CustomPluList::Category.Type )				csvOut.AppendString( PluRecord.GetCategoryText(nMod) );
			else if ( nField == CustomPluList::Allergen1.Type )				csvOut.AppendString( PluRecord.GetAllergenText(0) );
			else if ( nField == CustomPluList::Allergen2.Type )				csvOut.AppendString( PluRecord.GetAllergenText(1) );
			else if ( nField == CustomPluList::Allergen3.Type )				csvOut.AppendString( PluRecord.GetAllergenText(2) );
			else if ( nField == CustomPluList::Allergen4.Type )				csvOut.AppendString( PluRecord.GetAllergenText(3) );
			else if ( nField == CustomPluList::Allergen5.Type )				csvOut.AppendString( PluRecord.GetAllergenText(4) );
			else if ( nField == CustomPluList::Allergen6.Type )				csvOut.AppendString( PluRecord.GetAllergenText(5) );
			else if ( nField == CustomPluList::PriceType.Type )				csvOut.AppendString( PluRecord.GetPriceTypeText() );
			else if ( nField == CustomPluList::AgeVerificationType.Type )	csvOut.AppendString( PluRecord.GetAgeVerificationTypeText() );
			else if ( nField == CustomPluList::KPNotAlone.Type )			csvOut.AppendYesOrBlank( PluRecord.GetKPNotAloneFlag() );
			else if ( nField == CustomPluList::KPConsolidateTopItem.Type )	csvOut.AppendYesOrBlank( PluRecord.GetKPConsolidateTopItemFlag() );
			else if ( nField == CustomPluList::AutoCoversQty.Type )			csvOut.AppendYesOrBlank( PluRecord.GetAutoCoversQtyFlag() );
			else if ( nField == CustomPluList::Discountable.Type )			csvOut.AppendYesOrBlank( PluRecord.GetDiscountableFlag() );
			else if ( nField == CustomPluList::StaffDiscount.Type )			csvOut.AppendYesOrBlank( PluRecord.GetStaffDiscountFlag() );
			else if ( nField == CustomPluList::FreeText.Type )				csvOut.AppendYesOrBlank( PluRecord.GetFreeTextFlag() );
			else if (nField == CustomPluList::FreeTextDescription.Type)
			{
				if (PluRecord.GetFreeTextFlag() == FALSE)
				{
					csvOut.AppendString("N/A");
				}
				else
				{
					csvOut.AppendYesOrBlank(PluRecord.GetFreeTextDescriptionFlag());
				}
			}
			else if (nField == CustomPluList::AccountRequired.Type)			csvOut.AppendYesOrBlank(PluRecord.GetAccountRequiredFlag());
			else if ( nField == CustomPluList::SalesTracking.Type )			csvOut.AppendYesOrBlank( PluRecord.GetSalesTrackingFlag() );
			else if ( nField == CustomPluList::EANOnReceipt.Type )			csvOut.AppendYesOrBlank( PluRecord.GetEANOnReceiptFlag() );
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
			else if ( nField == CustomPluList::AgressoCostCentre.Type )		csvOut.AppendString ( PluRecord.GetAgressoCostCentre() );
			else if ( nField == CustomPluList::AgressoProject.Type )		csvOut.AppendString ( PluRecord.GetAgressoProject() );
			else if ( nField == CustomPluList::AllowModifier.Type)			csvOut.AppendYesOrBlank(PluRecord.GetAllowModifierFlag());
			//PRICE SET OVERRIDEABLE FIELDS
			else if ( nField == CustomPluList::EposText.Type )				m_Helpers.AppendPriceSetEposTexts( csvOut, PluRecord, nMod, bPriceSet );
			else if ( nField == CustomPluList::KeyText.Type )				m_Helpers.AppendPriceSetKeyTexts( csvOut, PluRecord, nMod, bPriceSet );
			else if ( nField == CustomPluList::ReportCost.Type )			m_Helpers.AppendPriceSetDouble( csvOut, PluRecord, CustomPluList::ReportCost.Type, bPriceSet );
			else if ( nField == CustomPluList::Menu.Type )					m_Helpers.AppendPriceSetInt( csvOut, PluRecord, CustomPluList::Menu.Type, nMod, bPriceSet );
			else if ( nField == CustomPluList::Qualifier.Type )				m_Helpers.AppendPriceSetInt( csvOut, PluRecord, CustomPluList::Qualifier.Type, nMod, bPriceSet );
			else if ( nField == CustomPluList::Extra.Type )					m_Helpers.AppendPriceSetInt( csvOut, PluRecord, CustomPluList::Extra.Type, nMod, bPriceSet );
			else if ( nField == CustomPluList::Price1.Type )				m_Helpers.AppendPriceSetPrices( csvOut, PluRecord, nMod, 0, bPriceSet );
			else if ( nField == CustomPluList::Price2.Type )				m_Helpers.AppendPriceSetPrices( csvOut, PluRecord, nMod, 1, bPriceSet );
			else if ( nField == CustomPluList::Price3.Type )				m_Helpers.AppendPriceSetPrices( csvOut, PluRecord, nMod, 2, bPriceSet );
			else if ( nField == CustomPluList::Price4.Type )				m_Helpers.AppendPriceSetPrices( csvOut, PluRecord, nMod, 3, bPriceSet );
			else if ( nField == CustomPluList::Price5.Type )				m_Helpers.AppendPriceSetPrices( csvOut, PluRecord, nMod, 4, bPriceSet );
			else if ( nField == CustomPluList::Price6.Type )				m_Helpers.AppendPriceSetPrices( csvOut, PluRecord, nMod, 5, bPriceSet );
			else if ( nField == CustomPluList::Price7.Type )				m_Helpers.AppendPriceSetPrices( csvOut, PluRecord, nMod, 6, bPriceSet );
			else if ( nField == CustomPluList::Price8.Type )				m_Helpers.AppendPriceSetPrices( csvOut, PluRecord, nMod, 7, bPriceSet );
			else if ( nField == CustomPluList::Price9.Type )				m_Helpers.AppendPriceSetPrices( csvOut, PluRecord, nMod, 8, bPriceSet );
			else if ( nField == CustomPluList::Price10.Type )				m_Helpers.AppendPriceSetPrices( csvOut, PluRecord, nMod, 9, bPriceSet );
			else if ( nField == CustomPluList::OrderPrinterGroup.Type )		m_Helpers.AppendPriceSetOrderGroups( csvOut, PluRecord, nMod, bPriceSet );
			else if ( nField == CustomPluList::Priority.Type )				m_Helpers.AppendPriceSetInt( csvOut, PluRecord, CustomPluList::Priority.Type, nMod, bPriceSet );
			else if ( nField == CustomPluList::TaxBand.Type)				m_Helpers.AppendPriceSetTaxBands( csvOut, PluRecord, nMod, bPriceSet);
			/*
			{
			CString strTaxBand = PluRecord.GetModifierTaxBandStringRaw(nMod);

			if (strTaxBand == "*")
			{
				strTaxBand.Format("[%s]", PluRecord.GetBaseTaxBandString());
			}

			csvOut.AppendString(strTaxBand);
			}
			*/
		}	
		m_ReportFile.WriteLine( csvOut.GetLine() );
	}
	if ( m_bSeparate ) m_ReportFile.WriteLine ( "<LI>" );
}

/**********************************************************************/
