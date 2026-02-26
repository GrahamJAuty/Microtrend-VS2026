/**********************************************************************/
#include "..\CommonEcrStock\PluDefineFilterDlg.h"
/**********************************************************************/

void CPluDefineFilterDlg::InitStockFields()
{
	ShowAndEnableWindow(&m_staticSupplier, FALSE);
	ShowAndEnableWindow(&m_editSupplier, FALSE);
	ShowAndEnableWindow(&m_comboSupplier, FALSE);
	ShowAndEnableWindow(&m_checkNonStock, FALSE);

	MoveControl(&m_staticStockCode, 24, 170);
	MoveControl(&m_editStockCode, 79, 170);

	m_staticStockCode.SetWindowText("Stock code");

	m_editStockCode.LimitText(Stock::StockCode.Max);	
	m_editStockCode.SetWindowText(m_PluFilterArray.GetStockmanStockCodeFilter());

	m_checkUnlinked.SetCheck(m_PluFilterArray.GetStockmanShowUnlinkedPluFlag());
	m_checkLinked.SetCheck(m_PluFilterArray.GetStockmanShowLinkedPluFlag());
	m_checkRecipe.SetCheck(m_PluFilterArray.GetStockmanShowRecipePluFlag());
}

/**********************************************************************/

void CPluDefineFilterDlg::KillFocusSupplier()
{
}

/**********************************************************************/

void CPluDefineFilterDlg::SelectSupplier()
{
}

/**********************************************************************/

void CPluDefineFilterDlg::SaveStockFields()
{
	m_PluFilterArray.SetStockmanStockCodeFilter(GetEditBoxText(m_editStockCode));
	m_PluFilterArray.SetStockmanShowUnlinkedPluFlag(IsTicked(m_checkUnlinked));
	m_PluFilterArray.SetStockmanShowLinkedPluFlag(IsTicked(m_checkLinked));
	m_PluFilterArray.SetStockmanShowRecipePluFlag(IsTicked(m_checkRecipe));
}

/**********************************************************************/
