#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorAccount : public CSQLFieldValidatorBase 
{
public:
	static void SetUserID(__int64 n, __int64& nTarget);
	static void SetFullname(CString str, CString& strTarget);
	static void SetForename(CString str, CString& strTarget);
	static void SetSurname(CString str, CString& strTarget);
	static void SetDOB(CString str, CString& strTarget);
	static void SetExpiryDate(CString str, CString& strTarget);
	static void SetGender(CString str, CString& strTarget);
	static void SetGroupNo(int n, int& nTarget);
	static void SetSchemeNo(int n, int& nTarget);
	static void SetInfo(CString str, CString& strTarget);
	static void SetQRInfo(CString str, CString& strTarget);
	static void SetAddress(CString str, CString& strTarget);
	static void SetPhone(CString str, CString& strTarget);
	/*****/
	static void SetExternalRef(CString str, CString& strTarget);
	static void SetPreviousUserID(__int64 n, __int64& nTarget);
	static void SetNextUserID(__int64 n, __int64& nTarget);
	/*****/
	static void SetPurse1Balance(double d, double& dTarget);
	static void SetPurse1LastSpendDate(CString str, CString& strTarget);
	static void SetPurse1LastSpendTime(CString str, CString& strTarget);
	static void SetPurse1LastSpend(double d, double& dTarget);
	static void SetPurse1SpendToDate(double d, double& dTarget);
	/*****/
	static void SetPurse2Balance(double d, double& dTarget);
	static void SetPurse2LastSpendDate(CString str, CString& strTarget);
	static void SetPurse2LastSpendTime(CString str, CString& strTarget);
	static void SetPurse2LastSpend(double d, double& dTarget);
	static void SetPurse2SpendToDate(double d, double& dTarget);
	static void SetPurse2RefreshedDate(CString str, CString& strTarget);
	static void SetPurse2RefreshedTime(CString str, CString& strTarget);
	/*****/
	static void SetCashLastSpendDate(CString str, CString& strTarget);
	static void SetCashLastSpendTime(CString str, CString& strTarget);
	static void SetCashLastSpend(double d, double& dTarget);
	static void SetCashSpendToDate(double d, double& dTarget);
	/*****/
	static void SetPoints(int n, int& nTarget);
	static void SetPointsToDate(int n, int& nTarget);
	/*****/
	static void SetHotlistCode(int n, int& nTarget);
	static void SetHotlistDate(CString str, CString& strTarget);
	static void SetHotlistTime(CString str, CString& strTarget);
	/*****/
	static void SetMaxSpend(double d, double& dTarget);
	static void SetMaxOverdraft(double d, double& dTarget);
	/*****/
	static void SetAlertCodes(CString str, CString& strTarget);
};

//**********************************************************************

