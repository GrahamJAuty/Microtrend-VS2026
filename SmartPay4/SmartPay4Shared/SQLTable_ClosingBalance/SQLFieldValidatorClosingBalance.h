#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorClosingBalance : public CSQLFieldValidatorBase 
{
public:
	static void SetKeyDate(CString str, CString& strTarget);
	/*****/
	static void SetDate(CString str, CString& strTarget);
	static void SetTime(CString str, CString& strTarget);
	/*****/
	static void SetArchiveDate(CString str, CString& strTarget);
	static void SetArchiveTime(CString str, CString& strTarget);
	/*****/
	static void SetPurse1LiabilityAudit(double d, double& dTarget);
	static void SetPurse1CreditAudit(double d, double& dTarget);
	/*****/
	static void SetPurse2BalanceAudit(double d, double& dTarget);
	/*****/
	static void SetPurse3LiabilityAudit(double d, double& dTarget);
	static void SetPurse3CreditAudit(double d, double& dTarget);
	/*****/
	static void SetPurse1LiabilityAdjust(double d, double& dTarget);
	static void SetPurse1CreditAdjust(double d, double& dTarget);
	/*****/
	static void SetPurse3LiabilityAdjust(double d, double& dTarget);
	static void SetPurse3CreditAdjust(double d, double& dTarget);
};

//**********************************************************************

