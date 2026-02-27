#pragma once
/**********************************************************************/

class CPersistentSelectionEdit : public CEdit
{
public:
    DECLARE_MESSAGE_MAP()
	afx_msg UINT OnGetDlgCode() { return ( CEdit::OnGetDlgCode() & ~DLGC_HASSETSEL ); }
};

/**********************************************************************/
