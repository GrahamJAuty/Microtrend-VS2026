#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "SmartEntertainmentWebAPISalesDate.h"
/**********************************************************************/

class CSmartEntertainmentNetworkDateDlg : public CDialog
{
	DECLARE_DYNAMIC(CSmartEntertainmentNetworkDateDlg)

public:
	CSmartEntertainmentNetworkDateDlg( CSmartEntertainmentWebAPISalesDate& dates, CWnd* pParent = NULL);
	virtual ~CSmartEntertainmentNetworkDateDlg();

	enum { IDD = IDD_SMARTENT_NETWORK_DATE };

public:
	CDateTimeCtrl m_DatePicker;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	
	DECLARE_MESSAGE_MAP()

private:
	CSmartEntertainmentWebAPISalesDate& m_dates;
};

/**********************************************************************/
#endif
/**********************************************************************/
