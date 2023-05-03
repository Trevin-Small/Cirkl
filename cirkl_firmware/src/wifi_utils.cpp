#include "secrets.h"
#include "ui.h"
#include "TRGB.h"
#include "wifi_utils.h"

#include "WiFi.h"
#include "time.h"
#include "HTTPClient.h"
#include <Arduino_JSON.h>

// All WiFi tasks are handled here, including time, weather, and gallery updates
void wifi_task(void *param) {

  WiFi.mode(WIFI_STA);
  delay(100);
  Serial.println("scan start");
  delay(500);
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
    Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
    }
  }
  Serial.println("");
  WiFi.disconnect();

  //WifiLocation location(MAPS_API_KEY);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    vTaskDelay(100);
  }

  Serial.printf("\r\n-- wifi connect success! --\r\n");
  Serial.println("Connected to: " + WiFi.SSID());

  //location_t loc = location.getGeoFromWiFi();
  //Serial.printf("Lat: %f, Lon: %f\n", loc.lat, loc.lon);

  const char* ntpServer = "pool.ntp.org";
  const long  gmtOffset_sec = -18000;
  const int   daylightOffset_sec = 3600;
  struct tm timeinfo;

  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  String str;
  HTTPClient http_client;

  delay(100);
  String rsp;
  bool is_get_http = false;

  unsigned long time_millis = 0;
  unsigned long weather_millis = 0;
  char time_buf[6];
  char date_buf[24];
  int icon_data[2];

  JSONVar weather_data;

  do {

    if (millis() - time_millis > 5000 || time_millis == 0) {
      if(!getLocalTime(&timeinfo)){
        Serial.println("Failed to obtain time");
      } else {

        int hour = timeinfo.tm_hour;

        if (hour == 0) {
          hour = 12;
        } else if (hour > 12) {
          hour -= 12;
        }

        sprintf(time_buf, "%d:%02d", hour, timeinfo.tm_min);
        lv_msg_send(MSG_TIME_UPDATE, time_buf);

        strftime(date_buf, sizeof(date_buf), "%A, %b %d", &timeinfo);
        lv_msg_send(MSG_DATE_UPDATE, date_buf);

        if (System.is_asleep) {
          lv_refr_now(System.display);
        }

      }

      time_millis = millis();
    }


    if (millis() - weather_millis > 1200000 || weather_millis == 0) {
      //loc = location.getGeoFromWiFi();
      std::string weather_url = "https://api.openweathermap.org/data/2.5/weather?lat=" + trgb.getLatitude() + "&lon=" + trgb.getLongitude() + "&appid=" + std::string(WEATHER_API_KEY) + "&units=imperial";
      Serial.println(weather_url.c_str());
      http_client.begin(weather_url.c_str());
      int http_code = http_client.GET();
      Serial.printf("HTTP get code: %d\n", http_code);

      if (http_code > 0) {
        if (http_code == HTTP_CODE_OK) {
          rsp = http_client.getString();
          Serial.println(rsp);
          weather_data = JSON.parse(rsp);

          String temperature = JSON.stringify(weather_data["main"]["temp"]).substring(0, 2) + "F";
          lv_msg_send(MSG_TEMPERATURE_UPDATE, temperature.c_str());

          int weather_id = std::atoi(JSON.stringify(weather_data["weather"][0]["id"]).c_str());
          int weather_day = JSON.stringify(weather_data["weather"][0]["icon"]).c_str()[3] == 'd' ? 1 : 0;
          icon_data[0] = weather_id;
          icon_data[1] = weather_day;
          lv_msg_send(MSG_WEATHER_ICON_UPDATE, &icon_data);

          if (System.is_asleep) {
            lv_refr_now(System.display);
          }

        } else {
          Serial.printf("fail to get http client,code:%d\n", http_code);
          is_get_http = true;
        }
      } else {
        Serial.println("HTTP GET failed.");
        is_get_http = true;
      }

      weather_millis = millis();
    }


    delay(100);

  } while (System.wifi_active);

  http_client.end();
  WiFi.disconnect();

  str = "#00ff00 WIFI detection function completed #";

  vTaskDelete(NULL);
}
