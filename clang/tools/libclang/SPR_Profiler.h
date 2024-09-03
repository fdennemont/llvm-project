#pragma once

#include <stdint.h>

namespace SPR_PROFILER{
	struct THREAD_PROTECTOR;
	struct SCOPE_CTX_t;
	struct SRCLOC_t;
}

typedef void (__fastcall* GPTHREADPROTECTION_ENABLE)(SPR_PROFILER::THREAD_PROTECTOR*, char const* szDesc, int bGPUThread);
typedef void (__fastcall* GPTHREADPROTECTION_DISABLE)(SPR_PROFILER::THREAD_PROTECTOR*);
typedef void (__fastcall* GSCOPE_CTX_T_ONENTER)(SPR_PROFILER::SCOPE_CTX_t*, const SPR_PROFILER::SRCLOC_t& loc, int bWait);
typedef void (__fastcall* GSCOPE_CTX_T_ONLEAVE)(SPR_PROFILER::SCOPE_CTX_t*);
typedef void (__fastcall* SRCLOC_INIT)(SPR_PROFILER::SRCLOC_t*, const char* function, const char* file, uint32_t line, char const* name, uint32_t color);

extern GPTHREADPROTECTION_ENABLE gThreadProtection_Enable;
extern GPTHREADPROTECTION_DISABLE gThreadProtection_Disable;
extern GSCOPE_CTX_T_ONENTER gSCOPE_CTX_t_OnEnter;
extern GSCOPE_CTX_T_ONLEAVE gSCOPE_CTX_t_OnLeave;
extern SRCLOC_INIT gSRCLOC_Init;


namespace SPR_PROFILER{
	//--------------------------------
	struct SRCLOC_t{
		void* Data;	
	};
	//--------------------------------
	struct SRCLOC:SRCLOC_t{
    SRCLOC(const char* function, const char* file, uint32_t line, char const* name=NULL, uint32_t color=0){
    	if (gSRCLOC_Init){
    		gSRCLOC_Init(this, function, file, line, name, color);
    	}
  	}
  };
	//--------------------------------
	struct SCOPE_CTX_t{
		void* data;
		void OnEnter(const SRCLOC_t& loc, int bWait){
			if (gSCOPE_CTX_t_OnEnter)
				gSCOPE_CTX_t_OnEnter(this, loc, bWait);
		}
		void OnLeave(){
			if (gSCOPE_CTX_t_OnLeave)
				gSCOPE_CTX_t_OnLeave(this);
		}
	};
	//--------------------------------
	struct THREAD_PROTECTOR{
		void*										pThreadManager;
		uint32_t								iSlot;
		void*										pWriter;
		void Enable(char const* szDesc, int bGPUThread){
			if (gThreadProtection_Enable){
				gThreadProtection_Enable(this, szDesc, bGPUThread);
			}
		}
		void Disable(){
			if (gThreadProtection_Disable){
				gThreadProtection_Disable(this);
			}
		}
		//--
		[[nodiscard]] THREAD_PROTECTOR(char const* szDesc){
			Enable(szDesc, false);
		}
		//--
		~THREAD_PROTECTOR(){
			Disable();
		}
		//--
	};
	//--------------------------------
}


#define PROFILER_COMBINE1(X,Y) X##Y  // helper macro
#define PROFILER_COMBINE(X,Y) PROFILER_COMBINE1(X,Y)
#define PROFILER_LOC_UNIQUE_NAME PROFILER_COMBINE(_ProfilerLoc_,__LINE__)
#define PROFILER_MAKE_STATIC_LOC(_DESC_) static SPR_PROFILER::SRCLOC PROFILER_LOC_UNIQUE_NAME(__FUNCTION__, __FILE__, __LINE__, _DESC_)

#define PROFILER_WATCH_ON(_DESC_)					PROFILER_MAKE_STATIC_LOC(_DESC_); SPR_PROFILER::SCOPE_CTX_t _CTX_NAME_; _CTX_NAME_.OnEnter(PROFILER_LOC_UNIQUE_NAME, false);
#define PROFILER_WATCH_OFF()							_CTX_NAME_.OnLeave();
#define PROFILER_WAIT_ON(_DESC_)					PROFILER_MAKE_STATIC_LOC(_DESC_); SPR_PROFILER::SCOPE_CTX_t _CTX_NAME_; _CTX_NAME_.OnEnter(PROFILER_LOC_UNIQUE_NAME, true);
#define PROFILER_WAIT_OFF()								_CTX_NAME_.OnLeave();
#define PROFILER_REGISTER_THREAD(_DESC_)	SPR_PROFILER::THREAD_PROTECTOR PROFILER_COMBINE(_Profiler_Thread_Protector_,__LINE__)((_DESC_));

