#include <iostream>
#include <sstream>
#include <set>
#include <string>
#include <tins/tins.h>
#include <curl/curl.h>

using std::set;
using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::runtime_error;

using namespace Tins;

class ScopeExit {
public:
  ScopeExit(std::function<void()> f): f_(f) {}
  ~ScopeExit() { f_(); }
private:
  std::function<void()> f_;
};

struct PostData {
  const char *ptr;
  size_t size;
};

size_t read_data(void *ptr, size_t size, size_t nmemb, void *userp) {
  PostData *post_data = (PostData*)userp;

  size_t byte_len = size * nmemb;
  if (post_data->size < byte_len) {
    byte_len = post_data->size;
  }
  memcpy(ptr, post_data->ptr, byte_len);
  post_data->ptr += byte_len;
  post_data->size -= byte_len;
  return byte_len;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, void *userp) {
  FILE *f = (FILE*)userp;
  return fwrite(ptr, size, nmemb, f);
}

void post(const char *message) {
  // Get a curl object
  CURL *curl = curl_easy_init();
  if (!curl) {
    fprintf(stderr, "curl_easy_init() failed\n");
    return;
  }
  ScopeExit curl_close([&curl] {
    curl_easy_cleanup(curl);
  });

//   curl_easy_setopt(curl, CURLOPT_HTTPHEADER, "Accept: application/json");

  // Set url
  curl_easy_setopt(curl, CURLOPT_URL, "http://<raspi IP>:3000/captures");  //<raspi IP>をラズパイのIPアドレスに書き換える
  // Set HTTP method to POST
  curl_easy_setopt(curl, CURLOPT_POST, 1L);

  // Set callback on sending data
  PostData post_data;
  post_data.ptr = message;
  post_data.size = strlen(message);
  curl_easy_setopt(curl, CURLOPT_READDATA, &post_data);
  curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_data);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(message));

  // Set callback on receiving data
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, stdout);

  // Execute
  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
  }
}

class BeaconSniffer {
public:
    void run(const string& iface);
private:
    typedef Dot11::address_type address_type;
    typedef set<address_type> ssids_type;
 
    bool callback(PDU& pdu);
     
    ssids_type ssids;
};
 
void BeaconSniffer::run(const std::string& iface) {
    SnifferConfiguration config;
    config.set_promisc_mode(true);
    config.set_filter("type mgt subtype probe-req");
    config.set_rfmon(true);
    Sniffer sniffer(iface, config);
    sniffer.sniff_loop(make_sniffer_handler(this, &BeaconSniffer::callback));
}
 
bool BeaconSniffer::callback(PDU& pdu) {
    // Get the Dot11 layer
    const Dot11ManagementFrame& beacon = pdu.rfind_pdu<Dot11ManagementFrame>();
    // All beacons must have from_ds == to_ds == 0
    if (!beacon.from_ds() && !beacon.to_ds()) {
        // Get the AP address
        address_type addr = beacon.addr2();
        // Look it up in our set
        ssids_type::iterator it = ssids.find(addr);
        if (it == ssids.end()) {
            // First time we encounter this BSSID.
            try {
                /* If no ssid option is set, then Dot11::ssid will throw 
                 * a std::runtime_error.
                 */
                string ssid = beacon.ssid();
                // Save it so we don't show it again.
                ssids.insert(addr);

                if(ssid == "<SSID>"){  //<SSID>をアクセスポイントのSSIDに書き換える
                    // Display the tuple "address - ssid".
                    cout << addr << " - " << ssid << endl;

                    std::stringstream post_data;

                    post_data << "capture[capture]=" << addr;

                    post(post_data.str().c_str());
                }
            }
            catch (runtime_error&) {
                // No ssid, just ignore it.
            }
        }
    }
    return true;
}
 
int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: " <<* argv << " <interface>" << endl;
        return 1;
    }
    string interface = argv[1];
    BeaconSniffer sniffer;
    sniffer.run(interface);
}