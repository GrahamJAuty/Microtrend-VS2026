/**********************************************************************/
#include "..\SPosStockManager5\StockMonitor.h"
#include "..\SPosStockManager5\StockOptionsIni.h"
/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "LocationCSVArray.h"
#include "PluDatabaseHeader.h"
#include "ReportHelpers.h"
#include "WalletStampRulePluMapper.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "CustomExportManager.h"
/**********************************************************************/

CCustomExportManagerBase::CCustomExportManagerBase()
{
	m_bDoPlu = FALSE;
	m_bApparentStock = FALSE;
	m_bAlternativeExports = FALSE;
}

/**********************************************************************/

CCustomExportManagerPlu::CCustomExportManagerPlu( bool bApparentStock, bool bAlternativeExports ) : CCustomExportManagerBase()
{
	m_bDoPlu = TRUE;
	m_bApparentStock = bApparentStock;
	m_bAlternativeExports = bAlternativeExports;

	if ( SysInfo.GotLoyaltyOrSmartPayLink() )
	{
		WritePluPointsFile();
	}
	
	int nDbIdx = DataManager.GetActiveDbIdx();

	CPriceSetExportInfo info;
	info.m_strLabel = "MASTER";
	info.m_strPath1.Format ( "%s\\exports", dbDatabase.GetFolderPathSysset( nDbIdx ) );
	info.m_strPath2 = info.m_strPath1;

	ProcessExports( info );
}

/**********************************************************************/

CCustomExportManagerPriceSet::CCustomExportManagerPriceSet( CPriceSetExportInfo& info ) : CCustomExportManagerBase()
{
	m_bDoPlu = TRUE;
	ProcessExports( info );
}

/**********************************************************************/

CCustomExportManagerSCard::CCustomExportManagerSCard() : CCustomExportManagerBase()
{
	if (SysInfo.GotLoyaltyLink() == TRUE)
	{
		if (EcrmanOptions.GetPluExportPurchaseHistoryFlag())
		{
			for (int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++)
			{
				CDataManagerInfo info;
				DataManager.OpenDatabaseReadOnly(nDbIdx, info, FALSE);
				WritePluPointsFile();
			}
		}
	}
}

/**********************************************************************/

void CCustomExportManagerBase::ProcessExports(CPriceSetExportInfo& info)
{
	bool bCreateFile = (info.m_strLabel == "MASTER");

	CString strCommandFilename = (m_bAlternativeExports) ? "exports2.dat" : "exports.dat";
	GetDataProgramPath(strCommandFilename);

	CSSFile fileCommand;
	if (fileCommand.Open(strCommandFilename, "rb") == FALSE)
	{
		return;
	}

	CString strBuffer;
	while (fileCommand.ReadString(strBuffer))
	{
		CCSV csvIn(strBuffer);

		CString strLineType = csvIn.GetString(0);
		::TrimSpaces(strLineType, FALSE);
		strLineType.MakeUpper();

		if (strLineType == "OUTPUT")
		{
			bCreateFile = FALSE;

			for (int n = 1; n < csvIn.GetSize(); n++)
			{
				CString str = csvIn.GetString(n);
				str.MakeUpper();

				if ((str == info.m_strLabel) || (str == "ALL"))
				{
					bCreateFile = TRUE;
					break;
				}
			}

			continue;
		}

		if (bCreateFile == FALSE)
		{
			continue;
		}

		if (csvIn.GetSize() < 3)
		{
			continue;
		}

		int nDbIdx = -1;

		if (strLineType == "PLU" ||
			strLineType == "PLU_APPARENT" ||
			strLineType == "PLU_STOCK" ||
			strLineType == "BARCODE" ||
			strLineType == "BARCODE_APPARENT" ||
			strLineType == "BARCODE_STOCK" ||
			strLineType == "SUPPLIER")
		{
			if (m_bDoPlu == TRUE)
			{
				nDbIdx = DataManager.GetActiveDbIdx();
			}
		}

		if ((nDbIdx < 0) || (nDbIdx >= dbDatabase.GetSize()))
		{
			continue;
		}

		CString strOwnFolder = "";	
		CString strFilename = csvIn.GetString(1);
		
		::TrimSpaces(strFilename, FALSE);
		if ((strFilename == "") || (strFilename == "*") || ( strFilename == "#") )
		{
			continue;
		}

		if ( strFilename.Left(1) == "#")
		{
			strOwnFolder = strFilename.Mid(1);

			strFilename = csvIn.GetString(2);
			::TrimSpaces(strFilename, FALSE);
			if ((strFilename == "") || (strFilename == "*"))
			{
				continue;
			}
		}

		if (strFilename.Left(1) == "*")
		{
			COleDateTime timeNow = COleDateTime::GetCurrentTime();

			CString strPrefix = "";
			strPrefix.Format("%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d_",
				timeNow.GetYear(),
				timeNow.GetMonth(),
				timeNow.GetDay(),
				timeNow.GetHour(),
				timeNow.GetMinute(),
				timeNow.GetSecond());

			strFilename = strFilename.Mid(1);
			strFilename = strPrefix + strFilename;
		}

		if (strOwnFolder != "")
		{
			csvIn.RemoveAt(2);
		}
		
		csvIn.RemoveAt(1);
		csvIn.RemoveAt(0);

		CString strPath = "";

		if (strOwnFolder == "")
		{
			if (::ExistSubdirectory(info.m_strPath1) == FALSE)
			{
				::MakeSubdirectory(info.m_strPath1);
			}

			if (::ExistSubdirectory(info.m_strPath2) == FALSE)
			{
				::MakeSubdirectory(info.m_strPath2);
			}

			strPath = info.m_strPath2;
			strPath += "\\";
			strPath += strFilename;
		}
		else
		{
			if ( ::ExistSubdirectory(strOwnFolder) == FALSE )
			{
				::MakeSubdirectory(strOwnFolder);
			}

			CString strExtra = "";
			strExtra.Format("\\DB%3.3d", dbDatabase.GetDbNo(nDbIdx));	

			if (::ExistSubdirectory(strOwnFolder + strExtra) == FALSE)
			{
				::MakeSubdirectory(strOwnFolder + strExtra);
			}

			strPath += strOwnFolder;
			strPath += strExtra;
			strPath += "\\";
			strPath += strFilename;
		}

		CString strExportHeader = csvIn.GetLine();

		if (strLineType == "PLU")
		{
			ProcessPluExport(strPath, strExportHeader);
		}
		else if ((strLineType == "PLU_APPARENT") || (strLineType == "PLU_STOCK"))
		{
			ProcessPluApparentExport(strPath, strExportHeader);
		}
		else if (strLineType == "BARCODE")
		{
			ProcessBarcodeExport(strPath, strExportHeader);
		}
		else if ((strLineType == "BARCODE_APPARENT") || (strLineType == "BARCODE_STOCK"))
		{
			ProcessBarcodeApparentExport(strPath, strExportHeader);
		}
		else if (strLineType == "SUPPLIER")
		{
			ProcessSupplierExport(strPath, strExportHeader);
		}
	}
}

/**********************************************************************/

void CCustomExportManagerBase::GetPluExportHeaders(const bool bAllPluFields, const CString strRequestedHeader, CString& strDatabaseHeader, CString& strExportHeader) const
{
	if (DealerFlags.GetLegacyPluExportFlag() == FALSE)
	{
		strDatabaseHeader = PluDatabaseHeader.GetDatabaseHeader();
		
		if (FALSE == bAllPluFields)
		{
			strExportHeader = PluDatabaseHeader.GetUpdatedHeader(strRequestedHeader);
		}
	}
	else
	{
		strDatabaseHeader = PluDatabaseHeader.GetLegacyHeader();

		if (FALSE == bAllPluFields)
		{
			strExportHeader = PluDatabaseHeader.GetLegacyHeader(strRequestedHeader);
		}
	}

	if (TRUE == bAllPluFields)
	{
		strExportHeader = strDatabaseHeader;
	}
}

/**********************************************************************/

bool CCustomExportManagerBase::CompareLabel(const CString strFileLabel, CString strDatabaseLabel) const
{
	strDatabaseLabel.MakeUpper();
	return (strFileLabel == strDatabaseLabel);
}

/**********************************************************************/

void CCustomExportManagerBase::RemoveHeaderBlanks(CString& strHeader, const bool bRemoveEcrmanStock, const bool bForBarcodeExport) const
{
	CCSV csv(strHeader);
	for (int n = csv.GetSize() - 1; n >= 0; n--)
	{
		bool bRemoveField = FALSE;	

		CString strField = csv.GetString(n);
		if (strField == "")
		{
			bRemoveField = TRUE;
		}
		else 
		{
			if (TRUE == bRemoveEcrmanStock)
			{
				strField.MakeUpper();
				if (CompareLabel(strField, Plu::ApparentLevel.Label) == TRUE)
				{
					bRemoveField = TRUE;
				}
				else if (CompareLabel(strField, Plu::MinimumLevel.Label) == TRUE)
				{
					bRemoveField = TRUE;
				}
				else if (CompareLabel(strField, Plu::SupplierRef.Label) == TRUE)
				{
					bRemoveField = TRUE;
				}
				else if (CompareLabel(strField, Plu::SupplierNo.Label) == TRUE)
				{
					bRemoveField = TRUE;
				}
				else if (CompareLabel(strField, Plu::NonStockItem.Label) == TRUE)
				{
					bRemoveField = TRUE;
				}
			}

			if (( FALSE == bRemoveField) && ( TRUE == bForBarcodeExport))
			{
				strField.MakeUpper();
				if (CompareLabel(strField, Plu::PrintShelf.Label) == TRUE)
				{
					bRemoveField = TRUE;
				}
				else if (CompareLabel(strField, Plu::PrintProduct.Label) == TRUE)
				{
					bRemoveField = TRUE;
				}
			}
		}

		if (TRUE == bRemoveField)
		{
			csv.RemoveAt(n);
		}
	}
	strHeader = csv.GetLine();
}

/**********************************************************************/

CString CCustomExportManagerBase::GetBarcodeDatabaseHeader() const
{
	CString strDatabaseHeader = CBarcodeCSVRecord::GetHeaderForCombinedExport();
	return strDatabaseHeader;
}

/**********************************************************************/

CString CCustomExportManagerBase::GetBarcodeDatabaseLine(CBarcodeCSVRecord& BarcodeRecord) const
{
	CCSV csv(',', '"', FALSE, FALSE, TRUE);
	BarcodeRecord.ConvertToCSVForCombinedExport(csv);
	CString strLine = csv.GetLine();
	return strLine;
}

/**********************************************************************/

void CCustomExportManagerBase::ProcessPluExport( CString& strPath, CString& strExportHeader )
{
	bool bAllPluFields = WantAllPluFields(strExportHeader);

	CString strPluDatabaseHeader = "";
	CString strExportHeaderToUse = "";
	GetPluExportHeaders(bAllPluFields, strExportHeader, strPluDatabaseHeader, strExportHeaderToUse);

	RemoveHeaderBlanks(strExportHeaderToUse, SysInfo.IsStockSystem(), FALSE);

	CSSExportFilterFile fileExportPlu ( strPluDatabaseHeader, strExportHeaderToUse );
	if (fileExportPlu.Open(strPath, "wb") == FALSE)
	{
		return;
	}

	//LOOP THROUGH SELECTED RANGE OF PLU RECORDS
	StatusProgress.Lock( TRUE, "Exporting PLU records" );
	int nCount = DataManager.Plu.GetSize();
	for ( int nPluIdx = 0; nPluIdx < nCount; nPluIdx++ )
	{
		StatusProgress.SetPos ( nPluIdx, nCount );

		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt ( nPluIdx, PluRecord );

#ifdef STOCKMAN_SYSTEM
		CRecipeCSVRecord RecipeRecord;
		FixPluPurchaseCost(nPluIdx, PluRecord, RecipeRecord);
#endif

		//EXPORT THE PLU RECORD
		CCSV csv( ',', '"', FALSE, FALSE, TRUE);
		PluRecord.CreateExportLine( csv );
		fileExportPlu.WriteData ( csv.GetLine() );
	}
	StatusProgress.Unlock();
	fileExportPlu.Close();
}

/**********************************************************************/

void CCustomExportManagerBase::ProcessBarcodeExport(CString& strPath, CString& strExportHeader)
{
	bool bAllPluFields = WantAllPluFields(strExportHeader);

	DataManager.Barcode.CreatePluBarcodeList();

	CString strPluDatabaseHeader = "";
	CString strExportHeaderToUse = "";
	GetPluExportHeaders(bAllPluFields, strExportHeader, strPluDatabaseHeader, strExportHeaderToUse);

	CString strBarcodeDatabaseHeader = GetBarcodeDatabaseHeader();
	CString strComma = ",";

	CString strDatabaseHeader = "";
	strDatabaseHeader += strBarcodeDatabaseHeader;
	strDatabaseHeader += strComma;
	strDatabaseHeader += strPluDatabaseHeader;

	RemoveHeaderBlanks(strExportHeaderToUse, SysInfo.IsStockSystem(), TRUE);
	strExportHeaderToUse = strBarcodeDatabaseHeader + strComma + strExportHeaderToUse;

	CSSExportFilterFile fileExportBarcode(strDatabaseHeader, strExportHeaderToUse);
	if (fileExportBarcode.Open(strPath, "wb") == FALSE)
	{
		return;
	}

	//LOOP THROUGH SELECTED RANGE OF PLU RECORDS
	StatusProgress.Lock(TRUE, "Exporting Barcode information");
	int nCount = DataManager.Barcode.GetSize();
	for (int nProgress = 0; nProgress < nCount; nProgress++)
	{
		StatusProgress.SetPos(nProgress, nCount);

		CBarcodeCSVRecord BarcodeRecord;
		DataManager.Barcode.GetAt(nProgress, BarcodeRecord);

		int nPluIdx;
		CPluCSVRecord PluRecord;
		__int64 nPluNo = BarcodeRecord.GetPluNoInt();
		if (DataManager.Plu.FindPluByNumber(nPluNo, nPluIdx) == TRUE)
		{
			DataManager.Plu.GetAt(nPluIdx, PluRecord);

#ifdef STOCKMAN_SYSTEM
			CRecipeCSVRecord RecipeRecord;
			FixPluPurchaseCost(nPluIdx, PluRecord, RecipeRecord);
#endif

			CCSV csvPlu(',', '"', FALSE, FALSE, TRUE);
			PluRecord.CreateExportLine(csvPlu);
			CString strPluLine = csvPlu.GetLine();

			CString strLine = "";
			strLine += GetBarcodeDatabaseLine(BarcodeRecord);
			strLine += ",";
			strLine += strPluLine;

			fileExportBarcode.WriteData(strLine);
		}
	}
	StatusProgress.Unlock();
	fileExportBarcode.Close();
}

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

void CCustomExportManagerBase::CheckOptionalStockFields(CString& strHeader, bool& bWantDbApparentLevel, bool& bWantStockQty, bool& bWantPluStockLink) const
{
	bWantDbApparentLevel = FALSE;
	bWantStockQty = FALSE;
	bWantPluStockLink = FALSE;

	CCSV csvHeaderTest(strHeader);

	for (int n = 0; n < csvHeaderTest.GetSize(); n++)
	{
		CString strField = csvHeaderTest.GetString(n);
		strField.MakeUpper();

		if (strField == "DBAPPARENTLEVEL")
		{
			bWantDbApparentLevel = TRUE;
		}
		else if (strField == "STOCKQTY")
		{
			bWantStockQty = TRUE;
		}
		else if (strField == "PLUSTOCKLINK")
		{
			bWantPluStockLink = TRUE;
		}

		if (bWantDbApparentLevel && bWantStockQty && bWantPluStockLink)
		{
			break;
		}
	}
}

/**********************************************************************/

void CCustomExportManagerBase::ReattachStockHeaders(const bool bWantDbApparentLevel, const bool bWantStockQty, const bool bWantPluStockLink, const CString& strStockDatabaseHeader, CString& strExportHeaderToUse) const
{
	//IF THE EXPORT HEADER STARTS WITH * THEN ANY FIELDS OTHER THAN THE
	//PLU FIELDS WILL HAVE BEEN REMOVED FROM THE HEADER TO USE. 
	//THIS FUNCTION REATTACHES THE STOCK FIELDS AND ANY OPTIONAL FIELDS
	//REQUESTED IN THE EXPORT HEADER.

	if (strExportHeaderToUse != "")
	{
		strExportHeaderToUse += ",";
	}

	strExportHeaderToUse += strStockDatabaseHeader;

	if (bWantDbApparentLevel)
	{
		strExportHeaderToUse += ",DbApparentLevel";
	}

	if (bWantStockQty)
	{
		strExportHeaderToUse += ",StockQty";
	}

	if (bWantPluStockLink)
	{
		strExportHeaderToUse += ",PluStockLink";
	}
}

/**********************************************************************/

void CCustomExportManagerBase::FixPluPurchaseCost(const int nPluIdx, CPluCSVRecord& PluRecord, CRecipeCSVRecord& RecipeRecord) const
{
	DataManager.Plu.GetRecipe(nPluIdx, PluRecord.GetPluNoInt(), RecipeRecord);

	//USE COST PRICE FROM STOCK RECORD FOR EXPORT UNLESS NON STOCK ITEM
	if (RecipeRecord.GetPluStockLinkFlag() == TRUE)
	{
		PluRecord.SetPurchaseCost(DataManager.Plu.GetPluPurchaseCost(PluRecord));
	}
}

/**********************************************************************/

void CCustomExportManagerBase::ProcessPluApparentExport(CString& strPath, CString& strExportHeader)
{
	bool bAllPluFields = WantAllPluFields(strExportHeader);

	CPtrArray CustomFieldPtrArray;
	DataManager.GetCustomFieldPtrArray(CustomFieldPtrArray);

	bool bWantDbApparentLevel = FALSE;
	bool bWantStockQty = FALSE;
	bool bWantPluStockLink = FALSE;
	CheckOptionalStockFields(strExportHeader, bWantDbApparentLevel, bWantStockQty, bWantPluStockLink);

	//CANNOT CREATE EXPORT FILE WITH APPARENT STOCK IF APPARENT STOCK NOT LOADED
	if ((FALSE == m_bApparentStock) && (TRUE == bWantDbApparentLevel))
	{
		return;
	}

	CString strPluDatabaseHeader = "";
	CString strExportHeaderToUse = "";
	GetPluExportHeaders(bAllPluFields, strExportHeader, strPluDatabaseHeader, strExportHeaderToUse);

	int nSuppCount = StockOptions.GetStockMaxSuppliersPerItem();
	CString strStockDatabaseHeader = CStockCSVRecord::GetHeader(nSuppCount);

	if (TRUE == bAllPluFields)
	{
		ReattachStockHeaders(bWantDbApparentLevel, bWantStockQty, bWantPluStockLink, strStockDatabaseHeader, strExportHeaderToUse);
	}

	CString strDatabaseHeader = "";
	strDatabaseHeader += "DbApparentLevel,StockQty,PluStockLink,";
	strDatabaseHeader += strStockDatabaseHeader;
	strDatabaseHeader += ",";
	strDatabaseHeader += strPluDatabaseHeader;

	RemoveHeaderBlanks(strExportHeaderToUse, TRUE, FALSE);

	CSSExportFilterFile fileExportPlu(strDatabaseHeader, strExportHeaderToUse);
	if (fileExportPlu.Open(strPath, "wb") == FALSE)
	{
		return;
	}

	//DUMMY STOCK RECORD TO FILL SPACE FOR UNLINKED PLU ITEMS
	CStockCSVRecord stockDummy;
	CCSV csvDummy(',', '"', FALSE, FALSE, TRUE);
	stockDummy.ConvertToCSVSimpleExport(csvDummy, DataManager.Supplier, CustomFieldPtrArray, nSuppCount);
	CString strStockDummy = csvDummy.GetLine();

	//LOOP THROUGH SELECTED RANGE OF PLU RECORDS
	StatusProgress.Lock(TRUE, "Exporting PLU records");
	int nCount = DataManager.Plu.GetSize();
	for (int nPluIdx = 0; nPluIdx < nCount; nPluIdx++)
	{
		StatusProgress.SetPos(nPluIdx, nCount);

		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt(nPluIdx, PluRecord);

		CRecipeCSVRecord RecipeRecord;
		FixPluPurchaseCost(nPluIdx, PluRecord, RecipeRecord);

		//EXPORT THE PLU RECORD
		CCSV csvPlu(',', '"', FALSE, FALSE, TRUE);
		PluRecord.CreateExportLine(csvPlu);
		CString strPluLine = csvPlu.GetLine();

		CString strStockLine = strStockDummy;
		double dApparent = 0.0;
		double dStockQty = 0.0;

		if (RecipeRecord.GetSize() == 1)
		{
			CString strStockCode = RecipeRecord.GetStockCode(0);

			int nStockIdx;
			if (DataManager.Stock.FindStockItemByCode(strStockCode, nStockIdx) == TRUE)
			{
				CStockCSVRecord StockRecord;
				DataManager.Stock.GetAt(nStockIdx, StockRecord);

				CCSV csvStock(',', '"', FALSE, FALSE, TRUE);
				StockRecord.ConvertToCSVSimpleExport(csvStock, DataManager.Supplier, CustomFieldPtrArray, nSuppCount);
				strStockLine = csvStock.GetLine();

				CStockLevelsCSVRecord StockLevels;
				DataManager.Stock.GetDatabaseTotals(nStockIdx, StockRecord, StockLevels);
				dApparent = StockLevels.GetApparentQty();

				dStockQty = RecipeRecord.GetStockQty(0);
			}
		}

		CCSV csvExtra(',', '"', FALSE, FALSE, TRUE);
		csvExtra.Add(dApparent, 3);
		csvExtra.Add(dStockQty, 5);
		csvExtra.Add(RecipeRecord.GetPluStockLinkFlag());
		CString strExtra = csvExtra.GetLine();

		CString strLine;
		strLine.Format("%s,%s,%s",
			(const char*)strExtra,
			(const char*)strStockLine,
			(const char*)strPluLine);

		fileExportPlu.WriteData(strLine);
	}

	StatusProgress.Unlock();
	fileExportPlu.Close();
}

/**********************************************************************/

void CCustomExportManagerBase::ProcessBarcodeApparentExport( CString& strPath, CString& strExportHeader )
{
	bool bAllPluFields = WantAllPluFields(strExportHeader);

	CPtrArray CustomFieldPtrArray;
	DataManager.GetCustomFieldPtrArray( CustomFieldPtrArray );

	bool bWantDbApparentLevel = FALSE;
	bool bWantStockQty = FALSE;
	bool bWantPluStockLink = FALSE;
	CheckOptionalStockFields(strExportHeader, bWantDbApparentLevel, bWantStockQty, bWantPluStockLink);

	//CANNOT CREATE EXPORT FILE WITH APPARENT STOCK IF APPARENT STOCK NOT LOADED
	if ((FALSE == m_bApparentStock) && (TRUE == bWantDbApparentLevel))
	{
		return;
	}

	DataManager.Barcode.CreatePluBarcodeList();

	int nSuppCount = StockOptions.GetStockMaxSuppliersPerItem();
	CString strStockDatabaseHeader = CStockCSVRecord::GetHeader(nSuppCount);
	CString strBarcodeDatabaseHeader = GetBarcodeDatabaseHeader();
	
	CString strPluDatabaseHeader = "";
	CString strExportHeaderToUse = "";
	GetPluExportHeaders(bAllPluFields, strExportHeader, strPluDatabaseHeader, strExportHeaderToUse);

	if (TRUE == bAllPluFields)
	{
		ReattachStockHeaders(bWantDbApparentLevel, bWantStockQty, bWantPluStockLink, strStockDatabaseHeader, strExportHeaderToUse);
	}

	CString strDatabaseHeader = "DbApparentLevel,StockQty,PluStockLink,";
	strDatabaseHeader += strBarcodeDatabaseHeader;
	strDatabaseHeader += ",";
	strDatabaseHeader += strStockDatabaseHeader;
	strDatabaseHeader += ",";
	strDatabaseHeader += strPluDatabaseHeader;
	
	CString strComma = ",";
	RemoveHeaderBlanks(strExportHeaderToUse, TRUE, TRUE);
	strExportHeaderToUse = strBarcodeDatabaseHeader + strComma + strExportHeaderToUse;

	CSSExportFilterFile fileExportBarcode ( strDatabaseHeader, strExportHeaderToUse );
	if (fileExportBarcode.Open(strPath, "wb") == FALSE)
	{
		return;
	}

	//DUMMY STOCK RECORD TO FILL SPACE FOR UNLINKED PLU ITEMS
	CStockCSVRecord stockDummy;
	CCSV csvDummy( ',', '"', FALSE, FALSE, TRUE);
	stockDummy.ConvertToCSVSimpleExport ( csvDummy, DataManager.Supplier, CustomFieldPtrArray, nSuppCount );
	CString strStockDummy = csvDummy.GetLine();		
	
	//LOOP THROUGH SELECTED RANGE OF PLU RECORDS
	StatusProgress.Lock( TRUE, "Exporting Barcode information" );
	int nCount = DataManager.Barcode.GetSize();
	for ( int nProgress = 0; nProgress < nCount; nProgress++ )
	{
		StatusProgress.SetPos ( nProgress, nCount );	
		
		CBarcodeCSVRecord BarcodeRecord;
		DataManager.Barcode.GetAt( nProgress, BarcodeRecord );

		int nPluIdx;
		CPluCSVRecord PluRecord;
		if ( DataManager.Plu.FindPluByNumber( BarcodeRecord.GetPluNoInt(), nPluIdx ) == TRUE )
		{
			DataManager.Plu.GetAt( nPluIdx, PluRecord );

			CRecipeCSVRecord RecipeRecord;
			FixPluPurchaseCost(nPluIdx, PluRecord, RecipeRecord);

			CString strBarcodeLine = GetBarcodeDatabaseLine(BarcodeRecord);

			CCSV csvPlu( ',', '"', FALSE, FALSE, TRUE);
			PluRecord.CreateExportLine ( csvPlu );
			CString strPluLine = csvPlu.GetLine();

			CString strStockLine = strStockDummy;
			double dApparent = 0.0;
			double dStockQty = 0.0;

			if ( RecipeRecord.GetSize() == 1 )
			{
				CString strStockCode = RecipeRecord.GetStockCode(0);
				
				int nStockIdx;
				if ( DataManager.Stock.FindStockItemByCode( strStockCode, nStockIdx ) == TRUE )
				{
					CStockCSVRecord StockRecord;
					DataManager.Stock.GetAt( nStockIdx, StockRecord );
				
					CCSV csvStock( ',', '"', FALSE, FALSE, TRUE);
					StockRecord.ConvertToCSVSimpleExport ( csvStock, DataManager.Supplier, CustomFieldPtrArray, nSuppCount );
					strStockLine = csvStock.GetLine();
				
					CStockLevelsCSVRecord StockLevels;
					DataManager.Stock.GetDatabaseTotals( nStockIdx, StockRecord, StockLevels );
					dApparent = StockLevels.GetApparentQty();

					dStockQty = RecipeRecord.GetStockQty(0);
				}
			}

			CCSV csvExtra( ',', '"', FALSE, FALSE, TRUE);
			csvExtra.Add( dApparent, 3 );
			csvExtra.Add( dStockQty, 5 );
			csvExtra.Add( RecipeRecord.GetPluStockLinkFlag() );
			CString strExtra = csvExtra.GetLine();
		
			CString strLine;
			strLine.Format ( "%s,%s,%s,%s",
				(const char*) strExtra,
				(const char*) strBarcodeLine,
				(const char*) strStockLine,
				(const char*) strPluLine );

			fileExportBarcode.WriteData ( strLine );
		}
	}
	StatusProgress.Unlock();
	fileExportBarcode.Close();
}

/**********************************************************************/
#else
/**********************************************************************/

void CCustomExportManagerBase::ProcessPluApparentExport( CString& strPath, CString& strExportHeader )
{	
	bool bAllPluFields = WantAllPluFields(strExportHeader);

	CString strPluDatabaseHeader = "";
	CString strExportHeaderToUse = "";
	GetPluExportHeaders(bAllPluFields, strExportHeader, strPluDatabaseHeader, strExportHeaderToUse);

	CString strDatabaseHeader = "DbApparentLevel,";
	strDatabaseHeader += strPluDatabaseHeader;

	if (TRUE == bAllPluFields)
	{
		strExportHeaderToUse += ",DbApparentLevel";
	}

	RemoveHeaderBlanks(strExportHeaderToUse, FALSE, FALSE);

	CSSExportFilterFile fileExportPlu ( strDatabaseHeader, strExportHeaderToUse );
	if (fileExportPlu.Open(strPath, "wb") == FALSE)
	{
		return;
	}

	//LOOP THROUGH SELECTED RANGE OF PLU RECORDS
	StatusProgress.Lock( TRUE, "Exporting PLU records" );
	int nCount = DataManager.Plu.GetSize();
	for ( int nPluIdx = 0; nPluIdx < nCount; nPluIdx++ )
	{
		StatusProgress.SetPos ( nPluIdx, nCount );
		
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt ( nPluIdx, PluRecord );

		//EXPORT THE PLU RECORD
		CCSV csv( ',', '"', FALSE, FALSE, TRUE);
		PluRecord.CreateExportLine ( csv );

		csv.InsertAt( 0, PluRecord.GetApparentLevel(), 3 );

		fileExportPlu.WriteData ( csv.GetLine() );
	}
	StatusProgress.Unlock();
	fileExportPlu.Close();
}

/**********************************************************************/

void CCustomExportManagerBase::ProcessBarcodeApparentExport( CString& strPath, CString& strExportHeader )
{
	bool bAllPluFields = (strExportHeader.Left(1) == "*");

	DataManager.Barcode.CreatePluBarcodeList();

	CString strPluDatabaseHeader = "";
	CString strExportHeaderToUse = "";
	GetPluExportHeaders(bAllPluFields, strExportHeader, strPluDatabaseHeader, strExportHeaderToUse);

	CString strBarcodeDatabaseHeader = GetBarcodeDatabaseHeader();

	if (TRUE == bAllPluFields)
	{
		strExportHeaderToUse += ",DbApparentLevel";
	}

	CString strDatabaseHeader = "";
	strDatabaseHeader += "DbApparentLevel,";
	strDatabaseHeader += strBarcodeDatabaseHeader;
	strDatabaseHeader += ",";
	strDatabaseHeader += strPluDatabaseHeader;

	CString strComma = ",";
	RemoveHeaderBlanks(strExportHeaderToUse, FALSE, TRUE);
	strExportHeaderToUse = strBarcodeDatabaseHeader + strComma + strExportHeaderToUse;

	CSSExportFilterFile fileExportBarcode ( strDatabaseHeader, strExportHeaderToUse );
	if (fileExportBarcode.Open(strPath, "wb") == FALSE)
	{
		return;
	}

	//LOOP THROUGH SELECTED RANGE OF PLU RECORDS
	StatusProgress.Lock( TRUE, "Exporting Barcode information" );
	int nCount = DataManager.Barcode.GetSize();
	for ( int nProgress = 0; nProgress < nCount; nProgress++ )
	{
		StatusProgress.SetPos ( nProgress, nCount );	
		
		CBarcodeCSVRecord BarcodeRecord;
		DataManager.Barcode.GetAt( nProgress, BarcodeRecord );

		int nPluIdx;
		CPluCSVRecord PluRecord;
		__int64 nPluNo = BarcodeRecord.GetPluNoInt();
		if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) == TRUE )
		{
			DataManager.Plu.GetAt( nPluIdx, PluRecord );

			CCSV csvPlu( ',', '"', FALSE, FALSE, TRUE);
			PluRecord.CreateExportLine ( csvPlu );
			CString strPluLine = csvPlu.GetLine();

			CCSV csvExtra( ',', '"', FALSE, FALSE, TRUE);
			csvExtra.Add( PluRecord.GetApparentLevel(), 3 );
		
			CString strLine = csvExtra.GetLine();
			strLine += ",";
			strLine += GetBarcodeDatabaseLine(BarcodeRecord);
			strLine += ",";
			strLine += strPluLine;

			fileExportBarcode.WriteData ( strLine );
		}
	}
	StatusProgress.Unlock();
	fileExportBarcode.Close();
}

/**********************************************************************/
#endif
/**********************************************************************/

void CCustomExportManagerBase::ProcessSupplierExport( CString& strPath, CString& strExportHeader )
{
#ifdef STOCKMAN_SYSTEM
	CString strDatabaseHeader = CSupplierCSVRecord::GetHeader();

	CSSExportFilterFile fileExportSupplier ( strDatabaseHeader, strExportHeader );
	if ( fileExportSupplier.Open ( strPath, "wb" ) == FALSE )
		return;
	
	//LOOP THROUGH SUPPLIERS
	StatusProgress.Lock( TRUE, "Exporting supplier data" );
	int nCount = DataManager.Supplier.GetSize();
	for ( int nProgress = 0; nProgress < nCount; nProgress++ )
	{
		StatusProgress.SetPos ( nProgress, nCount );	
		
		CSupplierCSVRecord SupplierRecord;
		DataManager.Supplier.GetAt ( nProgress, SupplierRecord );
		
		//EXPORT THE SUPPLIER
		CCSV csv( ',', '"', FALSE, FALSE, TRUE);
		SupplierRecord.ConvertToCSV ( csv );
		fileExportSupplier.WriteData ( csv.GetLine() );
	}
	StatusProgress.Unlock();
	fileExportSupplier.Close();
#endif
}

/**********************************************************************/

bool CCustomExportManagerBase::WantStockLevels()
{
	bool bResult = FALSE;

#ifdef STOCKMAN_SYSTEM

	CString strCommandFilename = "exports.dat";
	GetDataProgramPath( strCommandFilename );
	
	CSSFile fileCommand;
	if ( fileCommand.Open ( strCommandFilename, "rb" ) == FALSE )
		return FALSE;

	CString strBuffer;
	while ( fileCommand.ReadString ( strBuffer ) )
	{
		CCSV csvIn ( strBuffer );
		
		CString strLineType = csvIn.GetString(0);
		::TrimSpaces ( strLineType, FALSE );
		strLineType.MakeUpper();

		if (  strLineType == "PLU_APPARENT" || 
				strLineType == "PLU_STOCK" ||
				strLineType == "BARCODE_APPARENT" ||
				strLineType == "BARCODE_STOCK" )
		{

			CString strTest = csvIn.GetLine();
			strTest.MakeUpper();
	
			if ( strTest.Find( "DBAPPARENTLEVEL" ) != -1 )
			{
				bResult = TRUE;
				break;
			}
		}
	}

#endif

	return bResult;
}

/**********************************************************************/

void CCustomExportManagerBase::WritePluPointsFile()
{
	CString strPath = dbDatabase.GetFolderPathData( DataManager.GetActiveDbIdx() );
	strPath += "\\PLUPTS.CSV";

	bool bSmartPay = FALSE;
	bool bLoyalty = FALSE;
	CWalletStampRulePluMapper PluMapper;
	
	switch (SysInfo.GetLoyaltyOrSmartPayType())
	{
	case LOYALTY_TYPE_SMP_V4:
		bSmartPay = TRUE;
		PluMapper.BuildMap();
		break;

	case LOYALTY_TYPE_LOY_V4:
		bLoyalty = TRUE;
		PluMapper.BuildMap();
		break;
	}

	CSSFile filePoints;
	if (filePoints.Open(strPath, "wb") == TRUE)
	{
		filePoints.WriteLine("PluNo,Text,Points,DeptNo,TaxCode,ModType,StmpOffID,PCtrlID,StmpWgt");
		
		for ( int nPluIdx = 0; nPluIdx < DataManager.Plu.GetSize(); nPluIdx++ )
		{
			CPluCSVRecord PluRecord;
			DataManager.Plu.GetAt( nPluIdx, PluRecord );

			__int64 nPluNo = PluRecord.GetPluNoInt();

			if ( SysInfo.IsModifiableSSPluNo( nPluNo ) == TRUE )
			{
				CCSV csv;
				csv.Add( nPluNo * 10 );
				csv.Add( PluRecord.GetReportText() );
				csv.Add( PluRecord.GetPoints() );
				csv.Add( PluRecord.GetBaseDeptNo());
				csv.Add( PluRecord.GetBaseTaxBandString());
				csv.Add(1);

				if (TRUE == bLoyalty)
				{
					csv.Add(PluMapper.GetRuleIDByPlu(nPluNo * 10));
					csv.Add("");
				}
				else if (TRUE == bSmartPay)
				{
					csv.Add("");
					csv.Add(PluMapper.GetRuleIDByPlu(nPluNo * 10));
				}
				else
				{
					csv.Add("");
					csv.Add("");
				}

				if (TRUE == bSmartPay)
				{
					csv.Add(PluRecord.GetStampWeight());
				}
				else
				{
					csv.Add("1");
				}

				filePoints.WriteLine( csv.GetLine() );

				for (int nMod = 1; nMod <= 9; nMod++)
				{
					if (PluRecord.GetModifierEnable(nMod) == TRUE)
					{
						double dMultiplier = DataManager.Modifier.GetMultiplierForPoints(nMod);
						double dPoints = (double)PluRecord.GetPoints() * dMultiplier;

						CString strPoints;
						strPoints.Format("%.5f", dPoints);

						CString strIntPoints = strPoints.SpanExcluding(".");
						int nPoints = atoi(strIntPoints);

						CString strText = PluRecord.GetModifierEposText(nMod);

						if ("" == strText)
						{
							strText = PluRecord.GetReportText();
						}

						CCSV csv{};
						csv.Add((nPluNo * 10) + nMod);
						csv.Add(strText);
						csv.Add(nPoints);
						csv.Add(PluRecord.GetModifierDeptNoForReport(nMod));
						csv.Add(PluRecord.GetModifierTaxBandStringForReport(nMod));
						csv.Add(2);

						if (TRUE == bLoyalty)
						{
							csv.Add(PluMapper.GetRuleIDByPlu((nPluNo * 10) + nMod));
							csv.Add("");
						}
						else if (TRUE == bSmartPay)
						{
							csv.Add("");
							csv.Add(PluMapper.GetRuleIDByPlu((nPluNo * 10) + nMod));
						}
						else
						{
							csv.Add("");
							csv.Add("");
						}

						if (TRUE == bSmartPay)
						{
							double dStampWeight = PluRecord.GetStampWeight();
							dStampWeight *= dMultiplier;
							CString strStampWeight = "";
							strStampWeight.Format("%.0f", dStampWeight);
							csv.Add(strStampWeight);
						}
						else
						{
							csv.Add("1");
						}

						filePoints.WriteLine(csv.GetLine());
					}
				}
			}
			else
			{
				CCSV csv{};
				csv.Add(nPluNo);
				csv.Add(PluRecord.GetReportText());
				csv.Add(PluRecord.GetPoints());
				csv.Add(PluRecord.GetBaseDeptNo());
				csv.Add(PluRecord.GetBaseTaxBandString());
				csv.Add(3);

				if (TRUE == bLoyalty)
				{
					csv.Add(PluMapper.GetRuleIDByPlu(nPluNo)); 
					csv.Add("");
				}
				else if (TRUE == bSmartPay)
				{
					csv.Add("");
					csv.Add(PluMapper.GetRuleIDByPlu(nPluNo));
				}
				else
				{
					csv.Add("");
					csv.Add("");
				}

				if (TRUE == bSmartPay)
				{
					csv.Add(PluRecord.GetStampWeight());
				}
				else
				{
					csv.Add("1");
				}

				filePoints.WriteLine(csv.GetLine());
			}
		}
	}
}

/**********************************************************************/

void CCustomExportManagerBase::WriteWalletStampRuleFile()
{
	CString strPath = "";
	CString strHeader = "";

	if (EcrmanOptions.GetFeaturesGlobalWalletStampRuleFlag() == TRUE)
	{
		strPath = "SysFiles";
		GetDataProgramPath(strPath);
	}
	else
	{
		strPath = dbDatabase.GetFolderPathData(DataManager.GetActiveDbIdx());
	}

	switch (SysInfo.GetLoyaltyOrSmartPayType())
	{
	case LOYALTY_TYPE_SMP_V4:
		strPath += "\\PURCHASECTRL.CSV";
		strHeader = "PURCHASECTRLID,";
		break;

	case LOYALTY_TYPE_LOY_V4:
	default:
		strPath += "\\STAMPOFFER.CSV";
		strHeader = "STAMPOFFERID,EDITION,";
		break;
	}

	strHeader += "ENABLE,THRESHOLD,EXPIRY,DESCRIPTION";

	CSSFile fileOffers;
	if (fileOffers.Open(strPath, "wb") == TRUE)
	{
		fileOffers.WriteLine(strHeader);

		for (int nRuleIdx = 0; nRuleIdx < DataManager.WalletStampRule.GetSize(); nRuleIdx++)
		{
			CWalletStampRuleCSVRecord Rule;
			DataManager.WalletStampRule.GetAt(nRuleIdx, Rule);

			CCSV csv;
			csv.Add(Rule.GetRuleID());

			switch (SysInfo.GetLoyaltyOrSmartPayType())
			{
			case LOYALTY_TYPE_SMP_V4:
				break;

			case LOYALTY_TYPE_LOY_V4:
			default:
				csv.Add(Rule.GetEdition());
				break;
			}

			csv.Add(Rule.GetEnableFlag());
			csv.Add(Rule.GetThreshold());
			csv.Add(Rule.GetExpiry());
			csv.Add(Rule.GetDescription());
			fileOffers.WriteLine(csv.GetLine());
		}
	}
}

/**********************************************************************/

void CCustomExportManagerBase::WriteDepartmentFile()
{
	CString strPath = "";

	if (EcrmanOptions.GetGlobalDepartmentFlag() == TRUE)
	{
		strPath = "SysFiles";
		GetDataProgramPath(strPath);
	}
	else
	{
		strPath = dbDatabase.GetFolderPathData(DataManager.GetActiveDbIdx());
	}

	strPath += "\\LOYDEPT.CSV";

	CSSFile fileDept;
	if (fileDept.Open(strPath, "wb") == TRUE)
	{
		fileDept.WriteLine("DEPTNO,NAME,BONUSTRIP,BONUSTYPE,BONUSPTS,BONUSVAL,BONUSPCNT,GENSPENDBONUS");

		for (int nDeptIdx = 0; nDeptIdx < DataManager.Department.GetSize(); nDeptIdx++)
		{
			CDepartmentCSVRecord Dept;
			DataManager.Department.GetAt(nDeptIdx, Dept);

			CCSV csv;
			csv.Add(Dept.GetDeptNo());
			csv.Add(Dept.GetReportText());
			csv.Add(Dept.GetLoyaltyBonusTrip(),2);
			csv.Add(Dept.GetLoyaltyBonusType());
			csv.Add(Dept.GetLoyaltyBonusPoints());
			csv.Add(Dept.GetLoyaltyBonusValue(),2);
			csv.Add(Dept.GetLoyaltyBonusPercent());
			csv.Add(Dept.GetLoyaltyGeneralSpendBonus() ? "1" : "0");

			fileDept.WriteLine(csv.GetLine());
		}
	}
}

/**********************************************************************/

void CCustomExportManagerBase::WriteLoyaltySchemeFile()
{
	if (DataManager.GetActiveDbIdx() == 0)
	{
		CString strPath = dbDatabase.GetFolderPathData(0);
		strPath += "\\LOYSCHEME.CSV";

		CSSFile fileScheme;
		if (fileScheme.Open(strPath, "wb") == TRUE)
		{
			fileScheme.WriteLine("SCHEMENO,SCHEMENAME");

			for (int nSchemeIdx = 0; nSchemeIdx < DataManager.LoyaltyScheme.GetSize(); nSchemeIdx++)
			{
				CLoyaltySchemeCSVRecord Scheme;
				DataManager.LoyaltyScheme.GetAt(nSchemeIdx, Scheme);

				CCSV csv;
				csv.Add(Scheme.GetSchemeNo());
				csv.Add(Scheme.GetSchemeName());
				
				fileScheme.WriteLine(csv.GetLine());
			}
		}
	}
}

/**********************************************************************/

void CCustomExportManagerBase::WriteOperatorFile()
{
	if (EcrmanOptions.GetFeaturesServerSettingsType() != SERVER_SYS)
	{
		return;
	}

	CString strPath = dbDatabase.GetFolderPathData(0);

	strPath += "\\OPNAMES.CSV";

	CSSFile fileOP;
	if (fileOP.Open(strPath, "wb") == TRUE)
	{
		fileOP.WriteLine("OPNO,OPNAME");

		for (int nOpIdx = 0; nOpIdx < DataManagerNonDb.Server.GetSize(); nOpIdx++)
		{
			CServerCSVRecord Server;
			DataManagerNonDb.Server.GetAt(nOpIdx, Server);

			CCSV csv;
			csv.Add(Server.GetServerNo());
			csv.Add(Server.GetReportName());
			
			fileOP.WriteLine(csv.GetLine());
		}
	}
}

/**********************************************************************/

void CCustomExportManagerBase::WriteEposTerminalFile()
{
	CString strPath = dbDatabase.GetFolderPathData(0);

	strPath += "\\TERMNAMES.CSV";

	CSSFile fileEP;
	if (fileEP.Open(strPath, "wb") == TRUE)
	{
		fileEP.WriteLine("TERMNO,TERMNAME");

		for (int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++)
		{
			for (int nTermIdx = 0; nTermIdx < dbLocation.GetTerminalCount(nLocIdx); nTermIdx++)
			{
				CCSV csv;
				csv.Add(dbLocation.GetTNo(nLocIdx,nTermIdx));
				csv.Add(dbLocation.GetTerminalName(nLocIdx, nTermIdx));
				fileEP.WriteLine(csv.GetLine());
			}
		}
	}
}

/**********************************************************************/

void CCustomExportManagerBase::WriteDbInfoFile()
{
	CString strPath = "SysFiles";
	GetDataProgramPath(strPath);

	strPath += "\\LOYDBINFO.CSV";

	CSSFile fileDb;
	if (fileDb.Open(strPath, "wb") == TRUE)
	{
		fileDb.WriteLine("DBNO,DBNAME");

		for (int nIdx = 0; nIdx < dbDatabase.GetSize(); nIdx++)
		{
			int nDbNo = dbDatabase.GetDbNo(nIdx);
			CString strDbName = dbDatabase.GetName(nIdx);
			TrimSpaces(strDbName, FALSE);

			if (strDbName == "")
			{
				strDbName.Format("Database %d", nDbNo);
			}

			CCSV csv;
			csv.Add(nDbNo);
			csv.Add(strDbName);

			fileDb.WriteLine(csv.GetLine());
		}
	}
}

/**********************************************************************/


