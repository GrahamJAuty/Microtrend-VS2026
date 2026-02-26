//*******************************************************************
#include "LoyaltyManager.h"
//*******************************************************************
#include "PhotoIDDlg.h"
//*******************************************************************

CPhotoIDDlg::CPhotoIDDlg(CSQLRowAccountFull& RowAccount, const char* szImageFilename, CWnd* pParent /*=NULL*/)
	: CDialog(CPhotoIDDlg::IDD, pParent)
{
	m_strImageFilename = szImageFilename;
	m_strCardName = RowAccount.GetFullname();
	m_strTitle.Format("Photo ID - Account %s", System.FormatCardNo(RowAccount.GetUserIDString()));
}

//*******************************************************************

CPhotoIDDlg::~CPhotoIDDlg()
{
	if ((HBITMAP)m_image != NULL)
	{
		m_image.Destroy();
	}
}

//*******************************************************************

void CPhotoIDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PICTURE, m_staticPicture);
	DDX_Text(pDX, IDC_EDIT_CARDNAME, m_strCardName);
}

//*******************************************************************

BEGIN_MESSAGE_MAP(CPhotoIDDlg, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//*******************************************************************
// CPhotoIDDlg message handlers

BOOL CPhotoIDDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText ( m_strTitle );
	m_image.Load ( m_strImageFilename );

	return TRUE;
}

//*******************************************************************

void CPhotoIDDlg::OnPaint()
{
	CPaintDC dc(this);

	if ((HBITMAP)m_image != NULL)
	{
		CRect rect;
		m_staticPicture.GetWindowRect(rect);
		ScreenToClient(&rect);

		//		if ( keep aspect ratio )
		{
			CPoint ptCentreFrame = rect.CenterPoint();

			double dcxMax = rect.Width();
			double dcyMax = rect.Height();
			double dAspectRatio = (double)m_image.GetWidth() / (double)m_image.GetHeight();
			double dcxNew = dcxMax;
			double dcyNew = dcxNew / dAspectRatio;

			if (dcyNew > dcyMax)
			{
				dcyNew = dcyMax;
				dcxNew = dcyNew * dAspectRatio;
			}
			rect.right = rect.left + (int)dcxNew;
			rect.bottom = rect.top + (int)dcyNew;

			CPoint ptCentreImage = rect.CenterPoint();
			int dx = ptCentreFrame.x - ptCentreImage.x;
			int dy = ptCentreFrame.y - ptCentreImage.y;
			rect.OffsetRect(dx, dy);
		}

		dc.SetStretchBltMode(COLORONCOLOR);
		m_image.StretchBlt(dc.m_hDC, rect.left, rect.top, rect.Width(), rect.Height(), 0, 0, m_image.GetWidth(), m_image.GetHeight(), SRCCOPY);
	}
}

//*******************************************************************
