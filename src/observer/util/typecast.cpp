#include "typecast.h"
#include "common/log/log.h"

bool check_date(int y, int m, int d)
{
    static int mon[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool leap = (y%400==0 || (y%100 && y%4==0));
    return y > 0
        && (m > 0)&&(m <= 12)
        && (d > 0)&&(d <= ((m==2 && leap)?1:0) + mon[m]);
}

RC string_to_date(const char* str, int32_t &date)
{
  // 将 date:2020-01-01 转换成 int:20200101存储
  int y, m, d;
  sscanf(str, "%d-%d-%d", &y, &m, &d);
  bool isLegal = check_date(y, m, d);
  if (!isLegal) {
    LOG_WARN("date check failure");
    return RC::SCHEMA_FIELD_TYPE_MISMATCH;
  }
  date = y * 10000 + m * 100 + d;
  return RC::SUCCESS;
}
