﻿#include "system_wrappers/source/rw_lock_generic.h"

#include "system_wrappers/interface/condition_variable_wrapper.h"
#include "system_wrappers/interface/critical_section_wrapper.h"

namespace gn
{

	RWLockGeneric::RWLockGeneric()
		: readers_active_(0),
		writer_active_(false),
		readers_waiting_(0),
		writers_waiting_(0)
	{
		critical_section_ = CriticalSectionWrapper::CreateCriticalSection();
		read_condition_ = ConditionVariableWrapper::CreateConditionVariable();
		write_condition_ = ConditionVariableWrapper::CreateConditionVariable();
	}

	RWLockGeneric::~RWLockGeneric()
	{
		delete write_condition_;
		delete read_condition_;
		delete critical_section_;
	}

	void RWLockGeneric::AcquireLockExclusive()
	{
		CriticalSectionScoped cs(critical_section_);
		if (writer_active_ || readers_active_ > 0)
		{
			++writers_waiting_;
			while (writer_active_ || readers_active_ > 0)
			{
				write_condition_->SleepCS(*critical_section_);
			}
			--writers_waiting_;
		}
		writer_active_ = true;
	}

	void RWLockGeneric::ReleaseLockExclusive()
	{
		CriticalSectionScoped cs(critical_section_);
		writer_active_ = false;
		if (writers_waiting_ > 0)
		{
			write_condition_->Wake();
		} else if (readers_waiting_ > 0)
		{
			read_condition_->WakeAll();
		}
	}

	void RWLockGeneric::AcquireLockShared()
	{
		CriticalSectionScoped cs(critical_section_);
		if (writer_active_ || writers_waiting_ > 0)
		{
			++readers_waiting_;

			while (writer_active_ || writers_waiting_ > 0)
			{
				read_condition_->SleepCS(*critical_section_);
			}
			--readers_waiting_;
		}
		++readers_active_;
	}

	void RWLockGeneric::ReleaseLockShared()
	{
		CriticalSectionScoped cs(critical_section_);
		--readers_active_;
		if (readers_active_ == 0 && writers_waiting_ > 0)
		{
			write_condition_->Wake();
		}
	}

}  // namespace gn
