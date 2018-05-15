//
// Created by abel walga on 02/05/2018.
//
#include "odncrypto/odn_cpabe.hpp"
#include "odncrypto/algorithms.hpp"
#include "odncrypto/sscipher.hpp"

using namespace ::std;
using namespace ::odn::crypto;

const cpabe_msk odn_cpabe::generate_authority(size_t klinear) {
  // msk param
  const bilinear_pairing pairing(klinear);
  vector<field_zr> a_k;
  vector<field_zr> b_k;
  vector<field_zr> d_k;
  vector<group_g> g_k;

  // generate a <- Zp^k
  // generate b <- Zp^k
  // generate d <- Zp^(k+1)
  for (size_t i = 0; i != klinear; ++i) {
    a_k.push_back(group<field_zr>::zr().random());
    b_k.push_back(group<field_zr>::zr().random());
  }
  // choose, h,g
  const group_g g = group<group_g>::g().random();
  const group_h h = group<group_h>::h().random();

  // compute g_k[i] = g ^ d[i]
  for (size_t i = 0, l = klinear + 1; i != l; ++i) {
    d_k.push_back(group<field_zr>::zr().random());
    g_k.push_back(g ^ d_k[i]);
  }

  return cpabe_msk(pairing, g, h, a_k, b_k, d_k, g_k);
}

const cpabe_pk odn_cpabe::generate_pk(const odn::crypto::cpabe_msk &msk) {
  // pk param
  const bilinear_pairing pairing(msk.pairing_param);
  const group_h h(msk.h);
  vector<group_h> h_k;
  vector<group_gt> t_k;
  // e_gh
  const group_gt egh = group<group_gt>::gt_unity();

  // generate h_k[i] = h ^ a[i]
  // generate t_k[i] = e (g,h) ^ (a[i]*d[i] + d[k])
  for (size_t i = 0, l = msk.pairing_param.klinear(); i != l; ++i) {
    h_k.push_back(h ^ msk.a_k[i]);
    t_k.push_back(egh(msk.g, h) ^ ((msk.a_k[i] * msk.d_k[i]) + msk.d_k[l]));
  }

  return cpabe_pk(pairing, h, h_k, t_k);
}

cpabe_sk odn_cpabe::generate_sk(const odn::crypto::cpabe_msk &msk, const std::set<string> attributes) {
  const bilinear_pairing pairing(msk.pairing_param);
  vector<field_zr> r_k;  // klinear + 1
  vector<field_zr> br_k; // klinear + 1
  // sk elements
  vector<group_h> sk0;// G2[klinear + 1]
  vector<group_g> sk1;// G1[klinear + 1
  map<string, vector<group_g>> sky;// G1[size_attributes]

  /**********************************************************
   * compute sk0[i] = h ^ (r_k[i] * msk.b_k[i]) i in [0..k+1(
   **********************************************************/
  // generate r_k <- Zp^(k+1)
  // compute br_k[i] = b_k[i] * r_k[i]
  // sk0[i] = h ^ ( b[i] * r[i] )
  field_zr r_k_sum = group<field_zr>::zr_zero();
  for (size_t i = 0, l = pairing.klinear(); i != l; ++i) {
    r_k.push_back(group<field_zr>::zr().random());
    br_k.push_back(msk.b_k[i] * r_k[i]);
    sk0.push_back(msk.h ^ (br_k[i]));
    r_k_sum += r_k[i];   // SUM ( i = [0..k[, r[i])
  }
  br_k.push_back(r_k_sum);// br [k] = SUM ( i = [0..k[, r[i])
  sk0.push_back(msk.h ^ (br_k[pairing.klinear()]));// sk0[k] = h ^ br [k]

  /**********************************************************
   * compute sk1[i] = {(g^d[t]) * PI(l = [0..k+1[, Hash(concat("01", l, t)) ^ (br[l]/a[t])) *  g ^ (sigma/a[t]) t in 0..k, g ^ d[i] * g ^ (-sigma)}
   **********************************************************/
  const field_zr sigma = group<field_zr>::zr().random();
  for (size_t t = 0, l = pairing.klinear(); t != l; ++t) {
    // (g^d[t]) * PI(l = [0..k+1[, Hash(concat("01", l, t)) ^ (br[l]/a[t]))
    group_g product_hash_cst(msk.g_k[t]);

    for (size_t l = 0, i = pairing.klinear() + 1; l != i; ++l) {

      const string input_hash = string("01") + to_string(l + 1) + to_string(t + 1);
      const group_g map_hash = group<group_g>::g_map(input_hash);// Hash(concat(0, l, t)) TODO hash

      product_hash_cst += (map_hash ^ (br_k[l] / msk.a_k[t]));
    }

    product_hash_cst += (msk.g ^ (sigma / msk.a_k[t]));
    // sk1[t] = (g^d[t]) * PI(l = [0..k+1[, Hash(concat("01", l, t)) ^ (br[l]/a[t])) *  g ^ (sigma/a[t])
    sk1.push_back(product_hash_cst);
  }
  // sk1[k] = g ^ d[k] * g ^ (-sigma)
  sk1.push_back(msk.g_k[pairing.klinear()] + (msk.g ^ (-sigma)));

  /**********************************************************
   * compute sky[attr] = { PI(l = [0..k+1[, Hash(concat(attr, l, t)) ^ (br[l]/a[t])) * g ^ (sigma_attr/a[t]), g ^ (-sigma) }
   **********************************************************/
  for (auto &attr : attributes) {
    // sigma_y <- Zp
    const field_zr sigma_y = group<field_zr>::zr().random();

    for (size_t t = 0; t != pairing.klinear(); ++t) {
      // PI(l = [0..k+1[, Hash(concat(attr, l, t)) ^ (br[l]/a[t]))
      group_g product_hash_attr = group<group_g>::g_zero();
      for (size_t l = 0; l != (pairing.klinear() + 1); ++l) {

        string input_hash = attr + to_string(l + 1) + to_string(t + 1);
        // TODO crypto_generichash(sessionKey.key, KEY_SIZE, key, keylen, NULL, 0)
        const group_g map_hash = group<group_g>::g_map(input_hash);// Hash(concat(attr, l, t))
        product_hash_attr += (map_hash ^ (br_k[l] / msk.a_k[t]));
      }
      // g ^ (sigma_attr/a[t])
      product_hash_attr += (msk.g ^ (sigma_y / msk.a_k[t]));
      sky[attr].push_back(product_hash_attr);
    }
    // sk_attr[k] = g ^ (-sigma)
    sky[attr].push_back(msk.g ^ (-sigma_y));
  }
  return cpabe_sk(pairing, sk0, sk1, sky);
}

std::pair<sscipher_session, cpabe_cipher_text> odn_cpabe::encrypt_session(const odn::crypto::cpabe_pk &pk,
                                                                          const odn::crypto::binary_policy_tree &policy) {
  // generate s <-Zp^k
  vector<field_zr> s_k;// klinear + 1
  // cipher components
  vector<group_h> ct0;
  vector<group_g> cty;
  /**********************************************************
   * compute ct0[i] = h ^ s_k[i] i in [0..k+1(
   **********************************************************/
  field_zr sum_sk = group<field_zr>::zr_zero();
  for (size_t t = 0, k = pk.pairing_parm.klinear(); t != k; ++t) {
    s_k.push_back(group<field_zr>::zr().random());
    sum_sk += s_k[t];
    ct0.push_back(pk.h_k[t] ^ s_k[t]);
  }
  ct0.push_back(pk.h ^ sum_sk);
  /**********************************************************
   * compute cty
   **********************************************************/
  vector<group_g> pre_hash_table;
  // compute msp
  const msp lw_msp = lsss::lewko_water(policy);
  for (size_t j = 0, num_cols = lw_msp.depth(); j != num_cols; ++j) {// TODO check depth < size
    for (size_t l = 0, k = pk.pairing_parm.klinear(); l != k + 1; ++l) {

      for (size_t t = 0; t != k; ++t) {
        const string input_hash = string("0") + to_string(j + 1) + to_string(l + 1) + to_string(t + 1);

        pre_hash_table.push_back(group<group_g>::g_map(input_hash) ^ s_k[t]);// Hash(concat(0, col, l, t)) ^ s[t]
      }
    }
  }
  // compute msp
  auto modCol = pk.pairing_parm.klinear() * (pk.pairing_parm.klinear() + 1);
  for (auto leaf : policy.attributes()) {
    auto attribute = leaf.first;
    auto rho_attr = lw_msp.get_share(leaf.first);// TODO != -1
    auto row_share = lw_msp[rho_attr];// TODO check size < depth
    for (size_t l = 0, k = pk.pairing_parm.klinear(); l != k + 1; ++l) {

      // PI (t=[0..k],  Hash(concat(attr, col, l, t)) ^ s[t] * PI (j=[0..col],  (Hash(concat("0", col, l, t)) ^ s[t]) ^ M(attr, col)))
      group_g product_hash_attr = group<group_g>::g_zero();
      for (size_t t = 0; t != k; ++t) {

        const string hash_input = attribute + to_string(l + 1) + to_string(t + 1);
        product_hash_attr += group<group_g>::g_map(hash_input) ^ s_k[t];

        for (size_t j = 0, num_cols_cols_share = row_share.size(); j != num_cols_cols_share;
             ++j) { // TODO optimize loop
          group_g hash_attr(pre_hash_table[j * modCol + l * k + t]);// Hash(concat(0, col, l, t)) ^ s[t]
          product_hash_attr += (hash_attr
              ^ group<field_zr>::zr_map(row_share[j]));// (Hash(concat("0", col, l, t)) ^ s[t]) ^ M(attr, col)))
        }
      }
      cty.push_back(product_hash_attr);
    }
  }
  /**********************************************************
   * compute ct = PI (i=[0..k[, T[i] ^ s[i]) * gt <- GT
   **********************************************************/
  // PI (i=[0..k[, T[i] ^ s[i]) * msg
  group_gt ct = group<group_gt>::gt_unity();
  for (size_t t = 0, k = pk.pairing_parm.klinear(); t != k; ++t) {
    // T[i] ^ s[i]
    ct *= (pk.t_k[t] ^ s_k[t]);
  }
  const group_gt secret = group<group_gt>::gt().random();
  ct *= secret;

  const cpabe_cipher_text cipher_text(pk.pairing_parm, policy, ct0, cty, ct);
  // create session with secret
  return {sscipher::session(secret, cipher_text), cipher_text};
}

sscipher_session odn_cpabe::decrypt_session(const odn::crypto::cpabe_sk &sk,
                                            const odn::crypto::cpabe_cipher_text &cipher_text) {
  // TODO check pairing params
  group_gt num(cipher_text.ct);
  group_gt den = group<group_gt>::gt_unity();

  set<string> attributes;
  for (auto &sky_components : sk.sky) {
    attributes.insert(sky_components.first);
  }

  auto min_authorization = cipher_text.policy.root().prune(cipher_text.policy, attributes);
  auto lw_msp = lsss::lewko_water(cipher_text.policy);

  for (size_t l = 0, k = sk.pairing_param.klinear(); l != k + 1; ++l) {
    // PI(i=[0..col], cty[attr][l] ^ gamma[i])
    group_g ct_gamma = group<group_g>::g_zero();
    // sk1[l] * PI(i=[0..col], sk[attr[i]][l] ^ gamma[i])
    group_g sk_gamma(sk.sk1[l]);
    for (auto &attr : min_authorization) {
      auto ct_row = lw_msp.get_share(attr);// TODO check - 1

      ct_gamma += cipher_text.cty[ct_row * (k + 1) + l];
      sk_gamma += sk.sky.find(attr)->second[l];
    }
    // e(ct_gamma, sk0[l])
    num *= group<group_gt>::gt_unity()(ct_gamma, sk.sk0[l]);
    // e(sk_gamma, ct0[l])
    den *= group<group_gt>::gt_unity()(sk_gamma, cipher_text.ct0[l]);
  }
  const group_gt secret = num / den;

  // create session with secret
  return sscipher::session(secret, cipher_text);
}
