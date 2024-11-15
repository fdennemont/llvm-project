#pragma once
//-------------------------------------------------------------------------------------------------------------
namespace SPR_PROFILER{
	struct THREAD_PROTECTOR;
	struct SCOPE_CTX_t;
	struct SRCLOC_t;
}

typedef void (__fastcall* GPTHREADPROTECTION_ENABLE)(SPR_PROFILER::THREAD_PROTECTOR*, char const* szDesc, int bGPUThread);
typedef void (__fastcall* GPTHREADPROTECTION_DISABLE)(SPR_PROFILER::THREAD_PROTECTOR*);
typedef void (__fastcall* GSCOPE_CTX_T_ONENTER)(SPR_PROFILER::SCOPE_CTX_t*, const SPR_PROFILER::SRCLOC_t& loc, int bWait);
typedef void (__fastcall* GSCOPE_CTX_T_ONLEAVE)(SPR_PROFILER::SCOPE_CTX_t*);
typedef void (__fastcall* SRCLOC_INIT)(SPR_PROFILER::SRCLOC_t*, const char* function, const char* file, unsigned int line, char const* name, unsigned int color);
typedef void* (__fastcall* ALLOCATE)(int Size);
typedef void (__fastcall* FREE)(void* Data);

extern GPTHREADPROTECTION_ENABLE gThreadProtection_Enable;
extern GPTHREADPROTECTION_DISABLE gThreadProtection_Disable;
extern GSCOPE_CTX_T_ONENTER gSCOPE_CTX_t_OnEnter;
extern GSCOPE_CTX_T_ONLEAVE gSCOPE_CTX_t_OnLeave;
extern SRCLOC_INIT gSRCLOC_Init;
extern ALLOCATE gAllocate;
extern FREE gFree;
//-------------------------------------------------------------------------------------------------------------
