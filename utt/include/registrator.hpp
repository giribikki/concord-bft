// UTT
//
// Copyright (c) 2020-2022 VMware, Inc. All Rights Reserved.
//
// This product is licensed to you under the Apache 2.0 license (the "License").
// You may not use this product except in compliance with the Apache 2.0
// License.
//
// This product may include a number of subcomponents with separate copyright
// notices and license terms. Your use of these subcomponents is subject to the
// terms and conditions of the sub-component's license, as noted in the LICENSE
// file.

#pragma once
#include "commitment.hpp"
#include "UTTParams.hpp"
#include <string>
#include <memory>
#include <vector>
#include <map>
namespace libutt {
class RegAuthShareSK;
class RegAuthPK;
class RegAuthSK;
class RegAuthSharePK;
}  // namespace libutt
namespace libutt::api {
class Registrator {
  /**
   * @brief The registrator replica object responsible for the (distributed) registration procedure
   *
   */
 public:
  /**
   * @brief Construct a new Registrator replica object
   *
   * @param id The replica ID
   * @param registartor_secret_key The replica's serialized share of the secret registration key
   * @param validation_keys Other replicas verification keys
   * @param registration_public_key The serialized registration public key
   */
  Registrator(uint16_t id,
              const std::string& registartor_secret_key,
              const std::map<uint16_t, std::string>& validation_keys,
              const std::string& registration_public_key);

  /**
   * @brief Sign the user RCM (also marked as rcm1)
   *
   * @param pid_hash The client's id hash as a CurvePoint
   * @param s2 The replica's part of the secret. In the distributed case s2 much be deterministic among all replicas. It
   * doesn't have to be a secret but it does need to be unpredictable
   * @param rcm1 The user commitment on its own s1
   * @return std::pair<types::CurvePoint, types::Signature> returns the choosen s2 and the share signature
   */
  std::pair<types::CurvePoint, types::Signature> signRCM(const types::CurvePoint& pid_hash,
                                                         const types::CurvePoint& s2,
                                                         const Commitment& rcm1) const;

  /**
   * @brief Validates a given full RCM against a given signature. In the regular flow the rcm is validated as part of
   * the transaction's split proof. So this method is mostly for testing
   *
   * @param comm The user full RCM
   * @param sig The user's RCM signature
   * @return true if valid
   * @return false if not
   */
  bool validateRCM(const Commitment& comm, const types::Signature& sig) const;

  /**
   * @brief Validates a partial signature generated by other replica
   *
   * @param id the id of the signature generator
   * @param pid_hash the pid hash of the client whom we validates its rcm
   * @param s2 The replica's part of the secret. In the distributed case s2 much be deterministic among all replicas. It
   * doesn't have to be a secret but it does need to be unpredictable
   * @param rcm1 The user commitment on its own s1
   * @param sig the partial signature to validate
   * @return true if valid
   * @return false if not
   */
  bool validatePartialRCMSig(uint16_t id,
                             const types::CurvePoint& pid_hash,
                             const types::CurvePoint& s2,
                             const Commitment& rcm1,
                             const types::Signature& sig) const;
  uint16_t getId() const { return id_; }

 private:
  std::uint16_t id_;
  std::unique_ptr<RegAuthShareSK> rsk_;
  std::unique_ptr<RegAuthPK> rpk_;
  std::map<uint16_t, std::unique_ptr<RegAuthSharePK>> validation_keys_;
};
}  // namespace libutt::api