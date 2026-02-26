/**********************************************************************/
#include "..\CommonEcrStockTray\PluTypeHelpers.h"
/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "DatabaseCSVArray.h"
#include "DateRangeHelpers.h"
#include "HexBitArray.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "ServerPresetArray.h"
#include "ServerRestrictionCSVArray.h"
#include "SQLIn.h"
#include "StatusProgress.h"
#include "SysInfo.h"
#include "WizardImportInfo.h"
/**********************************************************************/
#include "SQLImporter.h"
/**********************************************************************/

CSQLImporter::CSQLImporter(const char* szFilename)
{
	m_strFilename = szFilename;
	m_nServerPresetLocIdx = 0;
	m_bWantBarcode = TRUE;
	m_bWantDept = TRUE;
	m_bWantCategory = TRUE;
	m_bWantGroup = TRUE;
	m_bWantAllergen = TRUE;
	m_bWantMixMatch = TRUE;
	m_bWantWalletStampRule = TRUE;
	m_bWantPaidInOutText = TRUE;
	m_bWantTimePlan = TRUE;
	m_bWantModifier = TRUE;
	m_bWantPayMeth = TRUE;
	m_bWantServer = TRUE;
	m_bWantServerLogon = TRUE;
	m_bWantServerRestrict = TRUE;
	m_bWantServerPreset = TRUE;
	m_bWantPlu = TRUE;
	m_bWantTaxRate = TRUE;
	m_bWantLoyaltyScheme = TRUE;
	m_bWantTableArea = TRUE;
	m_bWantTableName = TRUE;
	m_bWantFunctionSystem = TRUE;
	m_nWantFunctionDatabaseType = WIZARD_IMPORT_MERGE;
	m_nWantFunctionLocationType = WIZARD_IMPORT_MERGE;
	m_bWantCCMacroSystem = TRUE;
	m_bWantCCMacroDatabase = TRUE;
	m_bWantCCMacroLocation = TRUE;
	m_bWantCCReasonSystem = TRUE;
	m_bWantCCReasonDatabase = TRUE;
	m_bWantCCReasonLocation = TRUE;
	m_bCreateStock = FALSE;

	if (SysInfo.GetMaxBasePluLen() == 0)
	{
		m_bWantModifier = FALSE;
	}

	m_nCustomersAdded = 0;
	m_nCustomersModified = 0;

	m_bClearReasonSystem = FALSE;
	m_bClearReasonDatabase = FALSE;
	m_bClearReasonLocation = FALSE;

	Reset();
}

/**********************************************************************/

void CSQLImporter::Reset()
{
	m_bGotFile = FALSE;
	m_nTotalBarcode = 0;
	m_nTotalCustomer = 0;
	m_nTotalDept = 0;
	m_nTotalCategory = 0;
	m_nTotalGroup = 0;
	m_nTotalAllergen = 0;
	m_nTotalMixMatch = 0;
	m_nTotalWalletStampRule = 0;
	m_nTotalOfferGroup = 0;
	m_nTotalOfferEntry = 0;
	m_nTotalPaidInOutText = 0;
	m_nTotalTimePlan = 0;
	m_nTotalTimePlanEntry = 0;
	m_nTotalModifier = 0;
	m_nTotalPayMeth = 0;
	m_nTotalPromotion = 0;
	m_nTotalServer = 0;
	m_nTotalServerLogon = 0;
	m_nTotalServerRestrict = 0;
	m_nTotalServerPreset = 0;
	m_nTotalBasePlu = 0;
	m_nTotalModifierPlu = 0;
	m_nTotalTaxRate = 0;
	m_nTotalLoyaltyScheme = 0;
	m_nTotalTableArea = 0;
	m_nTotalTableName = 0;
	m_nTotalFunction = 0;
	m_nTotalCCMacro = 0;
	m_nTotalCCReason = 0;
	m_bChangeDatabase = FALSE;
	m_bGotLegacyPayments = FALSE;
	m_bGotLegacyServers = FALSE;
	m_arrayOfferGroups.RemoveAll();

	for (int n = 0; n < REASONTEXT_TYPES; n++)
	{
		m_bHaveClearedReasonSystem[n] = FALSE;
		m_bHaveClearedReasonDatabase[n] = FALSE;
		m_bHaveClearedReasonLocation[n] = FALSE;
	}
}

/**********************************************************************/

void CSQLImporter::ImportFile(bool bChangeDatabase, int nDbIdx, int nLocIdx)
{
	Reset();
	m_bChangeDatabase = bChangeDatabase;

	CSSFile fileSQL;
	if (fileSQL.Open(m_strFilename, "rb") == FALSE)
	{
		return;
	}

	m_bGotFile = TRUE;

	CString str;
	if (bChangeDatabase == TRUE)
	{
		str = "Importing SQL data";
	}
	else
	{
		str = "Checking SQL data";
	}

	StatusProgress.Lock(TRUE, str);

	int nPassNo = 1;
	if (TRUE == m_bWantPlu)				ImportProducts(fileSQL, nPassNo++);
	if (TRUE == m_bWantDept)				ImportDepartments(fileSQL, nPassNo++);
	if (TRUE == m_bWantCategory)			ImportCategories(fileSQL, nPassNo++);
	if (TRUE == m_bWantPaidInOutText)		ImportPaidInOutTexts(fileSQL, nPassNo++);
	if (TRUE == m_bWantGroup)				ImportGroups(fileSQL, nPassNo++);
	if (TRUE == m_bWantAllergen)			ImportAllergens(fileSQL, nPassNo++);
	if (TRUE == m_bWantMixMatch)
	{
		ImportMixMatch(fileSQL, nPassNo++);
	}
	if (TRUE == m_bWantWalletStampRule)
	{
		ImportWalletStampRules(fileSQL, nPassNo++);
	}
	if ((TRUE == m_bWantWalletStampRule) || (TRUE == m_bWantMixMatch))
	{
		ImportOfferGroup(fileSQL, nPassNo++);
		ImportOfferEntry(fileSQL, nPassNo++);
		ImportPromotion(fileSQL, nPassNo++);
	}
	if (TRUE == m_bWantTimePlan)
	{
		ImportTimePlan(fileSQL, nPassNo++);
		ImportTimePlanEntry(fileSQL, nPassNo++);
	}
	if (TRUE == m_bWantModifier)		ImportModifiers(fileSQL, nPassNo++);
	if (TRUE == m_bWantPayMeth)			ImportPaymentMethods(fileSQL, nPassNo++);
	if (TRUE == m_bWantServer)			ImportServers(fileSQL, nPassNo++);
	if (TRUE == m_bWantServerLogon)		ImportServerLogons(fileSQL, nPassNo++);
	if (TRUE == m_bWantServerRestrict)	ImportServerRestricts(fileSQL, nPassNo++);
	if (TRUE == m_bWantServerPreset)	ImportServerPresets(fileSQL, nPassNo++);
	if (TRUE == m_bWantBarcode)			ImportBarcodes(fileSQL, nPassNo++);
	if (TRUE == m_bWantTaxRate)			ImportTaxRates(fileSQL, nPassNo++);
	if (TRUE == m_bWantLoyaltyScheme)	ImportLoyaltySchemes(fileSQL, nPassNo++);
	if (TRUE == m_bWantTableArea)		ImportTableAreas(fileSQL, nPassNo++);
	if (TRUE == m_bWantTableName)		ImportTableNames(fileSQL, nPassNo++);

	if ((TRUE == m_bWantFunctionSystem) || (m_nWantFunctionDatabaseType != WIZARD_IMPORT_NONE) || (m_nWantFunctionLocationType != WIZARD_IMPORT_NONE))
	{
		ImportFunctions(fileSQL, nPassNo++);
	}

	if ((TRUE == m_bWantCCMacroSystem) || (TRUE == m_bWantCCMacroDatabase) || (TRUE == m_bWantCCMacroLocation))
	{
		ImportCCMacros(fileSQL, nPassNo++);
	}

	if ((TRUE == m_bWantCCReasonSystem) || (TRUE == m_bWantCCReasonDatabase) || (TRUE == m_bWantCCReasonLocation))
	{
		ImportCCReasons(fileSQL, nPassNo++, nDbIdx, nLocIdx);
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

void CSQLImporter::ImportCustomersOnly( bool bAuto )
{
	Reset();
	
	CSSFile fileSQL;
	if (fileSQL.Open(m_strFilename, "rb") == FALSE)
	{
		return;
	}

	m_bGotFile = TRUE;

	StatusProgress.Lock( TRUE, "Importing SQL data" );
	ImportCustomers( fileSQL, bAuto, "Customers" );
	ImportCustomers( fileSQL, bAuto, "Customers2" );
	StatusProgress.Unlock();
}

/**********************************************************************/

void CSQLImporter::ImportProducts(CSSFile& fileSQL, int nPassNo)
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add("[PLU]");
	csvHeader.Add("[Description]");
	csvHeader.Add("[ShortDescription]");
	csvHeader.Add("[Price1]");
	csvHeader.Add("[Price2]");
	csvHeader.Add("[Price3]");
	csvHeader.Add("[Price4]");
	csvHeader.Add("[Price5]");
	csvHeader.Add("[Price6]");
	csvHeader.Add("[Price7]");
	csvHeader.Add("[Price8]");
	csvHeader.Add("[Price9]");
	csvHeader.Add("[Price10]");
	csvHeader.Add("[StampWeight]");
	csvHeader.Add("[Dept]");
	csvHeader.Add("[VAT]");
	csvHeader.Add("[ProdType]");
	csvHeader.Add("[ExternalProcess]");
	csvHeader.Add("[ProdCat]");
	csvHeader.Add("[AllergyCode1]");
	csvHeader.Add("[AllergyCode2]");
	csvHeader.Add("[AllergyCode3]");
	csvHeader.Add("[AllergyCode4]");
	csvHeader.Add("[AllergyCode5]");
	csvHeader.Add("[AllergyCode6]");
	csvHeader.Add("[ListMenu]");
	csvHeader.Add("[ListQual]");
	csvHeader.Add("[ListExtra]");
	csvHeader.Add("[Dsp1]");
	csvHeader.Add("[Dsp2]");
	csvHeader.Add("[Dsp3]");
	csvHeader.Add("[Dsp4]");
	csvHeader.Add("[Dsp5]");
	csvHeader.Add("[Dsp6]");
	csvHeader.Add("[Dsp7]");
	csvHeader.Add("[Dsp8]");
	csvHeader.Add("[Dsp9]");
	csvHeader.Add("[Dsp10]");
	csvHeader.Add("[Dsp11]");
	csvHeader.Add("[Dsp12]");
	csvHeader.Add("[Dsp13]");
	csvHeader.Add("[Dsp14]");
	csvHeader.Add("[Dsp15]");
	csvHeader.Add("[Dsp16]");
	csvHeader.Add("[DspPriority]");
	csvHeader.Add("[Discount]");
	csvHeader.Add("[PriceType]");
	csvHeader.Add("[FreeText]");
	csvHeader.Add("[AccountReqd]");
	csvHeader.Add("[DiscountStaff]");
	csvHeader.Add("[AllowTracking]");
	csvHeader.Add("[AllowPLUEANOnReceipt]");
	csvHeader.Add("[ShowDescriptionWithFreetext]");
	csvHeader.Add("[LinkType]");
	csvHeader.Add("[LinkTallyMode]");
	csvHeader.Add("[AgeVerify]");
	csvHeader.Add("[AutoQuantity]");
	csvHeader.Add("[KPNotAlone]");
	csvHeader.Add("[KPConsolidateTop]");
	csvHeader.Add("[V$SoldOut]");
	csvHeader.Add("[ImageFilename]");
	csvHeader.Add("[AllowModifier]");

	for (int n = 1; n <= MAX_LINK_PLU_ITEMS; n++)
	{
		CString strLabel;
		strLabel.Format("[TiedPLU%d]", n);
		csvHeader.Add(strLabel);
	}

	csvHeader.Add("[KPImage]");

	CSQLIn SQLIn(fileSQL, "Products", csvHeader.GetLine());

	CString strBuffer = "";
	int nTypeCount = GetFileTypeCount();
	while (SQLIn.ReadLine(strBuffer))
	{
		StatusProgress.SetPos(fileSQL, nPassNo, nTypeCount);
		ImportProduct(strBuffer);
	}
}

/**********************************************************************/

void CSQLImporter::ImportProduct(CString& strBuffer)
{
	CCSV csvIn(strBuffer);

	CSQLPluNoInfo infoPlu;
	infoPlu.m_nEposPluNo = csvIn.GetInt64(0);

	ProcessPluNo(infoPlu);
	if (FALSE == infoPlu.m_bValid)
	{
		return;
	}

	if ((infoPlu.m_nEposPluNo >= 99999000) && (infoPlu.m_nEposPluNo <= 100001000))
	{
		if (!csvIn.IsEmpty(15))
		{
			if (csvIn.GetInt(15) == 90)
			{
				return;
			}
		}
	}

	if (0 == infoPlu.m_nModifier)
	{
		m_nTotalBasePlu++;
	}
	else
	{
		m_nTotalModifierPlu++;
	}

	if (m_bChangeDatabase == FALSE)
	{
		return;
	}

	CPluCSVRecord PluRecord;

	int nPluIdx;
	bool bNewPluItem;
	if (DataManager.Plu.FindPluByNumber(infoPlu.m_nBasePluNo, nPluIdx) == TRUE)
	{
		DataManager.Plu.GetAt(nPluIdx, PluRecord);
		bNewPluItem = FALSE;
	}
	else
	{
		PluRecord.SetPluNo(infoPlu.m_nBasePluNo);

#ifdef STOCKMAN_SYSTEM
		nPluIdx = DataManager.Plu.InsertRecord(PluRecord, "", TRUE);
#else
		nPluIdx = DataManager.Plu.InsertRecord(PluRecord, TRUE);
#endif

		bNewPluItem = TRUE;
	}

	if (0 == infoPlu.m_nModifier)
	{
		if (!csvIn.IsEmpty(1))	PluRecord.SetEposText(csvIn.GetString(1));
		if (!csvIn.IsEmpty(2))	PluRecord.SetKeyText(csvIn.GetString(2));

		for (int n = 0; n < 10; n++)
		{
			if (!csvIn.IsEmpty(n + 3))	PluRecord.SetPrice(n, csvIn.GetDouble(n + 3));
		}

		if (!csvIn.IsEmpty(13))	PluRecord.SetStampWeight(csvIn.GetInt(13));
		if (!csvIn.IsEmpty(14))	PluRecord.SetBaseDeptNo(csvIn.GetInt(14));
		if (!csvIn.IsEmpty(15))	PluRecord.SetBaseTaxBandString(csvIn.GetString(15));
		//PLU TYPE IS A SPECIAL CASE - SEE BELOW
		if (!csvIn.IsEmpty(17))	PluRecord.SetExternalProcess(csvIn.GetInt(17));
		if (!csvIn.IsEmpty(18))	PluRecord.SetBaseAnalysisCategory(csvIn.GetInt(18) + 1);

		for (int n = 0; n < 6; n++)
		{
			if (!csvIn.IsEmpty(n + 19))	PluRecord.SetAllergenNo(n, csvIn.GetInt(n + 19));
		}

		if (!csvIn.IsEmpty(25))	PluRecord.SetFirstMenu(csvIn.GetInt(25));
		if (!csvIn.IsEmpty(26))	PluRecord.SetFirstQualifier(csvIn.GetInt(26));
		if (!csvIn.IsEmpty(27))	PluRecord.SetFirstExtra(csvIn.GetInt(27));

		for (int n = 0; n < 16; n++)
		{
			if (!csvIn.IsEmpty(n + 28))	PluRecord.SetOrderPrinterFlag(n, csvIn.GetBool(n + 28));
		}

		if (!csvIn.IsEmpty(44))	PluRecord.SetPriority(csvIn.GetInt(44));
		if (!csvIn.IsEmpty(45))	PluRecord.SetDiscountableFlag(csvIn.GetBool(45));
		if (!csvIn.IsEmpty(45))	PluRecord.SetPriceType(csvIn.GetInt(46));
		if (!csvIn.IsEmpty(47))	PluRecord.SetFreeTextFlag(csvIn.GetBool(47));
		if (!csvIn.IsEmpty(48))	PluRecord.SetAccountRequiredFlag(csvIn.GetBool(48));
		if (!csvIn.IsEmpty(49))	PluRecord.SetStaffDiscountFlag(csvIn.GetBool(49));
		if (!csvIn.IsEmpty(50))	PluRecord.SetSalesTrackingFlag(csvIn.GetBool(50));
		if (!csvIn.IsEmpty(51))	PluRecord.SetEANOnReceiptFlag(csvIn.GetBool(51));
		if (!csvIn.IsEmpty(52))	PluRecord.SetFreeTextDescriptionFlag(csvIn.GetBool(52));
		if (!csvIn.IsEmpty(53))	PluRecord.SetLinkType(csvIn.GetInt(53));
		if (!csvIn.IsEmpty(54))	PluRecord.SetLinkSellTiedItemFlag(csvIn.GetBool(54));
		if (!csvIn.IsEmpty(55))	PluRecord.SetAgeVerificationType(csvIn.GetInt(55));
		if (!csvIn.IsEmpty(56))	PluRecord.SetAutoCoversQtyFlag(csvIn.GetBool(56));
		if (!csvIn.IsEmpty(57))	PluRecord.SetKPNotAloneFlag(csvIn.GetBool(57));
		if (!csvIn.IsEmpty(58))	PluRecord.SetKPConsolidateTopItemFlag(csvIn.GetBool(58));
		if (!csvIn.IsEmpty(59))	PluRecord.SetSoldOutFlag(csvIn.GetBool(59));
		if (!csvIn.IsEmpty(60))	PluRecord.SetImageFilename(csvIn.GetString(60));
		if (!csvIn.IsEmpty(61))	PluRecord.SetAllowModifierFlag(csvIn.GetBool(61));
		//KP IMAGE IS A SPECIAL CASE - SEE BELOW

		if (PluRecord.GetLinkType() != 1)
		{
			PluRecord.SetLinkSellTiedItemFlag(FALSE);
		}

		if (!csvIn.IsEmpty(16))
		{
			int nSPOSType = csvIn.GetInt(16);
			int nMicrotrendType = CPluTypeHelpers::SPOSToMicrotrend(nSPOSType);

			if ((0 == nSPOSType) || (0 != nMicrotrendType))
			{
				PluRecord.SetMicrotrendPluType(nMicrotrendType);
			}
		}

		CCSV csvLink;
		for (int x = 0; x < MAX_LINK_PLU_ITEMS; x++)
		{
			if (!csvIn.IsEmpty(62 + x))
			{
				csvLink.Add(csvIn.GetString(62 + x));
			}
		}
		PluRecord.SetLinkTableString(csvLink.GetLine());

		CString strImage = csvIn.GetString(62 + MAX_LINK_PLU_ITEMS);
		if (strImage.GetLength() >= 4)
		{
			int nDbIdx = DataManager.GetActiveDbIdx();

			CString strFolder = dbDatabase.GetFolderPathDataPluImage(nDbIdx);
			if (::ExistSubdirectory(strFolder) == FALSE)
			{
				::CreateSubdirectory(strFolder);
			}

			CString strFile;
			strFile.Format("%s\\%*.*I64d.dat",
				(const char*)strFolder,
				infoPlu.m_nImageFileNoLen,
				infoPlu.m_nImageFileNoLen,
				infoPlu.m_nImageFileNo);

			char buffer[1024];

			int nBufferPos = 0;

			CSSFile file;
			if (file.Open(strFile, "wb") == TRUE)
			{
				StatusProgress.Lock(TRUE, "Creating Image File");

				int nLength = strImage.GetLength();
				for (int nFilePos = 2; nFilePos <= nLength - 2; nFilePos += 2)
				{
					StatusProgress.SetPos(nFilePos, nLength);

					CString strHex = strImage.Mid(nFilePos, 2);
					buffer[nBufferPos++] = ::HexToInt(strHex);

					if (1024 == nBufferPos)
					{
						file.Write(buffer, 1024, 1);
						nBufferPos = 0;
					}
				}

				if (0 != nBufferPos)
				{
					file.Write(buffer, nBufferPos, 1);
				}

				StatusProgress.Unlock();
			}

			PluRecord.SetKPImagePrintFlag(TRUE);
		}
	}
	else
	{
		if (TRUE == bNewPluItem)
		{
			if (!csvIn.IsEmpty(1))	PluRecord.SetEposText(csvIn.GetString(1));
			if (!csvIn.IsEmpty(14))	PluRecord.SetBaseDeptNo(csvIn.GetInt(14));
		}

		PluRecord.SetModifierEnable(infoPlu.m_nModifier, TRUE);
		if (!csvIn.IsEmpty(1))	PluRecord.SetModifierEposText(infoPlu.m_nModifier, TRUE, csvIn.GetString(1));
		if (!csvIn.IsEmpty(2))	PluRecord.SetModifierKeyText(infoPlu.m_nModifier, TRUE, csvIn.GetString(2));
		if (!csvIn.IsEmpty(3))	PluRecord.SetModifierPrice(infoPlu.m_nModifier, 0, csvIn.GetDouble(3));
		if (!csvIn.IsEmpty(4))	PluRecord.SetModifierPrice(infoPlu.m_nModifier, 1, csvIn.GetDouble(4));
		if (!csvIn.IsEmpty(5))	PluRecord.SetModifierPrice(infoPlu.m_nModifier, 2, csvIn.GetDouble(5));
		if (!csvIn.IsEmpty(6))	PluRecord.SetModifierPrice(infoPlu.m_nModifier, 3, csvIn.GetDouble(6));
		if (!csvIn.IsEmpty(7))	PluRecord.SetModifierPrice(infoPlu.m_nModifier, 4, csvIn.GetDouble(7));
		if (!csvIn.IsEmpty(8))	PluRecord.SetModifierPrice(infoPlu.m_nModifier, 5, csvIn.GetDouble(8));
		if (!csvIn.IsEmpty(9))	PluRecord.SetModifierPrice(infoPlu.m_nModifier, 6, csvIn.GetDouble(9));
		if (!csvIn.IsEmpty(10))	PluRecord.SetModifierPrice(infoPlu.m_nModifier, 7, csvIn.GetDouble(10));
		if (!csvIn.IsEmpty(11))	PluRecord.SetModifierPrice(infoPlu.m_nModifier, 8, csvIn.GetDouble(11));
		if (!csvIn.IsEmpty(12))	PluRecord.SetModifierPrice(infoPlu.m_nModifier, 9, csvIn.GetDouble(12));

		if (!csvIn.IsEmpty(14))
		{
			int nDeptNo = csvIn.GetInt(14);

			if ((TRUE == bNewPluItem) || (nDeptNo != PluRecord.GetBaseDeptNo()))
			{
				PluRecord.SetModifierDeptNoRaw(infoPlu.m_nModifier, nDeptNo);
			}
			else
			{
				PluRecord.SetModifierDeptNoRaw(infoPlu.m_nModifier, -1);
			}
		}

		if (!csvIn.IsEmpty(15))
		{
			CString strModTaxBand = csvIn.GetString(15);

			if ((TRUE == bNewPluItem) || (strModTaxBand != PluRecord.GetBaseTaxBandString()))
			{
				PluRecord.SetModifierTaxBandStringRaw(infoPlu.m_nModifier, strModTaxBand);
			}
			else
			{
				PluRecord.SetModifierTaxBandStringRaw(infoPlu.m_nModifier, "*");
			}
		}

		if (!csvIn.IsEmpty(18))
		{
			int nModCategory = csvIn.GetInt(18) + 1;

			if ((TRUE == bNewPluItem) || (nModCategory != PluRecord.GetBaseAnalysisCategory()))
			{
				PluRecord.SetModifierAnalysisCategoryRaw(infoPlu.m_nModifier, nModCategory);
			}
			else
			{
				PluRecord.SetModifierAnalysisCategoryRaw(infoPlu.m_nModifier, -1);
			}
		}

		if (!csvIn.IsEmpty(16))
		{
			int nPluType = -1;

			switch (csvIn.GetInt(16))
			{
			case 0:		nPluType = 0;		break;	//GENERAL
			case 41:	nPluType = 4;		break;	//QUALIFIER
			}

			if ((TRUE == bNewPluItem) || (nPluType != PluRecord.GetMicrotrendPluType()))
			{
				PluRecord.SetModifierMicrotrendPluType(infoPlu.m_nModifier, nPluType);
			}
			else
			{
				PluRecord.SetModifierMicrotrendPluType(infoPlu.m_nModifier, -1);
			}
		}

		if (!csvIn.IsEmpty(25))
		{
			int nModFirstMenu = csvIn.GetInt(25);

			if ((TRUE == bNewPluItem) || (nModFirstMenu != PluRecord.GetFirstMenu()))
			{
				PluRecord.SetModifierFirstMenu(infoPlu.m_nModifier, nModFirstMenu);
			}
			else
			{
				PluRecord.SetModifierFirstMenu(infoPlu.m_nModifier, -1);
			}
		}

		if (!csvIn.IsEmpty(26))
		{
			int nModFirstQualifier = csvIn.GetInt(26);

			if ((TRUE == bNewPluItem) || (nModFirstQualifier != PluRecord.GetFirstQualifier()))
			{
				PluRecord.SetModifierFirstQualifier(infoPlu.m_nModifier, nModFirstQualifier);
			}
			else
			{
				PluRecord.SetModifierFirstQualifier(infoPlu.m_nModifier, -1);
			}
		}

		if (!csvIn.IsEmpty(27))
		{
			int nModFirstExtra = csvIn.GetInt(27);

			if ((TRUE == bNewPluItem) || (nModFirstExtra != PluRecord.GetFirstExtra()))
			{
				PluRecord.SetModifierFirstExtra(infoPlu.m_nModifier, nModFirstExtra);
			}
			else
			{
				PluRecord.SetModifierFirstExtra(infoPlu.m_nModifier, -1);
			}
		}
	}

	DataManager.Plu.SavePluRecord(nPluIdx, PluRecord);

#ifdef STOCKMAN_SYSTEM
	if (bNewPluItem == TRUE)
	{
		CString strPluNo = PluRecord.GetPluNoString();
		if (m_bCreateStock == FALSE)
		{
			CRecipeCSVRecord RecipeRecord;
			RecipeRecord.SetRecipePluNo(strPluNo);
			DataManager.Plu.SetRecipe(-1, RecipeRecord);
		}
		else
		{
			CString strStockCode = BarcodePluNoTable.ConvertStockCode(strPluNo);

			CRecipeCSVRecord RecipeRecord;
			RecipeRecord.SetRecipePluNo(strPluNo);
			RecipeRecord.AddStockCode(strStockCode, 1.0);
			DataManager.Plu.SetRecipe(-1, RecipeRecord);

			//CREATE THE STOCK ITEM IF IT DOES NOT EXIST ALREADY
			CStockCSVRecord StockRecord;
			StockRecord.SetStockCode(strStockCode);
			StockRecord.SetDescription(PluRecord.GetReportText());
			StockRecord.SetCategory(PluRecord.GetBaseDeptNo());
			int nStockIdx;
			DataManager.Stock.InsertRecord(StockRecord, nStockIdx);
		}
	}
#endif
}

/**********************************************************************/

void CSQLImporter::ImportDepartments( CSSFile& fileSQL, int nPassNo )
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add( "[DptNo]" );
	csvHeader.Add( "[GrpNo]" );
	csvHeader.Add( "[Name]" );
	csvHeader.Add( "[TicketsMode]" );
	csvHeader.Add( "[Tickets]" );
	csvHeader.Add( "[QtyDP]" );
	csvHeader.Add( "[ValNeg]" );
	csvHeader.Add( "[NonTurnover]" );
	csvHeader.Add( "[SpecimenLearnPLU]" );
	csvHeader.Add( "[ImageFilename]" );

	CSQLIn SQLIn ( fileSQL, "Depts", csvHeader.GetLine() );

	CString strBuffer;
	int nTypeCount = GetFileTypeCount();
	while ( SQLIn.ReadLine( strBuffer ) )
	{
		StatusProgress.SetPos( fileSQL, nPassNo, nTypeCount );
		ImportDepartment( strBuffer );
	}
}

/**********************************************************************/

void CSQLImporter::ImportDepartment( CString& strBuffer )
{
	CCSV csvIn( strBuffer );

	//FIDDLE - DEPT ZERO IS ALWAYS PROMO
	int nDeptNo = csvIn.GetInt(0);
	if ( nDeptNo < 1 || nDeptNo > Dept::DeptNo.Max )
		return;

	m_nTotalDept++;
	
	if ( m_bChangeDatabase == FALSE )
		return;

	int nDeptIdx;
	CDepartmentCSVRecord DeptRecord;

	if ( DataManager.Department.FindDeptByNumber( nDeptNo, nDeptIdx ) == TRUE )
		DataManager.Department.GetAt( nDeptIdx, DeptRecord );
	else
	{
		DeptRecord.SetDeptNo( nDeptNo );
		DataManager.Department.InsertAt( nDeptIdx, DeptRecord );
	}

	if ( !csvIn.IsEmpty(1) ) DeptRecord.SetEposGroup( csvIn.GetInt(1) );
	if ( !csvIn.IsEmpty(2) ) DeptRecord.SetEposText( csvIn.GetString(2) );
	if ( !csvIn.IsEmpty(3) ) DeptRecord.SetTicketsMode( csvIn.GetInt(3) );
	if ( !csvIn.IsEmpty(4) ) DeptRecord.SetTickets( csvIn.GetInt(4) );
	if ( !csvIn.IsEmpty(5) ) DeptRecord.SetDPQtyFlag( csvIn.GetBool(5) );
	if ( !csvIn.IsEmpty(6) ) DeptRecord.SetNegativeFlag( csvIn.GetBool(6) );
	if ( !csvIn.IsEmpty(7) ) DeptRecord.SetNonAddFlag( csvIn.GetInt(7) != 0 );
	if ( !csvIn.IsEmpty(8) ) DeptRecord.SetSpecimenLearnPlu( csvIn.GetInt64(8) );
	if ( !csvIn.IsEmpty(9) ) DeptRecord.SetImageFilename( csvIn.GetString(9) );

	DataManager.Department.SetAt( nDeptIdx, DeptRecord );
}

/**********************************************************************/

void CSQLImporter::ImportCategories( CSSFile& fileSQL, int nPassNo )
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add( "[FieldName]" );
	csvHeader.Add( "[FieldIndex]" );
	csvHeader.Add( "[iValue]" );
	csvHeader.Add( "[fValue]" );
	csvHeader.Add( "[sValue]" );
	
	CSQLIn SQLIn ( fileSQL, "SysValues", csvHeader.GetLine() );

	CString strBuffer;
	int nTypeCount = GetFileTypeCount();
	while ( SQLIn.ReadLine( strBuffer ) )
	{
		StatusProgress.SetPos( fileSQL, nPassNo, nTypeCount );
		ImportCategory( strBuffer );
	}
}

/**********************************************************************/

void CSQLImporter::ImportCategory( CString& strBuffer )
{
	CCSV csvIn( strBuffer );

	CString strType = csvIn.GetString(0);
	if ( strType != "Category" )
		return;

	int nCatNo = csvIn.GetInt(1);
	if ( nCatNo < 1 || nCatNo > MAX_ACAT_NO )
		return;

	m_nTotalCategory++;
	
	if ( m_bChangeDatabase == FALSE )
		return;

	CAnalysisCategoryCSVRecord Category;
	
	DataManager.AnalysisCategory.GetCategory( nCatNo, Category );
	if ( !csvIn.IsEmpty(4) ) Category.SetEposText( csvIn.GetString(4) );
	if ( !csvIn.IsEmpty(2) ) Category.SetType( csvIn.GetInt(2) );
	DataManager.AnalysisCategory.SetCategory( nCatNo, Category );
}

/**********************************************************************/

void CSQLImporter::ImportPaidInOutTexts( CSSFile& fileSQL, int nPassNo )
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add( "[FieldName]" );
	csvHeader.Add( "[FieldIndex]" );
	csvHeader.Add( "[iValue]" );
	csvHeader.Add( "[fValue]" );
	csvHeader.Add( "[sValue]" );
	
	CSQLIn SQLIn ( fileSQL, "SysValues", csvHeader.GetLine() );

	CString strBuffer;
	int nTypeCount = GetFileTypeCount();
	while ( SQLIn.ReadLine( strBuffer ) )
	{
		StatusProgress.SetPos( fileSQL, nPassNo, nTypeCount );
		ImportPaidInOutText( strBuffer );
	}
}

/**********************************************************************/

void CSQLImporter::ImportPaidInOutText( CString& strBuffer )
{
	CCSV csvIn( strBuffer );

	CString strType = csvIn.GetString(0);
	if ( ( strType != "PAIDIO.IN" ) && ( strType != "PAIDIO.OUT" ) )
		return;

	int nNo = csvIn.GetInt(1);
	if ( nNo < 1 || nNo > PAIDIO_TEXT_COUNT )
		return;

	m_nTotalPaidInOutText++;
	
	if ( m_bChangeDatabase == FALSE )
		return;

	CPaidInOutTextCSVRecord Text;

	if ( strType != "PAIDIO.OUT" )
	{
		DataManager.PaidInText.GetPaidInOutText( nNo, Text );
		if ( !csvIn.IsEmpty(4) ) Text.SetEposText( csvIn.GetString(4) );
		DataManager.PaidInText.SetPaidInOutText( nNo, Text );
	}
	else
	{
		DataManager.PaidOutText.GetPaidInOutText( nNo, Text );
		if ( !csvIn.IsEmpty(4) ) Text.SetEposText( csvIn.GetString(4) );
		DataManager.PaidOutText.SetPaidInOutText( nNo, Text );
	}
}

/**********************************************************************/

void CSQLImporter::ImportLoyaltySchemes( CSSFile& fileSQL, int nPassNo )
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add( "[SchemeNo]" );
	csvHeader.Add( "[Active]" );
	csvHeader.Add( "[Name]" );
	csvHeader.Add( "[Promotion]" );
	csvHeader.Add( "[PromoType]" );
	csvHeader.Add( "[CheckDigit]" );
	csvHeader.Add( "[Points]" );
	csvHeader.Add( "[LowerID]" );
	csvHeader.Add( "[UpperID]" );
	csvHeader.Add( "[Format]" );
	csvHeader.Add( "[IsLocal]" );
	csvHeader.Add( "[StaffCard]" );
	csvHeader.Add( "[LoyMacro]" );
	csvHeader.Add( "[LimitDay]" );
	csvHeader.Add( "[LimitWeek]" );
	csvHeader.Add( "[LimitMonth1]" );
	csvHeader.Add( "[LimitMonth2]" );
	csvHeader.Add( "[LimitMonth3]" );
	csvHeader.Add( "[LimitMonth4]" );
	csvHeader.Add( "[LimitMonth5]" );
	csvHeader.Add( "[LimitMonth6]" );
	csvHeader.Add( "[LimitMonth7]" );
	csvHeader.Add( "[LimitMonth8]" );
	csvHeader.Add( "[LimitMonth9]" );
	csvHeader.Add( "[LimitMonth10]" );
	csvHeader.Add( "[LimitMonth11]" );
	csvHeader.Add( "[LimitMonth12]" );
	csvHeader.Add( "[StaffCreditEnable]" );
	csvHeader.Add( "[Promotion2]" );
	csvHeader.Add( "[Promotion3]" );
	csvHeader.Add( "[Promotion4]" );
	csvHeader.Add( "[Promotion5]" );
	csvHeader.Add( "[PaymentPermissions]" );
	csvHeader.Add( "[PaymentPermissionsMode]" );
	
	CSQLIn SQLIn ( fileSQL, "Loyalty5", csvHeader.GetLine() );

	CString strBuffer;
	int nTypeCount = GetFileTypeCount();
	while ( SQLIn.ReadLine( strBuffer ) )
	{
		StatusProgress.SetPos( fileSQL, nPassNo, nTypeCount );
		ImportLoyaltyScheme( strBuffer );
	}
}

/**********************************************************************/

void CSQLImporter::ImportLoyaltyScheme( CString& strBuffer )
{
	CCSV csvIn( strBuffer );

	int nSchemeNo = csvIn.GetInt(0);
	if (nSchemeNo < LoyaltyScheme::SchemeNo.Min || nSchemeNo > LoyaltyScheme::SchemeNo.Max)
	{
		return;
	}

	m_nTotalLoyaltyScheme++;
	
	if (m_bChangeDatabase == FALSE)
	{
		return;
	}

	int nSchemeIdx = 0;
	CLoyaltySchemeCSVRecord SchemeRecord;

	if (DataManager.LoyaltyScheme.FindSchemeByNumber(nSchemeNo, nSchemeIdx) == TRUE)
	{
		DataManager.LoyaltyScheme.GetAt(nSchemeIdx, SchemeRecord);
	}
	else
	{
		SchemeRecord.SetSchemeNo( nSchemeNo );
		DataManager.LoyaltyScheme.InsertAt( nSchemeIdx, SchemeRecord );
	}

	if ( !csvIn.IsEmpty(1) )	SchemeRecord.SetActiveFlag( csvIn.GetBool(1) );
	if ( !csvIn.IsEmpty(2) )	SchemeRecord.SetSchemeName( csvIn.GetString(2) );

	if ( (!csvIn.IsEmpty(3)) && (!csvIn.IsEmpty(4)) )
	{
		SchemeRecord.SetActionTypeAndSelection( csvIn.GetInt(4), csvIn.GetInt(3) );
	}

	if ( !csvIn.IsEmpty(5) )	SchemeRecord.SetCheckDigitFlag( csvIn.GetBool(5) );
	if ( !csvIn.IsEmpty(7) )	SchemeRecord.SetNumberStartLower( csvIn.GetString(7) );
	if ( !csvIn.IsEmpty(8) )	SchemeRecord.SetNumberStartUpper( csvIn.GetString(8) );
	if ( !csvIn.IsEmpty(9) )	SchemeRecord.SetFormat( csvIn.GetString(9) );
	if ( !csvIn.IsEmpty(11) )	SchemeRecord.SetStaffCardFlag( csvIn.GetBool(11) );
	if ( !csvIn.IsEmpty(13) )	SchemeRecord.SetStaffLimitDay( csvIn.GetInt(13) );
	if ( !csvIn.IsEmpty(14) )	SchemeRecord.SetStaffLimitWeek( csvIn.GetInt(14) );

	for ( int n = 0; n < 12; n++ )
	{
		if ( !csvIn.IsEmpty( n + 15 ) )
		{
			SchemeRecord.SetStaffLimitMonth( n, csvIn.GetInt( n + 15 ) );
		}
	}

	if ( !csvIn.IsEmpty(27) )	SchemeRecord.SetStaffCreditFlag( csvIn.GetBool(27) );
	if ( !csvIn.IsEmpty(28) )	SchemeRecord.SetPromoNo( 1, csvIn.GetInt(28) );
	if ( !csvIn.IsEmpty(29) )	SchemeRecord.SetPromoNo( 2, csvIn.GetInt(29) );
	if ( !csvIn.IsEmpty(30) )	SchemeRecord.SetPromoNo( 3, csvIn.GetInt(30) );
	if ( !csvIn.IsEmpty(31) )	SchemeRecord.SetPromoNo( 4, csvIn.GetInt(31) );
	if ( !csvIn.IsEmpty(32) )	SchemeRecord.SetDeniedPaymentsFromVPM( csvIn.GetString(32) );
	
	DataManager.LoyaltyScheme.SetAt( nSchemeIdx, SchemeRecord );
}

/**********************************************************************/

void CSQLImporter::ImportTableNames( CSSFile& fileSQL, int nPassNo )
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add( "[TblNo]" );
	csvHeader.Add( "[Name]" );
	csvHeader.Add( "[Account]" );
	csvHeader.Add( "[StatusAutoEnable]" );
	csvHeader.Add( "[StatusAutoPaidToUnused]" );
	csvHeader.Add( "[LimitEnable]" );
	csvHeader.Add( "[Limit]" );

	CSQLIn SQLIn ( fileSQL, "Tables", csvHeader.GetLine() );

	CString strBuffer;
	int nTypeCount = GetFileTypeCount();
	while ( SQLIn.ReadLine( strBuffer ) )
	{
		StatusProgress.SetPos( fileSQL, nPassNo, nTypeCount );
		ImportTableName( strBuffer );
	}
}

/**********************************************************************/

void CSQLImporter::ImportTableName( CString& strBuffer )
{
	CCSV csvIn( strBuffer );

	int nTableNo = csvIn.GetInt(0);
	if ( nTableNo < SPosTableName::TableNo.Min || nTableNo > SPosTableName::TableNo.Max )
		return;

	m_nTotalTableName++;
	
	if ( m_bChangeDatabase == FALSE )
		return;

	int nTableIdx;
	CTableNameCSVRecord Table;

	if ( DataManagerNonDb.TableNames.FindTableByNumber( nTableNo, nTableIdx ) == TRUE )
		DataManagerNonDb.TableNames.GetAt( nTableIdx, Table );
	else
	{
		Table.SetTableNo( nTableNo );
		DataManagerNonDb.TableNames.InsertAt( nTableIdx, Table );
	}

	if ( !csvIn.IsEmpty(1) )	Table.SetTableName( csvIn.GetString(1) );
	if ( !csvIn.IsEmpty(2) )	Table.SetAccountTableFlag( csvIn.GetBool(2) );
	if ( !csvIn.IsEmpty(3) )	Table.SetAutoStatusEnableFlag( csvIn.GetBool(3) );
	if ( !csvIn.IsEmpty(4) )	Table.SetAutoStatusUnusedAfterPaymentFlag( csvIn.GetBool(4) );
	if ( !csvIn.IsEmpty(5) )	Table.SetEnableLimitFlag( csvIn.GetBool(5) );
	if ( !csvIn.IsEmpty(6) )	Table.SetLimitPounds( csvIn.GetInt(6) );

	DataManagerNonDb.TableNames.SetAt( nTableIdx, Table );
}

/**********************************************************************/

void CSQLImporter::ImportTableAreas(CSSFile& fileSQL, int nPassNo)
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add("[AreaID]");
	csvHeader.Add("[AreaName]");
	csvHeader.Add("[TblnoLo]");
	csvHeader.Add("[TblnoHi]");
	csvHeader.Add("[TableOpenMacro]");
	csvHeader.Add("[AutoTabsOnly]");
	csvHeader.Add("[KPEnable1]");
	csvHeader.Add("[KPEnable2]");
	csvHeader.Add("[KPEnable3]");
	csvHeader.Add("[KPEnable4]");
	csvHeader.Add("[KPEnable5]");
	csvHeader.Add("[KPEnable6]");
	csvHeader.Add("[KPEnable7]");
	csvHeader.Add("[KPEnable8]");
	csvHeader.Add("[KPEnable9]");
	csvHeader.Add("[KPEnable10]");
	csvHeader.Add("[KPEnable11]");
	csvHeader.Add("[KPEnable12]");
	csvHeader.Add("[KPEnable13]");
	csvHeader.Add("[KPEnable14]");
	csvHeader.Add("[KPEnable15]");
	csvHeader.Add("[KPEnable16]");
	csvHeader.Add("[SuppressCovers]");

	CSQLIn SQLIn(fileSQL, "TableAreas", csvHeader.GetLine());

	CString strBuffer;
	int nTypeCount = GetFileTypeCount();
	while (SQLIn.ReadLine(strBuffer))
	{
		StatusProgress.SetPos(fileSQL, nPassNo, nTypeCount);
		ImportTableArea(strBuffer);
	}
}

/**********************************************************************/

void CSQLImporter::ImportTableArea( CString& strBuffer )
{
	CCSV csvIn( strBuffer );

	int nAreaNo = csvIn.GetInt(0);
	if ( nAreaNo < SPosTableArea::AreaNo.Min || nAreaNo > SPosTableArea::AreaNo.Max )
		return;

	m_nTotalTableArea++;
	
	if ( m_bChangeDatabase == FALSE )
		return;

	int nAreaIdx;
	CTableAreaCSVRecord Area;

	if (DataManagerNonDb.TableAreas.FindAreaByNumber(nAreaNo, nAreaIdx) == TRUE)
	{
		DataManagerNonDb.TableAreas.GetAt(nAreaIdx, Area);
	}
	else
	{
		Area.SetAreaNo( nAreaNo );
		DataManagerNonDb.TableAreas.InsertAt( nAreaIdx, Area );
	}

	if ( !csvIn.IsEmpty(1) )	Area.SetAreaName( csvIn.GetString(1) );
	if ( !csvIn.IsEmpty(2) )	Area.SetTableNoLo( csvIn.GetInt(2) );
	if ( !csvIn.IsEmpty(3) )	Area.SetTableNoHi( csvIn.GetInt(3) );
	if ( !csvIn.IsEmpty(4) )	Area.SetTableOpenMacro( csvIn.GetInt(4) );
	if ( !csvIn.IsEmpty(5) )	Area.SetAutoTabsOnlyFlag( csvIn.GetBool(5) );
	
	for ( int n = 1; n <= 16; n++ )
	{
		if ( !csvIn.IsEmpty(n+5) )	Area.SetKPFlag( n, csvIn.GetBool(n+5) );
	}

	if ( !csvIn.IsEmpty(22) )	Area.SetSuppressCoversFlag( csvIn.GetBool(22) );
	
	DataManagerNonDb.TableAreas.SetAt( nAreaIdx, Area );
}

/**********************************************************************/

void CSQLImporter::ImportGroups( CSSFile& fileSQL, int nPassNo )
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add( "[GrpNo]" );
	csvHeader.Add( "[Name]" );
	
	CSQLIn SQLIn ( fileSQL, "Groups", csvHeader.GetLine() );

	CString strBuffer;
	int nTypeCount = GetFileTypeCount();
	while ( SQLIn.ReadLine( strBuffer ) )
	{
		StatusProgress.SetPos( fileSQL, nPassNo, nTypeCount );
		ImportGroup( strBuffer );
	}
}

/**********************************************************************/

void CSQLImporter::ImportGroup( CString& strBuffer )
{
	CCSV csvIn( strBuffer );

	//FIDDLE - GROUP NUMBER ZERO IS ALWAYS PROMO
	int nGroupNo = csvIn.GetInt(0);
	if ( nGroupNo < 1 || nGroupNo > EposGroup::GroupNo.Max )
		return;

	m_nTotalGroup++;

	if ( m_bChangeDatabase == FALSE )
		return;

	int nGroupIdx;
	CGroupCSVRecordEpos GroupRecord;

	if ( DataManager.EposGroup.FindGroupByNumber( nGroupNo, nGroupIdx ) == TRUE )
		DataManager.EposGroup.GetAt( nGroupIdx, GroupRecord );
	else
	{
		GroupRecord.SetGroupNo( nGroupNo );
		DataManager.EposGroup.InsertAt( nGroupIdx, GroupRecord );
	}

	if ( !csvIn.IsEmpty(1) )	GroupRecord.SetEposText( csvIn.GetString(1) );
		
	DataManager.EposGroup.SetAt( nGroupIdx, GroupRecord );
}

/**********************************************************************/

void CSQLImporter::ImportAllergens( CSSFile& fileSQL, int nPassNo )
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add( "[AllergyCode]" );
	csvHeader.Add( "[Description]" );
	
	CSQLIn SQLIn ( fileSQL, "Allergies", csvHeader.GetLine() );

	CString strBuffer;
	int nTypeCount = GetFileTypeCount();
	while ( SQLIn.ReadLine( strBuffer ) )
	{
		StatusProgress.SetPos( fileSQL, nPassNo, nTypeCount );
		ImportAllergen( strBuffer );
	}
}

/**********************************************************************/

void CSQLImporter::ImportAllergen( CString& strBuffer )
{
	CCSV csvIn( strBuffer );

	int nAllergenNo = csvIn.GetInt(0);
	if ( nAllergenNo < Allergen::AllergenNo.Min || nAllergenNo > Allergen::AllergenNo.Max )
		return;

	m_nTotalAllergen++;

	if ( m_bChangeDatabase == FALSE )
		return;

	int nAllergenIdx;
	CGroupCSVRecordAllergen AllergenRecord;

	if ( DataManagerNonDb.Allergen.FindGroupByNumber( nAllergenNo, nAllergenIdx ) == TRUE )
		DataManagerNonDb.Allergen.GetAt( nAllergenIdx, AllergenRecord );
	else
	{
		AllergenRecord.SetGroupNo( nAllergenNo );
		DataManagerNonDb.Allergen.InsertAt( nAllergenIdx, AllergenRecord );
	}

	if ( !csvIn.IsEmpty(1) )	AllergenRecord.SetEposText( csvIn.GetString(1) );
		
	DataManagerNonDb.Allergen.SetAt( nAllergenIdx, AllergenRecord );
}

/**********************************************************************/

void CSQLImporter::ImportMixMatch(CSSFile& fileSQL, int nPassNo)
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add("[MMNo]");
	csvHeader.Add("[Enable]");
	csvHeader.Add("[UseTP]");
	csvHeader.Add("[DateStart]");
	csvHeader.Add("[DateEnd]");
	csvHeader.Add("[MMAction]");
	csvHeader.Add("[MMParam1]");
	csvHeader.Add("[OfferGroupID1]");
	csvHeader.Add("[OfferGroupID2]");
	csvHeader.Add("[OfferGroupID3]");
	csvHeader.Add("[OfferGroupID4]");
	csvHeader.Add("[OfferGroupID5]");
	csvHeader.Add("[Count1]");
	csvHeader.Add("[Count2]");
	csvHeader.Add("[Count3]");
	csvHeader.Add("[Count4]");
	csvHeader.Add("[Count5]");
	csvHeader.Add("[MMValue]");
	csvHeader.Add("[Description]");
	csvHeader.Add("[OrderMode]");
	csvHeader.Add("[UseGate]");
	csvHeader.Add("[MMParam2]");
	csvHeader.Add("[AutoLock]");
	csvHeader.Add("[PriceBandMode]");

	CSQLIn SQLIn(fileSQL, "MixMatchPlus", csvHeader.GetLine());

	CString strBuffer;
	int nTypeCount = GetFileTypeCount();
	while (SQLIn.ReadLine(strBuffer))
	{
		StatusProgress.SetPos(fileSQL, nPassNo, nTypeCount);
		ImportMixMatch(strBuffer);
	}
}

/**********************************************************************/

void CSQLImporter::ImportMixMatch(CString& strBuffer)
{
	CCSV csvIn(strBuffer);
	int nMixMatchNo = csvIn.GetInt(0);

	if (nMixMatchNo < MixMatch::MixMatchNo.Min || nMixMatchNo > MixMatch::MixMatchNo.Max)
	{
		return;
	}
	
	m_nTotalMixMatch++;

	if (m_bChangeDatabase == FALSE)
	{
		return;
	}

	CMixMatchCSVRecord MixMatch;

	int nMixMatchIdx;
	if (DataManager.MixMatch.FindTableByNumber(nMixMatchNo, nMixMatchIdx) == TRUE)
	{
		DataManager.MixMatch.GetAt(nMixMatchIdx, MixMatch);
	}
	else
	{
		MixMatch.SetMixMatchNo(nMixMatchNo);
		DataManager.MixMatch.InsertAt(nMixMatchIdx, MixMatch);
	}

	if (!csvIn.IsEmpty(1))		MixMatch.SetEnableFlag(csvIn.GetBool(1));
	if (!csvIn.IsEmpty(2))		MixMatch.SetActivationType(csvIn.GetInt(2));
	if (!csvIn.IsEmpty(3))		MixMatch.SetStartDate(csvIn.GetString(3));
	if (!csvIn.IsEmpty(4))		MixMatch.SetEndDate(csvIn.GetString(4));

	if ((!csvIn.IsEmpty(5)) && (!csvIn.IsEmpty(6)))
	{
		int nAction = csvIn.GetInt(5);
		switch (nAction)
		{
		case MIXMATCH_ACTION_DISCOUNT_PLU:
		case MIXMATCH_ACTION_FIXCOST_PLU:
		case MIXMATCH_ACTION_FIXCOST_PLU_2:
		{
			CSQLPluNoInfo infoPluNo;
			infoPluNo.m_nEposPluNo = csvIn.GetInt64(6);
			ProcessPluNo(infoPluNo);

			if (TRUE == infoPluNo.m_bValid)
			{
				MixMatch.SetActionPluNo(nAction, infoPluNo.m_nBasePluNo);
			}
			else
			{
				MixMatch.SetActionNone();
			}
		}
		break;

		case MIXMATCH_ACTION_FIXCOST_AMOUNT:
			MixMatch.SetActionCost(csvIn.GetInt(6));
			break;

		case MIXMATCH_ACTION_PROMO:
			MixMatch.SetActionPromo(csvIn.GetInt(6));
			break;

		case MIXMATCH_ACTION_FREE_ITEM:
			MixMatch.SetActionBucketFreeItem(csvIn.GetInt(6));
			break;

		case MIXMATCH_ACTION_FREE_BUCKET:
			MixMatch.SetActionBucketFreeBucket(csvIn.GetInt(6));
			break;

		case MIXMATCH_ACTION_FIXCOST_BUCKET:
			MixMatch.SetActionBucketFixedBucket(csvIn.GetInt(21), csvIn.GetInt(6));
			break;

		case MIXMATCH_ACTION_NONE:
			MixMatch.SetActionNone();
			break;
		}
	}

	if (!csvIn.IsEmpty(7))	MixMatch.SetOfferGroup1(csvIn.GetInt(7));
	if (!csvIn.IsEmpty(8))	MixMatch.SetOfferGroup2(csvIn.GetInt(8));
	if (!csvIn.IsEmpty(9))	MixMatch.SetOfferGroup3(csvIn.GetInt(9));
	if (!csvIn.IsEmpty(10))	MixMatch.SetOfferGroup4(csvIn.GetInt(10));
	if (!csvIn.IsEmpty(11))	MixMatch.SetOfferGroup5(csvIn.GetInt(11));
	if (!csvIn.IsEmpty(12))	MixMatch.SetBucketCount1(csvIn.GetInt(12));
	if (!csvIn.IsEmpty(13))	MixMatch.SetBucketCount2(csvIn.GetInt(13));
	if (!csvIn.IsEmpty(14))	MixMatch.SetBucketCount3(csvIn.GetInt(14));
	if (!csvIn.IsEmpty(15))	MixMatch.SetBucketCount4(csvIn.GetInt(15));
	if (!csvIn.IsEmpty(16))	MixMatch.SetBucketCount5(csvIn.GetInt(16));
	if (!csvIn.IsEmpty(17))	MixMatch.SetMinSpend(csvIn.GetDouble(17));
	if (!csvIn.IsEmpty(18))	MixMatch.SetDescription(csvIn.GetString(18));
	if (!csvIn.IsEmpty(19))	MixMatch.SetOrderMode(csvIn.GetInt(19));
	if (!csvIn.IsEmpty(20))	MixMatch.SetUseGate(csvIn.GetInt(20));
	if (!csvIn.IsEmpty(22))	MixMatch.SetAutoLockFlag(csvIn.GetInt(22));
	if (!csvIn.IsEmpty(23))	MixMatch.SetUseCurrentPriceBandFlag(csvIn.GetInt(23) != 0);

	DataManager.MixMatch.SetAt(nMixMatchIdx, MixMatch);
}

/**********************************************************************/

void CSQLImporter::ImportWalletStampRules(CSSFile& fileSQL, int nPassNo)
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add("[StampOfferID]");
	csvHeader.Add("[Edition]");
	csvHeader.Add("[Enabled]");
	csvHeader.Add("[OfferGroupID]");
	csvHeader.Add("[Threshold]");
	csvHeader.Add("[Expiry]");
	csvHeader.Add("[Description]");
	csvHeader.Add("[OfferType]");
	csvHeader.Add("[Action]");

	CSQLIn SQLIn(fileSQL, "StampOffers", csvHeader.GetLine());

	CString strBuffer;
	int nTypeCount = GetFileTypeCount();
	while (SQLIn.ReadLine(strBuffer))
	{
		StatusProgress.SetPos(fileSQL, nPassNo, nTypeCount);
		ImportWalletStampRule(strBuffer);
	}
}

/**********************************************************************/

void CSQLImporter::ImportWalletStampRule(CString& strBuffer)
{
	CCSV csvIn(strBuffer);
	int nRuleID = csvIn.GetInt(0);

	if (nRuleID < WalletStampRule::RuleID.Min || nRuleID > WalletStampRule::RuleID.Max)
	{
		return;
	}

	m_nTotalWalletStampRule++;

	if (m_bChangeDatabase == FALSE)
	{
		return;
	}

	CWalletStampRuleCSVRecord Rule;

	int nRuleIdx = 0;
	bool bNewRule = FALSE;
	if (DataManager.WalletStampRule.FindRuleByID(nRuleID, nRuleIdx) == TRUE)
	{
		DataManager.WalletStampRule.GetAt(nRuleIdx, Rule);
	}
	else
	{
		Rule.SetRuleID(nRuleID);
		bNewRule = TRUE;
	}

	if (!csvIn.IsEmpty(1))		Rule.SetEdition(csvIn.GetInt(1));
	if (!csvIn.IsEmpty(2))		Rule.SetEnableFlag(csvIn.GetBool(2));
	if (!csvIn.IsEmpty(3))		Rule.SetOfferGroupNo(csvIn.GetInt(3));
	if (!csvIn.IsEmpty(4))		Rule.SetThreshold(csvIn.GetInt(4));
	if (!csvIn.IsEmpty(5))		Rule.SetExpiry(csvIn.GetInt(5));
	if (!csvIn.IsEmpty(6))		Rule.SetDescription(csvIn.GetString(6));
	
	int nOfferType = csvIn.GetInt(7);
	int nAction = csvIn.GetInt(8);

	bool bAccept = FALSE;
	switch (SysInfo.GetLoyaltyOrSmartPayType())
	{
	case LOYALTY_TYPE_LOY_V4:
		bAccept = (nOfferType == 0);
		//bAccept &= ((nAction == 1) || (nAction == 2));
		//Rule.SetAction(nAction);
		Rule.SetAction(1);
		break;

	case LOYALTY_TYPE_SMP_V4:
		bAccept = (nOfferType == 1);
		//bAccept &= (nAction == 3);
		Rule.SetAction(3);
		Rule.SetEdition(1);
		break;
	}

	if (TRUE == bAccept)
	{
		if (TRUE == bNewRule)
		{
			DataManager.WalletStampRule.InsertAt(nRuleIdx, Rule);
		}
		else
		{
			DataManager.WalletStampRule.SetAt(nRuleIdx, Rule);
		}
	}
}

/**********************************************************************/

void CSQLImporter::ImportOfferGroup( CSSFile& fileSQL, int nPassNo )
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add( "[OfferGroupID]" );
	csvHeader.Add( "[OfferGroupName]" );
	
	CSQLIn SQLIn ( fileSQL, "OfferGroups", csvHeader.GetLine() );

	CString strBuffer;
	int nTypeCount = GetFileTypeCount();
	while ( SQLIn.ReadLine( strBuffer ) )
	{
		StatusProgress.SetPos( fileSQL, nPassNo, nTypeCount );
		ImportOfferGroup( strBuffer );
	}
}

/**********************************************************************/

void CSQLImporter::ImportOfferGroup( CString& strBuffer )
{
	CCSV csvIn( strBuffer );
	int nGroupNo = csvIn.GetInt(0);

	if ( nGroupNo < OfferGroups::GroupNo.Min || nGroupNo > OfferGroups::GroupNo.Max )
		return;

	m_nTotalOfferGroup++;

	if ( m_bChangeDatabase == FALSE )
	{
		CString strLabel;
		strLabel.Format( "%3.3d", nGroupNo );

		CSortedStringItem item;
		item.m_strItem = strLabel;
		m_arrayOfferGroups.Consolidate( item );
		return;
	}

	COfferGroupsCSVRecord Group( NODE_OFFER_GROUP );

	int nGroupIdx;
	if ( DataManager.OfferGroup.FindTableByNumber( nGroupNo, nGroupIdx ) == TRUE )
		DataManager.OfferGroup.GetAt( nGroupIdx, Group );
	else
	{
		Group.SetGroupNo( nGroupNo );
		DataManager.OfferGroup.InsertAt( nGroupIdx, Group );
	}

	if ( !csvIn.IsEmpty(1) )		Group.SetGroupName( csvIn.GetString(1) );
	
	DataManager.OfferGroup.SetAt( nGroupIdx, Group );
}

/**********************************************************************/

void CSQLImporter::ImportOfferEntry( CSSFile& fileSQL, int nPassNo )
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add( "[OfferGroupID]" );
	csvHeader.Add( "[OfferGroupType]" );
	csvHeader.Add( "[ID]" );
	
	CSQLIn SQLIn ( fileSQL, "OfferGroupEntries", csvHeader.GetLine() );

	CString strBuffer;
	int nTypeCount = GetFileTypeCount();
	while ( SQLIn.ReadLine( strBuffer ) )
	{
		StatusProgress.SetPos( fileSQL, nPassNo, nTypeCount );
		ImportOfferEntry( strBuffer );
	}
}

/**********************************************************************/

void CSQLImporter::ImportOfferEntry( CString& strBuffer )
{
	CCSV csvIn( strBuffer );
	int nGroupNo = csvIn.GetInt(0);

	if ( nGroupNo < OfferGroups::GroupNo.Min || nGroupNo > OfferGroups::GroupNo.Max )
		return;

	m_nTotalOfferEntry++;

	if ( m_bChangeDatabase == FALSE )
	{
		CString strLabel;
		strLabel.Format( "%3.3d", nGroupNo );

		CSortedStringItem item;
		item.m_strItem = strLabel;
		m_arrayOfferGroups.Consolidate( item );
		return;
	}

	COfferGroupEntriesCSVRecord Entry;
	Entry.SetGroupNo( nGroupNo );
	
	if ( ( !csvIn.IsEmpty(1) ) && ( !csvIn.IsEmpty(2) ) )
	{
		int nEntryType = csvIn.GetInt(1);
		
		switch( nEntryType )
		{
		case OFFERENTRY_PLU:
		case OFFERENTRY_PLU_EXCLUDE:
			{
				CSQLPluNoInfo info;
				info.m_nEposPluNo = csvIn.GetInt64(2);
				ProcessPluNo( info );

				Entry.SetEntryPluNo( nEntryType, info.m_nBasePluNo );

				int nIndex;
				if ( DataManager.OfferGroupEntry.FindEntryKey( Entry, nIndex ) == TRUE )
				{
					DataManager.OfferGroupEntry.GetAt( nIndex, Entry );
					Entry.SetSingleModifierFlag( info.m_nModifier );
					DataManager.OfferGroupEntry.SetAt( nIndex, Entry );
				}
				else
				{
					Entry.ClearModifierFlags();
					Entry.SetSingleModifierFlag( info.m_nModifier );
					DataManager.OfferGroupEntry.InsertRecord( Entry, nIndex );
				}
			}
			break;

		case OFFERENTRY_DEPT:
		case OFFERENTRY_DEPT_SAMEPLU:
			{
				Entry.SetEntryDeptNo( nEntryType, csvIn.GetInt(2) );

				int nIndex;	
				DataManager.OfferGroupEntry.InsertRecord( Entry, nIndex );
			}
			break;

		case OFFERENTRY_GROUP:
			{
				Entry.SetEntryGroupNo( csvIn.GetInt(2) );

				int nIndex;	
				DataManager.OfferGroupEntry.InsertRecord( Entry, nIndex );
			}
			break;
		}
	}

	COfferGroupsCSVRecord Group( NODE_OFFER_GROUP );

	int nGroupIdx;
	if ( DataManager.OfferGroup.FindTableByNumber( nGroupNo, nGroupIdx ) == FALSE )
	{
		Group.SetGroupNo( nGroupNo );
		Group.SetGroupName( "" );
		DataManager.OfferGroup.InsertAt( nGroupIdx, Group );
	}
}

/**********************************************************************/

void CSQLImporter::ImportTimePlan( CSSFile& fileSQL, int nPassNo )
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add( "[PlanNo]" );
	csvHeader.Add( "[Name]" );
	
	CSQLIn SQLIn ( fileSQL, "TimePlans", csvHeader.GetLine() );

	CString strBuffer;
	int nTypeCount = GetFileTypeCount();
	while ( SQLIn.ReadLine( strBuffer ) )
	{
		StatusProgress.SetPos( fileSQL, nPassNo, nTypeCount );
		ImportTimePlan( strBuffer );
	}
}

/**********************************************************************/

void CSQLImporter::ImportTimePlan( CString& strBuffer )
{
	CCSV csvIn( strBuffer );
	int nPlanNo = csvIn.GetInt(0);

	if ( nPlanNo < TimePlans::PlanNo.Min || nPlanNo > TimePlans::PlanNo.Max )
		return;

	m_nTotalTimePlan++;

	if ( m_bChangeDatabase == FALSE )
	{
		CString strLabel;
		strLabel.Format( "%3.3d", nPlanNo );

		CSortedStringItem item;
		item.m_strItem = strLabel;
		m_arrayTimePlans.Consolidate( item );
		return;
	}

	CTimePlansCSVRecord Plan;

	int nPlanIdx;
	if ( DataManager.TimePlan.FindPlanByNumber( nPlanNo, nPlanIdx ) == TRUE )
		DataManager.TimePlan.GetAt( nPlanIdx, Plan );
	else
	{
		Plan.SetPlanNo( nPlanNo );
		DataManager.TimePlan.InsertAt( nPlanIdx, Plan );
	}

	if ( !csvIn.IsEmpty(1) )		Plan.SetPlanName( csvIn.GetString(1) );
	
	DataManager.TimePlan.SetAt( nPlanIdx, Plan );
}

/**********************************************************************/

void CSQLImporter::ImportTimePlanEntry( CSSFile& fileSQL, int nPassNo )
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add( "[PlanNo]" );
	csvHeader.Add( "[EntryNo]" );
	csvHeader.Add( "[Active]" );
	csvHeader.Add( "[Name]" );
	csvHeader.Add( "[Type]" );
	csvHeader.Add( "[Sunday]" );
	csvHeader.Add( "[Monday]" );
	csvHeader.Add( "[Tuesday]" );
	csvHeader.Add( "[Wednesday]" );
	csvHeader.Add( "[Thursday]" );
	csvHeader.Add( "[Friday]" );
	csvHeader.Add( "[Saturday]" );
	csvHeader.Add( "[StartHours]" );
	csvHeader.Add( "[StartMins]" );
	csvHeader.Add( "[EndHours]" );
	csvHeader.Add( "[EndMins]" );
	csvHeader.Add( "[EndPeriod]" );
	csvHeader.Add( "[IPam1]" );
	csvHeader.Add( "[IPam2]" );
	csvHeader.Add( "[SPam1]" );
	csvHeader.Add( "[DateFrom]" );
	csvHeader.Add( "[DateTo]" );
	
	CSQLIn SQLIn ( fileSQL, "TimePlanEntrys", csvHeader.GetLine() );

	CString strBuffer;
	int nTypeCount = GetFileTypeCount();
	while ( SQLIn.ReadLine( strBuffer ) )
	{
		StatusProgress.SetPos( fileSQL, nPassNo, nTypeCount );
		ImportTimePlanEntry( strBuffer );
	}
}

/**********************************************************************/

void CSQLImporter::ImportTimePlanEntry( CString& strBuffer )
{
	CCSV csvIn( strBuffer );

	int nPlanNo = csvIn.GetInt(0);
	if ( nPlanNo < TimePlans::PlanNo.Min || nPlanNo > TimePlanEntries::PlanNo.Max )
		return;

	int nEntryNo = csvIn.GetInt(1);
	if ( nEntryNo < TimePlanEntries::EntryNo.Min || nEntryNo > TimePlanEntries::EntryNo.Max )
		return;

	//Template entries are marked with 987 in field 18 at download.
	//This field is not used by SPOS.
	//By not re-importing these entries we avoid a feedback loop.
	if (EcrmanOptions.GetFeaturesTimePlanTemplateFlag() == TRUE)
	{
		if ( csvIn.GetInt(18) == 987)
		{
			return;
		}
	}

	m_nTotalTimePlanEntry++;

	if ( m_bChangeDatabase == FALSE )
	{
		CString strLabel;
		strLabel.Format( "%3.3d", nPlanNo );

		CSortedStringItem item;
		item.m_strItem = strLabel;
		m_arrayTimePlans.Consolidate( item );
		return;
	}

	CTimePlanEntriesCSVRecord Entry;
	Entry.SetPlanNo( nPlanNo );
	Entry.SetEntryNo( nEntryNo );
	
	int nEntryIdx;
	if ( DataManager.TimePlanEntry.FindKey( Entry, nEntryIdx ) == TRUE )
		DataManager.TimePlanEntry.GetAt( nEntryIdx, Entry );
	else
		DataManager.TimePlanEntry.InsertAt( nEntryIdx, Entry );

	if ( !csvIn.IsEmpty(2) )	Entry.SetActiveFlag( csvIn.GetBool(2) );
	if ( !csvIn.IsEmpty(3) )	Entry.SetName( csvIn.GetString(3) );

	if ( ( !csvIn.IsEmpty(4) ) && ( !csvIn.IsEmpty(17) ) )
		Entry.SetTypeAndSelection( csvIn.GetInt(4), csvIn.GetInt(17) );

	if ( !csvIn.IsEmpty(5) )	Entry.SetSundayFlag( csvIn.GetBool(5) );
	if ( !csvIn.IsEmpty(6) )	Entry.SetMondayFlag( csvIn.GetBool(6) );
	if ( !csvIn.IsEmpty(7) )	Entry.SetTuesdayFlag( csvIn.GetBool(7) );
	if ( !csvIn.IsEmpty(8) )	Entry.SetWednesdayFlag( csvIn.GetBool(8) );
	if ( !csvIn.IsEmpty(9) )	Entry.SetThursdayFlag( csvIn.GetBool(9) );
	if ( !csvIn.IsEmpty(10) )	Entry.SetFridayFlag( csvIn.GetBool(10) );
	if ( !csvIn.IsEmpty(11) )	Entry.SetSaturdayFlag( csvIn.GetBool(11) );
	if ( !csvIn.IsEmpty(12) )	Entry.SetStartHour( csvIn.GetInt(12) );
	if ( !csvIn.IsEmpty(13) )	Entry.SetStartMinute( csvIn.GetInt(13) );
	if ( !csvIn.IsEmpty(14) )	Entry.SetEndHour( csvIn.GetInt(14) );
	if ( !csvIn.IsEmpty(15) )	Entry.SetEndMinute( csvIn.GetInt(15) );
	if ( !csvIn.IsEmpty(16) )	Entry.SetEndPeriod( csvIn.GetInt(16) );
	if ( !csvIn.IsEmpty(20) )	Entry.SetStartDate( csvIn.GetDouble(20 ) + 2.0 );
	if ( !csvIn.IsEmpty(21) )	Entry.SetEndDate( csvIn.GetDouble(21 ) + 2.0 );

	DataManager.TimePlanEntry.SetAt( nEntryIdx, Entry );
	
	CTimePlansCSVRecord Plan;

	int nPlanIdx;
	if ( DataManager.TimePlan.FindPlanByNumber( nPlanNo, nPlanIdx ) == FALSE )
	{
		Plan.SetPlanNo( nPlanNo );
		Plan.SetPlanName( "" );
		DataManager.TimePlan.InsertAt( nPlanIdx, Plan );
	}
}

/**********************************************************************/

void CSQLImporter::ImportPromotion( CSSFile& fileSQL, int nPassNo )
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add( "[PromNo]" );
	csvHeader.Add( "[Type]" );
	csvHeader.Add( "[FlexPromoType]" );
	csvHeader.Add( "[pcValue]" );
	csvHeader.Add( "[mdValue]" );
	csvHeader.Add( "[Trip]" );
	csvHeader.Add( "[DptNo]" );
	csvHeader.Add( "[Gratuity]" );
	csvHeader.Add( "[Vat]" );
	csvHeader.Add( "[Description]" );
	csvHeader.Add( "[Usage]" );
	csvHeader.Add( "[AllowMyPromoOnPromo]" );
	csvHeader.Add( "[AllowPromoOnMyPromo]" );
	csvHeader.Add( "[Reasons]" );
	csvHeader.Add( "[TimeplanMode]" );
	csvHeader.Add( "[UseGate]" );
	csvHeader.Add( "[FilterType]" );
	csvHeader.Add( "[FilterOfferGroup]" );
	csvHeader.Add( "[FlexBoundByCov1]" );
	csvHeader.Add( "[FlexBoundByCov2]" );
	csvHeader.Add( "[FlexBoundByCov3]" );
	csvHeader.Add( "[FlexBoundByCov4]" );
	csvHeader.Add( "[FlexBoundByVal1]" );
	csvHeader.Add( "[FlexBoundByVal2]" );
	csvHeader.Add( "[FlexBoundByVal3]" );
	csvHeader.Add( "[FlexBoundByVal4]" );
	csvHeader.Add( "[FlexpcValue1]" );
	csvHeader.Add( "[FlexpcValue2]" );
	csvHeader.Add( "[FlexpcValue3]" );
	csvHeader.Add( "[FlexpcValue4]" );
	csvHeader.Add( "[FlexpcValue5]" );
	csvHeader.Add( "[AnalysisNew]" );
	csvHeader.Add( "[CatFlags]" );
	csvHeader.Add( "[AnalysisCats]" );
	csvHeader.Add( "[MarkdownUpTo]");
	
	CSQLIn SQLIn ( fileSQL, "Promos", csvHeader.GetLine() );

	CString strBuffer;
	int nTypeCount = GetFileTypeCount();
	while ( SQLIn.ReadLine( strBuffer ) )
	{
		StatusProgress.SetPos( fileSQL, nPassNo, nTypeCount );
		ImportPromotion( strBuffer );
	}
}

/**********************************************************************/

void CSQLImporter::ImportPromotion( CString& strBuffer )
{
	CCSV csvIn( strBuffer );
	int nPromoNo = csvIn.GetInt(0);

	if ((nPromoNo < Promotions::PromoNo.Min) || (nPromoNo > Promotions::PromoNo.Max))
	{
		return;
	}

	m_nTotalPromotion++;

	if (m_bChangeDatabase == FALSE)
	{
		return;
	}

	CPromotionsCSVRecord Promo;

	int nPromoIdx = 0;
	if (DataManager.Promotion.FindTableByNumber(nPromoNo, nPromoIdx) == TRUE)
	{
		DataManager.Promotion.GetAt(nPromoIdx, Promo);
	}
	else
	{
		Promo.SetPromoNo( nPromoNo );
		DataManager.Promotion.InsertAt( nPromoIdx, Promo );
	}
	
	bool bAnalysisNew = FALSE;
	int nCatFlags = 0;
	CString strCatFlags = "";

	if ( !csvIn.IsEmpty(1) ) Promo.SetPromoType( csvIn.GetInt(1) );
	if ( !csvIn.IsEmpty(2) ) Promo.SetFlexType( csvIn.GetInt(2) );
	if ( !csvIn.IsEmpty(3) ) Promo.SetPercent( csvIn.GetDouble(3) );
	if ( !csvIn.IsEmpty(4) ) Promo.SetMarkdown( csvIn.GetDouble(4) );
	if ( !csvIn.IsEmpty(5) ) Promo.SetTrip( csvIn.GetDouble(5) );
	if ( !csvIn.IsEmpty(6) ) Promo.SetDeptNoEpos( csvIn.GetInt(6) );
	if ( !csvIn.IsEmpty(7) ) Promo.SetGratuityFlag( csvIn.GetBool(7) );
	if ( !csvIn.IsEmpty(8) ) Promo.SetSPOSApplyTaxFlag( csvIn.GetBool(8) );
	if ( !csvIn.IsEmpty(9) ) Promo.SetDescription( csvIn.GetString(9) );
	if ( !csvIn.IsEmpty(10) ) Promo.SetUsage( csvIn.GetInt(10) );
	if ( !csvIn.IsEmpty(11) ) Promo.SetMyPromoOnPromoFlag( csvIn.GetBool(11) );
	if ( !csvIn.IsEmpty(12) ) Promo.SetPromoOnMyPromoFlag( csvIn.GetBool(12) );
	if ( !csvIn.IsEmpty(13) ) Promo.SetReasonsFlag( csvIn.GetBool(13) );
	if ( !csvIn.IsEmpty(14) ) Promo.SetTimeplanMode( csvIn.GetInt(14) );
	if ( !csvIn.IsEmpty(15) ) Promo.SetUseGate( csvIn.GetInt(15) );
	if ( !csvIn.IsEmpty(16) ) Promo.SetFilterType( csvIn.GetInt(16) );
	if ( !csvIn.IsEmpty(17) ) Promo.SetFilterOfferGroup( csvIn.GetInt(17) );
	if ( !csvIn.IsEmpty(18) ) Promo.SetFlexBoundCovers( 1, csvIn.GetInt(18) );
	if ( !csvIn.IsEmpty(19) ) Promo.SetFlexBoundCovers( 2, csvIn.GetInt(19) );
	if ( !csvIn.IsEmpty(20) ) Promo.SetFlexBoundCovers( 3, csvIn.GetInt(20) );
	if ( !csvIn.IsEmpty(21) ) Promo.SetFlexBoundCovers( 4, csvIn.GetInt(21) );
	if ( !csvIn.IsEmpty(22) ) Promo.SetFlexBoundValue( 1, csvIn.GetDouble(22) );
	if ( !csvIn.IsEmpty(23) ) Promo.SetFlexBoundValue( 2, csvIn.GetDouble(23) );
	if ( !csvIn.IsEmpty(24) ) Promo.SetFlexBoundValue( 3, csvIn.GetDouble(24) );
	if ( !csvIn.IsEmpty(25) ) Promo.SetFlexBoundValue( 4, csvIn.GetDouble(25) );
	if ( !csvIn.IsEmpty(26) ) Promo.SetFlexPercent( 0, csvIn.GetDouble(26) );
	if ( !csvIn.IsEmpty(27) ) Promo.SetFlexPercent( 1, csvIn.GetDouble(27) );
	if ( !csvIn.IsEmpty(28) ) Promo.SetFlexPercent( 2, csvIn.GetDouble(28) );
	if ( !csvIn.IsEmpty(29) ) Promo.SetFlexPercent( 3, csvIn.GetDouble(29) );
	if ( !csvIn.IsEmpty(30) ) Promo.SetFlexPercent( 4, csvIn.GetDouble(30) );
	if ( !csvIn.IsEmpty(31) ) bAnalysisNew = csvIn.GetBool(31);
	if ( !csvIn.IsEmpty(32) ) nCatFlags = csvIn.GetInt(32);
	if ( !csvIn.IsEmpty(33) ) strCatFlags = csvIn.GetString(33);
	if ( !csvIn.IsEmpty(34) ) Promo.SetUpToFlag(csvIn.GetBool(34));

	if (FALSE == bAnalysisNew)
	{
		Promo.SetLegacyAnalysis(nCatFlags);
	}
	else
	{
		CHexBitArray HexBitArray(100);
		CCSV csvFlags( strCatFlags, ';' );

		for ( int n = 0; ( n < csvFlags.GetSize() ) && ( n < 100 ); n++ )
		{
			CString strNum = csvFlags.GetString(n);
			
			if (strNum != "")
			{
				HexBitArray.SetBit(atoi(strNum), TRUE);
			}
		}

		CString strHex = "";
		HexBitArray.TranslateToHexString( strHex );
		Promo.SetFilterAnalysis( strHex );
	}

	Promo.ValidateFlexSettings();
	Promo.ValidateUpToFlag();

	DataManager.Promotion.SetAt( nPromoIdx, Promo );
}

/**********************************************************************/

void CSQLImporter::ImportModifiers( CSSFile& fileSQL, int nPassNo )
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add( "[ModNo]" );
	csvHeader.Add( "[Mult]" );
	csvHeader.Add( "[Name]" );

	CSQLIn SQLIn ( fileSQL, "PLUModifiers", csvHeader.GetLine() );

	CString strBuffer;
	int nTypeCount = GetFileTypeCount();
	while ( SQLIn.ReadLine( strBuffer ) )
	{
		StatusProgress.SetPos( fileSQL, nPassNo, nTypeCount );
		ImportModifier( strBuffer );
	}
}

/**********************************************************************/

void CSQLImporter::ImportModifier( CString& strBuffer )
{
	CCSV csvIn( strBuffer );

	int nModNo = csvIn.GetInt(0);
	if ( nModNo < 1 || nModNo > 9 )
		return;

	m_nTotalModifier++;

	if ( m_bChangeDatabase == FALSE )
		return;

	if ( !csvIn.IsEmpty(1) ) DataManager.Modifier.SetMultiplierForQty( nModNo, csvIn.GetDouble(1) );
	if ( !csvIn.IsEmpty(2) ) DataManager.Modifier.SetName( nModNo, csvIn.GetString(2) );
}

/**********************************************************************/

void CSQLImporter::ImportPaymentMethods( CSSFile& fileSQL, int nPassNo )
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add ( "[PymNo]" );
	csvHeader.Add ( "[Name]" );
	csvHeader.Add ( "[PaymentType]" );
	csvHeader.Add ( "[Floor]" );
	csvHeader.Add ( "[Tender]" );
	csvHeader.Add ( "[OverTender]" );
	csvHeader.Add ( "[Change]" );
	csvHeader.Add ( "[Gratuity]" );
	csvHeader.Add ( "[Drawer]" );
	csvHeader.Add ( "[Cashback]" );
	csvHeader.Add ( "[NonBank]" );
	csvHeader.Add ( "[PayText]" );
	csvHeader.Add ( "[RctPrint]" );
	csvHeader.Add ( "[AccountAllow]" );
	csvHeader.Add ( "[CashDeclaration]" );
	csvHeader.Add ( "[FuncNo]" );
	csvHeader.Add ( "[ForexNo]" );
	csvHeader.Add ( "[ForHH]" );
	csvHeader.Add ( "[WithCashLimitDrawer]" );
	csvHeader.Add ( "[DepositChange]" );
	csvHeader.Add ( "[DepositChangeAsVoucherMin]" );
	csvHeader.Add ( "[ExternAppParam1]" );
	csvHeader.Add ( "[ChangeAsCashback]" );
	
	CSQLIn SQLIn ( fileSQL, "PayMeths", csvHeader.GetLine() );

	CString strBuffer;
	int nTypeCount = GetFileTypeCount();
	while ( SQLIn.ReadLine( strBuffer ) )
	{
		StatusProgress.SetPos( fileSQL, nPassNo, nTypeCount );
		ImportPaymentMethod( strBuffer );
	}
}

/**********************************************************************/

void CSQLImporter::ImportPaymentMethod( CString& strBuffer )
{
	CCSV csvIn( strBuffer );

	//IGNORE LEGACY PAYMENT METHODS
	if ( csvIn.IsEmpty(2) == TRUE )
	{
		m_bGotLegacyPayments = TRUE;
		return;
	}

	int nPayMethNo = csvIn.GetInt(0);
	if ( nPayMethNo < Payment::PaymentNo.Min || nPayMethNo > Payment::PaymentNo.Max )
		return;

	m_nTotalPayMeth++;

	if ( m_bChangeDatabase == FALSE )
		return;

	int nPayMethIdx;
	CPaymentCSVRecord PayMethRecord;

	if ( DataManager.Payment.FindPaymentByNumber( nPayMethNo, nPayMethIdx ) == TRUE )
		DataManager.Payment.GetAt( nPayMethIdx, PayMethRecord );
	else
	{
		PayMethRecord.SetPaymentNo( nPayMethNo );
		DataManager.Payment.InsertAt( nPayMethIdx, PayMethRecord );
	}

	if ( !csvIn.IsEmpty(1) ) PayMethRecord.SetEPOSName( csvIn.GetString(1) );
	if ( !csvIn.IsEmpty(2) ) PayMethRecord.SetPaymentType( csvIn.GetInt(2) );	
	if ( !csvIn.IsEmpty(3) ) PayMethRecord.SetLimit( csvIn.GetDouble(3) );
	if ( !csvIn.IsEmpty(4) ) PayMethRecord.SetTender( csvIn.GetBool(4) );	
	if ( !csvIn.IsEmpty(5) ) PayMethRecord.SetOverTender( csvIn.GetBool(5) );
	if ( !csvIn.IsEmpty(6) ) PayMethRecord.SetChange( csvIn.GetBool(6) );
	if ( !csvIn.IsEmpty(7) ) PayMethRecord.SetGratuity( csvIn.GetBool(7) );
	if ( !csvIn.IsEmpty(8) ) PayMethRecord.SetDrawer( csvIn.GetBool(8) );
	
	if ( !csvIn.IsEmpty(9) && !csvIn.IsEmpty(2) )
	{
		switch( PayMethRecord.GetPaymentType() )
		{
		case PAYMENT_TYPE_EFT:
		case PAYMENT_TYPE_EFT_CNP:
			PayMethRecord.SetEFTCashback( csvIn.GetBool(9) );
			break;
		}
	}
		
	if ( !csvIn.IsEmpty(10) ) PayMethRecord.SetNonBnk( csvIn.GetBool(10) );
		
	if ( !csvIn.IsEmpty(11) )
	{
		CString strText = csvIn.GetString(11);
		PayMethRecord.ImportSQLText( strText );
	}
		
	if ( !csvIn.IsEmpty(12) ) PayMethRecord.SetReceipt( csvIn.GetBool(12) );
	if ( !csvIn.IsEmpty(13) ) PayMethRecord.SetAccountAllow( csvIn.GetBool(13) );
	if ( !csvIn.IsEmpty(14) ) PayMethRecord.SetCashDeclarationFlag( csvIn.GetBool(14) );
	if ( !csvIn.IsEmpty(15) ) PayMethRecord.SetFunctionNo( csvIn.GetInt(15) );
	if ( !csvIn.IsEmpty(16) ) PayMethRecord.SetForexNo( csvIn.GetInt(16) );
	if ( !csvIn.IsEmpty(17) ) PayMethRecord.SetHandheld( csvIn.GetBool(17) );
	if ( !csvIn.IsEmpty(18) ) PayMethRecord.SetCashLimitFlag( csvIn.GetBool(18) );
	if ( !csvIn.IsEmpty(19) ) PayMethRecord.SetDepositChangeType( csvIn.GetInt(19) );
	if ( !csvIn.IsEmpty(20) ) PayMethRecord.SetDepositChangeVoucherMin( csvIn.GetInt(20) );
	if ( !csvIn.IsEmpty(21) ) PayMethRecord.SetExternalParams( csvIn.GetString(21) );
	if ( !csvIn.IsEmpty(22) ) PayMethRecord.SetChangeAsCashbackFlag( csvIn.GetBool(22) );

	DataManager.Payment.SetAt( nPayMethIdx, PayMethRecord );
}

/**********************************************************************/

void CSQLImporter::ImportServers(CSSFile& fileSQL, int nPassNo)
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add("[SrvNo]");
	csvHeader.Add("[Name]");
	csvHeader.Add("[Auth]");
	csvHeader.Add("[Password]");
	csvHeader.Add("[Drawer]");
	csvHeader.Add("[FlipView]");
	csvHeader.Add("[TrainingMode]");
	csvHeader.Add("[LogonMacro]");
	csvHeader.Add("[OpenTableMode]");
	csvHeader.Add("[TradingColourScheme]");

	CSQLIn SQLIn(fileSQL, "Servers", csvHeader.GetLine());

	CString strBuffer = "";
	int nTypeCount = GetFileTypeCount();
	while (SQLIn.ReadLine(strBuffer))
	{
		StatusProgress.SetPos(fileSQL, nPassNo, nTypeCount);
		ImportServer(strBuffer);
	}
}

/**********************************************************************/

void CSQLImporter::ImportServer(CString& strBuffer)
{
	CCSV csvIn(strBuffer);

	//IGNORE LEGACY SERVERS
	if (csvIn.IsEmpty(6) == TRUE)
	{
		m_bGotLegacyServers = TRUE;
		return;
	}

	int nServerNo = csvIn.GetInt(0);
	if (nServerNo < Server::ServerNo.Min || nServerNo > Server::ServerNo.Max)
	{
		return;
	}

	m_nTotalServer++;

	if (m_bChangeDatabase == FALSE)
	{
		return;
	}

	int nServerIdx = 0;
	CServerCSVRecord ServerRecord;

	if (DataManagerNonDb.Server.FindServerByNumber(nServerNo, nServerIdx) == TRUE)
	{
		DataManagerNonDb.Server.GetAt(nServerIdx, ServerRecord);
	}
	else
	{
		ServerRecord.SetServerNo(nServerNo);
		DataManagerNonDb.Server.InsertAt(nServerIdx, ServerRecord);
	}

	if (!csvIn.IsEmpty(1))	ServerRecord.SetReceiptName(csvIn.GetString(1));
	if (!csvIn.IsEmpty(2))	ServerRecord.SetAuthority(csvIn.GetInt(2));
	if (!csvIn.IsEmpty(3))	ServerRecord.SetPassword(csvIn.GetInt(3));
	if (!csvIn.IsEmpty(4))	ServerRecord.SetDrawer(csvIn.GetInt(4));
	if (!csvIn.IsEmpty(5))	ServerRecord.SetFlipViewFlag(csvIn.GetBool(5));
	if (!csvIn.IsEmpty(6))	ServerRecord.SetTrainingModeFlag(csvIn.GetBool(6));
	if (!csvIn.IsEmpty(7))	ServerRecord.SetLogonMacro(csvIn.GetInt(7));
	if (!csvIn.IsEmpty(8))	ServerRecord.SetOpenTableMode(csvIn.GetInt(8));
	if (!csvIn.IsEmpty(9))	ServerRecord.SetTradingColourScheme(csvIn.GetInt(9));

	DataManagerNonDb.Server.SetAt(nServerIdx, ServerRecord);
}

/**********************************************************************/

void CSQLImporter::ImportServerLogons(CSSFile& fileSQL, int nPassNo)
{
	fileSQL.Seek(0, CFile::begin);
	CCSV csvHeader;
	csvHeader.Add("[KeyValue]");
	csvHeader.Add("[SrvNo]");

	CSQLIn SQLIn(fileSQL, "CLCKeys", csvHeader.GetLine());
	CString strBuffer = "";
	int nTypeCount = GetFileTypeCount();
	while (SQLIn.ReadLine(strBuffer))
	{
		StatusProgress.SetPos(fileSQL, nPassNo, nTypeCount);
		ImportServerLogon(strBuffer);
	}
}

/**********************************************************************/

void CSQLImporter::ImportServerLogon(CString& strBuffer)
{
	CCSV csvIn(strBuffer);
	
	CString strLogon = csvIn.GetString(0);

	if (strLogon.GetLength() != 18)
	{
		return;
	}

	if (strLogon.Left(2) != "0x")
	{
		return;
	}

	if ( strLogon.Right(8) != "00000000")
	{
		return;
	}

	if ( ::TestNumericHex( strLogon.Mid(2, 8) ) == FALSE )
	{
		return;
        
	}

	int nServerNo = csvIn.GetInt(1);
	if ( ( nServerNo < Server::ServerNo.Min ) || ( nServerNo > Server::ServerNo.Max ) )
	{
		return;
	}

	m_nTotalServerLogon++;

	if (m_bChangeDatabase == FALSE)
	{
		return;
	}

	__int64 nLogonNo = HexToInt64(strLogon.Mid(2, 8));	

	CServerLogonCSVRecord LogonRecord;
	LogonRecord.SetLogonNo(nLogonNo);
	LogonRecord.SetServerNo(nServerNo);

	int nPos = 0;
	if (DataManagerNonDb.ServerLogon.FindLogonByNumber(nLogonNo, nPos) == TRUE)
	{
		DataManagerNonDb.ServerLogon.SetAt(nPos, LogonRecord);
	}
	else
	{
		DataManagerNonDb.ServerLogon.InsertAt(nPos, LogonRecord);
	}
}

/**********************************************************************/

void CSQLImporter::ImportServerRestricts( CSSFile& fileSQL, int nPassNo )
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add( "[FieldName]" );
	csvHeader.Add( "[FieldIndex]" );
	csvHeader.Add( "[iValue]" );
	csvHeader.Add( "[fValue]" );
	csvHeader.Add( "[sValue]" );
	
	CSQLIn SQLIn ( fileSQL, "SysValues", csvHeader.GetLine() );

	CString strBuffer;
	int nTypeCount = GetFileTypeCount();
	while ( SQLIn.ReadLine( strBuffer ) )
	{
		StatusProgress.SetPos( fileSQL, nPassNo, nTypeCount );
		ImportServerRestrict( strBuffer );
	}
}

/**********************************************************************/

void CSQLImporter::ImportServerRestrict( CString& strBuffer )
{
	CCSV csvIn( strBuffer );

	CString strType = csvIn.GetString(0);
	
	if ( strType.GetLength() != 16 )
		return;
	
	if ( strType.Left(11) != "ServerTills" )
		return;

	CString strNum = strType.Right(5);
	if ( ::TestNumeric( strNum ) == FALSE )
		return;

	int nServerNo = atoi( strNum );
	if ( ( nServerNo < Server::ServerNo.Min ) || ( nServerNo > Server::ServerNo.Max ) )
		return;

	m_nTotalServerRestrict++;
	
	if ( m_bChangeDatabase == FALSE )
		return;

	if ( !csvIn.IsEmpty(1) )
	{
		int nIdx;
		if ( DataManagerNonDb.Server.FindServerByNumber( nServerNo, nIdx ) == TRUE )
		{
			CServerCSVRecord ServerRecord;
			DataManagerNonDb.Server.GetAt( nIdx, ServerRecord );
			
			CServerRestrictionCSVRecord RestrictRecord;
			DataManagerNonDb.ServerRestriction.GetRestrictions( ServerRecord, RestrictRecord );

			CString str = RestrictRecord.GetRestrictions();
			if ( str != "" ) str += ";";

			CString strExtra;
			strExtra.Format( "%d", csvIn.GetInt(1) );
			str += strExtra;

			RestrictRecord.SetRestrictions( str );
			DataManagerNonDb.ServerRestriction.SetRestrictions( RestrictRecord );
		}
	}
}

/**********************************************************************/

void CSQLImporter::ImportServerPresets( CSSFile& fileSQL, int nPassNo )
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add( "[TillNo]" );
	csvHeader.Add( "[Preset]" );
	csvHeader.Add( "[SrvNo]" );
	
	CSQLIn SQLIn ( fileSQL, "PresetSrvrs", csvHeader.GetLine() );

	int nCurrentTNo = 0;

	CString strBuffer;
	int nTypeCount = GetFileTypeCount();
	while ( SQLIn.ReadLine( strBuffer ) )
	{
		StatusProgress.SetPos( fileSQL, nPassNo, nTypeCount );
		ImportServerPreset( nCurrentTNo, strBuffer );
	}

	if ( ( nCurrentTNo >= 1 ) && ( TRUE == m_bChangeDatabase ) )
	{
		CDataManagerInfo info;
		DataManagerNonDb.WriteServerPresetTerminal( info );
		DataManagerNonDb.CloseServerPresetTerminal( info );
	}
}

/**********************************************************************/

void CSQLImporter::ImportServerPreset( int& nCurrentTNo, CString& strBuffer )
{
	CCSV csvIn( strBuffer );

	int nTillNo = csvIn.GetInt(0);
	int nPresetNo = csvIn.GetInt(1);
	int nServerNo = csvIn.GetInt(2);

	if ( ( nTillNo < 1 ) || ( nTillNo > 999 ) )
		return;

	if ( ( nPresetNo < 1 ) || ( nPresetNo > MAX_SVRPRESETNO ) )
		return;

	if ( ( nServerNo < 0 ) || ( nServerNo > Server::ServerNo.Max ) )
		return;

	m_nTotalServerPreset++;

	if ( m_bChangeDatabase == FALSE )
		return;

	if ( nTillNo != nCurrentTNo )
	{
		CDataManagerInfo info;
			
		if ( nCurrentTNo != 0 )
		{
			DataManagerNonDb.WriteServerPresetTerminal( info );
			DataManagerNonDb.CloseServerPresetTerminal( info );
		}

		int nTermIdx;
		if ( dbLocation.FindTerminalByNumber( m_nServerPresetLocIdx, nTillNo, nTermIdx ) == TRUE )
		{
			DataManagerNonDb.OpenServerPresetTerminal( m_nServerPresetLocIdx, nTillNo, DB_READWRITE, info );
			nCurrentTNo = nTillNo;
		}
		else
		{
			nCurrentTNo = 0;
		}
	}

	if ( nCurrentTNo != 0 )
	{
		CServerPresetMiniInfo infoPreset;
		infoPreset.m_nPresetNo = nPresetNo;
		infoPreset.m_nServerNo = nServerNo;
		DataManagerNonDb.ServerPresetTerminal.UpdatePreset( infoPreset );
	}
}

/**********************************************************************/

void CSQLImporter::ImportBarcodes( CSSFile& fileSQL, int nPassNo )
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add( "[PLU]" );
	csvHeader.Add( "[Barcode]" );

	CSQLIn SQLIn ( fileSQL, "Barcodes", csvHeader.GetLine() );

	CString strBuffer;
	int nTypeCount = GetFileTypeCount();
	while ( SQLIn.ReadLine( strBuffer ) )
	{
		StatusProgress.SetPos( fileSQL, nPassNo, nTypeCount );
		ImportBarcode( strBuffer );
	}
}

/**********************************************************************/

void CSQLImporter::ImportBarcode( CString& strBuffer )
{		
	CCSV csvIn( strBuffer );

	__int64 nBarcodeNo = csvIn.GetInt64(1);
	if ( ( nBarcodeNo < 1 ) || ( nBarcodeNo > Super.MaxBarcodeInt() ) )
		return;
	
	CSQLPluNoInfo infoPlu;
	infoPlu.m_nEposPluNo = csvIn.GetInt64(0);
	ProcessPluNo( infoPlu );
	if ( FALSE == infoPlu.m_bValid )
		return;

	m_nTotalBarcode++;

	if ( m_bChangeDatabase == FALSE )
		return;

	CBarcodeCSVRecord Barcode;

	int nBarcodeIdx;
	if ( DataManager.Barcode.FindBarcodeByNumber( nBarcodeNo, nBarcodeIdx ) == TRUE )
		DataManager.Barcode.GetAt( nBarcodeIdx, Barcode );
	else
	{
		Barcode.SetBarcodeNo( nBarcodeNo );
		Barcode.SetPrintShelfFlag( TRUE );
		Barcode.SetPrintProductFlag( TRUE );
		DataManager.Barcode.InsertAt( nBarcodeIdx, Barcode );
	}

	Barcode.SetPluNo( infoPlu.m_nBasePluNo );
	if ( EcrmanOptions.GetPluBarcodeModifierFlag() == TRUE ) Barcode.SetModifier( infoPlu.m_nModifier );
	DataManager.Barcode.SetAt( nBarcodeIdx, Barcode );
	DataManager.Barcode.InvalidatePluBarcodeList();
}

/**********************************************************************/

void CSQLImporter::ImportTaxRates(CSSFile& fileSQL, int nPassNo)
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add("[Vatv1]");
	csvHeader.Add("[Vatv2]");
	csvHeader.Add("[Vatv3]");
	csvHeader.Add("[Vatv4]");
	csvHeader.Add("[Vatv5]");
	csvHeader.Add("[Vatv6]");
	csvHeader.Add("[Vatv7]");
	csvHeader.Add("[Vatv8]");
	csvHeader.Add("[Vatv9]");
	csvHeader.Add("[Vatv10]");
	csvHeader.Add("[Vatv11]");
	csvHeader.Add("[Vatv12]");

	CSQLIn SQLIn(fileSQL, "CfgSys", csvHeader.GetLine());

	CString strBuffer;
	int nTypeCount = GetFileTypeCount();
	while (SQLIn.ReadLine(strBuffer))
	{
		StatusProgress.SetPos(fileSQL, nPassNo, nTypeCount);
		ImportTaxRates(strBuffer);
	}
}

/**********************************************************************/

void CSQLImporter::ImportTaxRates( CString& strBuffer )
{		
	CCSV csvIn( strBuffer );

	for ( int nTaxRate = 1; nTaxRate <= MAX_TAX_BANDS; nTaxRate++ )
	{
		CTaxRateInfo TaxRateInfo;
		DataManager.TaxRates.GetTaxRateInfo( nTaxRate, TaxRateInfo );
		TaxRateInfo.m_dTaxRate = csvIn.GetDouble( nTaxRate - 1 );
		DataManager.TaxRates.SetTaxRateInfo( nTaxRate, TaxRateInfo );
	}

	m_nTotalTaxRate = 12;
}

/**********************************************************************/

void CSQLImporter::ImportCustomers( CSSFile& fileSQL, bool bAuto, const char* szTableName )
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add( "[ID]" );
	csvHeader.Add( "[FirstName]" );
	csvHeader.Add( "[LastName]" );
	csvHeader.Add( "[Address1]" );
	csvHeader.Add( "[Address2]" );
	csvHeader.Add( "[Address3]" );
	csvHeader.Add( "[Postcode]" );
	csvHeader.Add( "[Email]" );
	csvHeader.Add( "[Temporary]" );
	csvHeader.Add( "[Active]" );
	csvHeader.Add( "[Tel1]" );
	csvHeader.Add( "[Tel2]" );
	csvHeader.Add( "[Tel3]" );
	csvHeader.Add( "[CreationTime]" );

	CSQLIn SQLIn ( fileSQL, szTableName, csvHeader.GetLine() );

	CString strBuffer;
	int nTypeCount = GetFileTypeCount();
	while ( SQLIn.ReadLine( strBuffer ) )
	{
		StatusProgress.SetPos( fileSQL );
		ImportCustomer( strBuffer, bAuto );
	}
}

/**********************************************************************/

void CSQLImporter::ImportCustomer( CString& strBuffer, bool bAuto )
{		
	CCSV csvIn( strBuffer );

	if ( !csvIn.IsEmpty(0) )
	{
		int nID = csvIn.GetInt(0);

		if ( ( nID >= Customer::CustomerID.Min ) && ( nID <= Customer::CustomerID.Max ) )
		{
			bool bTemporary = FALSE;
			bool bActive = TRUE;

			if ( !csvIn.IsEmpty(8) ) bTemporary = ( csvIn.GetInt(8) == 1 );
			if ( !csvIn.IsEmpty(9) ) bActive = ( csvIn.GetInt(9) == 1 );

			if ( TRUE == bActive )
			{
				CCustomerCSVArray* pTargetArrayInsert;
				CCustomerCSVArray* pTargetArrayRemove;
				
				if ( TRUE == bTemporary )
				{
					pTargetArrayInsert = &DataManagerNonDb.CustomerOneOff;
					pTargetArrayRemove = &DataManagerNonDb.CustomerAccount;
				}
				else
				{
					pTargetArrayInsert = &DataManagerNonDb.CustomerAccount;
					pTargetArrayRemove = &DataManagerNonDb.CustomerOneOff;
				}

				CCustomerCSVRecord CustomerRecord;
				CustomerRecord.SetCustomerID( nID );

				bool bInsertExists = FALSE;
				bool bRemoveExists = FALSE;

				int nInsertIdx;
				if ( pTargetArrayInsert -> FindCustomerByID( nID, nInsertIdx ) == TRUE )
				{
					pTargetArrayInsert -> GetAt( nInsertIdx, CustomerRecord );
					bInsertExists = TRUE; 
				}

				int nRemoveIdx;
				if ( pTargetArrayRemove -> FindCustomerByID( nID, nRemoveIdx ) == TRUE )
				{
					if ( FALSE == bInsertExists )
						pTargetArrayRemove -> GetAt( nRemoveIdx, CustomerRecord );
					
					bRemoveExists = TRUE; 
				}

				if ( ( TRUE == bInsertExists ) ||( TRUE == bRemoveExists ) )
				{
					if ( TRUE == bAuto )
						CustomerRecord.SetDeletionStatus( CUSTOMER_DELETIONSTATUS_PROTECTED );

					m_nCustomersModified++;
				}
				else
				{
					if ( TRUE == bAuto )
						CustomerRecord.SetDeletionStatus( CUSTOMER_DELETIONSTATUS_PROTECTED );
					else
						CustomerRecord.SetDeletionStatus( CUSTOMER_DELETIONSTATUS_CANDELETE );

					m_nCustomersAdded++;
				}

				//NEED POS CREATION DATE TO HELP CALCLUATE DELTION DATE
				if ( !csvIn.IsEmpty(13) ) CustomerRecord.SetPosCreationTime( csvIn.GetDouble(13) );
				
				if ( FALSE == bInsertExists )
				{
					CustomerRecord.InitCreateAndDeleteDates(bTemporary);
					pTargetArrayInsert -> InsertAt( nInsertIdx, CustomerRecord );
				}

				if ( TRUE == bRemoveExists )
					pTargetArrayRemove -> RemoveAt( nRemoveIdx );

				if ( ( !csvIn.IsEmpty(1) ) || ( !csvIn.IsEmpty(2) )	)
				{
					CustomerRecord.SetCustomerName1( csvIn.GetString(1) );
					CustomerRecord.SetCustomerName2( csvIn.GetString(2) );
				}

				if ( !csvIn.IsEmpty(3) ) CustomerRecord.SetAddress1( csvIn.GetString(3) );
				if ( !csvIn.IsEmpty(4) ) CustomerRecord.SetAddress2( csvIn.GetString(4) );
				if ( !csvIn.IsEmpty(5) ) CustomerRecord.SetAddress3( csvIn.GetString(5) );
				if ( !csvIn.IsEmpty(6) ) CustomerRecord.SetPostcode( csvIn.GetString(6) );
				if ( !csvIn.IsEmpty(7) ) CustomerRecord.SetEmail( csvIn.GetString(7) );
				if ( !csvIn.IsEmpty(10) ) CustomerRecord.SetPhone1( csvIn.GetString(10) );
				if ( !csvIn.IsEmpty(11) ) CustomerRecord.SetPhone2( csvIn.GetString(11) );
				if ( !csvIn.IsEmpty(12) ) CustomerRecord.SetPhone3( csvIn.GetString(12) );
				
				CustomerRecord.SetCustomerID( nID );
				pTargetArrayInsert -> SetAt( nInsertIdx, CustomerRecord );
			}
		}
	}
}

/**********************************************************************/

void CSQLImporter::ImportFunctions( CSSFile& fileSQL, int nPassNo )
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add( "[FuncNo]" );
	csvHeader.Add( "[Auth]" );
	csvHeader.Add( "[Password]" );
	csvHeader.Add( "[Audit]" );
	csvHeader.Add( "[Log]" );

	CSQLIn SQLIn ( fileSQL, "Functions", csvHeader.GetLine() );

	CString strBuffer;
	int nTypeCount = GetFileTypeCount();
	while ( SQLIn.ReadLine( strBuffer ) )
	{
		StatusProgress.SetPos( fileSQL, nPassNo, nTypeCount );
		ImportFunction( strBuffer );
	}
}

/**********************************************************************/

void CSQLImporter::ImportFunction( CString& strBuffer )
{
	CCSV csvIn( strBuffer );

	m_nTotalFunction++;
	
	if (m_bChangeDatabase == FALSE)
	{
		return;
	}

	int nFuncNo = csvIn.GetInt(0);
	
	CFunctionAuthMiniInfo infoMini;
	infoMini.m_nFuncNo = csvIn.GetInt(0);
	if ( !csvIn.IsEmpty(1) ) infoMini.m_nAuthLevel = csvIn.GetInt(1);
	if ( !csvIn.IsEmpty(2) ) infoMini.m_bPassword = csvIn.GetBool(2);
	if ( !csvIn.IsEmpty(3) ) infoMini.m_bAudit = csvIn.GetBool(3);
	if ( !csvIn.IsEmpty(4) ) infoMini.m_bLog = csvIn.GetBool(4);

	if (TRUE == m_bWantFunctionSystem)
	{
		DataManagerNonDb.FunctionSystem.UpdateFunction(infoMini);
	}

	if (WIZARD_IMPORT_CLEAR == m_nWantFunctionDatabaseType)
	{
		DataManagerNonDb.FunctionDatabase.UpdateFunction(infoMini);
	}
	else if (WIZARD_IMPORT_MERGE == m_nWantFunctionDatabaseType)
	{
		CFunctionAuthMiniInfo infoCheck;
		infoCheck.m_nFuncNo = nFuncNo;

		DataManagerNonDb.FunctionDatabase.GetFunction(infoCheck);
		if (infoCheck.m_nAuthLevel >= 0)
		{
			DataManagerNonDb.FunctionDatabase.UpdateFunction(infoMini);
		}
	}

	if (WIZARD_IMPORT_CLEAR == m_nWantFunctionLocationType)
	{
		DataManagerNonDb.FunctionLocation.UpdateFunction(infoMini);
	}
	else if (WIZARD_IMPORT_MERGE == m_nWantFunctionLocationType)
	{
		CFunctionAuthMiniInfo infoCheck;
		infoCheck.m_nFuncNo = nFuncNo;

		DataManagerNonDb.FunctionLocation.GetFunction(infoCheck);
		if (infoCheck.m_nAuthLevel >= 0)
		{
			DataManagerNonDb.FunctionLocation.UpdateFunction(infoMini);
		}
	}
}

/**********************************************************************/

void CSQLImporter::ImportCCMacros( CSSFile& fileSQL, int nPassNo )
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add( "[MacNo]" );
	csvHeader.Add( "[MacText]" );
	csvHeader.Add( "[Description]" );
	
	CSQLIn SQLIn ( fileSQL, "Macros", csvHeader.GetLine() );

	CString strBuffer;
	int nTypeCount = GetFileTypeCount();
	while ( SQLIn.ReadLine( strBuffer ) )
	{
		StatusProgress.SetPos( fileSQL, nPassNo, nTypeCount );
		ImportCCMacro( strBuffer );
	}
}

/**********************************************************************/

void CSQLImporter::ImportCCMacro( CString& strBuffer )
{
	CCSV csvIn( strBuffer );

	m_nTotalCCMacro++;
	
	if ( m_bChangeDatabase == FALSE )
		return;

	if ( TRUE == m_bWantCCMacroSystem )
	{
		ProcessMacro( DataManagerNonDb.CCMacroSystem, csvIn );
	}

	if ( TRUE == m_bWantCCMacroDatabase )
	{
		ProcessMacro( DataManagerNonDb.CCMacroDatabase, csvIn );
	}

	if ( TRUE == m_bWantCCMacroLocation )
	{
		ProcessMacro( DataManagerNonDb.CCMacroLocation, csvIn );
	}
}

/**********************************************************************/

void CSQLImporter::ProcessMacro( CCascadingMacroCSVArray& MacroArray, CCSV& csvIn )
{
	int nMacroNo = csvIn.GetInt(0);

	CCascadingMacroCSVRecord Macro;
	Macro.SetMacroNo( nMacroNo );

	int nMacroIdx;
	if ( MacroArray.FindMacroByNumber( nMacroNo, nMacroIdx ) == TRUE )
	{
		MacroArray.GetAt( nMacroIdx, Macro );
	}
	else
	{
		MacroArray.InsertAt( nMacroIdx, Macro );
	}

	if ( !csvIn.IsEmpty(1) ) Macro.SetLocalMacroData( csvIn.GetString(1) );
	if ( !csvIn.IsEmpty(2) ) Macro.SetLocalMacroName( csvIn.GetString(2) );
	Macro.SetLocalMacroFlag( TRUE );
	Macro.SetParentMacroTypeExternal( NODE_PLACEHOLDER );
	Macro.SetParentMacroName( "" );
	Macro.SetParentMacroData( "" );
	
	MacroArray.SetAt( nMacroIdx, Macro );
}

/**********************************************************************/

void CSQLImporter::ImportCCReasons(CSSFile& fileSQL, int nPassNo, int nDbIdx, int nLocIdx)
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add("[FieldName]");
	csvHeader.Add("[FieldIndex]");
	csvHeader.Add("[iValue]");
	csvHeader.Add("[fValue]");
	csvHeader.Add("[sValue]");

	CSQLIn SQLIn(fileSQL, "SysValues", csvHeader.GetLine());

	CString strBuffer;
	int nTypeCount = GetFileTypeCount();
	while (SQLIn.ReadLine(strBuffer))
	{
		StatusProgress.SetPos(fileSQL, nPassNo, nTypeCount);
		ImportCCReason(strBuffer, nDbIdx, nLocIdx);
	}
}

/**********************************************************************/

void CSQLImporter::ImportCCReason(CString& strBuffer, int nDbIdx, int nLocIdx )
{
	CCSV csvIn(strBuffer);

	CString strType = csvIn.GetString(0);
	strType.MakeUpper();
	if (strType.Left(7) != "REASON.")
	{
		return;
	}

	int nReasonNo = csvIn.GetInt(1);
	if ((nReasonNo < CascadingReason::ReasonNo.Min) || (nReasonNo > CascadingReason::ReasonNo.Max))
	{
		return;
	}

	int nReasonType = CCascadingReasonCSVArray::GetReasonTypeBySQLLabel(strType.Right(strType.GetLength() - 7));

	if ((nReasonType >= 0) && (nReasonType < REASONTEXT_TYPES))
	{
		m_nTotalCCReason++;

		if (m_bChangeDatabase == FALSE)
		{
			return;
		}

		if (TRUE == m_bWantCCReasonSystem)
		{
			if (DataManagerNonDb.CCReasonSystem.GetReasonType() != nReasonType)
			{
				CDataManagerInfo info;
				DataManagerNonDb.WriteCCReasonSystem(info);
				DataManagerNonDb.CloseCCReasonSystem(info);
				DataManagerNonDb.OpenCCReasonSystem(DB_READWRITE, nReasonType, info);
			}

			ProcessReason(DataManagerNonDb.CCReasonSystem, csvIn, m_bClearReasonSystem, m_bHaveClearedReasonSystem[nReasonType]);
			m_bHaveClearedReasonSystem[nReasonType] = TRUE;
		}

		if (TRUE == m_bWantCCReasonDatabase)
		{
			if (DataManagerNonDb.CCReasonDatabase.GetReasonType() != nReasonType)
			{
				CDataManagerInfo info;
				DataManagerNonDb.WriteCCReasonDatabase(info);
				DataManagerNonDb.CloseCCReasonDatabase(info);
				DataManagerNonDb.OpenCCReasonDatabase( nDbIdx, DB_READWRITE, nReasonType, info);
			}

			ProcessReason(DataManagerNonDb.CCReasonDatabase, csvIn, m_bClearReasonDatabase, m_bHaveClearedReasonDatabase[nReasonType]);
			m_bHaveClearedReasonDatabase[nReasonType] = TRUE;
		}

		if (TRUE == m_bWantCCReasonLocation)
		{
			if (DataManagerNonDb.CCReasonLocation.GetReasonType() != nReasonType)
			{
				CDataManagerInfo info;
				DataManagerNonDb.WriteCCReasonLocation(info);
				DataManagerNonDb.CloseCCReasonLocation(info);
				DataManagerNonDb.OpenCCReasonLocation(nLocIdx, DB_READWRITE, nReasonType, info);
			}

			ProcessReason(DataManagerNonDb.CCReasonLocation, csvIn, m_bClearReasonLocation, m_bHaveClearedReasonLocation[nReasonType]);
			m_bHaveClearedReasonLocation[nReasonType] = TRUE;
		}
	}
}

/**********************************************************************/

void CSQLImporter::ProcessReason(CCascadingReasonCSVArray& ReasonArray, CCSV& csvIn, bool bCanClear, bool bAlreadyCleared)
{
	if ( ( TRUE == bCanClear ) && (FALSE == bAlreadyCleared) )
	{
		ReasonArray.RemoveAt(0, ReasonArray.GetSize());
	}

	int nReasonNo = csvIn.GetInt(1);

	CCascadingReasonCSVRecord Reason;
	Reason.SetReasonNo(nReasonNo);

	int nReasonIdx;
	if (ReasonArray.FindReasonByNumber(nReasonNo, nReasonIdx) == TRUE)
	{
		ReasonArray.GetAt(nReasonIdx, Reason);
	}
	else
	{
		ReasonArray.InsertAt(nReasonIdx, Reason);
	}

	if (!csvIn.IsEmpty(2)) Reason.SetLocalManualEntryFlag(csvIn.GetInt(2) == -1);
	if (!csvIn.IsEmpty(4)) Reason.SetLocalReasonText(csvIn.GetString(4));
	Reason.SetLocalReasonFlag(TRUE);
	Reason.SetParentReasonConLevelExternal(NODE_PLACEHOLDER);
	Reason.SetParentManualEntryFlag(FALSE);
	Reason.SetParentReasonText("");

	ReasonArray.SetAt(nReasonIdx, Reason);
}

/**********************************************************************/

bool CSQLImporter::GotData()
{
	if (m_bGotFile == FALSE)
	{
		return FALSE;
	}
	else
	{
		return(
			(m_nTotalBarcode != 0) ||
			(m_nTotalDept != 0) ||
			(m_nTotalCategory != 0) ||
			(m_nTotalGroup != 0) ||
			(m_nTotalAllergen != 0) ||
			(m_nTotalMixMatch != 0) ||
			(m_nTotalWalletStampRule != 0) ||
			(m_nTotalOfferGroup != 0) ||
			(m_nTotalOfferEntry != 0) ||
			(m_nTotalTimePlan != 0) ||
			(m_nTotalTimePlanEntry != 0) ||
			(m_nTotalPromotion != 0) ||
			(m_nTotalModifier != 0) ||
			(m_nTotalPayMeth != 0) ||
			(m_nTotalServer != 0) ||
			(m_nTotalServerRestrict != 0) ||
			(m_nTotalServerPreset != 0) ||
			(m_nTotalBasePlu != 0) ||
			(m_nTotalModifierPlu != 0) ||
			(m_nTotalTaxRate != 0) ||
			(m_nTotalLoyaltyScheme != 0) ||
			(m_nTotalTableArea != 0) ||
			(m_nTotalTableName != 0) ||
			(m_nTotalFunction != 0) ||
			(m_nTotalCCMacro != 0) ||
			(m_nTotalCCReason != 0)
			);
	}
}

/**********************************************************************/

void CSQLImporter::AddToFileTypeCount(int& nCount, bool bCheck, int nAdd)
{
	if (TRUE == bCheck)
	{
		nCount += nAdd;
	}
}

/**********************************************************************/

int CSQLImporter::GetFileTypeCount()
{
	int nCount = 0;

	AddToFileTypeCount(nCount, m_bWantBarcode);
	AddToFileTypeCount(nCount, m_bWantDept);
	AddToFileTypeCount(nCount, m_bWantCategory);
	AddToFileTypeCount(nCount, m_bWantPaidInOutText);
	AddToFileTypeCount(nCount, m_bWantGroup);
	AddToFileTypeCount(nCount, m_bWantAllergen);
	AddToFileTypeCount(nCount, m_bWantTimePlan, 2);
	AddToFileTypeCount(nCount, m_bWantModifier);
	AddToFileTypeCount(nCount, m_bWantPayMeth);
	AddToFileTypeCount(nCount, m_bWantServer);
	AddToFileTypeCount(nCount, m_bWantServerRestrict);
	AddToFileTypeCount(nCount, m_bWantServerPreset);
	AddToFileTypeCount(nCount, m_bWantPlu);
	AddToFileTypeCount(nCount, m_bWantTaxRate);
	AddToFileTypeCount(nCount, m_bWantLoyaltyScheme);
	AddToFileTypeCount(nCount, m_bWantTableArea);
	AddToFileTypeCount(nCount, m_bWantTableName);
	AddToFileTypeCount(nCount, m_bWantMixMatch || m_bWantWalletStampRule, 4);
	AddToFileTypeCount(nCount, m_bWantMixMatch && m_bWantWalletStampRule );

	AddToFileTypeCount(nCount, (m_bWantFunctionSystem) || (m_nWantFunctionDatabaseType != WIZARD_IMPORT_NONE) || (m_nWantFunctionLocationType != WIZARD_IMPORT_NONE));

	return nCount;
}

/**********************************************************************/

bool CSQLImporter::LegacyWarning()
{
	if ((m_bGotLegacyPayments == FALSE) && (m_bGotLegacyServers == FALSE))
	{
		return TRUE;
	}

	CString strMsg = "";
	strMsg += AfxGetAppName();
	strMsg += " has detected import data from an obsolete version of Venta Pro.\n\n";

	strMsg += AfxGetAppName();
	strMsg += " cannot import this data because its format is incompatible with SPOS.\n\n";

	Prompter.Error(strMsg);
	return FALSE;
}

/**********************************************************************/

void CSQLImporter::GetOfferGroupList( CWordArray& array )
{
	array.RemoveAll();
	for ( int n = 0; n < m_arrayOfferGroups.GetSize(); n++ )
	{
		CSortedStringItem item;
		m_arrayOfferGroups.GetAt( n, item );
		array.Add( atoi( item.m_strItem ));
	}
}

/**********************************************************************/
