#ifndef PLATFORM_IMPL_SAMPLE_SAMPLE_WIFI_MEDIUM_H_
#define PLATFORM_IMPL_SAMPLE_SAMPLE_WIFI_MEDIUM_H_

#include "platform/api/wifi.h"

namespace location {
namespace nearby {
namespace sample {

class SampleWifiScanResult : public WifiScanResult {
 public:
  SampleWifiScanResult(const std::string& ssid,
                       std::int32_t signal_strength_dbm,
                       std::int32_t frequency_mhz,
                       WifiAuthType::Value auth_type)
      : ssid_(ssid),
        signal_strength_dbm_(signal_strength_dbm),
        frequency_mhz_(frequency_mhz),
        auth_type_(auth_type) {}
  ~SampleWifiScanResult() override {}

  std::string getSSID() const override { return ssid_; }
  std::int32_t getSignalStrengthDbm() const override {
    return signal_strength_dbm_;
  }
  std::int32_t getFrequencyMhz() const override { return frequency_mhz_; }
  WifiAuthType::Value getAuthType() const override { return auth_type_; }

 private:
  const std::string ssid_;
  const std::int32_t signal_strength_dbm_;
  const std::int32_t frequency_mhz_;
  const WifiAuthType::Value auth_type_;
};

class SampleWifiMedium : public WifiMedium {
 public:
  SampleWifiMedium();
  ~SampleWifiMedium() override;

  bool scan(Ptr<ScanResultCallback> scan_result_callback) override;
  WifiConnectionStatus::Value connectToNetwork(
      const std::string& ssid, const std::string& password,
      WifiAuthType::Value auth_type) override;
  bool verifyInternetConnectivity() override;
  std::string getIPAddress() override;

 private:
  static std::vector<SampleWifiScanResult> canned_scan_results_;

  // The SSID this Wifi stack is currently connected to; empty string if none.
  std::string current_ssid_;
};

}  // namespace sample
}  // namespace nearby
}  // namespace location

#endif  // PLATFORM_IMPL_SAMPLE_SAMPLE_WIFI_MEDIUM_H_
