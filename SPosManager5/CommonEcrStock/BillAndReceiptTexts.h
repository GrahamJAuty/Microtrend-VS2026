#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "EnhancedIniFile.h"
#include "LocationSelectorEntity.h"
/**********************************************************************/

class CBillAndReceiptTexts 
{
public:
	CBillAndReceiptTexts();
	bool SetLocSelEntity( CLocationSelectorEntity& LocSelEntity, bool bAllowFallback );
	void SetFilename( const char* sz ){ m_strDataFilename = sz; }
	void Reset();
	
	bool Read();
	bool Write();
	void DeleteFile();

	bool GetFatalReadErrorFlag() { return m_bFatalReadError; }
	void ClearFatalReadErrorFlag() { m_bFatalReadError = FALSE; }

	void CopyFrom( CBillAndReceiptTexts& source );	
	void SetDefaults();

	bool GotData();

	const char* GetLockFilename(){ return m_strLockFilename; }
	const char* GetParentFilename(){ return m_strParentFilename; }
	int GetParentConType(){ return m_nParentConType; }
	bool GetDataFileExistsFlag(){ return m_bDataFileExists; }
	
private:
	bool ReadInternal();
	void ReloadFromIniFile( CEnhancedIniFile& iniFile );
	void PrepareIniFile( CEnhancedIniFile& iniFile );
	
public:
	const char* GetBillText1()					{ return m_BillText1.GetValue(); }
	const char* GetBillText2()					{ return m_BillText2.GetValue(); }
	const char* GetBillText3()					{ return m_BillText3.GetValue(); }
	const char* GetBillText4()					{ return m_BillText4.GetValue(); }
	const char* GetBillText5()					{ return m_BillText5.GetValue(); }
	const char* GetBillText6()					{ return m_BillText6.GetValue(); }

public:
	const char* GetReceiptText1()				{ return m_ReceiptText1.GetValue(); }
	const char* GetReceiptText2()				{ return m_ReceiptText2.GetValue(); }
	const char* GetReceiptText3()				{ return m_ReceiptText3.GetValue(); }
	const char* GetReceiptText4()				{ return m_ReceiptText4.GetValue(); }
	const char* GetReceiptText5()				{ return m_ReceiptText5.GetValue(); }
	const char* GetReceiptText6()				{ return m_ReceiptText6.GetValue(); }

public:
	void SetBillText1( const char* sz )			{ m_BillText1.SetValue(sz); }
	void SetBillText2( const char* sz )			{ m_BillText2.SetValue(sz); }
	void SetBillText3( const char* sz )			{ m_BillText3.SetValue(sz); }
	void SetBillText4( const char* sz )			{ m_BillText4.SetValue(sz); }
	void SetBillText5( const char* sz )			{ m_BillText5.SetValue(sz); }
	void SetBillText6( const char* sz )			{ m_BillText6.SetValue(sz); }

public:
	void SetReceiptText1( const char* sz )			{ m_ReceiptText1.SetValue(sz); }
	void SetReceiptText2( const char* sz )			{ m_ReceiptText2.SetValue(sz); }
	void SetReceiptText3( const char* sz )			{ m_ReceiptText3.SetValue(sz); }
	void SetReceiptText4( const char* sz )			{ m_ReceiptText4.SetValue(sz); }
	void SetReceiptText5( const char* sz )			{ m_ReceiptText5.SetValue(sz); }
	void SetReceiptText6( const char* sz )			{ m_ReceiptText6.SetValue(sz); }

private:
	CIniFileString m_BillText1;
	CIniFileString m_BillText2;
	CIniFileString m_BillText3;
	CIniFileString m_BillText4;
	CIniFileString m_BillText5;
	CIniFileString m_BillText6;

private:
	CIniFileString m_ReceiptText1;
	CIniFileString m_ReceiptText2;
	CIniFileString m_ReceiptText3;
	CIniFileString m_ReceiptText4;
	CIniFileString m_ReceiptText5;
	CIniFileString m_ReceiptText6;
	
private:
	CString m_strLockFilename;
	CString m_strDataFilename;
	CString m_strParentFilename;
	int m_nParentConType;
	bool m_bDataFileExists;

private:
	bool m_bFatalReadError;
};

/**********************************************************************/
#endif
/**********************************************************************/
