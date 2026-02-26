#pragma once
/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/

class CPMSTermLineEdit : public CCSV
{
public:
	CPMSTermLineEdit ( const char* szCSVLine ) : CCSV(szCSVLine) {}
	const char* GetLineType() { return GetString(0); }

	//TRANSACTION LINE $TRAN_EDIT
	int GetTransactionNumber();
	const char* GetTransactionDate(); 
	const char* GetTransactionTime();
	int	GetTransactionValue();
	
	//ITEM LINE $ITEM_EDIT
	__int64 GetItemPluNo();
	double GetItemQty();
	int	GetItemPriceBand();
	int GetItemLinePrice();
	int GetItemDeptNo();
	int GetItemGroupNo();
	const char* GetItemTaxBand();
	int GetItemNumericTaxBand();

	void SetItemNumericTaxBand( int n );

	//ITEM DISCOUNT LINE $IDISC_EDIT
	__int64 GetItemDiscountPluNo();
	int GetItemDiscountAmount();

	//SUBTOTAL DISCOUNT LINE $IDISC_EDIT
	int GetSubtotalDiscountAmount();

	//PAYMENT LINE $PAYM_EDIT
	int GetPaymentNo();
	const char* GetPaymentText();
	int GetPaymentAmount();

private:
	CString m_strTemp;
};

/**********************************************************************/
#endif
/**********************************************************************/
