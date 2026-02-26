#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "SportsBookerWebAPISalesDateTime.h"
/**********************************************************************/

class CSportsBookerNetworkDateDlg : public CDialog
{
	DECLARE_DYNAMIC(CSportsBookerNetworkDateDlg)

public:
	CSportsBookerNetworkDateDlg( CSportsBookerWebAPISalesDateTime& dates, CWnd* pParent = NULL);
	virtual ~CSportsBookerNetworkDateDlg();

	enum { IDD = IDD_SPTBOOK_NETWORK_DATE };

public:
	CDateTimeCtrl m_TimePicker;
	CDateTimeCtrl m_DatePicker;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	
	DECLARE_MESSAGE_MAP()

private:
	CSportsBookerWebAPISalesDateTime& m_dates;
};

/**********************************************************************/
#endif
/**********************************************************************/
