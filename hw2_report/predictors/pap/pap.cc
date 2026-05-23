#include "pap.h"

bool pap::predict_branch(champsim::address ip)
{
  auto b = bank(ip);
  auto history = local_history[b];

  auto value = pht[b][history];
  return value.value() > (value.maximum / 2);
}

void pap::last_branch_result(champsim::address ip, champsim::address branch_target, bool taken, uint8_t branch_type)
{
  auto b = bank(ip);
  auto history = local_history[b];

  pht[b][history] += taken ? 1 : -1;
  local_history[b] = ((history << 1) | static_cast<std::uint64_t>(taken)) & HISTORY_MASK;
}
