//******************************************************************************
#pragma once
//******************************************************************************

class CBackgroundOptions  
{
public:
	CBackgroundOptions();
	void Read();
	void Save();
	bool IsSet();
	bool IsEODDay(int nDay);
	void SetEODDay(int nDay, bool bSet);

public:
	int m_nStandardImportMinutes;
	int m_nNewRecordsMinutes;
	int m_nWebPaymentMinutes;
	bool m_bWebPaymentSuspendOnError;
	CString m_strStandardStart;
	CString m_strStandardEnd;
	CString m_strNewRecordStart;
	CString m_strNewRecordEnd;

	bool m_bEODExport;
	
	CString m_strDateNextEODCheck;
	CString m_strTimeNextEODCheck;

	bool m_bEmailLowBalance;
	CString m_strDateNextLowBalanceCheck;
	CString m_strTimeNextLowBalanceCheck;

	bool m_bEmailPurchases;
	CString m_strDateNextEmailPurchase;
	CString m_strTimeEmailPurchase;

	CString m_strExportSalesStart;
	CString m_strExportSalesEnd;
	int m_nExportSalesMinutes;

	bool m_bEmailPointsAchievement;
	CString m_strDateNextPointsAchievement;					// next date for points achievement
	CString m_strTimeNextPointsAchievement;

	bool m_bEmailWeeklyPoints;
	CString m_strDateNextWeeklyPoints;						// next date for weekly points update
	CString m_strTimeNextWeeklyPoints;

private:
	CString m_strFilename;
	int m_nEODDayFlags;
};

//******************************************************************************
