#pragma once

// Runtime POSIX TZ string parser.
//
// ESPHome's time::RealTimeClock no longer exposes a runtime set_timezone()
// method (removed after the timezone was moved to a pre-parsed
// time::ParsedTimezone struct, normally only computed once at codegen time
// from the "timezone:" YAML option). This addon lets the user pick a
// timezone at runtime from a dropdown, so it needs to parse a POSIX TZ
// string (e.g. "CET-1CEST,M3.5.0,M10.5.0/3") on-device and push the result
// into time::set_global_tz() itself.

#include <cctype>
#include <cstdlib>
#include <string>

#include "esphome/components/time/posix_tz.h"
#include "esphome/core/log.h"

namespace esphome_media_player {
namespace posix_tz_runtime {

inline bool parse_offset_seconds(const std::string &s, size_t &pos, int32_t &out_seconds) {
  int sign = 1;
  if (pos < s.size() && (s[pos] == '+' || s[pos] == '-')) {
    sign = s[pos] == '-' ? -1 : 1;
    pos++;
  }
  size_t start = pos;
  while (pos < s.size() && isdigit((unsigned char) s[pos]))
    pos++;
  if (pos == start)
    return false;
  int hh = std::atoi(s.substr(start, pos - start).c_str());
  int mm = 0, ss = 0;
  if (pos < s.size() && s[pos] == ':') {
    pos++;
    start = pos;
    while (pos < s.size() && isdigit((unsigned char) s[pos]))
      pos++;
    if (pos == start)
      return false;
    mm = std::atoi(s.substr(start, pos - start).c_str());
    if (pos < s.size() && s[pos] == ':') {
      pos++;
      start = pos;
      while (pos < s.size() && isdigit((unsigned char) s[pos]))
        pos++;
      if (pos == start)
        return false;
      ss = std::atoi(s.substr(start, pos - start).c_str());
    }
  }
  out_seconds = sign * (hh * 3600 + mm * 60 + ss);
  return true;
}

inline bool skip_tz_name(const std::string &s, size_t &pos) {
  if (pos < s.size() && s[pos] == '<') {
    pos++;
    while (pos < s.size() && s[pos] != '>')
      pos++;
    if (pos >= s.size())
      return false;
    pos++;  // skip '>'
    return true;
  }
  size_t start = pos;
  while (pos < s.size() && isalpha((unsigned char) s[pos]))
    pos++;
  return pos > start;
}

inline bool parse_dst_rule(const std::string &s, size_t &pos, esphome::time::DSTRule &rule) {
  using esphome::time::DSTRuleType;
  rule = esphome::time::DSTRule{};
  rule.time_seconds = 7200;  // default 02:00:00
  if (pos < s.size() && s[pos] == 'M') {
    pos++;
    size_t start = pos;
    while (pos < s.size() && isdigit((unsigned char) s[pos]))
      pos++;
    if (pos == start)
      return false;
    int month = std::atoi(s.substr(start, pos - start).c_str());
    if (pos >= s.size() || s[pos] != '.')
      return false;
    pos++;
    start = pos;
    while (pos < s.size() && isdigit((unsigned char) s[pos]))
      pos++;
    if (pos == start)
      return false;
    int week = std::atoi(s.substr(start, pos - start).c_str());
    if (pos >= s.size() || s[pos] != '.')
      return false;
    pos++;
    start = pos;
    while (pos < s.size() && isdigit((unsigned char) s[pos]))
      pos++;
    if (pos == start)
      return false;
    int day_of_week = std::atoi(s.substr(start, pos - start).c_str());
    rule.type = DSTRuleType::MONTH_WEEK_DAY;
    rule.month = (uint8_t) month;
    rule.week = (uint8_t) week;
    rule.day_of_week = (uint8_t) day_of_week;
  } else {
    bool julian = pos < s.size() && s[pos] == 'J';
    if (julian)
      pos++;
    size_t start = pos;
    while (pos < s.size() && isdigit((unsigned char) s[pos]))
      pos++;
    if (pos == start)
      return false;
    int day = std::atoi(s.substr(start, pos - start).c_str());
    rule.type = julian ? DSTRuleType::JULIAN_NO_LEAP : DSTRuleType::DAY_OF_YEAR;
    rule.day = (uint16_t) day;
  }
  if (pos < s.size() && s[pos] == '/') {
    pos++;
    int32_t time_seconds;
    if (!parse_offset_seconds(s, pos, time_seconds))
      return false;
    rule.time_seconds = time_seconds;
  }
  return true;
}

// Parses a POSIX TZ string (the subset produced by IANA tzdata, e.g.
// "CET-1CEST,M3.5.0,M10.5.0/3" or "<+0545>-5:45") into a ParsedTimezone.
inline bool parse_posix_tz(const std::string &tz, esphome::time::ParsedTimezone &out) {
  using esphome::time::DSTRuleType;
  out = esphome::time::ParsedTimezone{};
  size_t pos = 0;
  if (!skip_tz_name(tz, pos))
    return false;
  int32_t std_offset;
  if (!parse_offset_seconds(tz, pos, std_offset))
    return false;
  out.std_offset_seconds = std_offset;

  if (pos >= tz.size()) {
    out.dst_start.type = DSTRuleType::NONE;
    out.dst_end.type = DSTRuleType::NONE;
    return true;
  }

  if (!skip_tz_name(tz, pos))
    return false;
  int32_t dst_offset;
  if (pos < tz.size() && (isdigit((unsigned char) tz[pos]) || tz[pos] == '+' || tz[pos] == '-')) {
    if (!parse_offset_seconds(tz, pos, dst_offset))
      return false;
  } else {
    dst_offset = std_offset - 3600;
  }
  out.dst_offset_seconds = dst_offset;

  if (pos < tz.size() && tz[pos] == ',') {
    pos++;
    if (!parse_dst_rule(tz, pos, out.dst_start))
      return false;
    if (pos >= tz.size() || tz[pos] != ',')
      return false;
    pos++;
    if (!parse_dst_rule(tz, pos, out.dst_end))
      return false;
  } else {
    // DST designation with no transition rule: nothing to switch on.
    out.dst_start.type = DSTRuleType::NONE;
    out.dst_end.type = DSTRuleType::NONE;
  }
  return true;
}

}  // namespace posix_tz_runtime

// Parses `tz` and, on success, applies it as the active global timezone.
inline void apply_posix_timezone(const std::string &tz) {
  esphome::time::ParsedTimezone parsed;
  if (posix_tz_runtime::parse_posix_tz(tz, parsed)) {
    esphome::time::set_global_tz(parsed);
  } else {
    ESP_LOGW("timezone", "Failed to parse POSIX TZ string: %s", tz.c_str());
  }
}

}  // namespace esphome_media_player
