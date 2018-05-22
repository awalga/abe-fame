//
// Created by abel walga on 02/05/2018.
//
#include "odncrypto/relic_import.h"
#include "odncrypto/group_arithmetics.hpp"
#include "odncrypto/group.hpp"

using namespace ::odn::crypto;

void group_arithmetics::mod_div(field_zr &quotient, const field_zr &num, const field_zr &den) {
  field_zr inv_den, c, d, e;

  bn_gcd_ext(c.z, d.z, e.z, den.z, den.r);// gcd : c = d*a + e*b

  field_zr one;
  bn_set_dig(one.z, 1);
  if (bn_cmp(one.z, c.z) == CMP_EQ) {
    // ((d % b) + b)%b
    bn_mod(inv_den.z, d.z, den.r);
    bn_add(inv_den.z, inv_den.z, den.r);
    bn_mod(inv_den.z, inv_den.z, den.r);

    bn_mul(quotient.z, num.z, inv_den.z);
    bn_mod(quotient.z, quotient.z, quotient.r);
  } else {

    THROW(ERR_NO_VALID);
  }
}

void group_arithmetics::sum(odn::crypto::group_g &product,
                            const odn::crypto::group_g &term,
                            const odn::crypto::field_zr &n) {
  g1_mul(product.g1, term.g1, n.z);
}

void group_arithmetics::sum(odn::crypto::group_h &product,
                            const odn::crypto::group_h &term,
                            const odn::crypto::field_zr &n) {
  // temporary(term, std::bind(ep2_mul, std::ref(product), std::placeholders::_1, std::ref(n)));
  g2_mul(product.g2, const_cast<g2_t &>(term.g2), n.z);
}

void group_arithmetics::product(odn::crypto::group_gt &product,
                                const odn::crypto::group_gt &term,
                                const odn::crypto::field_zr &pow) {
  gt_exp(product.gt, const_cast<gt_t &>(term.gt), const_cast<bn_t &>(pow.z));
}

void group_arithmetics::pair(odn::crypto::group_gt &gt, const odn::crypto::group_g &g, const odn::crypto::group_h &h) {
  pc_map(gt.gt, const_cast<g1_t &>(g.g1), const_cast<g2_t &>(h.g2));
}

