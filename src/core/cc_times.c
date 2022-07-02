#include "cc_times.h"

#define CC_TIME_SLOTS 64

static cc_uin32 slot;
// 时钟锁
static cc_atomic_t cc_time_lock;

// 当前事件戳
volatile cc_msec_t cc_current_msec;
// 缓存时间
volatile cc_time_t *cc_cached_time;
// 日志模块时间戳
volatile cc_str_t cc_cached_err_log_time;

// 当地时间与GMT时间差
static cc_in32 cached_gmtoff;
static cc_time_t cached_time[CC_TIME_SLOTS];
static cc_char cached_err_log_time[CC_TIME_SLOTS]
					[sizeof("1970/09/28 12:00:00")];

static char *week[] = {"Sun", "Mon", "Tue", 
		"Wed", "Thu", "Fri", "Sat"};
static char *months[] = {"Jan", "Feb", "Mar", "Apr", 
		"May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

static cc_msec_t cc_monotonic_time(time_t sec, cc_uin32 msec);

// 时钟初始化
void 
cc_init_time(void)
{
    cc_cached_err_log_time.len = sizeof("1970/09/28 12:00:00") - 1;

    cc_cached_time = &cached_time[0];

    cc_update_time();
}

// 更新时间戳
void
cc_update_time(void)
{
    cc_char *p0;
    time_t sec;
    cc_uin32 msec;
    cc_tm_t tm, gmt;
    cc_time_t *tp;
    struct timeval tv;

    // 获取时钟锁
    if (!cc_trylock(&cc_time_lock)) {
        return;
    }

    // 获取当前时间戳
    cc_gettimeofday(&tv);
    sec = tv.tv_sec;
    msec = tv.tv_usec / 1000;
    
    cc_current_msec = cc_monotonic_time(sec, msec);

    // 与上一次更新的时间进行对比，秒数相等更新毫秒，结束更新
    tp = &cached_time[slot];
    if (tp->sec == sec) {
        tp->msec = msec;
        cc_unlock(&cc_time_lock);
        return;
    }

    // 更新slot
    slot = slot == CC_TIME_SLOTS - 1 ? 0 : slot + 1;

    // 更新时间戳
    tp = &cached_time[slot];
    tp->sec = sec;
    tp->msec = msec;

    // 将time_t格式表示的时间转换成ngx_tm_t格式
    cc_gmtime(sec, &gmt);

    // 获取当地时间
    cc_localtime(sec, &tm);
    cached_gmtoff = cc_timezone(tm.cc_tm_isdst);
    tp->gmtoff = cached_gmtoff;

    p0 = &cached_err_log_time[slot][0];
    (void)sprintf(p0, "%4d/%02d/%02d %02d:%02d:%02d",
                       tm.cc_tm_year, tm.cc_tm_mon,
                       tm.cc_tm_mday, tm.cc_tm_hour,
                       tm.cc_tm_min, tm.cc_tm_sec);

    cc_cached_time = tp;
    cc_cached_err_log_time.data = p0;

    // 解锁
    cc_unlock(&cc_time_lock);
}

static cc_msec_t
cc_monotonic_time(time_t sec, cc_uin32 msec)
{
    return (cc_msec_t) sec * 1000 + msec;
}

// 将time_t格式表示的时间转换成ngx_tm_t格式
void
cc_gmtime(time_t t, cc_tm_t *tp)
{
    cc_in32 yday;
    cc_uin32 sec, min, hour, mday, mon, year, wday, days, leap;

    if (t < 0) {
        t = 0;
    }

    // 86400 = 24 * 60 * 60
    days = t / 86400;
    sec = t % 86400;

    // 支持年限截断至9999年12月31日23:59:59
    if (days > 2932896) {
        days = 2932896;
        sec = 86399;
    }

    /* 计算一周的第几天，January 1, 1970是周四 */
    wday = (4 + days) % 7;

    // 计算小时、分钟
    hour = sec / 3600;
    sec %= 3600;
    min = sec / 60;
    sec %= 60;

    // 计算年份、一年的第几天 
    days = days - (31 + 28) + 719527;
    year = (days + 2) * 400 / (365 * 400 + 100 - 4 + 1);
    yday = days - (365 * year + year / 4 - year / 100 + year / 400);
    if (yday < 0) {
        leap = (year % 4 == 0) && (year % 100 || (year % 400 == 0));
        yday = 365 + leap + yday;
        year--;
    }

    // 计算月份、一月的第几天
    mon = (yday + 31) * 10 / 306;
    mday = yday - (367 * mon / 12 - 30) + 1;
    
    // 对年份和月份作处理
    if (yday >= 306) {
        year++;
        mon -= 10;
    } else {
        mon += 2;
    }

    tp->cc_tm_sec = (cc_tm_sec_t)sec;
    tp->cc_tm_min = (cc_tm_min_t)min;
    tp->cc_tm_hour = (cc_tm_hour_t)hour;
    tp->cc_tm_mday = (cc_tm_mday_t)mday;
    tp->cc_tm_mon = (cc_tm_mon_t)mon;
    tp->cc_tm_year = (cc_tm_year_t)year;
    tp->cc_tm_wday = (cc_tm_wday_t)wday;
}
