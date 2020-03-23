#ifndef CORE_INTERNAL_BLE_ENDPOINT_CHANNEL_H_
#define CORE_INTERNAL_BLE_ENDPOINT_CHANNEL_H_

#include "core/internal/base_endpoint_channel.h"
#include "core/internal/medium_manager.h"
#include "platform/api/ble.h"
#include "platform/port/string.h"
#include "platform/ptr.h"
#include "proto/connections_enums.pb.h"

namespace location {
namespace nearby {
namespace connections {

template <typename Platform>
class BLEEndpointChannel : public BaseEndpointChannel<Platform> {
 public:
  static Ptr<BLEEndpointChannel<Platform> > createOutgoing(
      Ptr<MediumManager<Platform> > medium_manager, const string& channel_name,
      Ptr<BLESocket> ble_socket);
  static Ptr<BLEEndpointChannel<Platform> > createIncoming(
      Ptr<MediumManager<Platform> > medium_manager, const string& channel_name,
      Ptr<BLESocket> ble_socket);

  ~BLEEndpointChannel() override;

  proto::connections::Medium getMedium() override;

 protected:
  void closeImpl() override;

 private:
  BLEEndpointChannel(const string& channel_name, Ptr<BLESocket> ble_socket);

  ScopedPtr<Ptr<BLESocket> > ble_socket_;
};

}  // namespace connections
}  // namespace nearby
}  // namespace location

#include "core/internal/ble_endpoint_channel.cc"

#endif  // CORE_INTERNAL_BLE_ENDPOINT_CHANNEL_H_
