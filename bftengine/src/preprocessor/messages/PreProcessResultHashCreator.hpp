// Concord
//
// Copyright (c) 2021 VMware, Inc. All Rights Reserved.
//
// This product is licensed to you under the Apache 2.0 license (the "License"). You may not use this product except in
// compliance with the Apache 2.0 License.
//
// This product may include a number of subcomponents with separate copyright notices and license terms. Your use of
// these subcomponents is subject to the terms and conditions of the subcomponent's license, as noted in the LICENSE
// file.

#pragma once

#include "crypto/openssl/hash.hpp"
#include "SharedTypes.hpp"

namespace preprocessor {

class PreProcessResultHashCreator {
 public:
  static concord::crypto::openssl::SHA3_256::Digest create(const char* preProcessResultData,
                                                           uint32_t preProcessResultLen,
                                                           bftEngine::OperationResult preProcessResult,
                                                           uint16_t clientId,
                                                           ReqId reqSeqNum) {
    auto dataDigest = concord::crypto::openssl::SHA3_256();
    dataDigest.init();
    if (preProcessResult == bftEngine::OperationResult::SUCCESS)
      dataDigest.update(preProcessResultData, preProcessResultLen);
    dataDigest.update(&preProcessResult, sizeof(preProcessResult));
    dataDigest.update(&clientId, sizeof(clientId));
    dataDigest.update(&reqSeqNum, sizeof(reqSeqNum));
    return dataDigest.finish();
  }
};

}  // namespace preprocessor
