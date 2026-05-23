#include "pseudo_lru.h"

#include <cassert>

pseudo_lru::pseudo_lru(CACHE* cache) : pseudo_lru(cache, cache->NUM_SET, cache->NUM_WAY) {}

pseudo_lru::pseudo_lru(CACHE* cache, long sets, long ways)
    : replacement(cache), NUM_SET(sets), NUM_WAY(ways),
      tree_bits(static_cast<std::size_t>(sets),
                std::vector<uint8_t>(static_cast<std::size_t>(ways - 1), 0))
{
  // Tree-PLRU предполагает степень двойки: 2, 4, 8, ...
  assert(NUM_WAY > 1);
  assert((NUM_WAY & (NUM_WAY - 1)) == 0);
}

long pseudo_lru::find_victim(uint32_t triggering_cpu, uint64_t instr_id, long set,
                             const champsim::cache_block* current_set,
                             champsim::address ip, champsim::address full_addr,
                             access_type type)
{
  long node = 0;
  long left = 0;
  long right = NUM_WAY - 1;

  while (left != right) {
    long mid = (left + right) / 2;

    // bit = 0: victim direction is left
    // bit = 1: victim direction is right
    if (tree_bits.at(static_cast<std::size_t>(set)).at(static_cast<std::size_t>(node)) == 0) {
      node = 2 * node + 1;
      right = mid;
    } else {
      node = 2 * node + 2;
      left = mid + 1;
    }
  }

  return left;
}

void pseudo_lru::touch(long set, long way)
{
  long node = 0;
  long left = 0;
  long right = NUM_WAY - 1;

  while (left != right) {
    long mid = (left + right) / 2;

    if (way <= mid) {
      // Обращение ушло влево, значит в следующий раз предпочитать правую ветку.
      tree_bits.at(static_cast<std::size_t>(set)).at(static_cast<std::size_t>(node)) = 1;
      node = 2 * node + 1;
      right = mid;
    } else {
      // Обращение ушло вправо, значит в следующий раз предпочитать левую ветку.
      tree_bits.at(static_cast<std::size_t>(set)).at(static_cast<std::size_t>(node)) = 0;
      node = 2 * node + 2;
      left = mid + 1;
    }
  }
}

void pseudo_lru::replacement_cache_fill(uint32_t triggering_cpu, long set, long way,
                                        champsim::address full_addr, champsim::address ip,
                                        champsim::address victim_addr, access_type type)
{
  touch(set, way);
}

void pseudo_lru::update_replacement_state(uint32_t triggering_cpu, long set, long way,
                                          champsim::address full_addr, champsim::address ip,
                                          champsim::address victim_addr, access_type type,
                                          uint8_t hit)
{
  if (hit && access_type{type} != access_type::WRITE)
    touch(set, way);
}
