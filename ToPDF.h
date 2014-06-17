// ToPDF.h : CToPDF �̐錾

#pragma once
#include "resource.h"       // ���C�� �V���{��

#include "PDFGenen.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "DCOM �̊��S�T�|�[�g���܂�ł��Ȃ� Windows Mobile �v���b�g�t�H�[���̂悤�� Windows CE �v���b�g�t�H�[���ł́A�P��X���b�h COM �I�u�W�F�N�g�͐������T�|�[�g����Ă��܂���BATL ���P��X���b�h COM �I�u�W�F�N�g�̍쐬���T�|�[�g���邱�ƁA����т��̒P��X���b�h COM �I�u�W�F�N�g�̎����̎g�p�������邱�Ƃ���������ɂ́A_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ���`���Ă��������B���g�p�� rgs �t�@�C���̃X���b�h ���f���� 'Free' �ɐݒ肳��Ă���ADCOM Windows CE �ȊO�̃v���b�g�t�H�[���ŃT�|�[�g�����B��̃X���b�h ���f���Ɛݒ肳��Ă��܂����B"
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
			m.dwTypeData = _T("TIF��PDF�ɕϊ�");
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
