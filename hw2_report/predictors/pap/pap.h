#ifndef BRANCH_PAP_H
#define BRANCH_PAP_H

#include <array>
#include <cstdint>

#include "address.h"
#include "modules.h"
#include "msl/fwcounter.h"

class pap : champsim::modules::branch_predictor
{
  static constexpr std::size_t HISTORY_BITS = 4;
  static constexpr std::size_t PHT_ENTRIES = 1 << HISTORY_BITS;
  static constexpr std::size_t NUM_BANKS = 1024;
  static constexpr std::size_t PRIME = 1021;
  static constexpr std::size_t BITS = 2;
  static constexpr std::uint64_t HISTORY_MASK = PHT_ENTRIES - 1;

  std::array<std::uint64_t, NUM_BANKS> local_history {};
  std::array<std::array<champsim::msl::fwcounter<BITS>, PHT_ENTRIES>, NUM_BANKS> pht;

  [[nodiscard]] static constexpr auto bank(champsim::address ip)
  {
    return ip.to<unsigned long>() % PRIME;
  }

public:
  using branch_predictor::branch_predictor;

  bool predict_branch(champsim::address ip);
  void last_branch_result(champsim::address ip, champsim::address branch_target, bool taken, uint8_t branch_type);
};

#endif
