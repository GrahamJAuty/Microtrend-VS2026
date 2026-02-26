//$$******************************************************************
#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

struct CAccumulatorPlu
{
public:
	CAccumulatorPlu() 
	{ 
		m_nPluNo = 0;
		m_bDefaultSelect = FALSE;
	}

public:
	__int64 m_nPluNo;
	bool m_bDefaultSelect;
};

//$$******************************************************************

class CAccumulatorPropertyPage2 : public CSSPropertyPage
{
public:
	CAccumulatorPropertyPage2();
	virtual ~CAccumulatorPropertyPage2();

public:
	void SetReportInfo(CReportInfoAccumulator* pInfo);
	void SetPointer ( const char* szLabel );

	enum { IDD = IDD_PROPPAGE_ACCUMULATOR2 };
	CButton m_checkNewPage;
	CButton m_checkUnknown;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnButtonAll();
	afx_msg void OnButtonNone();
	afx_msg void OnButtonReset();
	DECLARE_MESSAGE_MAP()

private:
	void FillPluList(int nDbNo);

public:
	CArray<CAccumulatorPlu, CAccumulatorPlu> m_arrayPluNo;

private:
	CSSListMultiSelectCtrl m_listboxPlu;
	CString m_strReportLabel;
	int m_nColumnCount;

private:
	CReportInfoAccumulator* m_pReportInfo;
	int m_nLastDbNo;
};

//$$******************************************************************

