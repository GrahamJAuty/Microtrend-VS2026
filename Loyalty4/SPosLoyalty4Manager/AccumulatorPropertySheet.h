#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "AccumulatorPropertyPage1.h"
#include "AccumulatorPropertyPage2.h"
#include "ReportInfoAccumulator.h"
//$$******************************************************************
#define nACCUMULATE_LOWPOINTS		0				// low points usage
#define nACCUMULATE_LOWSPEND		1				// low spend
#define nACCUMULATE_RANKPOINTS		2				// high ranked by points
#define nACCUMULATE_RANKSPEND		3				// high ranked by spend
#define nACCUMULATE_ACTIVE			4				// used cards in period
#define nACCUMULATE_INACTIVE		5				// cards not used in period
#define nACCUMULATE_RANKPURCHASES	6				// rank purchasess
//$$******************************************************************
static const char* szDISCOUNT_PLUNO		= "..";
static const char* szDISCOUNT_ECRTEXT	= "Discount";
//$$******************************************************************

class CAccumulatorPropertySheet : public CSSPropertySheet
{
public:
	CAccumulatorPropertySheet( const char* szLabel, bool bPluPage, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CAccumulatorPropertySheet();

	void SetReportInfo( CReportInfoAccumulator* pReportInfo );

public:
	CAccumulatorPropertyPage1 m_Page1;
	CAccumulatorPropertyPage2 m_Page2;

public:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	CString m_strReportLabel;
	CReportInfoAccumulator* m_pReportInfo;
};

//$$******************************************************************
