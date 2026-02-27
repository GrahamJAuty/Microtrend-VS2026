#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorAccount : public CSQLFieldValidatorBase 
{
public:
	static void SetLeaverYear(int n, int& nTarget);
	static void SetUserID(__int64 n, __int64& nTarget);
	static void SetUsername(CString str, CString& strTarget);
	static void SetForename(CString str, CString& strTarget);
	static void SetSurname(CString str, CString& strTarget);
	static void SetDOB(CString str, CString& strTarget);
	static void SetExpiryDate(CString str, CString& strTarget);
	static void SetGender(CString str, CString& strTarget);
	static void SetGroupNo(int n, int& nTarget);
	static void SetDayGroups(CString str, CString& strTarget);
	static void SetGroupShiftStatus(int n, int& nTarget);
	static void SetGroupShiftStartDate(CString str, CString& strTarget);
	static void SetGroupShiftEndDate(CString str, CString& strTarget);
	static void SetGroupShiftFutureGroup(int n, int& nTarget);
	static void SetEmail(CString str, CString& strTarget);
	static void SetInfo1(CString str, CString& strTarget);
	static void SetInfo2(CString str, CString& strTarget);
	static void SetInfo3(CString str, CString& strTarget);
	static void SetInfo4(CString str, CString& strTarget);
	static void SetInfo5(CString str, CString& strTarget);
	static void SetInfo6(CString str, CString& strTarget);
	static void SetInfo7(CString str, CString& strTarget);
	static void SetInfo8(CString str, CString& strTarget);
	static void SetAllergyCodes(int n, int& nTarget);
	static void SetDietary(CString str, CString& strTarget);
	/*****/
	static void SetMemberID(CString str, CString& strTarget);
	static void SetExternalRef1(CString str, CString& strTarget);
	static void SetExternalRef2(CString str, CString& strTarget);
	static void SetPreviousUserID(__int64 n, __int64& nTarget);
	static void SetNextUserID(__int64 n, __int64& nTarget);
	static void SetLegacyPebblePurseID(CString str, CString& strTarget);
	/*****/
	static void SetPurse1Allow(int n, int& nTarget);
	static void SetPurse1Liability(double d, double& dTarget);
	static void SetPurse1Credit(double d, double& dTarget);
	static void SetPurse1LastSpendDate(CString str, CString& strTarget);
	static void SetPurse1LastSpendTime(CString str, CString& strTarget);
	static void SetPurse1LastSpend(double d, double& dTarget);
	static void SetPurse1SpendToDate(double d, double& dTarget);
	/*****/
	static void SetPurse2Allow(int n, int& nTarget);
	static void SetPurse2Balance(double d, double& dTarget);
	static void SetPurse2LastSpendDate(CString str, CString& strTarget);
	static void SetPurse2LastSpendTime(CString str, CString& strTarget);
	static void SetPurse2LastSpend(double d, double& dTarget);
	static void SetPurse2SpendToDate(double d, double& dTarget);
	static void SetPurse2RefreshFlags(int n, int& nTarget);
	static void SetPurse2RefreshedDate(CString str, CString& strTarget);
	static void SetPurse2RefreshedTime(CString str, CString& strTarget);
	/*****/
	static void SetPurse3Allow(int n, int& nTarget);
	static void SetPurse3Liability(double d, double& dTarget);
	static void SetPurse3Credit(double d, double& dTarget);
	static void SetPurse3LastSpendDate(CString str, CString& strTarget);
	static void SetPurse3LastSpendTime(CString str, CString& strTarget);
	static void SetPurse3LastSpend(double d, double& dTarget);
	static void SetPurse3SpendToDate(double d, double& dTarget);
	/*****/
	static void SetCashSpendToDate(double d, double& dTarget);
	/*****/
	static void SetPoints(int n, int& nTarget);
	static void SetPointsToDate(int n, int& nTarget);
	/*****/
	static void SetMaxSpend(double d, double& dTarget);
	/*****/
	static void SetMaxOverdraft(double d, double& dTarget);
	/*****/
	static void SetAlertCode(int n, int& nTarget);
	static void SetAlertDate(CString str, CString& strTarget);
	static void SetAlertTime(CString str, CString& strTarget);
};

//**********************************************************************

