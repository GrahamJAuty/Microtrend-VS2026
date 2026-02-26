#pragma once
/**********************************************************************/

class CCommonDoc : public CDocument
{
protected: 
	CCommonDoc();
	DECLARE_DYNCREATE(CCommonDoc)

public:

public:
	//{{AFX_VIRTUAL(CCommonDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

public:
	virtual ~CCommonDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//{{AFX_MSG(CCommonDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/**********************************************************************/

