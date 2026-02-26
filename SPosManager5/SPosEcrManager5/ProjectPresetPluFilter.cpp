/**********************************************************************/
#include "..\SPosEcrManager5\EcrmanSupplierSelector.h"
/**********************************************************************/
#include "..\CommonEcrStock\PresetPluFilter.h"
/**********************************************************************/

void CPresetPluFilter::CreateStockFields()
{
	m_EcrmanSupplierFilter.Create("EcrmanSupplierFilter", 0, 9999, ECRMANSUPPLIER_ALL);
	m_EcrmanShowNonStock.Create("EcrmanShowNonStock", TRUE);
	m_EcrmanSuppRef.Create("EcrmanSuppRef", Plu::SupplierRef.Max, "");
}

/**********************************************************************/

void CPresetPluFilter::ReadStockFields(CEnhancedIniFile& iniFile)
{
	iniFile.ReadInt(m_EcrmanSupplierFilter);
	iniFile.ReadBool(m_EcrmanShowNonStock);
	iniFile.ReadString(m_EcrmanSuppRef);
}

/**********************************************************************/

void CPresetPluFilter::WriteStockFields(CEnhancedIniFile& iniFile)
{
	iniFile.WriteInt(m_EcrmanSupplierFilter);
	iniFile.WriteBool(m_EcrmanShowNonStock);
	iniFile.WriteString(m_EcrmanSuppRef);
}

/**********************************************************************/
