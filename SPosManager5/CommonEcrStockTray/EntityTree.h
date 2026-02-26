#pragma once
/**********************************************************************/
#define SORT_TYPE_STRING 0
#define SORT_TYPE_INT 1
#define SORT_TYPE_INT64 2
#define SORT_TYPE_DATE 3
/**********************************************************************/

struct CEntityTreeLeaf
{
public:
	CEntityTreeLeaf();

public:
	int m_nNextBranchIdx;
	int m_nEntityIdx;
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

class CEntityTreeBranch
{
private:
	CEntityTreeBranch(int nSortType);
	~CEntityTreeBranch();

	void Clear();
	bool FindLeafString(CString& strLabelToFind, int& nLeafIdx);
	bool FindLeafInt(int nLabelToFind, int& nLeafIdx);
	bool FindLeafInt64(__int64 nLabelToFind, int& nLeafIdx);
	bool FindLeafDate(COleDateTime oleLabelToFind, int& nLeafIdx);

	int ConsolidateLeaf(bool bNew, bool bEndBranch, int nLeafIdx, int nNextBranchIdx, CArray<int, int>& arrayEntityIdx, int& nNextEntityIdx);
	int ConsolidateString(CString& string, bool bEndBranch, int nNextBranchIdx, CArray<int, int>& arrayEntityIdx, int& nNextEntityIdx, __int64& nNodeCount);
	int ConsolidateInt(int n, bool bEndBranch, int nNextBranchIdx, CArray<int, int>& arrayEntityIdx, int& nNextEntityIdx, __int64& nNodeCount);
	int ConsolidateInt64(__int64 n, bool bEndBranch, int nNextBranchIdx, CArray<int, int>& arrayEntityIdx, int& nNextEntityIdx, __int64& nNodeCount);
	int ConsolidateDate(COleDateTime date, bool bEndBranch, int nNextBranchIdx, CArray<int, int>& arrayEntityIdx, int& nNextEntityIdx, __int64& nNodeCount);

private:
	CArray<CString, CString>* GetStringKeys() { return (CArray<CString, CString>*) m_pArrayKeys; }
	CArray<int, int>* GetIntKeys() { return (CArray<int, int>*) m_pArrayKeys; }
	CArray<__int64, __int64>* GetInt64Keys() { return (CArray<__int64, __int64>*) m_pArrayKeys; }
	CArray<COleDateTime, COleDateTime>* GetDateKeys() { return (CArray<COleDateTime, COleDateTime>*) m_pArrayKeys; }

private:
	int m_nSortType;

private:
	void* m_pArrayKeys;
	CArray<CEntityTreeLeaf, CEntityTreeLeaf> m_arrayLeaves;

	friend class CEntityTree;
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

struct CEntityTreeFieldBuffer
{
public:
	CEntityTreeFieldBuffer();

public:
	int m_nFieldType;
	CString m_strBufferString;
	int m_nBufferInt;
	__int64 m_nBufferInt64;
	COleDateTime m_oleBufferDate;
};

/**********************************************************************/

class CEntityTree : public CPtrArray
{
public:
	CEntityTree();
	~CEntityTree();

public:
	void DestroyTree();
	
public:
	void ClearConsolidatedLevels();
	void ClearFieldStructure();
	void AddField( int nType );
	
public:
	void SetBufferString( int nPos, const CString& string );
	void SetBufferInt( int nPos, int n );
	void SetBufferInt64( int nPos, __int64 n );
	void SetBufferDate( int nPos, COleDateTime date );
	
public:
	__int64 GetNodeCount() { return m_nNodeCount; }
	int GetBranchCount(){ return GetSize(); }
	int GetBranchSize( int nBranchIdx );
	
	const char* GetLabelString( int nBranchIdx, int nLeafIdx );
	int GetLabelInt( int nBranchIdx, int nLeafIdx );
	__int64 GetLabelInt64( int nBranchIdx, int nLeafIdx );
	COleDateTime GetLabelDate( int nBranchIdx, int nLeafIdx );

	int GetNextBranchIdx( int nBranchIdx, int nLeafIdx );
	int GetEntityIdx( int nBranchIdx, int nLeafIdx );

protected:
	void Consolidate( CArray<int,int>& m_arrayEntityIdx, int& nNextEntityIdx );
	
protected:
	CEntityTreeBranch* GetBranch( int nBranchIdx ) { return (CEntityTreeBranch*) GetAt(nBranchIdx); }

protected:
	CArray<CEntityTreeFieldBuffer,CEntityTreeFieldBuffer> m_FieldBuffer;
	
protected:
	__int64 m_nNodeCount;
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

class CEntityTreeExportPaymentNode
{
public:
	CEntityTreeExportPaymentNode();

public:
	CEntityTreeExportPaymentNode& operator+ ( CEntityTreeExportPaymentNode& source );

public:
	int GetPaymentQty(){ return m_nPaymentQty; }
	__int64 GetInternalTranNo(){ return m_nInternalTranNo; }
	int GetCoversQty(){ return m_nCoversQty; }
	int GetTender(){ return m_nTender; }
	int GetAmount(){ return m_nAmount; }
	int GetGratuity(){ return m_nGratuity; }
	int GetCashback(){ return m_nCashback; }
	int GetSurplus(){ return m_nSurplus; }

public:
	void SetPaymentQty( int n ){ m_nPaymentQty = n; }
	void SetInternalTranNo( __int64 n ){ m_nInternalTranNo = n; }
	void SetCoversQty( int n ){ m_nCoversQty = n; }
	void SetTender( int n ){ m_nTender = n; }
	void SetAmount( int n ){ m_nAmount = n; }
	void SetGratuity( int n ){ m_nGratuity = n; }
	void SetCashback( int n ){ m_nCashback = n; }
	void SetSurplus( int n ){ m_nSurplus = n; }

public:
	void Clear();

private:
	int m_nPaymentQty;
	__int64 m_nInternalTranNo;
	int m_nCoversQty;
	int m_nTender;
	int m_nAmount;
	int m_nGratuity;
	int m_nCashback;
	int m_nSurplus;
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

class CEntityTreeExportPayment : public CEntityTree
{
public:
	CEntityTreeExportPayment();

public:
	void Consolidate( CEntityTreeExportPaymentNode& NodeToAdd );
	void ClearConsolidatedLevels();

public:
	void GetNode( int nBranchIdx, int nLeafIdx, CEntityTreeExportPaymentNode& Node );
	
private:
	int m_nNextEntityIdx;
	CArray<CEntityTreeExportPaymentNode,CEntityTreeExportPaymentNode> m_arrayNodes;
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

class CEntityTreeExportSalesNode
{
public:
	CEntityTreeExportSalesNode();

public:
	CEntityTreeExportSalesNode& operator+ ( CEntityTreeExportSalesNode& source );

public:
	double GetBaseQty(){ return m_dBaseQty; }
	double GetModQty(){ return m_dModQty; }
	double GetVal(){ return m_dVal; }
	double GetDiscountVal(){ return m_dDiscountVal; }
	double GetPremiumVal() { return m_dPremiumVal; }
	double GetTaxAmount(){ return m_dTaxAmount; }
	double GetCost(){ return m_dCost; }
	double GetWasteBaseQty(){ return m_dWasteBaseQty; }
	double GetWasteModQty(){ return m_dWasteModQty; }
	double GetWasteCost(){ return m_dWasteCost; }

public:
	void SetBaseQty( double d ){ m_dBaseQty = d; }
	void SetModQty( double d ){ m_dModQty = d; }
	void SetVal( double d ){ m_dVal = d; }
	void SetDiscountVal( double d ){ m_dDiscountVal = d; }
	void SetPremiumVal(double d) { m_dPremiumVal = d; }
	void SetTaxAmount( double d ){ m_dTaxAmount = d; }
	void SetCost( double d ){ m_dCost = d; }
	void SetWasteBaseQty( double d ){ m_dWasteBaseQty = d; }
	void SetWasteModQty( double d ){ m_dWasteModQty = d; }
	void SetWasteCost( double d ){ m_dWasteCost = d; }

public:
	void Clear();

private:
	double m_dBaseQty;
	double m_dModQty;
	double m_dVal;
	double m_dDiscountVal;
	double m_dPremiumVal;
	double m_dTaxAmount;
	double m_dCost;
	double m_dWasteBaseQty;
	double m_dWasteModQty;
	double m_dWasteCost;
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

class CEntityTreeExportSales : public CEntityTree
{
public:
	CEntityTreeExportSales();

public:
	void Consolidate( CEntityTreeExportSalesNode& NodeToAdd );
	void ClearConsolidatedLevels();

public:
	void GetNode( int nBranchIdx, int nLeafIdx, CEntityTreeExportSalesNode& Node );
	
private:
	int m_nNextEntityIdx;
	CArray<CEntityTreeExportSalesNode,CEntityTreeExportSalesNode> m_arrayNodes;
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

class CEntityTreeExportS4LabourNode
{
public:
	CEntityTreeExportS4LabourNode();

public:
	CEntityTreeExportS4LabourNode& operator+ ( CEntityTreeExportS4LabourNode& source );

public:
	double GetVal(){ return m_dVal; }
	double GetDiscount(){ return m_dDiscount; }
	double GetTaxAmount(){ return m_dTaxAmount; }
	
public:
	void SetVal( double d ){ m_dVal = d; }
	void SetDiscount( double d ){ m_dDiscount = d; }
	void SetTaxAmount( double d ){ m_dTaxAmount = d; }
	
public:
	void Clear();

private:
	double m_dVal;
	double m_dDiscount;
	double m_dTaxAmount;
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

class CEntityTreeExportS4Labour : public CEntityTree
{
public:
	CEntityTreeExportS4Labour();

public:
	void Consolidate( CEntityTreeExportS4LabourNode& NodeToAdd );
	void ClearConsolidatedLevels();

public:
	void GetNode( int nBranchIdx, int nLeafIdx, CEntityTreeExportS4LabourNode& Node );
	
private:
	int m_nNextEntityIdx;
	CArray<CEntityTreeExportS4LabourNode,CEntityTreeExportS4LabourNode> m_arrayNodes;
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

class CEntityTreeExportVoidNode
{
public:
	CEntityTreeExportVoidNode();

public:
	CEntityTreeExportVoidNode& operator+ ( CEntityTreeExportVoidNode& source );

public:
	int GetLineCount(){ return m_nLineCount; }
	double GetBaseQty(){ return m_dBaseQty; }
	double GetModQty(){ return m_dModQty; }
	double GetVal(){ return m_dVal; }

public:
	void SetLineCount( int n ){ m_nLineCount = n; }
	void SetBaseQty( double d ){ m_dBaseQty = d; }
	void SetModQty( double d ){ m_dModQty = d; }
	void SetVal( double d ){ m_dVal = d; }

public:
	void Clear();

private:
	int m_nLineCount;
	double m_dBaseQty;
	double m_dModQty;
	double m_dVal;
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

class CEntityTreeExportVoid : public CEntityTree
{
public:
	CEntityTreeExportVoid();

public:
	void Consolidate( CEntityTreeExportVoidNode& NodeToAdd );
	void ClearConsolidatedLevels();

public:
	void GetNode( int nBranchIdx, int nLeafIdx, CEntityTreeExportVoidNode& Node );
	
private:
	int m_nNextEntityIdx;
	CArray<CEntityTreeExportVoidNode,CEntityTreeExportVoidNode> m_arrayNodes;
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

class CEntityTreeExportDiscountNode
{
public:
	CEntityTreeExportDiscountNode();

public:
	CEntityTreeExportDiscountNode& operator+ ( CEntityTreeExportDiscountNode& source );

public:
	int GetLineCount(){ return m_nLineCount; }
	double GetDiscountVal(){ return m_dDiscountVal; }
	
public:
	void SetLineCount( int n ){ m_nLineCount = n; }
	void SetDiscountVal( double d ){ m_dDiscountVal = d; }
	
public:
	void Clear();

private:
	int m_nLineCount;
	double m_dDiscountVal;
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

class CEntityTreeExportDiscount : public CEntityTree
{
public:
	CEntityTreeExportDiscount();

public:
	void Consolidate( CEntityTreeExportDiscountNode& NodeToAdd );
	void ClearConsolidatedLevels();

public:
	void GetNode( int nBranchIdx, int nLeafIdx, CEntityTreeExportDiscountNode& Node );
	
private:
	int m_nNextEntityIdx;
	CArray<CEntityTreeExportDiscountNode,CEntityTreeExportDiscountNode> m_arrayNodes;
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

class CEntityTreeGeneralSalesNode
{
public:
	CEntityTreeGeneralSalesNode();

public:
	CEntityTreeGeneralSalesNode& operator+ ( CEntityTreeGeneralSalesNode& source );

public:
	double GetQty(int n);
	double GetVal(int n);

public:
	void SetQty( int n, double dQty );
	void SetVal( int n, double nVal );

public:
	void Clear();

public:
	double m_dQty[8];
	double m_dVal[8];
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

class CEntityTreeGeneralSales : public CEntityTree
{
public:
	CEntityTreeGeneralSales();

public:
	void Consolidate( CEntityTreeGeneralSalesNode& NodeToAdd );
	void ClearConsolidatedLevels();

public:
	void GetGrandTotalNode( CEntityTreeGeneralSalesNode& Node );
	void GetNode( int nBranchIdx, int nLeafIdx, CEntityTreeGeneralSalesNode& Node );
	
private:
	int m_nNextEntityIdx;
	CArray<CEntityTreeGeneralSalesNode,CEntityTreeGeneralSalesNode> m_arrayNodes;
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

class CEntityTreePluSalesHistoryBlock
{
public:
	CEntityTreePluSalesHistoryBlock();

public:
	void Clear();
	CEntityTreePluSalesHistoryBlock& operator+ ( CEntityTreePluSalesHistoryBlock& source );
	
public:
	virtual double GetBaseQty(){ return m_dBaseQty; }
	virtual double GetModQty(){ return m_dModQty; }
	virtual double GetTotalVal(){ return m_dTotalVal; }
	virtual double GetDiscountVal(){ return m_dDiscountVal; }
	virtual double GetPremiumVal() { return m_dPremiumVal; }
	virtual double GetTaxAmount(){ return m_dTaxAmount; }
	virtual double GetCost(){ return m_dCost; }
	virtual double GetWasteCost(){ return m_dWasteCost; }

public:
	void SetBaseQty( double d ){ m_dBaseQty = d; }
	void SetModQty( double d ){ m_dModQty = d; }
	void SetTotalVal( double d ){ m_dTotalVal = d; }
	void SetDiscountVal( double d ){ m_dDiscountVal = d; }
	void SetPremiumVal(double d) { m_dPremiumVal = d; }
	void SetTaxAmount( double d ){ m_dTaxAmount = d; }
	void SetCost( double d ){ m_dCost = d; }
	void SetWasteCost( double d ){ m_dWasteCost = d; }

protected:
	double m_dBaseQty;
	double m_dModQty;
	double m_dTotalVal;
	double m_dDiscountVal;
	double m_dPremiumVal;
	double m_dTaxAmount;
	double m_dCost;
	double m_dWasteCost;
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

class CEntityTreePluSalesHistoryNormalNode
{
public:
	CEntityTreePluSalesHistoryNormalNode();

public:
	virtual void Clear();
	CEntityTreePluSalesHistoryNormalNode& operator+ ( CEntityTreePluSalesHistoryNormalNode& source );

public:
	virtual int GetDailyColCount(){ return 0; }
	virtual void AppendDaily( int nReportMode, CCSV& csv, int& nCol, int nStartOffset = 0, int nEndOffset = 0 );

public:
	CEntityTreePluSalesHistoryBlock m_SalesBlock;	
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

class CEntityTreePluSalesHistoryTwoBlockNode
{
public:
	CEntityTreePluSalesHistoryTwoBlockNode();

public:
	virtual void Clear();
	CEntityTreePluSalesHistoryTwoBlockNode& operator+ ( CEntityTreePluSalesHistoryTwoBlockNode& source );

public:
	CEntityTreePluSalesHistoryBlock m_SalesBlock[2];
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

class CEntityTreePluSalesHistoryWeeklyOneTotalNode : public CEntityTreePluSalesHistoryNormalNode
{
public:
	CEntityTreePluSalesHistoryWeeklyOneTotalNode();

public:
	void Add ( CEntityTreePluSalesHistoryWeeklyOneTotalNode& source, int nHint );

public:
	void SetDailyVal( int n, double d ){ if ( ( n >= 0 ) && ( n < 7 ) ) m_dDailyVal[n] = d; }
	double GetDailyVal( int n ){ if ( ( n >= 0 ) && ( n < 7 ) ) return m_dDailyVal[n]; else return 0.0; }

public:
	void Clear();
	virtual int GetDailyColCount(){ return 7; }
	void AppendDaily( int nReportMode, CCSV& csv, int& nCol, int nStartOFfset = 0, int nEndOffset = 0 );

private:
	double m_dDailyVal[7];
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

class CEntityTreePluSalesHistoryWeeklyTwoTotalNode : public CEntityTreePluSalesHistoryNormalNode
{
public:
	CEntityTreePluSalesHistoryWeeklyTwoTotalNode();

public:
	void Add(CEntityTreePluSalesHistoryWeeklyTwoTotalNode& source, int nHint);

public:
	void SetDailyVal(int n, double d) { if ((n >= 0) && (n < 7)) m_dDailyVal[n] = d; }
	void SetDailyQty(int n, double d) { if ((n >= 0) && (n < 7)) m_dDailyQty[n] = d; }

public:
	double GetDailyVal(int n) { if ((n >= 0) && (n < 7)) return m_dDailyVal[n]; else return 0.0; }
	double GetDailyQty(int n) { if ((n >= 0) && (n < 7)) return m_dDailyQty[n]; else return 0.0; }

public:
	void Clear();
	virtual int GetDailyColCount() { return 14; }
	void AppendDaily(int nReportMode, CCSV& csv, int& nCol, int nStartOFfset = 0, int nEndOffset = 0);

private:
	double m_dDailyVal[7];
	double m_dDailyQty[7];
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

class CEntityTreePluSalesHistoryMonthlyNode : public CEntityTreePluSalesHistoryNormalNode
{
public:
	CEntityTreePluSalesHistoryMonthlyNode();

public:
	void Add ( CEntityTreePluSalesHistoryMonthlyNode& source, int nHint );

public:
	void SetDailyVal( int n, double d ){ if ( ( n >= 0 ) && ( n < 31 ) ) m_dDailyVal[n] = d; }
	double GetDailyVal( int n ){ if ( ( n >= 0 ) && ( n < 31 ) ) return m_dDailyVal[n]; else return 0.0; }

public:
	void Clear();
	virtual int GetDailyColCount(){ return 31; }
	void AppendDaily( int nReportMode, CCSV& csv, int& nCol, int nStartOFfset = 0, int nEndOffset = 0 );

private:
	double m_dDailyVal[31];
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

class CEntityTreePluSalesHistory : public CEntityTree
{
public:
	CEntityTreePluSalesHistory();

public:
	void Consolidate(CEntityTreePluSalesHistoryNormalNode& NodeToAdd);
	void Consolidate(CEntityTreePluSalesHistoryTwoBlockNode& NodeToAdd);
	void Consolidate(CEntityTreePluSalesHistoryWeeklyOneTotalNode& NodeToAdd, int nHint, bool bAllowRecursion = TRUE);
	void Consolidate(CEntityTreePluSalesHistoryWeeklyTwoTotalNode& NodeToAdd, int nHint, bool bAllowRecursion = TRUE);
	void Consolidate(CEntityTreePluSalesHistoryMonthlyNode& NodeToAdd, int nHint);
	void ClearConsolidatedLevels();

public:
	void GetGrandTotalNode(CEntityTreePluSalesHistoryNormalNode& Node);
	void GetGrandTotalNode(CEntityTreePluSalesHistoryTwoBlockNode& Node);
	void GetGrandTotalNode(CEntityTreePluSalesHistoryWeeklyOneTotalNode& Node);
	void GetGrandTotalNode(CEntityTreePluSalesHistoryWeeklyTwoTotalNode& Node);
	void GetGrandTotalNode(CEntityTreePluSalesHistoryMonthlyNode& Node);

public:
	void GetNode(int nBranchIdx, int nLeafIdx, CEntityTreePluSalesHistoryNormalNode& Node);
	void GetNode(int nBranchIdx, int nLeafIdx, CEntityTreePluSalesHistoryTwoBlockNode& Node);
	void GetNode(int nBranchIdx, int nLeafIdx, CEntityTreePluSalesHistoryWeeklyOneTotalNode& Node);
	void GetNode(int nBranchIdx, int nLeafIdx, CEntityTreePluSalesHistoryWeeklyTwoTotalNode& Node);
	void GetNode(int nBranchIdx, int nLeafIdx, CEntityTreePluSalesHistoryMonthlyNode& Node);

public:
	void ClearAutoFillDates() { m_bAutoFillDates = FALSE; }
	void SetAutoFillDates(COleDateTime& dateFrom, COleDateTime& dateTo, int nStartOffset, int nEndOffset);
	int GetWeeklyStartOffset(COleDateTime& oleWeekStart);
	int GetWeeklyEndOffset(COleDateTime& oleWeekStart);

private:
	int m_nNextEntityIdxNormal;
	int m_nNextEntityIdxTwoBlock;
	int m_nNextEntityIdxWeeklyOneTotal;
	int m_nNextEntityIdxWeeklyTwoTotal;
	int m_nNextEntityIdxMonthly;
	CArray<CEntityTreePluSalesHistoryNormalNode, CEntityTreePluSalesHistoryNormalNode> m_arrayNodesNormal;
	CArray<CEntityTreePluSalesHistoryTwoBlockNode, CEntityTreePluSalesHistoryTwoBlockNode> m_arrayNodesTwoBlock;
	CArray<CEntityTreePluSalesHistoryWeeklyOneTotalNode, CEntityTreePluSalesHistoryWeeklyOneTotalNode> m_arrayNodesWeeklyOneTotal;
	CArray<CEntityTreePluSalesHistoryWeeklyTwoTotalNode, CEntityTreePluSalesHistoryWeeklyTwoTotalNode> m_arrayNodesWeeklyTwoTotal;
	CArray<CEntityTreePluSalesHistoryMonthlyNode, CEntityTreePluSalesHistoryMonthlyNode> m_arrayNodesMonthly;

private:
	bool m_bAutoFillDates;
	COleDateTime m_autoFillDateFrom;
	COleDateTime m_autoFillDateTo;
	int m_nAutoFillStartOffset;
	int m_nAutoFillEndOffset;
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

class CEntityTreeStockSalesHistoryNormalNode
{
public:
	CEntityTreeStockSalesHistoryNormalNode();

public:
	CEntityTreeStockSalesHistoryNormalNode& operator+ ( CEntityTreeStockSalesHistoryNormalNode& source );

public:
	double GetQty(){ return m_dQty; }
	double GetTotalVal(){ return m_dTotalVal; }
	double GetTaxAmount(){ return m_dTaxAmount; }
	double GetCost(){ return m_dCost; }
	double GetWasteCost(){ return m_dWasteCost; }

public:
	void SetQty( double d ){ m_dQty = d; }
	void SetTotalVal( double d ){ m_dTotalVal = d; }
	void SetTaxAmount( double d ){ m_dTaxAmount = d; }
	void SetCost( double d ){ m_dCost = d; }
	void SetWasteCost( double d ){ m_dWasteCost = d; }

public:
	virtual void Clear();
	virtual void AppendDaily( int nReportMode, int nSubUnits, CCSV& csv, int nStartOffset = 0, int nEndOffset = 0 );

protected:
	double m_dQty;
	double m_dTotalVal;
	double m_dTaxAmount;
	double m_dCost;
	double m_dWasteCost;
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

class CEntityTreeStockSalesHistoryWeeklyNode : public CEntityTreeStockSalesHistoryNormalNode
{
public:
	CEntityTreeStockSalesHistoryWeeklyNode();

public:
	void Add ( CEntityTreeStockSalesHistoryWeeklyNode& source, int nHint );

public:
	void SetDailyVal( int n, double d ){ if ( ( n >= 0 ) && ( n < 7 ) ) m_dDailyVal[n] = d; }
	double GetDailyVal( int n ){ if ( ( n >= 0 ) && ( n < 7 ) ) return m_dDailyVal[n]; else return 0.0; }

public:
	void Clear();
	void AppendDaily( int nReportMode, int nSubUnits, CCSV& csv, int nStartOffset, int nEndOffset );

private:
	double m_dDailyVal[7];
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

class CEntityTreeStockSalesHistoryMonthlyNode : public CEntityTreeStockSalesHistoryNormalNode
{
public:
	CEntityTreeStockSalesHistoryMonthlyNode();

public:
	void Add ( CEntityTreeStockSalesHistoryMonthlyNode& source, int nHint );

public:
	void SetDailyVal( int n, double d ){ if ( ( n >= 0 ) && ( n < 31 ) ) m_dDailyVal[n] = d; }
	double GetDailyVal( int n ){ if ( ( n >= 0 ) && ( n < 31 ) ) return m_dDailyVal[n]; else return 0.0; }

public:
	void Clear();
	void AppendDaily( int nReportMode, int nSubUnits, CCSV& csv, int nStartOffset, int nEndOffset );

private:
	double m_dDailyVal[31];
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

class CEntityTreeStockSalesHistory : public CEntityTree
{
public:
	CEntityTreeStockSalesHistory();

public:
	void Consolidate( CEntityTreeStockSalesHistoryNormalNode& NodeToAdd );
	void Consolidate( CEntityTreeStockSalesHistoryWeeklyNode& NodeToAdd, int nHint, bool bAllowRecursion = TRUE );
	void Consolidate( CEntityTreeStockSalesHistoryMonthlyNode& NodeToAdd, int nHint );
	void ClearConsolidatedLevels();

public:
	void GetGrandTotalNode( CEntityTreeStockSalesHistoryNormalNode& Node );
	void GetGrandTotalNode( CEntityTreeStockSalesHistoryWeeklyNode& Node );
	void GetGrandTotalNode( CEntityTreeStockSalesHistoryMonthlyNode& Node );

public:
	void GetNode( int nBranchIdx, int nLeafIdx, CEntityTreeStockSalesHistoryNormalNode& Node );
	void GetNode( int nBranchIdx, int nLeafIdx, CEntityTreeStockSalesHistoryWeeklyNode& Node );
	void GetNode( int nBranchIdx, int nLeafIdx, CEntityTreeStockSalesHistoryMonthlyNode& Node );

public:
	void ClearAutoFillDates(){ m_bAutoFillDates = FALSE; }
	void SetAutoFillDates( COleDateTime& dateFrom, COleDateTime& dateTo, int nStartOffset, int nEndOffset );
	int GetWeeklyStartOffset( COleDateTime& oleWeekStart );
	int GetWeeklyEndOffset( COleDateTime& oleWeekStart );
	
private:
	int m_nNextEntityIdxNormal;
	int m_nNextEntityIdxWeekly;
	int m_nNextEntityIdxMonthly;
	CArray<CEntityTreeStockSalesHistoryNormalNode,CEntityTreeStockSalesHistoryNormalNode> m_arrayNodesNormal;
	CArray<CEntityTreeStockSalesHistoryWeeklyNode,CEntityTreeStockSalesHistoryWeeklyNode> m_arrayNodesWeekly;
	CArray<CEntityTreeStockSalesHistoryMonthlyNode,CEntityTreeStockSalesHistoryMonthlyNode> m_arrayNodesMonthly;

private:
	bool m_bAutoFillDates;
	COleDateTime m_autoFillDateFrom;
	COleDateTime m_autoFillDateTo;
	int m_nAutoFillStartOffset;
	int m_nAutoFillEndOffset;
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

class CEntityTreePluStockNode
{
public:
	CEntityTreePluStockNode();

public:
	CEntityTreePluStockNode& operator+ ( CEntityTreePluStockNode& source );

public:
	double GetApparentQty(){ return m_dApparentQty; }
	double GetApparentCost(){ return m_dApparentCost; }
	double GetApparentRetail(){ return m_dApparentRetail; }
	double GetDeliveryQty(){ return m_dDeliveryQty; }
	double GetDeliveryVal(){ return m_dDeliveryVal; }
	
public:
	void SetApparentQty( double d ){ m_dApparentQty = d; }
	void SetApparentCost( double d ){ m_dApparentCost = d; }
	void SetApparentRetail( double d ){ m_dApparentRetail = d; }
	void SetDeliveryQty( double d ){ m_dDeliveryQty = d; }
	void SetDeliveryVal( double d ){ m_dDeliveryVal = d; }
	
public:
	void Clear();

private:
	double m_dApparentQty;
	double m_dApparentCost;
	double m_dApparentRetail;
	double m_dDeliveryQty;
	double m_dDeliveryVal;
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

class CEntityTreePluStock : public CEntityTree
{
public:
	CEntityTreePluStock();

public:
	void Consolidate( CEntityTreePluStockNode& NodeToAdd );
	void ClearConsolidatedLevels();

public:
	void GetGrandTotalNode( CEntityTreePluStockNode& Node );
	void GetNode( int nBranchIdx, int nLeafIdx, CEntityTreePluStockNode& Node );
	
private:
	int m_nNextEntityIdx;
	CArray<CEntityTreePluStockNode,CEntityTreePluStockNode> m_arrayNodes;
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/


