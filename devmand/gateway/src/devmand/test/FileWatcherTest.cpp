// Copyright (c) Facebook, Inc. and its affiliates. All Rights Reserved.

#include <experimental/filesystem>
#include <iostream>

#include <devmand/FileWatcher.h>
#include <devmand/test/EventBaseTest.h>
#include <devmand/test/Notifier.h>

namespace devmand {
namespace test {

class FileWatcherTest : public EventBaseTest {
 public:
  FileWatcherTest() = default;

  ~FileWatcherTest() override = default;
  FileWatcherTest(const FileWatcherTest&) = delete;
  FileWatcherTest& operator=(const FileWatcherTest&) = delete;
  FileWatcherTest(FileWatcherTest&&) = delete;
  FileWatcherTest& operator=(FileWatcherTest&&) = delete;

 protected:
  std::function<void(FileWatchEvent)> eventHandler =
      [this](FileWatchEvent watchEvent) {
        std::cerr << "Event " << static_cast<int>(watchEvent.event) << " with #"
                  << expectedNumEvents << " expected remaining" << std::endl;
        EXPECT_LE(1, expectedNumEvents);
        events.push_back(watchEvent);
        if (--expectedNumEvents == 0) {
          eventNotifier.notify();
        }
      };

  std::function<void(FileWatchEvent)> failEventHandler = [](FileWatchEvent) {
    FAIL() << "Unexpected event";
  };

  void expectEvent(FileWatchEvent watchEvent) {
    EXPECT_EQ(watchEvent.event, events.front().event);
    EXPECT_EQ(watchEvent.filename, events.front().filename);
    events.pop_front();
  }

  std::experimental::filesystem::path getFileToWatch(
      const std::string& dir = "") const {
    std::string ret = "/tmp/";
    if (not dir.empty()) {
      ret += dir + "/";
    }
    auto* testInfo = ::testing::UnitTest::GetInstance()->current_test_info();
    ret += testInfo->name();
    return ret;
  }

 protected:
  Notifier eventNotifier;
  std::list<FileWatchEvent> events;
  unsigned int expectedNumEvents = 0;
};

TEST_F(FileWatcherTest, InitialEvent) {
  FileWatcher watcher(eventBase);
  auto filepath = getFileToWatch();
  EXPECT_TRUE(FileUtils::touch(filepath));

  expectedNumEvents = 3;
  EXPECT_TRUE(watcher.addWatch(filepath, eventHandler, true));
  eventNotifier.wait();
  EXPECT_EQ(3, events.size());
  expectEvent({FileEvent::Open, ""});
  expectEvent({FileEvent::Attrib, ""});
  expectEvent({FileEvent::CloseWrite, ""});
}

TEST_F(FileWatcherTest, InitialNoEvent) {
  FileWatcher watcher(eventBase);
  FileWatcher watcher2(eventBase);
  auto filepath = getFileToWatch();
  auto filepath2 = filepath.native() + "2";
  EXPECT_TRUE(FileUtils::touch(filepath));
  EXPECT_TRUE(FileUtils::touch(filepath2));

  expectedNumEvents = 3;
  EXPECT_TRUE(watcher.addWatch(filepath, failEventHandler, false));
  EXPECT_TRUE(watcher2.addWatch(filepath2, eventHandler, true));
  eventNotifier.wait();
  EXPECT_EQ(3, events.size());
  expectEvent({FileEvent::Open, ""});
  expectEvent({FileEvent::Attrib, ""});
  expectEvent({FileEvent::CloseWrite, ""});
}

TEST_F(FileWatcherTest, EventsAfterInitial) {
  FileWatcher watcher(eventBase);
  auto filepath = getFileToWatch();
  EXPECT_TRUE(FileUtils::touch(filepath));

  expectedNumEvents = 3;
  EXPECT_TRUE(watcher.addWatch(filepath, eventHandler, true));
  eventNotifier.wait();
  EXPECT_EQ(3, events.size());
  expectEvent({FileEvent::Open, ""});
  expectEvent({FileEvent::Attrib, ""});
  expectEvent({FileEvent::CloseWrite, ""});

  expectedNumEvents = 3;
  EXPECT_TRUE(FileUtils::touch(filepath));
  eventNotifier.wait();
  EXPECT_EQ(3, events.size());
  expectEvent({FileEvent::Open, ""});
  expectEvent({FileEvent::Attrib, ""});
  expectEvent({FileEvent::CloseWrite, ""});
}

TEST_F(FileWatcherTest, InitialEventOnDir) {
  FileWatcher watcher(eventBase);
  auto filepath = getFileToWatch("fwtest");
  auto dirname = filepath.parent_path();
  EXPECT_TRUE(FileUtils::mkdir(dirname));
  EXPECT_TRUE(FileUtils::touch(filepath));

  expectedNumEvents = 2;
  EXPECT_TRUE(watcher.addWatch(dirname, eventHandler, true));
  eventNotifier.wait();
  EXPECT_EQ(2, events.size());
  expectEvent({FileEvent::Attrib, ""});
  expectEvent({FileEvent::IsDir, ""});
}

TEST_F(FileWatcherTest, EventsAfterInitialOnDir) {
  FileWatcher watcher(eventBase);
  auto filepath = getFileToWatch("fwtest");
  auto dirname = filepath.parent_path();
  EXPECT_TRUE(FileUtils::mkdir(dirname));
  EXPECT_TRUE(FileUtils::touch(filepath));

  expectedNumEvents = 2;
  EXPECT_TRUE(watcher.addWatch(dirname, eventHandler, true));
  eventNotifier.wait();
  EXPECT_EQ(2, events.size());
  expectEvent({FileEvent::Attrib, ""});
  expectEvent({FileEvent::IsDir, ""});

  expectedNumEvents = 3;
  EXPECT_TRUE(FileUtils::touch(filepath));
  eventNotifier.wait();
  EXPECT_EQ(3, events.size());
  expectEvent({FileEvent::Open, filepath.filename()});
  expectEvent({FileEvent::Attrib, filepath.filename()});
  expectEvent({FileEvent::CloseWrite, filepath.filename()});
}

} // namespace test
} // namespace devmand
