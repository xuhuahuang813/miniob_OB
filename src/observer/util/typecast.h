#include <string>
#include "common/rc.h"
/**
 * check whether the date is valid or not
 * 规定合法值: 1970-01-01 ~ 2040-01-01
 * 闰年：leap year
 * @param y year
 * @param m month
 * @param d day
 * @return
 */
bool check_date(int y, int m, int d);

RC string_to_date(const char* str, int32_t &date);
