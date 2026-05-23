#include "gap.h"

bool gap::predict_branch(champsim::address ip)
{
  auto value = pht[bank(ip)][global_history];
  return value.value() > (value.maximum / 2);
}

void gap::last_branch_result(champsim::address ip, champsim::address branch_target, bool taken, uint8_t branch_type)
{
  pht[bank(ip)][global_history] += taken ? 1 : -1;
  global_history = ((global_history << 1) | static_cast<std::uint64_t>(taken)) & HISTORY_MASK;
}
