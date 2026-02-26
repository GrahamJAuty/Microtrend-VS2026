#pragma once
/**********************************************************************/
#define MAX_SCREEN_NO 999999999
/**********************************************************************/

class CSPOSKeyboardScreenRecord  
{
public:
	CSPOSKeyboardScreenRecord();
	void Reset();
	
public:
	int GetScreenNo()		{ return m_nScreenNo; }
	const char* GetName()	{ return m_strName; }
	//TRADING SCREEN
	int GetTradingScreenStyle()				{ return m_nTradingScreenStyle; }
	__int64 GetTradingListTopLevel()		{ return m_nTradingListTopLevel; }
	__int64 GetTradingListFunction()		{ return m_nTradingListFunction; }
	//LOGGED OFF SCREEN
	int GetLoggedOffScreenStyle()			{ return m_nLoggedOffScreenStyle; }
	__int64 GetLoggedOffListFunction()		{ return m_nLoggedOffListFunction; }
	__int64 GetLoggedOffListFunctionLarge()	{ return m_nLoggedOffListFunctionLarge; }
	//PAYMENT SCREEN
	__int64 GetPaymentListTopLevel()		{ return m_nPaymentListTopLevel; }
	__int64 GetPaymentListFunction()		{ return m_nPaymentListFunction; }

	__int64 GetListFastBar()				{ return m_nListFastBar; }
	const char* GetComboText();


	void SetScreenNo( int n );
	void SetName( const char* szName );
	//TRADING SCREEN
	void SetTradingScreenStyle( int n );
	void SetTradingListTopLevel( __int64 n );
	void SetTradingListFunction( __int64 n );
	//LOGGED OFF SCREEN
	void SetLoggedOffScreenStyle( int n );
	void SetLoggedOffListFunction( __int64 n );
	void SetLoggedOffListFunctionLarge( __int64 n );
	//PAYMENT SCREEN
	void SetPaymentListTopLevel( __int64 n );
	void SetPaymentListFunction( __int64 n );

	void SetListFastBar( __int64 n );

public:
	int GetTradingListTypeTopLevel();
	int GetTradingListTypeFunction();
	int GetLoggedOffListTypeFunction();
	
private:
	int m_nScreenNo;
	CString m_strName;
	//TRADING SCREEN
	int m_nTradingScreenStyle;
	__int64 m_nTradingListTopLevel;
	__int64 m_nTradingListFunction;
	//LOGGED OFF SCREEN
	int m_nLoggedOffScreenStyle;
	__int64 m_nLoggedOffListFunction;
	__int64 m_nLoggedOffListFunctionLarge;
	//PAYMENT SCREEN
	__int64 m_nPaymentListTopLevel;
	__int64 m_nPaymentListFunction;

	__int64 m_nListFastBar;
	
	CString m_strComboText;
};

/**********************************************************************/
