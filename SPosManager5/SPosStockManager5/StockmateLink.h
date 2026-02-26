#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockMonitor.h"
#include "StocktakeCSVArray.h"
/**********************************************************************/

class CStockmateLink
{
public:
	CStockmateLink(int nSpIdx,CStocktakeCSVRecord& Stocktake);
	
public:
	void CreateFiles(CWnd* pParent);
	bool ConvertStockmateCountFile();

private:
	bool CreateConfigFile(CString& strError);
	bool CreateProductsFile(CString& strError);
	CString GetProductLine(CStockCSVRecord& StockRecord, CStockLevelsCSVRecord& StockLevels);
	bool AppendPluInfo(CCSV& csvProduct, CStockCSVRecord& StockRecord);
	bool CreateSectionsFile(CString& strError);
	bool CreateSubsectionsFile(CString& strError);
	bool CreateCommandFile(CString& strError);
	
public:
	CString GetStockmateDataFolder() { return "c:\\spos data\\hhcomms\\out1"; }
	CString GetStockmateConfigFilename() { return GetStockmateDataFolder() + "\\hh_config.txt"; }
	CString GetStockmateProductsFilename() { return GetStockmateDataFolder() + "\\hh_products1.txt"; }
	CString GetStockmateSectionsFilename() { return GetStockmateDataFolder() + "\\hh_sections.txt"; }
	CString GetStockmateSubsectionsFilename() { return GetStockmateDataFolder() + "\\hh_subsections.txt"; }
	CString GetStockmateCommandFilename() { return GetStockmateDataFolder() + "\\SPOS_SEND.BAT"; }
	CString GetStockmateCountFilename() { return GetStockmateDataFolder() + "\\hh1_count1.txt"; }
	CString GetStockmateClosingStockFilename() { return GetStockmateDataFolder() + "\\ClosingStock.dat"; }

private:
	CString InQuotes(CString strText);

private:
	CStocktakeCSVRecord& m_Stocktake;
	int m_nSpIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/


