#pragma once
//**********************************************************************
#define nTLOGFILE_DPQTY		3
#define nTLOGFILE_DPVALUE	2
//**********************************************************************
#define nTLOGTYPE_PLU		0
#define	nTLOGTYPE_DEPT		1
#define nTLOGTYPE_DISC_SUBT	2
#define nTLOGTYPE_DISC_ITEM	3
//**********************************************************************
#define nV1_ITEMFIELDS		5			// v1 = nType, szNo, szText, dQty, dValue
#define nV2_ITEMFIELDS		6			// v2 = nType, szNo, szText, dQty, dValue, points
#define nV3_ITEMFIELDS		6			// v3 = nType, szNo, szText, dQty, dValue, points
#define nV4_ITEMFIELDS		6			// v4 = nType, szNo, szText, dQty, dValue, points
#define nV5_ITEMFIELDS		7			// v5 = nType, szNo, szText, dQty, dValue, points, flags
#define nV6_ITEMFIELDS		7			// v6 = nType, szNo, szText, dQty, dValue, points, flags
#define nV7_ITEMFIELDS		7			// v6 = nType, szNo, szText, dQty, dValue, points, flags
//**********************************************************************

class CTLogRecord_base
{
public:
	CTLogRecord_base();
	CTLogRecord_base(CString strLine);

	bool WriteLine(CSSFile* file);
	bool ReadLine(CSSFile* file, bool bFullRead = TRUE);
	void Reset();

	void SetDate(CString strDate) { m_date.SetDate(strDate); }
	void SetTime(CString strTime) { m_time.SetTime(strTime); }

	const char* GetDate() { return m_date.GetDate(); }
	const char* GetTime() { return m_time.GetTime(); }
	int GetYear() { return m_date.GetYear(); }
	int GetMonth() { return m_date.GetMonth(); }
	int GetDay() { return m_date.GetDay(); }
	int GetHour() { return m_time.GetHour(); }
	int GetMinute() { return m_time.GetMinute(); }

	void SetCCNo(int n) { m_nCCNo = n; }
	int GetCCNo() { return m_nCCNo; }

	void SetDbNo(int n) { m_nDbNo = n; }
	int GetDbNo() { return m_nDbNo; }

	void SetFolderSet(int n) { m_nFolderSet = n; }
	int GetFolderSet() { return m_nFolderSet; }

	void SetTerminalNo(int n) { m_nTerminalNo = n; }
	int GetTerminalNo() { return m_nTerminalNo; }

	void SetVoidPurse1Spend() { m_dPurse1Spend = -(m_dPurse1Spend); }
	void AddToPurse1Spend(double dValue) { m_dPurse1Spend += dValue; }
	double GetPurse1Spend() { return m_dPurse1Spend; }
	void SetPurse1(double dValue) { m_dPurse1 = dValue; }

	void SetVoidPurse2Spend() { m_dPurse2Spend = -(m_dPurse2Spend); }
	void AddToPurse2Spend(double dValue) { m_dPurse2Spend += dValue; }
	double GetPurse2Spend() { return m_dPurse2Spend; }
	void SetPurse2(double dValue) { m_dPurse2 = dValue; }

	double GetTotalSpend() { return m_dPurse1Spend + m_dPurse2Spend; }

	int  GetItemCounter();
	void SaveItemData(int nType, CString strPluNo, CString strText, double dQty, double dValue, int nPoints, int nFlags);
	
	void ExtractItemData(int nIndex);
	int GetItemType() { return m_nItemType; }
	CString GetItemNo() { return m_strItemNo; }
	CString GetItemText() { return m_strItemText; }
	double GetItemLineQty() { return m_dItemLineQty; }
	double GetItemLineVal() { return m_dItemLineVal; }
	int GetItemPoints() { return m_nItemPoints; }
	int GetItemFlags() { return m_nItemFlags; }

	void ApplyBonusPointsMultiplier(int nBonusMultiplier);

private:
	void ReadDataV1(CCSV& csv);
	void ReadDataV2(CCSV& csv);
	void ReadDataV3(CCSV& csv);
	void ReadDataV4(CCSV& csv);
	void ReadDataV5(CCSV& csv);
	void ReadDataV6(CCSV& csv);
	void ReadDataV7(CCSV& csv);
	void ReadItemFields(CCSV& csv, int nFieldIdx, int nItemCount, int nItemFields);
	bool IsVoidItem(int nType, CString strPluNo, CString strText, int nFlags, double dQty, double dValue);
	bool IsDuplicate(int nType, CString strPluNo, CString strText, int nFlags, double dQty, double dValue, int nPoints);

public:
	CSSDate m_date;
	CSSTime m_time;

protected:
	int m_nCCNo;
	int m_nDbNo;
	int m_nFolderSet;
	int m_nTerminalNo;
	double m_dPurse1Spend;				// spend from Purse1
	double m_dPurse2Spend;				// spend from Purse2
	double m_dPurse1;					// balance of purse after sale
	double m_dPurse2;					// balance of purse after sale
	CCSV m_csvItemData;					// holds "Type,pluno,desc,qty,value,PluPoints" per item sale

	int m_nItemType;
	CString m_strItemNo;
	CString m_strItemText;
	double m_dItemLineQty;
	double m_dItemLineVal;
	int m_nItemPoints;
	int m_nItemFlags;
};

/**********************************************************************/
