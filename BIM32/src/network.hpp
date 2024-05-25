#include <ESP32Ping.h> // v1.6 https://github.com/marian-craciunescu/ESP32Ping

class Network {
  private:
    bool needToPing = true;

  public:
    void setNeedToPing(void);
    bool isConnected(void);
    void connect(void);
    void runAccessPoint(void);
};

/**
 * Checking for internet access
 */
bool Network::isConnected(void) {
  if(WiFi.status() == WL_CONNECTED) {
    if(needToPing) {
      needToPing = false;
      return Ping.ping(REMOTE_HOST);
    }
    else return true;
  }
  else return false;
}

/*
 * Set needToPing
 */
void Network::setNeedToPing(void) {
  needToPing = true;
}

/**
 * Connect to WiFi network
 */
void Network::connect(void) {
  WiFi.disconnect();
  Wifi.hostname("BIM32");
  Serial.println(SEPARATOR);
  Serial.println("Connecting to WiFi...");
  Serial.println("Known networks:");
  for(unsigned int i=0; i<NETWORKS; i++) {
    if(String(config.network_ssid(i)) != "") {
      Serial.printf("\tNet%d: %s\r\n", i, config.network_ssid(i));
    }
  }
  WiFi.mode(WIFI_STA);
  bool anySSID = false;
  for(unsigned int i=0; i<NETWORKS; i++) {
    if(String(config.network_ssid(i)) != "") anySSID = true;
  }
  if(anySSID) {
    if(WiFi.status() != WL_CONNECTED) {
      unsigned int numberOfNetworks = WiFi.scanNetworks();
      if(numberOfNetworks > 0) {
        int knownNetwork = -1;
        for(int i=0; i<numberOfNetworks; i++) {
          for(int k=0; k<NETWORKS; k++) {
            if(WiFi.SSID(i) == config.network_ssid(k)) {
              knownNetwork = k;
              break;
            }
          }  
        }
        if(knownNetwork >= 0) {
          Serial.print("Connecting to "); Serial.println(config.network_ssid(knownNetwork));
          WiFi.begin(config.network_ssid(knownNetwork), config.network_pass(knownNetwork));
          unsigned int attempts = 0;
          while(WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
            if(++attempts > 20) break;
          }
          if(WiFi.status() == WL_CONNECTED) Serial.println(" connected");
          else Serial.println(" failed");
        }
        else Serial.println("No known networks found"); 
      }
      else Serial.println("No networks found");
    }
    if(WiFi.status() == WL_CONNECTED) {
      WiFi.setAutoConnect(true);
      WiFi.setAutoReconnect(true);
      WiFi.persistent(true); 
      IPAddress ip;
      IPAddress subnet;
      IPAddress gateway;
      IPAddress dns1;
      IPAddress dns2;
      if(config.network_type()) {
        Serial.println("Configure static IP address");
        if(ip.fromString(config.network_ip()) and 
           gateway.fromString(config.network_gw()) and subnet.fromString(config.network_mask()) and
           dns1.fromString(config.network_dns1()) and dns2.fromString(config.network_dns2())
        ) WiFi.config(ip, gateway, subnet, dns1, dns2);
        else Serial.println("Error, switching back to dynamic IP");
      }
      Serial.print("IP address: "); Serial.println(WiFi.localIP().toString());
    }
  }
  else {
    Serial.println("No WiFi networks configured");
    runAccessPoint();
  }
}

/**
 * Start Access point
 */
void Network::runAccessPoint(void) {
  if(!global.apMode) {
    global.apMode = true;
    Serial.println();
    Serial.println(SEPARATOR);
    Serial.println("Start Access Point mode");
    Serial.print("Network: "); Serial.println(config.accessPoint_ssid());
    Serial.print("Password: "); Serial.println(config.accessPoint_pass());
    Serial.println("IP address: 192.168.4.1");
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.softAP(config.accessPoint_ssid(), config.accessPoint_pass());
    Serial.println(SEPARATOR);
    Serial.println();
  }
}
