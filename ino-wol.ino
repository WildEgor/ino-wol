// Internal libs
#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>
#include <WiFiClientSecure.h>
#include <WiFiUDP.h>
#include <Arduino.h>

// headers
#include "secrets.h"
#include "devices.h"
#include "macros.h"

// External libs
// Download to library directory (for example, C:/Users/<username>/Documents/Arduino/libraries/)
// https://github.com/koenieee/WakeOnLan-ESP8266
#include <WakeOnLan.h>
// https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <UniversalTelegramBot.h>
// https://github.com/bblanchon/ArduinoJson
#include <ArduinoJson.h>
// https://github.com/arkhipenko/TaskScheduler
#include <TaskScheduler.h>

WiFiUDP UDP;
// computer_ip magic packet
IPAddress computer_ip(255,255,255,255);
WiFiClientSecure wifiClient;
UniversalTelegramBot bot(telegramToken, wifiClient);

// tDelayBetweenMsgChecks interval (in milliseconds) when checking for new telegram messages
int tDelayBetweenMsgChecks = 1000;
// tLastTimeMsgChecked last time telegram messages' scan has been done
unsigned long tLastTimeMsgChecked;
void sendWOL();

// Scheduler for async tasks
Scheduler runner;
void pingDevices();
// tPingDevice ... 
Task tPingDevice(500, TASK_FOREVER, &pingDevices); 

// setup ESP
void setup() {
  // Comment this if you want to disable serial printing
  SERIAL_DEBUG;

  // add async task
  runner.addTask(tPingDevice);
  tPingDevice.enable();

  // Set WiFi to station mode and disconnect from an AP if it was Previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // attempt to connect to Wifi network:
  Sprint("connecting WI-FI: ");
  Sprintln(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Sprint(".");
    delay(500);
  }

  Sprint("\n");
  Sprintln("WI-FI connected!");
  Sprint("IP address: ");
  Sprintln(WiFi.localIP());

  // Required on 2.5 Beta or above.
  wifiClient.setInsecure();

  // longPoll keeps the request to Telegram open for the given amount of seconds if there are no messages
  // This hugely improves response time of the bot, but is only really suitable for projects
  // where the the initial interaction comes from Telegram as the requests will block the loop for
  // the length of the long poll
  bot.longPoll = 60;
  
  UDP.begin(9);
}

// main loop
void loop() {
  runner.execute();

  // getUpdates returns 1 if there is a new message from Telegram
  if (millis() > tLastTimeMsgChecked + tDelayBetweenMsgChecks)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    if (numNewMessages > 0) {
      handleNewMessages(numNewMessages);
    }

    tLastTimeMsgChecked = millis();
  }
}

// handleNewMessages telegram messages
void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = String(bot.messages[i].text);
    String type = String(bot.messages[i].type);

    // When a user first uses a bot they will send a "/start" command
    // So this is a good place to let the users know what commands are available
    if (text == F("/start")) {
      bot.sendMessage(chat_id, "/wol : returns list of devices to send WOL to\n", "Markdown");
      continue;
    }

     // show list available devices
    if (text == F("/wol")) {
      // Keyboard Json is an array of arrays.
      // The size of the main array is how many row options the keyboard has
      // The size of the sub arrays is how many coloums that row has
      // "The Text" property is what shows up in the keyboard
      // The "callback_data" property is the text that gets sent when pressed  
      String keyboardJson = "[";
      for(int i = 0; i< numDevices; i++) {
        keyboardJson += "[{ \"text\" : \"" + devices[i].name + "\", \"callback_data\" : \"WOL" + String(i) + "\" }]";
        if(i + 1 < numDevices){
          keyboardJson += ",";
        }
      }
      keyboardJson += "]";

      bot.sendMessageWithInlineKeyboard(chat_id, "Send WOL to following device:", "", keyboardJson);

      continue;
    }

    // If the type is a "callback_query", a inline keyboard button was pressed
    if (type ==  F("callback_query") && text.startsWith("WOL")) {
      Sprint("callback button pressed with text: ");
      Sprintln(text);
      
      text.replace("WOL", "");
      int index = text.toInt();
      WakeOnLan::sendWOL(computer_ip, UDP, devices[index].mac, sizeof devices[index].mac);

      devices[index].tCallbackQueryId = bot.messages[i].query_id;
      devices[index].rCounter = 5;

      Sprint("sending WOL to: ");
      Sprintln(devices[index].name);

      continue;
    }
  }
}

// pingDevice async task
void pingDevices() {
  for(int index = 0; index < numDevices; index++) {
    if (devices[index].tCallbackQueryId.isEmpty()) {
      continue;
    }

    if (Ping.ping(devices[index].ipAddr)) {
      Sprintln("ping successful");
      bot.answerCallbackQuery(devices[index].tCallbackQueryId, "OK!");
    } else {
      Sprintln("ping failed");
    }
    
    if (devices[index].rCounter <= 0) {
      devices[index].tCallbackQueryId = "";
      continue;
    }

    devices[index].rCounter -= 1;
  }
}