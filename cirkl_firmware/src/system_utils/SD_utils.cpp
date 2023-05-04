#include "system.h"
#include "location.h"
#include "SD_MMC.h"
#include <sstream>

#define BUF_SIZE 512

bool is_hex_character(char c) {
  // If character is 'x', 'X', [0-9], [A-F], [a-f]
  if (c == 88 || c == 120 || (48 <= c && c <= 57) || (65 <= c && c <= 70) || (97 <= c && c <= 102)) return true;
  return false;
}

void read_coordniates() {

  File * file = SD_MMC.open("/coordinates.txt");

  if (!file) {
    Serial.println("Failed to open \'coordinates.txt\'");
    return;
  }

  size_t len = file->size();
  uint8_t buf[BUF_SIZE];
  file->read(buf, BUF_SIZE);
  file->close();
  delete file;

  std::string longitude;
  std::string latitude;

  bool isLat = true;

  for (int i = 0; i < len; i++) {

    char c =  buf[i];

    if (c == ',') {
      isLat = false;
      continue;
    } else if (45 <= c && c <= 57) {
      if (isLat) {
        latitude += c;
      } else {
        longitude += c;
      }
    } else if (c != ',' && c != ' ' && c != '\n' && c != EOF) {
      Serial.println("In \'coordinates.txt\': Invalid coordinates!");
      Serial.print("Character: ");
      Serial.println(c);
      return;
    }

  }

  location.is_known = true;
  location.lat = latitude;
  location.lon = longitude;

  Serial.print("Coordinates: ");
  Serial.print(latitude.c_str());
  Serial.print(", ");
  Serial.println(longitude.c_str());

}

void read_color_palette() {

  File * file = SD_MMC.open("/color_palette.txt");

  if (!file) {
    Serial.println("Failed to open \'color_palette.txt\'");
    return;
  }

  size_t len = file->size();
  uint8_t buf[BUF_SIZE];
  file->read(buf, BUF_SIZE);
  file->close();
  delete file;

  int color_num = 0;
  uint32_t colors[4] = {0};
  std::string color_builder = "";

  std::stringstream ss;

  Serial.print("Color palette: ");

  for (int i = 0; i < len; i++) {

    char c = buf[i];

    if (color_builder.length() == 8) {

      Serial.print(color_builder.c_str());
      Serial.print(" ");

      colors[color_num] = stoi(color_builder.substr(2), 0, 16);

      color_num++;
      color_builder = "";
      continue;
    } else if (!is_hex_character(c) && c != ',') {
      Serial.println("In \'color_palette.txt\': Invalid color palette!");
      Serial.print("Invalid character: ");
      Serial.println(c);
      return;
    }

    color_builder += c;

  }

  Serial.println();

  for (int i = 0; i < 4; i++) {
    Serial.println(colors[i]);
  }

  System.color_palette = true;
  System.theme_main_color = lv_color_hex(colors[0]);
  System.font_main_color = lv_color_hex(colors[1]);
  System.font_accent_color = lv_color_hex(colors[2]);
  System.theme_accent_color = lv_color_hex(colors[3]);

}