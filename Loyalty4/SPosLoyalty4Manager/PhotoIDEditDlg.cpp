//*******************************************************************
#include "..\SPosLoyalty4Shared\FileRemover.h"
#include "..\SPosLoyalty4Shared\ImageHandler.h"
//*******************************************************************
#include "LoyaltyManager.h"
//*******************************************************************
#include "PhotoIDEditDlg.h"
//*******************************************************************

CPhotoIDEditDlg::CPhotoIDEditDlg( CSQLRowAccountFull& RowAccount, CPhotoIDFilenameStack* pPhotoIDFilenameStack, CWnd* pParent /*=NULL*/)
	: CDialog(CPhotoIDEditDlg::IDD, pParent)
{
	m_strInitialDir = "";
	m_strCardNo = RowAccount.GetUserIDString();
	m_strCardName = RowAccount.GetFullname();
	m_strTitle.Format("Photo ID - Account %s", System.FormatCardNo(m_strCardNo));
	m_pPhotoIDFilenameStack = pPhotoIDFilenameStack;
}

//*******************************************************************

CPhotoIDEditDlg::~CPhotoIDEditDlg()
{
	if ((HBITMAP)m_image != NULL)
	{
		m_image.Destroy();
	}
}

//*******************************************************************

void CPhotoIDEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PICTURE, m_staticPicture);
	DDX_Text(pDX, IDC_EDIT_CARDNAME, m_strCardName);
}

//*******************************************************************

BEGIN_MESSAGE_MAP(CPhotoIDEditDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_CHANGE, OnChange)
END_MESSAGE_MAP()

//*******************************************************************
// CPhotoIDDlg message handlers

BOOL CPhotoIDEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText ( m_strTitle );
	UpdatePhotoID(2);

	return TRUE;
}

//*******************************************************************

void CPhotoIDEditDlg::OnPaint()
{
	CPaintDC dc(this); 

	if ( (HBITMAP)m_image != NULL )
	{
		CRect rect;
		m_staticPicture.GetWindowRect ( rect );
		ScreenToClient ( &rect );

//		if ( keep aspect ratio )
		{
			CPoint ptCentreFrame = rect.CenterPoint();

			double dcxMax		= rect.Width();
			double dcyMax		= rect.Height();
			double dAspectRatio = (double) m_image.GetWidth() / (double) m_image.GetHeight();
			double dcxNew		= dcxMax;
			double dcyNew		= dcxNew / dAspectRatio;

			if ( dcyNew > dcyMax )
			{
				dcyNew = dcyMax;
				dcxNew = dcyNew * dAspectRatio;
			}
			rect.right = rect.left + (int) dcxNew;
			rect.bottom = rect.top + (int) dcyNew;

			CPoint ptCentreImage = rect.CenterPoint();
			int dx = ptCentreFrame.x - ptCentreImage.x;
			int dy = ptCentreFrame.y - ptCentreImage.y;
			rect.OffsetRect ( dx, dy );
		}

		dc.SetStretchBltMode ( COLORONCOLOR );
		m_image.StretchBlt ( dc.m_hDC, rect.left, rect.top, rect.Width(), rect.Height(), 0, 0, m_image.GetWidth(), m_image.GetHeight(), SRCCOPY );
	}
}

//*******************************************************************

void CPhotoIDEditDlg::OnChange()
{
	if ( m_pPhotoIDFilenameStack != NULL )
	{
		CFileDialog dlgFile ( TRUE, "", "", OFN_NOCHANGEDIR|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, "Image Files (*.bmp;*.gif;*.jpg)|*.bmp;*.gif;*.jpg||", NULL );
		dlgFile.m_ofn.lpstrTitle = "Select Image File";
		dlgFile.m_ofn.lpstrInitialDir = m_strInitialDir;
		
		if ( dlgFile.DoModal() == IDOK )
		{
			CString strFilepath = dlgFile.GetPathName();
			CString strFilename = dlgFile.GetFileName();
			CString strExt = dlgFile.GetFileExt();

			int nPathLen = strFilepath.GetLength();
			int nFileLen = strFilename.GetLength();

			if (nPathLen >= nFileLen)
			{
				m_strInitialDir = strFilepath.Left(nPathLen - nFileLen);
			}

			strExt.MakeUpper();
			if ( strExt == "BMP" || strExt == "GIF" || strExt == "JPG" )
			{
				m_pPhotoIDFilenameStack -> ImportFile( 3, strFilepath );
				UpdatePhotoID(3);
				Invalidate();
			}
		}
	}
}

//*******************************************************************

void CPhotoIDEditDlg::UpdatePhotoID(int nLevel)
{
	if (m_pPhotoIDFilenameStack != NULL)
	{
		if ((HBITMAP)m_image != NULL)
		{
			m_image.Destroy();
		}

		CString strFilename = m_pPhotoIDFilenameStack->GetImageFilename(nLevel);

		if (strFilename != "")
		{
			m_image.Load(strFilename);
		}
	}
}

//*******************************************************************

void CPhotoIDEditDlg::OnOK()
{
	if (m_pPhotoIDFilenameStack != NULL)
	{
		m_pPhotoIDFilenameStack->PromoteFile(3);
	}

	EndDialog(IDOK);
}

//*******************************************************************

void CPhotoIDEditDlg::OnCancel()
{
	if (m_pPhotoIDFilenameStack != NULL)
	{
		m_pPhotoIDFilenameStack->RemoveFile(3);
	}

	EndDialog(IDCANCEL);
}

//*******************************************************************
