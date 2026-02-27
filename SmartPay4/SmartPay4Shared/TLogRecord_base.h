
#pragma once
//*********************************************************************

#define nTLOGREC_DPQTY		3
#define nTLOGREC_DPVALUE	2

//#define nTLOGREC_VERSION		2		// v2 =  TerminalNo, Purse spend + Purse balances added before ItemData
//#define nTLOGREC_VERSION		3		// v3 =  Purse3 Spend & Purse3 Balance, TaxCode added before ItemData
//#define nTLOGREC_VERSION		4		// v4 =  ReceiptNo added before ItemData
//#define nTLOGREC_VERSION		5		// v5 =  Item value added as integer in p
#define nTLOGREC_VERSION		6		// v6 =  Added folder set

#define nTLOGREC_ITEMFIELDSv2	6		// ( nType, szNo, szText, dQty, dValue, nPoints )
#define nTLOGREC_ITEMFIELDSv3	7		// ( nType, szNo, szText, dQty, dValue, nPoints, Taxcode )
#define nTLOGREC_ITEMFIELDSv4	7
#define nTLOGREC_ITEMFIELDSv5	8		// ( nType, szNo, szText, dQty, nItemValue, dValue, nPoints, Taxcode )
#define nTLOGREC_ITEMFIELDS		8

#define nTLOGTYPE_PLU			0
#define nTLOGTYPE_DEPT			1
#define nTLOGTYPE_DISC_SUBT		2
#define nTLOGTYPE_PROMOTION		3
#define nTLOGTYPE_PLU2			4		// dept ( 4 digits ) + pluNo (1 - 13 digits)
#define nTLOGTYPE_SURCHARGE		5		// Group % discount
#define nTLOGTYPE_DISC_ITEM		6

//*********************************************************************

class CTLogRecord_base
{
public:
	CTLogRecord_base();
	CTLogRecord_base ( const char* szLine );

	bool WriteLine ( CSSFile* file );
	bool ReadLine ( CSSFile* file, bool bFullRead = TRUE );
	void Reset();
	void Init();

	void SetDate ( const char* szDate ) { m_date.SetDate(szDate); }
	void SetTime ( const char* szTime ) { m_time.SetTime(szTime); }

	const char* GetDate() { return m_date.GetDate(); }
	const char* GetTime() { return m_time.GetTime(); }
	int GetYear()	{ return m_date.GetYear();	}
	int GetMonth()	{ return m_date.GetMonth();	}
	int GetDay()	{ return m_date.GetDay();	}
	int GetHour()	{ return m_time.GetHour();	}
	int GetMinute()	{ return m_time.GetMinute();}

	void SetCCNo ( int n ) { m_nCCNo = n; }
	int GetCCNo() { return m_nCCNo; }

	void SetReceiptNo ( const char* szReceiptNo ) { m_strReceiptNo = szReceiptNo; }
	CString GetReceiptNo() { return m_strReceiptNo; }

	void SetFolderSet(int n) { m_nFolderSet = n; }
	int GetFolderSet() { return m_nFolderSet; }

	void SetTerminalNo ( int n ) { m_nTerminalNo = n; }
	int GetTerminalNo() { return m_nTerminalNo; }

	void SetVoidPurse1Spend() { m_dPurse1Spend = -(m_dPurse1Spend); }
	void AddToPurse1Spend ( double dValue ) { m_dPurse1Spend += dValue; }
	double GetPurse1Spend() { return m_dPurse1Spend; }
	void SetPurse1 ( double dValue ) { m_dPurse1 = dValue; }

	void SetVoidPurse2Spend() { m_dPurse2Spend = -(m_dPurse2Spend); }
	void AddToPurse2Spend ( double dValue ) { m_dPurse2Spend += dValue; }
	double GetPurse2Spend() { return m_dPurse2Spend; }
	void SetPurse2 ( double dValue ) { m_dPurse2 = dValue; }

	void SetVoidPurse3Spend() { m_dPurse3Spend = -(m_dPurse3Spend); }
	void AddToPurse3Spend ( double dValue ) { m_dPurse3Spend += dValue; }
	double GetPurse3Spend() { return m_dPurse3Spend; }
	void SetPurse3 ( double dValue ) { m_dPurse3 = dValue; }

	double GetTotalSpend() { return m_dPurse1Spend + m_dPurse2Spend + m_dPurse3Spend;  }

	int  GetItemCounter();
	void SaveItemData ( int nType, const char* szNo, const char* szText, double dLineQty, int nPerItemVal, double dLineVal, int nPoints, const char* szTaxCode );

	void ExtractItemData ( int nIndex );
	int GetItemType()		{ return m_nItemType; }
	CString GetItemNo()		{ return m_strItemNo; }
	int GetItemDeptNo()		{ return m_nItemDeptNo; }
	CString GetItemText()	{ return m_strItemText; }
	double GetItemLineQty()	{ return m_dLineQty; }
	double GetItemLineVal()	{ return m_dLineVal; }
	CString GetItemTax()	{ return m_strItemTax; }
	int GetPerItemVal()		{ return m_nPerItemVal; }
	int GetItemPoints()		{ return m_nItemPoints; }	

public:
	CSSDate m_date;
	CSSTime m_time;
	
protected:
	int m_nCCNo;
	int m_nFolderSet;
	int m_nTerminalNo;
	CString m_strReceiptNo;
	double m_dPurse1Spend;				// spend from Purse1
	double m_dPurse2Spend;				// spend from Purse2
	double m_dPurse3Spend;				// spend from Purse3
	double m_dPurse1;					// balance of purse1 after sale
	double m_dPurse2;					// balance of purse2 after sale
	double m_dPurse3;					// balance of purse3 after sale

	CCSV m_csvItemData;					// holds "Type,pluno,desc,qty,value,PluPoints,Taxcode" per item sale
	int m_nItemType;
	CString m_strItemNo;				// PluNo or GroupDiscoun No
	int m_nItemDeptNo;
	CString m_strItemText;
	double m_dLineQty;
	double m_dLineVal;
	CString m_strItemTax;
	int m_nPerItemVal;
	int m_nItemPoints;

private:
	void ReadDataV1to6 (CCSV& csvLine, int nVer );
	bool IsVoidItem ( int nType, const char* szNo, const char* szText, double dQty, double nValue );
	bool IsDuplicate ( int nType, const char* szNo, const char* szText, double dLineQty, int nPerItemVal, double dLineVal, int nPoints, const char* szTaxCode );
};

/**********************************************************************/
