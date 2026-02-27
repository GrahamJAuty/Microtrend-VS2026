#pragma once
/**********************************************************************/

class CWebPaymentListDatabase : public CSSInt64Database
{
public:
	CWebPaymentListDatabase() {}
	int Open ( const char* szFilename, int nMode );

	void SetPaymentID ( __int64 nID ){ CSSDatabase::Set ( "PaymentID", nID ); }
	__int64 GetPaymentID() { return CSSDatabase::GetInt64 ( "PaymentID" ); }

	void SetServiceID ( __int64 nID ){ CSSDatabase::Set ( "ServiceID", nID ); }
	__int64 GetServiceID() { return CSSDatabase::GetInt64 ( "ServiceID" ); }

	void SetAmount ( double dValue ) { CSSDatabase::Set ( "Amount", dValue, 2 ); }
	double GetAmount() { return CSSDatabase::GetDouble ( "Amount" ); }

	void SetDate ( const char* szDate ){ CSSDatabase::Set ( "Date", szDate ); }
	CString GetDate(){ return CSSDatabase::GetString ( "Date" ); }

	void SetTime ( const char* szTime ){ CSSDatabase::Set ( "Time", szTime ); }
	CString GetTime(){ return CSSDatabase::GetString ( "Time" ); }

	void SetUserID ( __int64 nCardNo ){ CSSDatabase::Set ( "UserID", nCardNo ); }
	void SetUserID ( const char* szCardNo ){ CSSDatabase::Set ( "UserID", szCardNo ); }
	CString GetUserID(){ return CSSDatabase::GetString ( "UserID" ); }

	void SetName ( const char* szName ){ CSSDatabase::Set ( "Name", szName ); }
	CString GetName(){ return CSSDatabase::GetString ( "Name" ); }
};
 