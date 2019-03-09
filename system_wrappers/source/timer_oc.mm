#import <Foundation/Foundation.h>
#include "system_wrappers/source/timer_oc.h"
#include "system_wrappers/interface/trace.h"
#include "modules/LogModule/log_module.h"

struct TimerItem
{
	int32_t timerID;
	id event_timerID;
	void* timer;
	void* userData;
	gn::LPTIMERCALLBACK callback;
};

@interface TimerExecutor : NSObject
{
@private
	TimerItem* item_;
    NSTimer* timer_;
}

-(bool)SetTimer:(TimerItem*)item secs:(double)secs;
-(void)KillTimer;
-(void)OnTimer:(id)obj;
@end

@implementation TimerExecutor

-(id)init
{
	self = [super init];
	if (self) {
	}

	item_ = nil;
	timer_ = nil;

	return self;
}

-(void)dealloc
{
	[self KillTimer];
    [super dealloc];
}

-(bool)SetTimer:(TimerItem*)item secs:(double)secs
{
    [self KillTimer];
    
	item_ = item;
	timer_ = [NSTimer scheduledTimerWithTimeInterval:secs target:self selector:@selector(OnTimer:) userInfo:nil repeats:YES];
    [timer_ retain];
    
	[[NSRunLoop mainRunLoop] addTimer:timer_ forMode:NSRunLoopCommonModes];
	if (timer_ == nil) {
		return false;
	}

	return true;
}

-(void)KillTimer
{
	if (timer_ != nil) {
		[timer_ invalidate];

        [timer_ release];
		timer_ = nil;
		item_ = nil;
	}
}

-(void)OnTimer:(id)obj
{
	if (item_ != nil) {
		((gn::TimerOc*)item_->timer)->OnTimer(item_);
	}
}

@end

namespace gn {

	static int32_t g_timer_id_index_ = 0;

	TimerOc::TimerOc(void)
		: crit_cs_(*CriticalSectionWrapper::CreateCriticalSection())
	{
	}

	TimerOc::~TimerOc(void)
	{
		{
			CriticalSectionScoped cs(&crit_cs_);
			if (timerList_.size() > 0)
			{
				for (TimerListItr it = timerList_.begin();
					it != timerList_.end(); ++it)
				{
					delete (TimerItem*)it->second;
				}
				timerList_.clear();
				GN_TRACE(kTraceWarning, kTraceUtility, 0,
					"Timer list is not empty.");
			}
		}
		delete &crit_cs_;
	}

	void TimerOc::OnTimer(void* item)
	{
		TimerItem* pitem = (TimerItem*)item;
		int32_t timerID = 0;
		void* userData = NULL;
		LPTIMERCALLBACK callback = NULL;
		{
			CriticalSectionScoped cs(&crit_cs_);
			TimerListItr it = timerList_.find(pitem->timerID);
			if (it != timerList_.end())
			{
				if (pitem->callback)
				{
					timerID = pitem->timerID;
					userData = pitem->userData;
					callback = pitem->callback;
				}
			}
		}
		if (callback)
		{
			callback(timerID,userData);
		}
	}

	int32_t TimerOc::SetTimer(uint32_t periodicMS,
		LPTIMERCALLBACK callback, void* userData)
	{
		CriticalSectionScoped cs(&crit_cs_);
		int32_t timerID = g_timer_id_index_++;
		TimerListItr it = timerList_.find(timerID);
		if (it != timerList_.end())
		{
			GN_TRACE(kTraceStateInfo, kTraceUtility, 0,
				"SetTimer with id:%d already exist,kill it.", it->first);
			TimerItem* pitem = (TimerItem*)it->second;
			[pitem->event_timerID KillTimer];
			delete (TimerItem*)it->second;
			timerList_.erase(it);
		}

		TimerItem* item = new TimerItem;;
		item->timer = this;
		item->timerID = timerID;
		item->userData = userData;
		item->callback = callback;
		item->event_timerID = [[TimerExecutor alloc] init];

		if (item->event_timerID == nil)
		{
			delete item;
			GN_TRACE(kTraceError, kTraceUtility, 0,
				"SetTimer alloc with id:%d set failed ", timerID);
			return -1;
		}

		timerList_[timerID] = item;

		if ([item->event_timerID SetTimer:item secs:(periodicMS * 1.0000f / 1000.f)] == false) {
			delete item;
			GN_TRACE(kTraceError, kTraceUtility, 0,
				"SetTimer with id:%d set failed ", timerID);
			return -1;
		}

		return timerID;
	}

	bool TimerOc::KillTimer(int32_t timerID)
	{
		CriticalSectionScoped cs(&crit_cs_);
		TimerListItr it = timerList_.find(timerID);
		if (it != timerList_.end())
		{
			TimerItem* pitem = (TimerItem*)it->second;
			[pitem->event_timerID KillTimer];
			delete (TimerItem*)it->second;
			timerList_.erase(it);
		}

		return true;
	}

	bool TimerOc::IsTimer(int32_t timerID)
	{
		CriticalSectionScoped cs(&crit_cs_);
		TimerListItr it = timerList_.find(timerID);

		return it != timerList_.end();
	}

}
