// ToPDF.h : CToPDF の宣言

#pragma once
#include "resource.h"       // メイン シンボル

#include "PDFGenen.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "DCOM の完全サポートを含んでいない Windows Mobile プラットフォームのような Windows CE プラットフォームでは、単一スレッド COM オブジェクトは正しくサポートされていません。ATL が単一スレッド COM オブジェクトの作成をサポートすること、およびその単一スレッド COM オブジェクトの実装の使用を許可することを強制するには、_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA を定義してください。ご使用の rgs ファイルのスレッド モデルは 'Free' に設定されており、DCOM Windows CE 以外のプラットフォームでサポートされる唯一のスレッド モデルと設定されていました。"
#endif



// CToPDF

class ATL_NO_VTABLE CToPDF :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CToPDF, &CLSID_ToPDF>,
	public IDropTarget,
	public IShellExtInit,
	public IContextMenu,
	public IToPDF
{
public:
	CToPDF()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TOPDF)


BEGIN_COM_MAP(CToPDF)
	COM_INTERFACE_ENTRY(IContextMenu)
	COM_INTERFACE_ENTRY(IDropTarget)
	COM_INTERFACE_ENTRY(IShellExtInit)
	COM_INTERFACE_ENTRY(IToPDF)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:

	// IDropTarget : public IUnknown
public:
    virtual HRESULT STDMETHODCALLTYPE DragEnter( 
        /* [unique][in] */ __RPC__in_opt IDataObject *pDataObj,
        /* [in] */ DWORD grfKeyState,
        /* [in] */ POINTL pt,
        /* [out][in] */ __RPC__inout DWORD *pdwEffect)
	{
		*pdwEffect &= Eat(pDataObj) ? DROPEFFECT_COPY : DROPEFFECT_NONE;
		return S_OK;
	}
    
    virtual HRESULT STDMETHODCALLTYPE DragOver( 
        /* [in] */ DWORD grfKeyState,
        /* [in] */ POINTL pt,
        /* [out][in] */ __RPC__inout DWORD *pdwEffect)
	{
		return S_OK;
	}
    
    virtual HRESULT STDMETHODCALLTYPE DragLeave( void)
	{
		return S_OK;
	}
    
    virtual HRESULT STDMETHODCALLTYPE Drop( 
        /* [unique][in] */ __RPC__in_opt IDataObject *pDataObj,
        /* [in] */ DWORD grfKeyState,
        /* [in] */ POINTL pt,
        /* [out][in] */ __RPC__inout DWORD *pdwEffect)
	{
		return Saveas();
	}

	HRESULT Saveas(bool allowUI = false);

	CAtlArray<CString> m_tiffs;

	bool Eat(IDataObject *pDataObj);

	// IShellExtInit : public IUnknown
public:
    virtual HRESULT STDMETHODCALLTYPE Initialize( 
        /* [in] */ LPCITEMIDLIST pidlFolder,
        /* [in] */ IDataObject *pdtobj,
        /* [in] */ HKEY hkeyProgID)
	{
		if (Eat(pdtobj))
			return S_OK;
		return E_FAIL;
	}

	// http://eternalwindows.jp/shell/shellex/shellex08.html

	// IContextMenu : public IUnknown
public:
    virtual HRESULT STDMETHODCALLTYPE QueryContextMenu( 
        /* [annotation][in] */ 
        __in  HMENU hmenu,
        /* [annotation][in] */ 
        __in  UINT indexMenu,
        /* [annotation][in] */ 
        __in  UINT idCmdFirst,
        /* [annotation][in] */ 
        __in  UINT idCmdLast,
        /* [annotation][in] */ 
        __in  UINT uFlags)
	{
		UINT id = idCmdFirst;

		MENUITEMINFO m = {sizeof(MENUITEMINFO)};

		m_iCmdTIF2PDF = -1;

		if (!(uFlags & CMF_DEFAULTONLY)) {
			m.fMask = MIIM_ID | MIIM_TYPE;
			m.fType = MFT_SEPARATOR;
			m.wID = id;
			InsertMenuItem(hmenu, indexMenu, TRUE, &m);

			++id;
			++indexMenu;
		}

		if (!(uFlags & CMF_DEFAULTONLY)) {
			m.fMask = MIIM_ID | MIIM_TYPE;
			m.fType = MFT_STRING;
			m.wID = id;
			m.dwTypeData = _T("TIFをPDFに変換");
			if (InsertMenuItem(hmenu, indexMenu, TRUE, &m)) {
				m_iCmdTIF2PDF = id - idCmdFirst;
			}

			++id;
			++indexMenu;
		}

		return MAKE_SCODE(SEVERITY_SUCCESS, FACILITY_NULL, id - idCmdFirst);
	}

	int m_iCmdTIF2PDF;
    
    virtual HRESULT STDMETHODCALLTYPE InvokeCommand( 
        /* [annotation][in] */ 
        __in  CMINVOKECOMMANDINFO *pici)
	{
		UINT iCmd = LOWORD(pici->lpVerb);

		if (HIWORD(pici->lpVerb) != 0)
			return E_INVALIDARG;

		if (iCmd == m_iCmdTIF2PDF) {
			return Saveas(true);
		}

		return S_OK;
	}
    
    virtual HRESULT STDMETHODCALLTYPE GetCommandString( 
        /* [annotation][in] */ 
        __in  UINT_PTR idCmd,
        /* [annotation][in] */ 
        __in  UINT uType,
        /* [annotation][in] */ 
        __reserved  UINT *pReserved,
        /* [annotation][out] */ 
        __out_awcount(!(uType & GCS_UNICODE), cchMax)  LPSTR pszName,
        /* [annotation][in] */ 
        __in  UINT cchMax)
	{
		return E_FAIL;
	}
    
};

OBJECT_ENTRY_AUTO(__uuidof(ToPDF), CToPDF)
