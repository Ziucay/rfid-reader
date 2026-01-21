#include <iButtonTag.h>                     // Include the library
#include "id_display.h"
#include "rotary_encoder.h"
#include "config_storage.h"

// Scanner configuration
#define PIN_PROBE 23
#define SERIAL_PRINT_INTERVAL 25

// LED light control
#define BLINK_INTERVAL 5
#define PIN_LED_CONTROL 32
#define MAX_DUTY_CYCLE 200 //Макс 255, отвечает за макс яркость

// Code debounce
int8_t last_ten_statuses[10]; //TODO: remove magic numbering
int current_last_status;
iButtonCode last_nonzero_code;

// flag to reduce display clear
int display_has_data;
int current_code_index;

enum controller_states {
  Init,
  NoCard,
  HasCard,
  BindIdToCard
};

controller_states current_controller_state = controller_states::Init;

long last_blink;
int is_fade = 0;
int duty_cycle = 0;

long last_output;


iButtonTag ibutton( PIN_PROBE );            // Setup iButtonTag on the pin

int current_id;
int current_id_binded;

int last_knob_value = -1;
void knobMove(long value){
  if (last_knob_value == -1)
  {
    last_knob_value = value;
    return;
  }
  int dir;
  if (value > last_knob_value || (last_knob_value == ENCODER_MAX && value == ENCODER_MIN))
  {
    Serial.println("Left");
    dir = -1;
  }
  else if  (value < last_knob_value || (last_knob_value == ENCODER_MIN && value == ENCODER_MAX))
  {
    Serial.println("Right");
    dir = 1;
  }
  else
  {
    dir = 0; //TODO: is it ever happens?
  }
  current_id += dir;
  if (current_id > 999)
  {
    current_id = 0;
  }
  else if (current_id < 0)
  {
    current_id = 999;
  }


  last_knob_value = value;
}

void buttonPress(unsigned long duration){
  Serial.println("Pressed!!!");
  if (display_has_data)
  {
    if (current_code_index == -1)
    {
      Serial.println("NEW CODE!!!");
      config.ids[config.codes_saved] = current_id;
      memcpy(config.codes[config.codes_saved], last_nonzero_code, sizeof(last_nonzero_code));
      config.codes_saved++;
    }
    else
    {
      Serial.println("NEW ID!!!");
      config.ids[current_code_index] = current_id;
    }
    current_id_binded = current_id;
    saveConfiguration(config);
  } 
  //Serial.printf( "My boop! button was down for %lu ms\n", duration );
} 

void setup(void) {
  Serial.begin(115200);
  pinMode(PIN_LED_CONTROL, OUTPUT);

  display_init();
  clearDisplay();

  encoder_init(knobMove, buttonPress);
  int8_t storage_status = storage_init();
  Serial.print("Storage status: ");
  Serial.println(storage_status);
  Serial.print("Version: ");
  Serial.println(config.version);
  ibutton.printCode( config.codes[0] );
  Serial.println();
  Serial.println(config.ids[0]);

  current_last_status = 0;
  display_has_data = 0;

  last_blink = millis();
}


void led_strip_animation(void)
{
    analogWrite(PIN_LED_CONTROL, duty_cycle);
    //Serial.println(duty_cycle);
    if (millis() - last_blink > BLINK_INTERVAL)
    {
      last_blink = millis();
      if (is_fade)
      {
        duty_cycle--;
        if (duty_cycle < 0)
        {
          duty_cycle = 0;
          is_fade = 0;
        }  
      }
    else
    {
      duty_cycle++;
      if (duty_cycle > MAX_DUTY_CYCLE)
      {
        duty_cycle = MAX_DUTY_CYCLE;
        is_fade = 1;
      }
    }  
  }
}

int16_t find_id_in_config(iButtonCode code)
{
  for(int i = 0;i < config.codes_saved; i++)
  {
    if (ibutton.equalCode(code, config.codes[i]))
    {
      current_code_index = i;
      return config.ids[i];
    }
  }
  current_code_index = -1;
  return 0;
}

int at_least_one_status_nonzero()
{
  for(int i = 0;i < 10;i++)
  {
    if (last_ten_statuses[i] > 0)
      return 1;
  }
  return 0;
}

void handle_card_read(void)
{
  iButtonCode code;                         // Variable to store ID-code

  int8_t status = ibutton.readCode( code ); // Try to read ID-code

  if (status > 0)
    {
      for (int i = 0;i < 8;i++)
      {
        last_nonzero_code[i] = code[i];
      }
    }
    

  last_ten_statuses[current_last_status] = status;
  current_last_status = (current_last_status + 1) % 10;


  if (millis() - last_output > SERIAL_PRINT_INTERVAL)
  {
    last_output = millis();
    Serial.print( "AT+CARD=");
    if (at_least_one_status_nonzero()) // Code read success
    {
      ibutton.printCode(code);
      
      current_id_binded = find_id_in_config(last_nonzero_code);
      if (!display_has_data)
      {
        current_id = current_id_binded;
      }
      Serial.println(current_id_binded);
      displayNumber(current_id);
      display_has_data = 1;
    }
    else {
      Serial.println( "0" );
      if (display_has_data)
      {
        clearDisplay();
        display_has_data = 0;
      }
      
    }

  }
}


void loop(void) {

  led_strip_animation();
  handle_card_read();
}

/*
D (298897) owb: owb_write_byte: 33
D (298907) owb: owb_read_bytes, len 8:
D (298907) owb: 01 f3 08 01 36 00 00 a8 
D (298947) owb: owb_write_byte: 33
D (298947) owb: owb_read_bytes, len 8:
D (298947) owb: 01 f3 08 01 36 00 00 a8 
AT+CARD=10

D (299017) owb: owb_write_byte: 33
D (299027) owb: owb_read_bytes, len 8:
D (299027) owb: 01 f3 08 01 36 00 00 a8 
D (299067) owb: owb_write_byte: 33
D (299067) owb: owb_read_bytes, len 8:
D (299067) owb: 01 f3 08 01 36 00 00 a8 
D (299137) owb: owb_write_byte: 33

*/