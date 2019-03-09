﻿#include "system_wrappers/interface/trace.h"
#include "system_wrappers/source/condition_variable_native_win.h"
#include "system_wrappers/source/critical_section_win.h"
#include "modules/LogModule/log_module.h"

namespace gn
{

	static HMODULE library = NULL;
	static bool win_support_condition_variables_primitive = false;

	PInitializeConditionVariable  PInitializeConditionVariable_;
	PSleepConditionVariableCS     PSleepConditionVariableCS_;
	PWakeConditionVariable        PWakeConditionVariable_;
	PWakeAllConditionVariable     PWakeAllConditionVariable_;

	typedef void (WINAPI *PInitializeConditionVariable)(PCONDITION_VARIABLE);
	typedef BOOL (WINAPI *PSleepConditionVariableCS)(PCONDITION_VARIABLE,
		PCRITICAL_SECTION, DWORD);
	typedef void (WINAPI *PWakeConditionVariable)(PCONDITION_VARIABLE);
	typedef void (WINAPI *PWakeAllConditionVariable)(PCONDITION_VARIABLE);

	ConditionVariableNativeWin::ConditionVariableNativeWin()
	{
	}

	ConditionVariableNativeWin::~ConditionVariableNativeWin()
	{
	}

	ConditionVariableWrapper* ConditionVariableNativeWin::Create()
	{
		ConditionVariableNativeWin* ret_val = new ConditionVariableNativeWin();
		if (!ret_val->Init())
		{
			delete ret_val;
			return NULL;
		}
		return ret_val;
	}

	bool ConditionVariableNativeWin::Init()
	{
		if (!library)
		{
			// Native implementation is supported on Vista+.
			library = LoadLibrary(TEXT("Kernel32.dll"));
			// TODO(henrike): this code results in an attempt to load the above dll
			// every time the previous attempt failed. Only try to load once.
			if (library)
			{
				// TODO(henrike): not thread safe as reading and writing to library is not
				// serialized. Fix.
				GN_TRACE(kTraceStateInfo, kTraceUtility, -1, "Loaded Kernel.dll");

				PInitializeConditionVariable_ =
					(PInitializeConditionVariable) GetProcAddress(
					library, "InitializeConditionVariable");
				PSleepConditionVariableCS_ = (PSleepConditionVariableCS) GetProcAddress(
					library, "SleepConditionVariableCS");
				PWakeConditionVariable_ = (PWakeConditionVariable) GetProcAddress(
					library, "WakeConditionVariable");
				PWakeAllConditionVariable_ = (PWakeAllConditionVariable) GetProcAddress(
					library, "WakeAllConditionVariable");

				if (PInitializeConditionVariable_ && PSleepConditionVariableCS_
					&& PWakeConditionVariable_ && PWakeAllConditionVariable_)
				{
					GN_TRACE(
						kTraceStateInfo, kTraceUtility, -1,
						"Loaded native condition variables");
					win_support_condition_variables_primitive = true;
				}
			}
		}
		if (!win_support_condition_variables_primitive)
		{
			return false;
		}
		PInitializeConditionVariable_(&condition_variable_);
		return true;
	}

	void ConditionVariableNativeWin::SleepCS(CriticalSectionWrapper& crit_sect)
	{
		SleepCS(crit_sect, INFINITE);
	}

	bool ConditionVariableNativeWin::SleepCS(CriticalSectionWrapper& crit_sect,
		unsigned long max_time_in_ms)
	{
		CriticalSectionWindows* cs =
			static_cast<CriticalSectionWindows*>(&crit_sect);
		BOOL ret_val = PSleepConditionVariableCS_(&condition_variable_,
			&(cs->crit), max_time_in_ms);
		return ret_val != 0;
	}

	void ConditionVariableNativeWin::Wake()
	{
		PWakeConditionVariable_(&condition_variable_);
	}

	void ConditionVariableNativeWin::WakeAll()
	{
		PWakeAllConditionVariable_(&condition_variable_);
	}

}  // namespace gn
