/**********************************************************************/
#include "StockReportSupplierDlgHelpers.h"
/**********************************************************************/

CStockReportSupplierDlgHelpers::CStockReportSupplierDlgHelpers( CSSListTaggedSelectCtrl& list, CSSComboBox& combo, CByteArray& flags, CWordArray& select ) : m_listSuppliers( list ), m_combo( combo ), m_SupplierFlags( flags ), m_wSuppSelectArray( select )
{
}

/**********************************************************************/

void CStockReportSupplierDlgHelpers::InitialiseSupplierFlags()
{
	for ( int nIndex = 0; nIndex <= DataManager.Supplier.GetSize(); nIndex++ )
		m_SupplierFlags.Add(0);

	for ( int nIndex = 0; nIndex < m_wSuppSelectArray.GetSize(); nIndex++ )
	{
		int nSuppIdx = m_wSuppSelectArray.GetAt ( nIndex );

		if ( nSuppIdx >= 0 && nSuppIdx < m_SupplierFlags.GetSize() )
			m_SupplierFlags.SetAt ( nSuppIdx, 1 );
	}
}

/**********************************************************************/

void CStockReportSupplierDlgHelpers::FillSupplierCombo()
{
	for ( int nIndex = 0; nIndex < DataManager.SupplierSet.GetSize(); nIndex++ )
		m_combo.AddString ( DataManager.SupplierSet.GetName ( nIndex ) );

	m_combo.InsertString ( 0, "Custom Selection" );
	m_combo.EnableWindow ( m_combo.GetCount() > 1 );
	m_combo.SetCurSel(0);
}

/**********************************************************************/

void CStockReportSupplierDlgHelpers::AddList( CByteArray& SupplierFlags )
{
	int nCurSel = m_listSuppliers.GetCurSel();
	
	m_listSuppliers.ClearList();

	for ( int nIndex = 0; nIndex < DataManager.Supplier.GetSize(); nIndex++ )
	{
		CSSListTaggedSelectItem item;
		item.m_strText = DataManager.Supplier.GetName ( nIndex );
		item.m_nData = nIndex + 1;
		item.SetTagState(0);

		if ( nIndex < SupplierFlags.GetSize() - 1 )
		{
			if ( SupplierFlags.GetAt ( nIndex + 1 ) == 1 )
			{
				item.SetTagState(1);
			}
		}

		m_listSuppliers.AddSortedItemToList( item );
	}

	CSSListTaggedSelectItem item;
	item.m_strText = "Unspecified";
	item.m_nData = 0;
	item.SetTagState(0);

	if ( SupplierFlags.GetSize() > 0 )
	{
		if ( SupplierFlags.GetAt(0) == 1 )
		{
			item.SetTagState(1);
		}
	}

	m_listSuppliers.InsertItemIntoList( 0, item );
	m_listSuppliers.SetCurSel( nCurSel );
}

/**********************************************************************/

void CStockReportSupplierDlgHelpers::HandleComboSelect()
{
	int nSelection = m_combo.GetCurSel();

	if ( nSelection < 0 || nSelection > DataManager.SupplierSet.GetSize() )
		return;

	if ( nSelection == 0 )
		AddList ( m_SupplierFlags );
	else
	{
		CSupplierSetCSVRecord SupplierSet;
		CByteArray SupplierFlags;
		DataManager.SupplierSet.GetAt ( nSelection - 1, SupplierSet );
		SupplierSet.GetSupplierFlags ( SupplierFlags );
		AddList ( SupplierFlags );
	}
}

/**********************************************************************/

void CStockReportSupplierDlgHelpers::UpdateSupplierSelection()
{
	m_wSuppSelectArray.RemoveAll();

	//build an array of selected suppliers
	for ( int nIndex = 0; nIndex < m_listSuppliers.GetItemCount(); nIndex++ )
	{
		CSSListTaggedSelectItem item;
		m_listSuppliers.GetListItem( nIndex, item );

		if ( item.GetTagState() == 1 )
		{
			WORD wSuppIdx = ( WORD ) item.m_nData;
			m_wSuppSelectArray.Add ( wSuppIdx );
		}
	}
}

/**********************************************************************/
