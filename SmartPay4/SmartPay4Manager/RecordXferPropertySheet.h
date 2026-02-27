#pragma once
//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
//$$******************************************************************
#include "RecordXferData.h"
#include "RecordXferPropertyPage1.h"
#include "RecordXferPropertyPage2.h"
//$$******************************************************************

class CRecordXferPropertySheet : public CSSAutoShutdownPropertySheet
{
public:
	CRecordXferPropertySheet(const char* szFromCardNo, const char* szToCardNo, bool bHotlistReplace, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CRecordXferPropertySheet();

public:
	void GetRecordXferData(CRecordXferData& data) { data = m_RecordXferData; }

private:
	CRecordXferPropertyPage1 m_Page1;
	CRecordXferPropertyPage2 m_Page2;

protected:
	DECLARE_MESSAGE_MAP()
	
private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	CSQLRowAccountFull m_RowAccountFrom;
	CRecordXferData m_RecordXferData;
	CString m_strFromCardNo;
	CString m_strToCardNo;
	bool m_bHotlistReplace;
};

//$$******************************************************************


