#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

class CProcessedOrderDetails  
{
public:
	CProcessedOrderDetails( bool bEmailMode );
	void ReadDeliveryInstructions();
	void WriteDeliveryInstructions();

public:
	CString m_strTopName;
	CString m_strTopAddress1;
	CString m_strTopAddress2;
	CString m_strTopAddress3;
	CString m_strTopPostcode;
	CString m_strTopPhone;
	CString m_strTopFax;
	CString m_strTopEmail;
	CString m_strInstructions1;
	CString m_strInstructions2;
	CString m_strInstructions3;
	CString m_strInstructions4;
	CString m_strInstructions5;
	CString m_strAddress1;
	CString m_strAddress2;
	CString m_strAddress3;
	CString m_strPostcode;
	CString m_strPhone;
	CString m_strFax;
	CString m_strEmail;
	CString m_strName;
	bool m_bDeliverAsOrdered;
	bool m_bEmailMode;
};

/**********************************************************************/
#endif
/**********************************************************************/
