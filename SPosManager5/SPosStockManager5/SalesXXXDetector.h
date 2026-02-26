#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#define SALESXXX_DELAY_FOREVER 0
#define SALESXXX_DELAY_10MIN 1
#define SALESXXX_DELAY_20MIN 2
#define SALESXXX_DELAY_30MIN 3
#define SALESXXX_DELAY_1HOUR 4
#define SALESXXX_DELAY_2HOUR 5
#define SALESXXX_DELAY_3HOUR 6
/**********************************************************************/

class CSalesXXXDetector  
{
public:
	CSalesXXXDetector();

public:
	void LookForSalesXXX();
	
	void SetReadyToGoFlag( bool b ){ m_bReadyToGo = b; }

	int GetSalesXXXSpIdx(){ return m_nSalesXXXSpIdx; }
	bool CheckSpSalesXXX();

	int GetDelayType(){ return m_nDelayType; }
	void SetDelayType( int n );
	void StartDelay();

private:
	void CheckSpIdx();
	void FindNextSp();
	
private:
	int m_nSpIdx;
	int m_nSalesXXXSpIdx;
	int m_nDelayType;

private:
	bool m_bReadyToGo;	
	COleDateTime m_oleTimeNextCheck;
};

/**********************************************************************/
extern CSalesXXXDetector SalesXXXDetector;
/**********************************************************************/
#endif
/**********************************************************************/
