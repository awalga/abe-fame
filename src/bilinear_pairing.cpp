//
// Created by abel walga on 02/05/2018.
//

#include "odncrypto/bilinear_pairing.hpp"
#include "odncrypto/group.hpp"

using namespace ::odn::crypto;

void bilinear_pairing::init_pairing(odn::crypto::bilinear_pairing &pairing) {
  int result = core_init();
  if (result != STS_OK) {
    core_clean();
    THROW(ERR_NO_VALID);
  }
  pc_param_set_any();
  pc_param_print();

  pairing.order_g1 = group<group_g>::order();
  pairing.order_g2 = group<group_h>::order();
  pairing.order_gt = group<group_gt>::order();
}

bool bilinear_pairing::operator==(const bilinear_pairing &rhs) const {
  return k == rhs.k &&
      order_g1 == rhs.order_g1 &&
      order_g2 == rhs.order_g2 &&
      order_gt == rhs.order_gt;
}
bool bilinear_pairing::operator!=(const bilinear_pairing &rhs) const {
  return !(rhs == *this);
}