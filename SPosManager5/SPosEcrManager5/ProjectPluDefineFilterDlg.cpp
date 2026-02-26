/**********************************************************************/
#include "..\CommonEcrStock\PluDefineFilterDlg.h"
/**********************************************************************/

void CPluDefineFilterDlg::InitStockFields()
{
	if (EcrmanOptions.GetFeaturesPluStockFlag() == FALSE)
	{
		ShowAndEnableWindow(&m_staticSupplier, FALSE);
		ShowAndEnableWindow(&m_editSupplier, FALSE);
		ShowAndEnableWindow(&m_comboSupplier, FALSE);
		ShowAndEnableWindow(&m_staticStockCode, FALSE);
		ShowAndEnableWindow(&m_editStockCode, FALSE);
		ShowAndEnableWindow(&m_checkNonStock, FALSE);
	}
	else
	{
		m_staticStockCode.SetWindowText("Reference");

		m_editStockCode.LimitText(Plu::SupplierRef.Max);
		m_editStockCode.SetWindowText(m_PluFilterArray.GetEcrmanSuppRefFilter());

		m_checkNonStock.SetCheck(m_PluFilterArray.GetEcrmanShowNonStockFlag());
		m_EcrmanSupplierSelector.FillEcrmanSupplierCombo(m_PluFilterArray.GetEcrmanSupplierFilter());
	}

	ShowAndEnableWindow(&m_checkUnlinked, FALSE);
	ShowAndEnableWindow(&m_checkLinked, FALSE);
	ShowAndEnableWindow(&m_checkRecipe, FALSE);
}

/**********************************************************************/

void CPluDefineFilterDlg::KillFocusSupplier()
{
	m_EcrmanSupplierSelector.SelectEcrmanSupplierFromEditBox();
}

/**********************************************************************/

void CPluDefineFilterDlg::SelectSupplier()
{
	m_EcrmanSupplierSelector.SelectEcrmanSupplierFromCombo();
}

/**********************************************************************/

void CPluDefineFilterDlg::SaveStockFields()
{
	if (EcrmanOptions.GetFeaturesPluStockFlag() == TRUE)
	{
		m_PluFilterArray.SetEcrmanSuppRefFilter(GetEditBoxText(m_editStockCode));
		m_PluFilterArray.SetEcrmanSupplierFilter(m_EcrmanSupplierSelector.GetEcrmanSupplierNo());
		m_PluFilterArray.SetEcrmanShowNonStockFlag(IsTicked(m_checkNonStock));
	}
}

/**********************************************************************/
