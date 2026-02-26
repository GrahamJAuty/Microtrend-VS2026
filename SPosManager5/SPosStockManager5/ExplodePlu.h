#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#define PLU_EXCEPTION_NONE 0
#define PLU_EXCEPTION_PLUNOTEXIST 1
#define PLU_EXCEPTION_PLUNOTLINKED 2
#define PLU_EXCEPTION_STOCKNOTEXIST 3
#define PLU_EXCEPTION_PLUISRECIPE 4
#define PLU_EXCEPTION_STOCKTAKE 5
#define PLU_EXCEPTION_PLUNONSTOCK 6
#define PLU_EXCEPTION_STOCKINACTIVE 7
/**********************************************************************/

class CExplodePluBase
{
public:
	CExplodePluBase();
	void SetPriceLevel( int n ) { m_nPriceLevel = n; }

public:
	int ExplodePlu ( __int64 nPluNo, CStringArray& StockDetails, int nReportType, double dSaleQty, double dSaleValue );
	int GetRedirectionSpNo( __int64 nPluNo );

protected:
	virtual int GetSpNo1() = 0;
	virtual int GetSpNo2() = 0;
	virtual int GetStkTakeNo() = 0;

protected:
	void CheckStockCode( CString& strStockCode, int& nStockIdx, CStockCSVRecord& StockRecord, int& nExceptionThisLine, int& nExceptionTextHint );
	const char* GetStockExceptionText( int nExceptionThisLine, int nExceptionTextHint );

protected:
	int m_nPriceLevel;

protected:
	CString m_strStockExceptionText;
};

/**********************************************************************/

class CExplodePluDatabase : public CExplodePluBase
{
public:
	CExplodePluDatabase();

protected:
	int GetSpNo1(){ return 0; }
	int GetSpNo2(){ return 0; }
	int GetStkTakeNo(){ return -1; }
};

/**********************************************************************/

class CExplodePluStockLevel : public CExplodePluBase
{
public:
	CExplodePluStockLevel( int nSpNo1, int nSpNo2 = 0 );

protected:
	int GetSpNo1(){ return m_nSpNo1; }
	int GetSpNo2(){ return m_nSpNo2; }
	int GetStkTakeNo(){ return -1; }

private:
	int m_nSpNo1;
	int m_nSpNo2;
};

/**********************************************************************/

class CExplodePluStockTake : public CExplodePluBase
{
public:
	CExplodePluStockTake( int nSpNo1, int nStkTakeNo );

protected:
	int GetSpNo1(){ return m_nSpNo1; }
	int GetSpNo2(){ return 0; }
	int GetStkTakeNo(){ return m_nStkTakeNo; }

private:
	int m_nSpNo1;
	int m_nStkTakeNo;
};

/**********************************************************************/
#endif			//__EXPLODEPLU_H__
/**********************************************************************/

