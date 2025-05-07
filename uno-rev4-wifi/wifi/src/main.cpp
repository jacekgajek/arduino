#include <includes.h>
#include <vector>
#include <SPI.h>

float humidity = 0;
float temperature = 0;
int lastDateTimeUpdate = 0;
int dateTimeUpdate = 0;
bool buzzerStatus = false;

void triggerPrintingWifiData();
void updateReadings();
void init();
void triggerBuzzerStart();

void setup() {
    init();
}

// the setup function runs once when you press reset or power the board
// the loop function runs over and over again forever
void loop()
{
    WiFiClient client = server.available(); // listen for incoming clients
    SPI.begin();

    if (printInfo)
    {
        printInfo = false;
        printWifiData();
    }
    if (millis() - lastDateTimeUpdate > 3600000) {
        Serial.println("Updating date and time...");
        myClock.update();
        Serial.println("Current date and time: " + myClock.getFormattedTime());
        lastDateTimeUpdate = millis();
    }

    buzzer.buzz();
    RTCTime time;
    RTC.getTime(time);

    updateReadings();

    if (client)
    {                                 // if you get a client,
        Serial.println("new client"); // print a message out the serial port
        String currentLine = "";      // make a String to hold incoming data from the client
        while (client.connected())
        { // loop while the client's connected
            if (client.available())
            {                           // if there's bytes to read from the client,
                char c = client.read(); // read a byte, then
                Serial.write(c);        // print it out to the serial monitor
                if (c == '\n')
                {
                    if (currentLine.startsWith("GET /alarm/")) {
                        auto len = currentLine.length();
                        int requestLength = 11;
                        auto hour = currentLine.substring(requestLength, requestLength + 2).toInt();
                        auto minute = currentLine.substring(requestLength + 2, requestLength + 4).toInt();
                        if (hour > 23 || hour < 0 || minute > 59 || minute < 0) {
                            Serial.println("Invalid alarm time: " + String(hour) + ":" + String(minute));
                        }
                        else {
                            Serial.println("Setting alarm to: " + String(hour) + ":" + String(minute));
                            auto alarmTime = AlarmTime{hour, minute};
                            myClock.setAlarm(alarmTime, triggerBuzzerStart);
                            EEPROM.put(0, alarmTime);
                        }
                    }
                    if (currentLine.startsWith("GET /noalarm")) {
                        Serial.println("Disabling alarm");
                        myClock.disableAlarm();
                        EEPROM.put(0, AlarmTime{-1, -1});
                    }
                    if (currentLine.endsWith("GET /H"))
                    {
                        digitalWrite(LED_PIN, HIGH); // GET /H turns the LED on
                    }
                    if (currentLine.endsWith("GET /L"))
                    {
                        digitalWrite(LED_PIN, LOW); // GET /L turns the LED off
                    }
                    // if the current line is blank, you got two newline characters in a row.
                    // that's the end of the client HTTP request, so send a response:
                    if (currentLine.length() == 0)
                    {
                        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                        // and a content-type so the client knows what's coming, then a blank line:

                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println();

                        // the content of the HTTP response follows the header:
                        client.print("<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\">"
                            "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
                            "<title>Alarm Control</title><style>"
                            ":root {--primary-color: #007bff;--secondary-color: #0056b3;"
                            "--background-color: #f3f4f6;--text-color: #333;--container-background: #fff;"
                            "--frame-background: #e9f5ff;--form-background: #f9f9f9;--form-focus-background: #fff;"
                            "--shadow-color: rgba(0, 0, 0, 0.1);--font-family: 'Arial', sans-serif;}"
                            "body {font-family: var(--font-family);background-color: var(--background-color);"
                            "color: var(--text-color);display: flex;justify-content: center;align-items: center;"
                            "min-height: 100vh;margin: 0;} .container {box-shadow: 0px 4px 12px var(--shadow-color);"
                            "border-radius: 12px;padding: 20px;background-color: var(--container-background);"
                            "max-width: 600px;width: 100%;} .data {font-size: 1.5rem;font-weight: bold;"
                            "margin: 20px 0;text-align: center;} .data span {color: var(--primary-color);}"
                            ".frame {padding: 20px;border: 2px solid var(--primary-color);border-radius: 10px;"
                            "background-color: var(--frame-background);margin-top: 20px;} .frame h2 {font-size: 1.8rem;"
                            "color: var(--primary-color);margin-bottom: 15px;text-align: center;} .form-group {"
                            "display: flex;align-items: center;justify-content: space-between;margin-bottom: 15px;}"
                            "label {font-size: 1rem;color: var(--text-color);margin-right: 10px;"
                            "min-width: 75px;text-align: right;} .input-field {font-size: 1rem;width: 100%;"
                            "max-width: 200px;text-align: center;padding: 10px;border: 2px solid #ccc;"
                            "border-radius: 8px;background-color: var(--form-background);transition: border-color 0.3s ease,"
                            "background-color 0.3s ease,box-shadow 0.3s ease;} .input-field:focus {"
                            "border-color: var(--primary-color);outline: none;background-color: var(--form-focus-background);"
                            "box-shadow: 0px 0px 5px var(--primary-color);} button {font-size: 1rem;padding: 12px 20px;"
                            "background-color: var(--primary-color);color: #fff;border: none;border-radius: 5px;"
                            "cursor: pointer;transition: background-color 0.3s ease, transform 0.3s ease;display: block;"
                            "margin: 0 auto;} button:hover {background-color: var(--secondary-color);"
                            "transform: translateY(-2px);} button:active {transform: translateY(0);} </style></head>"
                            "<body><div class=\"container\">");
               
               client.print("<p class=\"data\">Temperatura: <span id=\"temperature\">");
               client.print(String(temperature));
               client.print("</span>&deg;C</p>");
               
               client.print("<p class=\"data\">Wilgotność: <span id=\"humidity\">");
               client.print(String(humidity));
               client.print("</span>%</p>");
               
               client.print("<p class=\"data\">Czas: <span id=\"time\">");
               client.print(time.toString());
               client.print("</span></p>");
               
               client.print("<p class=\"data\">Alarm ustawiony na: ");
               if (myClock.isAlarmSet()) {
                   client.print("<span id=\"alarm\">" + String(myClock.getFormattedAlarm()) + "</span>");
               } else {
                   client.print("Brak alarmu");
               }
               client.print("</p>");
               
               client.print("<div class=\"frame\">"
                            "<h2>Ustaw Alarm</h2>"
                            "<form onsubmit=\"sendAlarmRequest(); return false;\">"
                            "<div class=\"form-group\">"
                            "<label for=\"hour\">Godzina:</label>"
                            "<input type=\"number\" id=\"hour\" name=\"hour\" class=\"input-field\" min=\"0\" max=\"23\" required placeholder=\"00\">"
                            "</div><div class=\"form-group\">"
                            "<label for=\"minute\">Minuta:</label>"
                            "<input type=\"number\" id=\"minute\" name=\"minute\" class=\"input-field\" min=\"0\" max=\"59\" required placeholder=\"00\">"
                            "</div><button type=\"submit\">Ustaw Alarm</button></form></div>"
                            "<script>function sendAlarmRequest() {const hour = document.getElementById('hour').value.padStart(2, '0');"
                            "const minute = document.getElementById('minute').value.padStart(2, '0');"
                            "const xhr = new XMLHttpRequest();xhr.open('GET', `/alarm/${hour}${minute}`, true);"
                            "xhr.onload = function() {if (xhr.status === 200) {window.location.href = '/';}};xhr.send();}"
                            "</script><div class=\"frame\"><form onsubmit=\"cancelAlarm(); return false;\">"
                            "<button type=\"submit\">Wy&#322;&#261;cz Alarm</button></form></div>"
                            "<script>function cancelAlarm() {const xhr = new XMLHttpRequest();"
                            "xhr.open('GET', `/noalarm`, true);xhr.onload = function() {if (xhr.status === 200) {"
                            "window.location.href = '/';}};xhr.send();}</script></div></body></html>");

                        // The HTTP response ends with another blank line:
                        client.println();
                        // break out of the while loop:
                        break;
                    }
                    else
                    { // if you got a newline, then clear currentLine:
                        currentLine = "";
                    }
                }
                else if (c != '\r')
                {                     // if you got anything else but a carriage return character,
                    currentLine += c; // add it to the end of the currentLine
                }

            }
        }
        // close the connection:
        client.stop();
        Serial.println("client disconnected");
    }
}

void updateReadings() {
    static long lastRead = 0;
    if (millis() - lastRead > READ_PERIOD)
    {
        lastRead = millis();
        humidity = dht.readHumidity();
        temperature = dht.readTemperature(false);
    }
}
