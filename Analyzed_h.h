

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Sun Feb 01 12:22:54 2009
 */
/* Compiler settings for .\Analyzed.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

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

#ifndef __Analyzed_h_h__
#define __Analyzed_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IAnalyzed_FWD_DEFINED__
#define __IAnalyzed_FWD_DEFINED__
typedef interface IAnalyzed IAnalyzed;
#endif 	/* __IAnalyzed_FWD_DEFINED__ */


#ifndef __Analyzed_FWD_DEFINED__
#define __Analyzed_FWD_DEFINED__

#ifdef __cplusplus
typedef class Analyzed Analyzed;
#else
typedef struct Analyzed Analyzed;
#endif /* __cplusplus */

#endif 	/* __Analyzed_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IAnalyzed_INTERFACE_DEFINED__
#define __IAnalyzed_INTERFACE_DEFINED__

/* interface IAnalyzed */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IAnalyzed;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F121F77B-196C-4d81-B297-4EDED0AEC4AF")
    IAnalyzed : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Initialize( 
            /* [in] */ OLE_HANDLE window,
            /* [in] */ LONG resolution,
            /* [in] */ VARIANT_BOOL blur) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Uninitialize( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetVisualization( 
            /* [out] */ LONG *index) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetVisualization( 
            /* [in] */ LONG index) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetVisualizationCount( 
            /* [out] */ LONG *count) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetVisualizationName( 
            /* [out] */ BSTR *name,
            /* [in] */ LONG index) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Update( 
            /* [in] */ OLE_HANDLE levels) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetColorBackground( 
            /* [out] */ LONG *color) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetColorBackground( 
            /* [in] */ LONG color) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetColorTop( 
            /* [out] */ LONG *color) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetColorTop( 
            /* [in] */ LONG color) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetColorTopEnd( 
            /* [out] */ LONG *color) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetColorTopEnd( 
            /* [in] */ LONG color) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetColorBottom( 
            /* [out] */ LONG *color) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetColorBottom( 
            /* [in] */ LONG color) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetColorBottomEnd( 
            /* [out] */ LONG *color) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetColorBottomEnd( 
            /* [in] */ LONG color) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetColorPeak( 
            /* [out] */ LONG *color) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetColorPeak( 
            /* [in] */ LONG color) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAnalyzedVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAnalyzed * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAnalyzed * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAnalyzed * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAnalyzed * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAnalyzed * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAnalyzed * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAnalyzed * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            IAnalyzed * This,
            /* [in] */ OLE_HANDLE window,
            /* [in] */ LONG resolution,
            /* [in] */ VARIANT_BOOL blur);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Uninitialize )( 
            IAnalyzed * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetVisualization )( 
            IAnalyzed * This,
            /* [out] */ LONG *index);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetVisualization )( 
            IAnalyzed * This,
            /* [in] */ LONG index);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetVisualizationCount )( 
            IAnalyzed * This,
            /* [out] */ LONG *count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetVisualizationName )( 
            IAnalyzed * This,
            /* [out] */ BSTR *name,
            /* [in] */ LONG index);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Update )( 
            IAnalyzed * This,
            /* [in] */ OLE_HANDLE levels);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetColorBackground )( 
            IAnalyzed * This,
            /* [out] */ LONG *color);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetColorBackground )( 
            IAnalyzed * This,
            /* [in] */ LONG color);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetColorTop )( 
            IAnalyzed * This,
            /* [out] */ LONG *color);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetColorTop )( 
            IAnalyzed * This,
            /* [in] */ LONG color);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetColorTopEnd )( 
            IAnalyzed * This,
            /* [out] */ LONG *color);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetColorTopEnd )( 
            IAnalyzed * This,
            /* [in] */ LONG color);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetColorBottom )( 
            IAnalyzed * This,
            /* [out] */ LONG *color);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetColorBottom )( 
            IAnalyzed * This,
            /* [in] */ LONG color);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetColorBottomEnd )( 
            IAnalyzed * This,
            /* [out] */ LONG *color);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetColorBottomEnd )( 
            IAnalyzed * This,
            /* [in] */ LONG color);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetColorPeak )( 
            IAnalyzed * This,
            /* [out] */ LONG *color);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetColorPeak )( 
            IAnalyzed * This,
            /* [in] */ LONG color);
        
        END_INTERFACE
    } IAnalyzedVtbl;

    interface IAnalyzed
    {
        CONST_VTBL struct IAnalyzedVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAnalyzed_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAnalyzed_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAnalyzed_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAnalyzed_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IAnalyzed_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IAnalyzed_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IAnalyzed_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IAnalyzed_Initialize(This,window,resolution,blur)	\
    ( (This)->lpVtbl -> Initialize(This,window,resolution,blur) ) 

#define IAnalyzed_Uninitialize(This)	\
    ( (This)->lpVtbl -> Uninitialize(This) ) 

#define IAnalyzed_GetVisualization(This,index)	\
    ( (This)->lpVtbl -> GetVisualization(This,index) ) 

#define IAnalyzed_SetVisualization(This,index)	\
    ( (This)->lpVtbl -> SetVisualization(This,index) ) 

#define IAnalyzed_GetVisualizationCount(This,count)	\
    ( (This)->lpVtbl -> GetVisualizationCount(This,count) ) 

#define IAnalyzed_GetVisualizationName(This,name,index)	\
    ( (This)->lpVtbl -> GetVisualizationName(This,name,index) ) 

#define IAnalyzed_Update(This,levels)	\
    ( (This)->lpVtbl -> Update(This,levels) ) 

#define IAnalyzed_GetColorBackground(This,color)	\
    ( (This)->lpVtbl -> GetColorBackground(This,color) ) 

#define IAnalyzed_SetColorBackground(This,color)	\
    ( (This)->lpVtbl -> SetColorBackground(This,color) ) 

#define IAnalyzed_GetColorTop(This,color)	\
    ( (This)->lpVtbl -> GetColorTop(This,color) ) 

#define IAnalyzed_SetColorTop(This,color)	\
    ( (This)->lpVtbl -> SetColorTop(This,color) ) 

#define IAnalyzed_GetColorTopEnd(This,color)	\
    ( (This)->lpVtbl -> GetColorTopEnd(This,color) ) 

#define IAnalyzed_SetColorTopEnd(This,color)	\
    ( (This)->lpVtbl -> SetColorTopEnd(This,color) ) 

#define IAnalyzed_GetColorBottom(This,color)	\
    ( (This)->lpVtbl -> GetColorBottom(This,color) ) 

#define IAnalyzed_SetColorBottom(This,color)	\
    ( (This)->lpVtbl -> SetColorBottom(This,color) ) 

#define IAnalyzed_GetColorBottomEnd(This,color)	\
    ( (This)->lpVtbl -> GetColorBottomEnd(This,color) ) 

#define IAnalyzed_SetColorBottomEnd(This,color)	\
    ( (This)->lpVtbl -> SetColorBottomEnd(This,color) ) 

#define IAnalyzed_GetColorPeak(This,color)	\
    ( (This)->lpVtbl -> GetColorPeak(This,color) ) 

#define IAnalyzed_SetColorPeak(This,color)	\
    ( (This)->lpVtbl -> SetColorPeak(This,color) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAnalyzed_INTERFACE_DEFINED__ */



#ifndef __Analyzed_LIBRARY_DEFINED__
#define __Analyzed_LIBRARY_DEFINED__

/* library Analyzed */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_Analyzed;

EXTERN_C const CLSID CLSID_Analyzed;

#ifdef __cplusplus

class DECLSPEC_UUID("CE097244-9355-4CF2-8890-0161227E14CF")
Analyzed;
#endif
#endif /* __Analyzed_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


