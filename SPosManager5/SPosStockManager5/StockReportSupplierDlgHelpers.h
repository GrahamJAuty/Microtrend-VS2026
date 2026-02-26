#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

class CStockReportSupplierDlgHelpers
{
public:
	CStockReportSupplierDlgHelpers( CSSListTaggedSelectCtrl& list, CSSComboBox& combo, CByteArray& flags, CWordArray& select );

public:
	void InitialiseSupplierFlags();
	void UpdateSupplierSelection();
	void FillSupplierCombo();
	void AddList( CByteArray& SupplierFlags );

public:
	void HandleComboSelect();

private:
	CSSListTaggedSelectCtrl& m_listSuppliers;
	CSSComboBox& m_combo;
	CByteArray& m_SupplierFlags;
	CWordArray& m_wSuppSelectArray;
};

/**********************************************************************/
#endif
/**********************************************************************/
