

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Tue Jun 17 09:32:32 2014
 */
/* Compiler settings for .\PDFGenen.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __PDFGenen_h__
#define __PDFGenen_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IToPDF_FWD_DEFINED__
#define __IToPDF_FWD_DEFINED__
typedef interface IToPDF IToPDF;
#endif 	/* __IToPDF_FWD_DEFINED__ */


#ifndef __ToPDF_FWD_DEFINED__
#define __ToPDF_FWD_DEFINED__

#ifdef __cplusplus
typedef class ToPDF ToPDF;
#else
typedef struct ToPDF ToPDF;
#endif /* __cplusplus */

#endif 	/* __ToPDF_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IToPDF_INTERFACE_DEFINED__
#define __IToPDF_INTERFACE_DEFINED__

/* interface IToPDF */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IToPDF;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D112D77B-C649-487B-832D-523EA0C3F58F")
    IToPDF : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IToPDFVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IToPDF * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IToPDF * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IToPDF * This);
        
        END_INTERFACE
    } IToPDFVtbl;

    interface IToPDF
    {
        CONST_VTBL struct IToPDFVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IToPDF_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IToPDF_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IToPDF_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IToPDF_INTERFACE_DEFINED__ */



#ifndef __PDFGenenLib_LIBRARY_DEFINED__
#define __PDFGenenLib_LIBRARY_DEFINED__

/* library PDFGenenLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_PDFGenenLib;

EXTERN_C const CLSID CLSID_ToPDF;

#ifdef __cplusplus

class DECLSPEC_UUID("8E325B2C-93E3-40D0-9EE0-8DD2C28F0398")
ToPDF;
#endif
#endif /* __PDFGenenLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


