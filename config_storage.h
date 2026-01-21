#pragma once
#include <LittleFS.h>
#include <iButtonTag.h>  
#include <ArduinoJson.h>

struct Config {
  char version[32] = "0.0.0";
  iButtonCode codes[128];
  int8_t ids[128];
  int8_t codes_saved;
};

extern Config config;

int8_t storage_init(void);
void saveConfiguration(const Config& config);