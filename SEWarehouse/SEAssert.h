#ifndef Swing_Assert_H
#define Swing_Assert_H

// �����ǵ�SE_ASSERT��,����΢��<assert.h>��dll����_assert

/* From VisualC 7.1 assert.h: Define _CRTIMP */
#ifndef _CRTIMP
#ifdef  _DLL
#define _CRTIMP __declspec(dllimport)
#else   /* ndef _DLL */
#define _CRTIMP
#endif  /* _DLL */
#endif  /* _CRTIMP */
/* End: Define _CRTIMP */

/* Define __cdecl for non-Microsoft compilers */
#if ( !defined(_MSC_VER) && !defined(__cdecl) )
#define __cdecl
#endif

#ifdef _MSC_VER

// �����C++����.
#ifdef __cplusplus
// ����C�������ķ�ʽ���������_assert����.
extern "C" {
#endif
// _CRTIMP��__attribute__((dllimport)) 
// __attribute__����Ϊ��������������ֵ,��Ŀ�����ñ����������Ż�����.
// �ؼ���__attribute__����Ϊ����(function attribute),
// ����(variable attribute)�ͽṹ��Ա(type attribute)������.
_CRTIMP void __cdecl _assert(const char *, const char *, unsigned);

#ifdef __cplusplus
}
#endif

#endif

#ifdef NDEBUG

// releaseʱȫ��ʧЧ.
#define SE_ASSERT(__ignore) ((void)0)
#define SE_ASSERTMSG(__ignore1, __ignore2) ((void)0)
#define SE_ASSERTALWAYS(__ignore) ((void)0)
#define SE_BREAKPOINT ((void)0)
#define SE_SAFEBREAKPOINT ((void)0)
#define SE_IFDEBUG(__ignore) ((void)0)
#define SE_DEBUGSTRING(__ignore) ("")

#else

// debugģʽ�û��ϵ��,ǿ�Ƴ����ж�,������.
#ifdef _MSC_VER
#define SE_SAFEBREAKPOINT try { _asm { int 3 }; } catch(...) {;}
#else
#define SE_SAFEBREAKPOINT ((void)0)
#endif

//----------------------------------------------------------------------------
// debugģʽ�û��ϵ��,ǿ�Ƴ����ж�,��������.
#define SE_BREAKPOINT \
if( 1 ) \
{ \
    SE_SAFEBREAKPOINT; \
    _assert("hard-coded breakpoint", __FILE__, __LINE__); \
}
//----------------------------------------------------------------------------
// CT DEBUG macro,���ʽֻ��debugģʽ��ִ��.
#define SE_IFDEBUG(expr) \
expr
//----------------------------------------------------------------------------
// ��������������ԭ����assert(exp).
#define SE_ASSERT(expr) \
if( !(expr) ) \
{ \
    SE_SAFEBREAKPOINT; \
    _assert(#expr, __FILE__, __LINE__); \
}
//----------------------------------------------------------------------------
// ������assert.
#define SE_ASSERTALWAYS(msg) \
if( 1 ) \
{ \
    SE_SAFEBREAKPOINT; \
    _assert(msg, __FILE__, __LINE__); \
}
//----------------------------------------------------------------------------
// ��������������ԭ����assert(exp),���ҿ����Զ�����ʾ����.
#define SE_ASSERTMSG(expr, msg) \
if( !(expr) ) \
{ \
    SE_SAFEBREAKPOINT; \
    _assert(msg, __FILE__, __LINE__); \
}
//----------------------------------------------------------------------------
#define SE_DEBUGSTRING(str) (str)
#endif

#endif

