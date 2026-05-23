#include "lru_lip.h"

#include <algorithm>
#include <cassert>

lru_lip::lru_lip(CACHE* cache) : lru_lip(cache, cache->NUM_SET, cache->NUM_WAY) {}

lru_lip::lru_lip(CACHE* cache, long sets, long ways)
    : replacement(cache), NUM_WAY(ways),
      last_used_cycles(static_cast<std::size_t>(sets * ways), 0)
{
}

long lru_lip::find_victim(uint32_t triggering_cpu, uint64_t instr_id, long set,
                          const champsim::cache_block* current_set,
                          champsim::address ip, champsim::address full_addr,
                          access_type type)
{
  auto begin = std::next(std::begin(last_used_cycles), set * NUM_WAY);
  auto end = std::next(begin, NUM_WAY);

  auto victim = std::min_element(begin, end);
  assert(begin <= victim);
  assert(victim < end);

  return std::distance(begin, victim);
}

void lru_lip::replacement_cache_fill(uint32_t triggering_cpu, long set, long way,
                                     champsim::address full_addr, champsim::address ip,
                                     champsim::address victim_addr, access_type type)
{
  auto begin = std::next(std::begin(last_used_cycles), set * NUM_WAY);
  auto end = std::next(begin, NUM_WAY);

  auto min_value = *std::min_element(begin, end);

  // LIP: вставляем новый блок как LRU, а не как MRU.
  last_used_cycles.at(static_cast<std::size_t>(set * NUM_WAY + way)) = min_value - 1;
}

void lru_lip::update_replacement_state(uint32_t triggering_cpu, long set, long way,
                                       champsim::address full_addr, champsim::address ip,
                                       champsim::address victim_addr, access_type type,
                                       uint8_t hit)
{
  // На hit ведём себя как обычный LRU.
  if (hit && access_type{type} != access_type::WRITE)
    last_used_cycles.at(static_cast<std::size_t>(set * NUM_WAY + way)) = cycle++;
}
