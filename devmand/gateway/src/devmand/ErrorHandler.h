// Copyright (c) Facebook, Inc. and its affiliates. All Rights Reserved.

#pragma once

#include <functional>
#include <string>

#include <folly/Format.h>
#include <folly/futures/Future.h>

namespace devmand {

class ErrorHandler final {
 public:
  ErrorHandler() = delete;
  virtual ~ErrorHandler() = delete;
  ErrorHandler(const ErrorHandler&) = delete;
  ErrorHandler& operator=(const ErrorHandler&) = delete;
  ErrorHandler(ErrorHandler&&) = delete;
  ErrorHandler& operator=(ErrorHandler&&) = delete;

 public:
  static void executeWithCatch(
      std::function<void()> runable,
      std::function<void()> onFailure = []() {});

  template <class Future>
  static auto thenError(Future&& f, std::function<void()> onFailure = []() {}) {
    return std::move(f)
        .thenError(
            folly::tag_t<std::exception>{},
            [onFailure](std::exception const& e) {
              onFailure();
              LOG(ERROR) << "Caught exception from future: " << e.what();
              trace();
            })
        .thenError([onFailure](folly::exception_wrapper) {
          onFailure();
          LOG(ERROR) << "Caught unknown exception from future";
          trace();
        });
  }

  static std::string getErrorMsg(
      const std::string& device,
      const std::string& channel,
      const std::string& path,
      const std::string& context);

 private:
  static void trace();

 private:
  constexpr const static char* errorTemplate =
      "Error on device {} channel {} for path {} with context {}.";
};

} // namespace devmand
