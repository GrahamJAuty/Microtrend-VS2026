#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "StampOfferReportConfig.h"
#include "DateSelectorRange.h"
#include "CommentHandler.h"
#include "ReportLocationControls.h"
//$$******************************************************************

class CStampOfferReportDlg : public CSSDialog
{
public:
	CStampOfferReportDlg( CStampOfferReportConfig& config, CWnd* pParent);
	~CStampOfferReportDlg();

public:
	enum { IDD = IDD_REPORT_STAMPOFFER };
	
	//date
	CSSComboBox m_comboDateRange;
	CDateTimeCtrl m_DatePickerFrom;
	CDateTimeCtrl m_DatePickerTo;
	//time
	CSSComboBox m_comboTimeRange;
	CDateTimeCtrl m_TimePickerFrom;
	CDateTimeCtrl m_TimePickerTo;
	//accounts
	CButton m_radioRange;
	CButton m_radioSingle;
	CButton m_radioList;
	CEdit m_editCardNoFrom;
	CEdit m_editCardNoTo;
	CButton m_buttonList;
	CButton m_checkNewPage;
	CButton m_checkSortByCardNo;
	CSSTabbedComboBox m_comboGroup;
	//terminals
	CSSTabbedComboBox m_comboDatabase;
	CSSTabbedComboBox m_comboFolderSet;
	CButton m_checkTerminals;
	CSSComboBox m_comboTerminals;
	//options
	CButton m_checkShowEposAward;
	CButton m_checkShowEposRedeem;
	CButton m_checkShowManualAdd;
	CButton m_checkShowManualEdit;
	CButton m_checkShowManualDelete;
	CButton m_checkShowHotlist;
	CButton m_checkShowTransfer;
	CSSTabbedComboBox m_comboOffer;
	CButton m_checkShowDeletedLines;
	//altkey
	CButton m_buttonAltKey1;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//date
	afx_msg void OnSelectDateRange();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	//time
	afx_msg void OnSelectTimeRange();
	//account selection
	afx_msg void OnRadioRange();
	afx_msg void OnRadioSingle();
	afx_msg void OnDoubleClickRadioSingle();
	afx_msg void OnRadioList();
	afx_msg void OnButtonList();
	afx_msg void OnDoubleClickRadioList();
	afx_msg void OnToggleSortByCardNo();
	//terminals
	afx_msg void OnSelectDatabase();
	afx_msg void OnSelectFolderSet();
	afx_msg void OnToggleTerminals();
	//altkey
	afx_msg void OnButtonAltKey1();
	
	DECLARE_MESSAGE_MAP()

private:
	void SetRangeOptions();
	void SetCardRangeType(int n);

private:
	CStampOfferReportConfig& m_ReportConfig;
	CDateSelectorRange m_DateSelectorRange;
	CReportLocationControls m_ReportLocationControls;
};

//$$******************************************************************

