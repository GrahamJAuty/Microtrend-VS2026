#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "EposTaskDatabase.h"
/**********************************************************************/

class CJobDownloadDlg : public CSSDialog
{
public:
	CJobDownloadDlg( CEposTaskDatabase& TaskDatabase, CWnd* pParent = NULL);  

	//{{AFX_DATA(CJobDownloadDlg)
	enum { IDD = IDD_JOB_DOWNLOAD };
	CButton	m_checkImages;
	CButton	m_checkMacro;
	CButton	m_checkServerName;
	CButton	m_checkServerPreset;
	CButton	m_checkCategory;
	CButton	m_checkBarcode;
	CButton	m_checkMixMatch;
	CButton	m_checkPromotion;
	CButton	m_checkWalletStampRule;
	CButton	m_checkAllOffer;
	CButton	m_checkTimePlan;
	CButton	m_checkLoyaltyScheme;
	CButton	m_checkTableArea;
	CButton	m_checkTableName;
	CButton	m_checkReceipt;
	CButton	m_checkFunction;
	CButton	m_checkPayment;
	CButton	m_checkModifier;
	CButton	m_checkGroup;
	CButton	m_checkAllergen;
	CButton	m_checkDepartment;
	CButton	m_checkKeyboard;
	CButton	m_checkResetMacro1;
	CButton m_checkTax;
	CButton	m_checkReason;
	int		m_nKbNo;
	//}}AFX_DATA
	CButton m_radioPlu1;
	CButton m_radioPlu2;
	CButton m_radioPlu3;
	CButton m_radioPlu4;
	CButton	m_radioPriceSet0;
	CButton	m_radioPriceSet1;
	CButton	m_radioPriceSet2;
	CButton	m_radioPriceSet3;
	CButton m_checkWithKeyboard;
	CButton	m_staticResetMacro2;
	CButton	m_checkResetMacro2;
	
	//{{AFX_VIRTUAL(CJobDownloadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CJobDownloadDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnToggleKeyboard();
	afx_msg void OnToggleWithKeyboard();
	afx_msg void OnRadioPlu1();
	afx_msg void OnRadioPlu2();
	afx_msg void OnRadioPlu3();
	afx_msg void OnRadioPlu4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	//RADIO BUTTON INDEX
	int	m_nPriceSet;
	int m_nPluDownloadType;
	int m_nInfoDownloadType;

private:
	void TogglePluDownloadType();
	void EnablePriceSets( bool bEnable );
	void AddJobsToTask();
	void ProcessTick( CButton& tick, bool bShow, int nXPos, int& nNextYPos );

private:
	CEdit* GetEditKeyboard();

private:
	CEposTaskDatabase& m_TaskDatabase;

private:
	bool m_bAllowPriceSets;
	bool m_bAllowKeyboards;
	bool m_bChangesMode;
};

/**********************************************************************/


