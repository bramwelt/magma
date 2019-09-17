// Copyright (c) Facebook, Inc. and its affiliates. All Rights Reserved.

#pragma once

#include <string>

#include <folly/io/async/AsyncSocket.h>

namespace devmand {
namespace channels {
namespace mikrotik {

class Channel;

// This is a simple class which handles the lifetime of the write buffer.
class WriteTask final : public folly::AsyncWriter::WriteCallback {
 public:
  WriteTask(Channel& channel_, const std::string& buffer_);
  WriteTask() = delete;
  virtual ~WriteTask() = default;
  WriteTask(const WriteTask&) = delete;
  WriteTask& operator=(const WriteTask&) = delete;
  WriteTask(WriteTask&&) = default;
  WriteTask& operator=(WriteTask&&) = delete;

 public:
  using Id = unsigned int;
  Id getId() const;

  void writeTo(std::shared_ptr<folly::AsyncSocket>& socket);

 private:
  void writeSuccess() noexcept override;
  void writeErr(
      size_t bytesWritten,
      const folly::AsyncSocketException& ex) noexcept override;

  static Id getNextId();

 private:
  Channel& channel;
  std::string buffer;
  Id id;

  static Id guid;
};

} // namespace mikrotik
} // namespace channels
} // namespace devmand
