#include <iostream>
#include <list>
#include <memory>
#include "sigslot.h"

class Test
{
private:
    std::thread th;
    std::mutex mtx;
    sigslot::signal<> sig;
    std::atomic<bool> running;

public:
    Test()
    {
        running = true;
        th = std::thread(&Test::run, this);
        sig.connect(this, &Test::fun, sigslot::connection_type::queued_connection, TQ("sigslot_thread"));
    }

    ~Test()
    {
        running = false;
        th.join();
    }

    void run()
    {
        while (running)
        {
            std::unique_lock<std::mutex> lock(mtx);
            std::clog << "sig emit ThreadID: " << std::this_thread::get_id() << std::endl;
            std::clog.flush();
            sig();
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    void fun()
    {
        std::unique_lock<std::mutex> lock(mtx);
        std::clog << "slot handle ThreadID: " << std::this_thread::get_id() << std::endl;
        std::clog.flush();
    }
};

struct DeviceInfo {
    std::string deviceId;
    std::string deviceName;
};

// Slot object with shared_ptr
class UiController1 {
public:
    ~UiController1() {}

    void onDevicePlugged(const std::shared_ptr<DeviceInfo>& info) {
        std::cout << "onDevicePlugged: " << info->deviceName << std::endl;
    }
};

// Slot object with observer
class UiController2 : public sigslot::observer {
public:
    ~UiController2() {
        // Needed to ensure proper disconnection prior to object destruction in multithreaded contexts.
        this->disconnect_all();
    }

    void onDevicePlugged(const std::shared_ptr<DeviceInfo>& info) {
        std::cout << "onDevicePlugged: " << info->deviceName << std::endl;
    }
};

class DeviceController {
public:
    std::list<std::shared_ptr<DeviceInfo>> getDeviceList();

    void mockCallback() {
        auto deviceInfo = std::make_shared<DeviceInfo>();
        deviceInfo->deviceId = "uuid-12345678900987654321";
        deviceInfo->deviceName = "microphone";
        onDeviceEventTriggered(deviceInfo);
    }
private:
    void onDeviceEventTriggered(const std::shared_ptr<DeviceInfo>& info) {
        pluggedSignal(info);
    }

public:
    sigslot::signal<const std::shared_ptr<DeviceInfo>&> pluggedSignal;
};

int main()
{
    // example
    // TQMgr->create({"sigslot_thread"});
    // Test ts;
    // while(1);
    // return 0;

    // create a task queue
    TQMgr->create({"worker"});

    auto dc = std::make_shared<DeviceController>();

    // example 1, slot object with shared_ptr
    auto ui1 = std::make_shared<UiController1>();
    dc->pluggedSignal.connect(ui1.get(), &UiController1::onDevicePlugged, sigslot::connection_type::auto_connection, TQ("worker"));

    // example 2, slot object with observer
    UiController2 ui2;
    dc->pluggedSignal.connect(&ui2, &UiController2::onDevicePlugged, sigslot::connection_type::queued_connection, TQ("worker"));

    dc->mockCallback();

    // expamle 3
    sigslot::signal<int> printSignal;

    printSignal.connect([](int x){
        std::cout << "Hello World: " << x << std::endl;
    }, sigslot::connection_type::blocking_queued_connection, TQ("worker"));

    printSignal(5);

    return 0;
}
