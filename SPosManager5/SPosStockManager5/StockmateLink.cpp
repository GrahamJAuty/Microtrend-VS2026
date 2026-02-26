/**********************************************************************/
#include "ExplodePlu.h"
#include "FileRemover.h"
#include "StatusProgress.h"
#include "StockAuditBaseReport.h"
#include "StockMonitor.h"
/**********************************************************************/
#include "StockmateLink.h" 
/**********************************************************************/

CStockmateLink::CStockmateLink(int nSpIdx, CStocktakeCSVRecord& Stocktake) : m_Stocktake(Stocktake)
{
	m_nSpIdx = nSpIdx;
}

/**********************************************************************/

void CStockmateLink::CreateFiles(CWnd* pParentWnd)
{
	{
		int nFileNo = 0;
		CFileRemover FileRemover;
		FileRemover.RemoveFolderContents(GetStockmateDataFolder(), nFileNo);
	}

	{
		CString strError = "";
		if (CreateConfigFile(strError) == FALSE)
		{
			Prompter.Error(strError);
			return;
		}
	}
	{
		CString strError = "";
		if (CreateProductsFile(strError) == FALSE)
		{
			Prompter.Error(strError);
			return;
		}
	}
	{
		CString strError = "";
		if (CreateSectionsFile(strError) == FALSE)
		{
			Prompter.Error(strError);
			return;
		}
	}
	{
		CString strError = "";
		if (CreateSubsectionsFile(strError) == FALSE)
		{
			Prompter.Error(strError);
			return;
		}
	}

	CString strExe = "C:\\SPOS_Link\\SPOS_Link.exe";
	SSspawnl(pParentWnd->GetSafeOwner(pParentWnd), P_WAIT, strExe, strExe, "L", "1", dbStockpoint.GetName(m_nSpIdx), "c:\\SPOS DATA", "1", NULL);
}

/**********************************************************************/

bool CStockmateLink::CreateConfigFile(CString& strError)
{
	strError = "";

	CSSFile fileConfig;
	if (fileConfig.Open(GetStockmateConfigFilename(), "wb") == FALSE)
	{
		strError = "Unable to create Stockmate configuration file.";
		return FALSE;
	}

	COleDateTime dateStocktake = m_Stocktake.GetOleDateTime();

	CString strPeriodDate = "";
	strPeriodDate.Format("%2.2d-%2.2d-%4.4d",
		dateStocktake.GetMonth(),
		dateStocktake.GetDay(),
		dateStocktake.GetYear());

	COleDateTime timeNow = COleDateTime::GetCurrentTime();

	CString strTime = "";
	strTime.Format("%2.2d:%2.2d",
		timeNow.GetHour(),
		timeNow.GetMinute());

	{
		CCSV csvConfig(',', '\'');
		csvConfig.Add(InQuotes(dbStockpoint.GetName(m_nSpIdx)));
		csvConfig.Add(InQuotes("1"));		//PUB NUMBER
		csvConfig.Add(1);					//NUMBER OF BARS
		csvConfig.Add(0);					//NOT USED
		csvConfig.Add(InQuotes("20.0"));	//VAT RATE
		csvConfig.Add(InQuotes(strPeriodDate));
		csvConfig.Add(InQuotes(strPeriodDate));
		csvConfig.Add(InQuotes(strTime));
		csvConfig.Add(0);					//OUTLET TYPE
		csvConfig.Add(InQuotes("£"));
		fileConfig.WriteLine(csvConfig.GetLine());
	}

	{
		CCSV csvConfig(',', '\'');
		csvConfig.Add(InQuotes(dbStockpoint.GetName(m_nSpIdx)));
		csvConfig.Add(1);
		fileConfig.WriteLine(csvConfig.GetLine());
	}

	fileConfig.WriteLine(InQuotes(m_Stocktake.GetComment()));

	return TRUE;
}

/**********************************************************************/

bool CStockmateLink::CreateSectionsFile(CString& strError)
{
	strError = "";

	CSSFile fileSections;
	if (fileSections.Open(GetStockmateSectionsFilename(), "wb") == FALSE)
	{
		strError = "Unable to create Stockmate Sections file.";
		return FALSE;
	}

	for (int n = 1; n <= 49; n++)
	{
		CString strText = DataManager.Category.GetText(n);
		::TrimSpaces(strText, FALSE);

		if (strText == "")
		{
			strText = DataManager.Category.GetDefaultText(n);
		}

		CString strNum = "";
		strNum.Format("%d", n);

		CCSV csvSection(',', '\'');
		csvSection.Add(InQuotes(strNum));
		csvSection.Add(InQuotes(strText));
		fileSections.WriteLine(csvSection.GetLine());
	}

	CCSV csvSection(',', '\'');
	csvSection.Add(InQuotes("50"));
	csvSection.Add(InQuotes("Other Categories"));
	fileSections.WriteLine(csvSection.GetLine());

	return TRUE;
}

/**********************************************************************/

bool CStockmateLink::CreateSubsectionsFile(CString& strError)
{
	strError = "";

	CSSFile fileSubsections;
	if (fileSubsections.Open(GetStockmateSubsectionsFilename(), "wb") == FALSE)
	{
		strError = "Unable to create Stockmate Subsections file.";
		return FALSE;
	}

	fileSubsections.WriteLine("\"1\",\"Not used\"");
	return TRUE;
}

/**********************************************************************/

bool CStockmateLink::CreateCommandFile(CString& strError)
{
	strError = "";

	CSSFile fileCommand;
	if (fileCommand.Open(GetStockmateCommandFilename(), "wb") == FALSE)
	{
		strError = "Unable to create Stockmate Command file.";
		return FALSE;
	}

	CString strCommand = "";
	strCommand.Format("\"C:\\SPOS_Link\\SPOS_Link.exe\" L 1 \"%s\" \"C:\\SPOS Data\" 1",
		dbStockpoint.GetName(m_nSpIdx));

	fileCommand.WriteLine(strCommand);

	return TRUE;
}

/**********************************************************************/

bool CStockmateLink::CreateProductsFile(CString& strError)
{
	strError = "";

	CSSFile fileProducts;
	if (fileProducts.Open(GetStockmateProductsFilename(), "wb") == FALSE)
	{
		strError = "Unable to create Stockmate products file.";
		return FALSE;
	}

	CString strMsg = "";
	strMsg.Format("Preparing Stockmate Products File (%s)", m_Stocktake.GetComment());
	StatusProgress.Lock(TRUE, strMsg);

	CStringArray arrayLines;
	int nStockItemCount = DataManager.Stock.GetSize();
	for (int nStockIdx = 0; nStockIdx < nStockItemCount; nStockIdx++)
	{
		StatusProgress.SetPos(nStockIdx, nStockItemCount);
		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt(nStockIdx, StockRecord);

		CStockLevelsCSVRecord StockLevels;
		DataManager.Stock.GetStockpointTotals(nStockIdx, StockRecord, dbStockpoint.GetSpNo(m_nSpIdx), StockLevels);

		if (StockLevels.GetStocktakeNo() == m_Stocktake.GetStocktakeNo())
		{
			arrayLines.Add(GetProductLine(StockRecord, StockLevels));
		}
	}

	StatusProgress.Unlock();

	if (arrayLines.GetSize() == 0)
	{
		strError = "Stockmate file not created : no items found in stocktake.";
		return FALSE;
	}

	CCSV csv(',', '\'');
	csv.Add(InQuotes(dbStockpoint.GetName(m_nSpIdx)));
	csv.Add(InQuotes(m_Stocktake.GetComment()));
	csv.Add(arrayLines.GetCount());
	csv.Add(1);
	csv.Add(0);
	csv.Add(0);
	csv.Add(0);
	csv.Add(16);
	fileProducts.WriteLine(csv.GetLine());

	for (int s = 0; s < arrayLines.GetSize(); s++)
	{
		fileProducts.WriteLine(arrayLines.GetAt(s));
	}

	return TRUE;
}

/**********************************************************************/

CString CStockmateLink::GetProductLine(CStockCSVRecord& StockRecord, CStockLevelsCSVRecord& StockLevels)
{
	int nSection = StockRecord.GetCategory();
	if ((nSection < 1) || (nSection > 50))
	{
		nSection = 50;
	}

	CString strItemText = StockRecord.GetStockmateText();
	if (strItemText.GetLength() == 0)
	{
		strItemText = StockRecord.GetStockUnitName();
		strItemText = strItemText.Left(8);
	}

	CCSV csvProduct(',', '\'');

	csvProduct.Add(StockRecord.GetStockCode());
	csvProduct.Add(InQuotes(StockRecord.GetDescription()));
	csvProduct.Add(InQuotes(""));	//BIN NUMBER
	csvProduct.Add(1);				//VINTAGE
	csvProduct.Add(nSection);
	csvProduct.Add(1);				//SUBSECTION
	csvProduct.Add(InQuotes(strItemText));
	csvProduct.Add(StockRecord.GetSubUnits(TRUE));				
	csvProduct.Add(1);
	csvProduct.Add(StockRecord.GetUnitCost(0), 2);

	{
		double dOpening = StockLevels.GetOpeningQty();
		double dApparent = StockLevels.GetStocktakeApparentQty();

		if (StockRecord.GetSubUnits(TRUE) > 1)
		{
			__int64 nStock, nSub;
			StockRecord.ConvertStockLevelToStockSub(dOpening, nStock, nSub);
			csvProduct.Add(nStock);
			csvProduct.Add(nSub);

			StockRecord.ConvertStockLevelToStockSub(dApparent - dOpening, nStock, nSub);
			csvProduct.Add(nStock);
			csvProduct.Add(nSub);
		}
		else
		{
			int nStock = int(floor(dOpening));
			int nSub = int(floor((dOpening - nStock) * 1000));
			csvProduct.Add(nStock);
			csvProduct.Add(nSub);

			double dXfer = dApparent - dOpening;
			nStock = int(floor(dXfer));
			nSub = int(floor((dXfer - nStock) * 1000));
			csvProduct.Add(nStock);
			csvProduct.Add(nSub);
		}
	}

	bool bGotPluInfo = AppendPluInfo(csvProduct, StockRecord);

	for (int n = 0; n < 5; n++)
	{
		bool bGotWrapper = FALSE;

		if (n < StockRecord.GetSupplierCount())
		{
			CString strWrapper = StockRecord.GetWrapper(n);
			if ((strWrapper != "") && (TestNumeric(strWrapper)))
			{
				csvProduct.Add(strWrapper);
				csvProduct.Add(StockRecord.GetYield(n), 2);
				bGotWrapper = TRUE;
			}
		}

		if (FALSE == bGotWrapper)
		{
			csvProduct.Add("0");
			csvProduct.Add("1");
		}
	}

	if (FALSE == bGotPluInfo)
	{
		csvProduct.Add("0");
		csvProduct.Add("1");
	}

	csvProduct.Add(0); //EMPTY WEIGHT
	csvProduct.Add(0); //FULL WEIGHT
	csvProduct.Add(0); //OPTIC WEIGHT
	csvProduct.Add(1); //NOT USED

	return csvProduct.GetLine();
}

/**********************************************************************/

bool CStockmateLink::AppendPluInfo(CCSV& csvProduct, CStockCSVRecord& StockRecord)
{
	bool bGotPluInfo = FALSE;

	int nStockPluIdx = 0;
	CString strStockCode = StockRecord.GetStockCode();
	if (DataManager.StockPlu.FindStockCode(strStockCode, nStockPluIdx) == TRUE)
	{
		__int64 nPluNo = 0;

		if (DataManager.StockPlu.GetPluNoCount(nStockPluIdx) > 0)
		{
			nPluNo = DataManager.StockPlu.GetPluNo(nStockPluIdx, 0);
		}
		else if (DataManager.StockPlu.GetRecipePluNoCount(nStockPluIdx) > 0)
		{
			nPluNo = DataManager.StockPlu.GetRecipePluNo(nStockPluIdx, 0);
		}

		if (nPluNo != 0)
		{
			int nPluIdx;
			if (DataManager.Plu.FindPluByNumber(nPluNo, nPluIdx) == TRUE)
			{
				CPluCSVRecord PluRecord;
				DataManager.Plu.GetAt(nPluIdx, PluRecord);

				bool bGotProfit = FALSE;
				CExplodePluDatabase ExplodePlu;

				CStringArray StockDetails;
				if (ExplodePlu.ExplodePlu(PluRecord.GetPluNoInt(), StockDetails, AUDIT_SALES, 1.00, PluRecord.GetPrice(0)) == PLU_EXCEPTION_NONE)
				{
					for (int nIndex = 0; nIndex < StockDetails.GetSize(); nIndex++)
					{
						CCSVExplodedPlu csvSale(StockDetails.GetAt(nIndex));

						if (strStockCode == csvSale.GetStockCode())
						{
							double dStockQty = csvSale.GetStockQty();
							double dStockVal = 0.0;

							if (dStockQty != 0.0)
							{
								dStockVal = csvSale.GetStockValue() / dStockQty;
							}

							csvProduct.Add(dStockVal, 2);			//SELLING PRICE
							csvProduct.Add("1");					//LIVE FLAG
							csvProduct.Add(PluRecord.GetPluNoInt());
							csvProduct.Add(dStockQty, 2);
							bGotPluInfo = TRUE;
						}
					}
				}
			}
		}
	}

	if (FALSE == bGotPluInfo)
	{
		csvProduct.Add("0.0"); //NO RETAIL PRICE
		csvProduct.Add("1");   //PRODUCT IS LIVE
	}

	return bGotPluInfo;
}

/**********************************************************************/

bool CStockmateLink::ConvertStockmateCountFile()
{
	CSSFile fileIn;
	if (fileIn.Open(GetStockmateCountFilename(), "rb") == FALSE)
	{
		return FALSE;
	}

	CSSFile fileOut;
	if (fileOut.Open(GetStockmateClosingStockFilename(), "wb") == FALSE)
	{
		return FALSE;
	}

	CString strBuffer = "";
	while (fileIn.ReadString(strBuffer) == TRUE)
	{
		CCSV csvStock(strBuffer);
		CString strStockCode = csvStock.GetString(0);
		int nWholeUnits = csvStock.GetInt(1);
		int nPartUnits = csvStock.GetInt(2);

		double dStockUnits = 0.0;

		int nStockIdx = 0;
		if (DataManager.Stock.FindStockItemByCode(strStockCode, nStockIdx) == TRUE)
		{
			CStockCSVRecord StockRecord;
			DataManager.Stock.GetAt(nStockIdx, StockRecord);
			int nSubUnitYield = DataManager.Stock.GetSubUnits(StockRecord);

			if (nSubUnitYield <= 1)
			{
				dStockUnits = nWholeUnits + (((double)nPartUnits) / 1000.0);
			}
			else
			{
				dStockUnits = nWholeUnits + (((double)nPartUnits) / nSubUnitYield);
			}

			CCSV csvOut;
			csvOut.Add(StockRecord.GetStockCode());
			csvOut.Add(dStockUnits, 5);
			fileOut.WriteLine(csvOut.GetLine());
		}
	}

	fileIn.Close();
	fileOut.Close();

	CFileRemover FileRemover;
	FileRemover.RemoveFile(GetStockmateCountFilename());

	return TRUE;
}

/**********************************************************************/

CString CStockmateLink::InQuotes(CString strText)
{
	CString strQuote = "\x022";
	return strQuote + strText + strQuote;
}

/**********************************************************************/
