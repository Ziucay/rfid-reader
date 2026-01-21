#include "config_storage.h"

const char* filename = "/config.json";
Config config;


int8_t storage_init()
{
  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return -1;
  }
  
  File file = LittleFS.open(filename, "r");
  if(!file){
    Serial.println("Failed to open file for reading");
    return -2;
  }

  // Allocate a temporary JsonDocument
  JsonDocument doc;

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, file);
  if (error)
  {
    Serial.println(F("Failed to read file, using default configuration"));
  }
    

  // Copy values from the JsonDocument to the Config
  config.codes_saved = doc["codes_saved"] | 0;
  //Serial.println(config.codes_saved);

  strlcpy(config.version,                  // <- destination
          doc["version"] | "1.0.0",  // <- source
          sizeof(config.version));         // <- destination's capacity
  for (int i = 0;i < config.codes_saved;i++)
  {
    for (int j = 0;j < 8;j++)
    {
      config.codes[i][j] = doc["codes"][i][j];
    }
    config.ids[i] = doc["ids"][i];
    
  }

  serializeJsonPretty(doc, Serial);

  file.close();
  return 0;
}

// Saves the configuration to a file
void saveConfiguration(const Config& config) {

  LittleFS.remove(filename);

  File file = LittleFS.open(filename, "w");
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }

  // Allocate a temporary JsonDocument
  JsonDocument doc;

  // Set the values in the document
  doc["version"] = config.version;
  doc["codes_saved"] = config.codes_saved;

  for (int i = 0;i < config.codes_saved;i++)
  {
    for (int j = 0;j < 8;j++)
    {
      doc["codes"][i][j] = config.codes[i][j];
    }
    doc["ids"][i] = config.ids[i];
  }

  serializeJsonPretty(doc, Serial);

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
  }
  // Close the file
  file.close();
}
