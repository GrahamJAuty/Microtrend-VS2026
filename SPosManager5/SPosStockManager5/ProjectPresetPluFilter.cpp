/**********************************************************************/
#include "..\CommonEcrStock\PresetPluFilter.h"
/**********************************************************************/

void CPresetPluFilter::CreateStockFields()
{
	m_StockmanStockCode.Create("StockmanStockCode", Stock::StockCode.Max, "");
	m_StockmanShowUnlinked.Create("StockmanShowUnlinked", TRUE);
	m_StockmanShowLinked.Create("StockmanShowLinked", TRUE);
	m_StockmanShowRecipe.Create("StockmanShowRecipe", TRUE);
}

/**********************************************************************/

void CPresetPluFilter::ReadStockFields(CEnhancedIniFile& iniFile)
{
	iniFile.ReadString(m_StockmanStockCode);
	iniFile.ReadBool(m_StockmanShowUnlinked);
	iniFile.ReadBool(m_StockmanShowLinked);
	iniFile.ReadBool(m_StockmanShowRecipe);
}

/**********************************************************************/

void CPresetPluFilter::WriteStockFields(CEnhancedIniFile& iniFile)
{
	iniFile.WriteString(m_StockmanStockCode);
	iniFile.WriteBool(m_StockmanShowUnlinked);
	iniFile.WriteBool(m_StockmanShowLinked);
	iniFile.WriteBool(m_StockmanShowRecipe);
}

/**********************************************************************/