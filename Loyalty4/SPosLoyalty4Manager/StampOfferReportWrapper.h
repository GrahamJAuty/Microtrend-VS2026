#pragma once
//$$******************************************************************

class CStampOfferReportWrapper
{
public:
	CStampOfferReportWrapper( CString strLabel, __int64 nUserID, CWnd* pParent);

public:
	void ShowReport();

private:
	CString m_strLabel;
	__int64 m_nUserID;
	CWnd* m_pParent;
};

//$$******************************************************************

