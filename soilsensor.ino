
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "pavi";
const char* password = "veeralak";

ESP8266WebServer server(80);

#define SOIL_PIN A0
#define LED_PIN D5

String getHTML() {

  int soilValue = analogRead(SOIL_PIN);

  String statusText;
  String color;

  if (soilValue < 500) {
    statusText = "SOIL IS WET";
    color = "#00C853";
    digitalWrite(LED_PIN, LOW);
  }
  else {
    statusText = "SOIL IS DRY";
    color = "#FF5252";
    digitalWrite(LED_PIN, HIGH);
  }

  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta http-equiv="refresh" content="2">
<title>Smart Soil Monitor</title>
<style>
body{
font-family:Arial;
background:#0f172a;
color:white;
text-align:center;
padding-top:80px;
}
.card{
background:white;
color:black;
width:350px;
margin:auto;
padding:25px;
border-radius:15px;
}
.status{
color:white;
font-size:24px;
font-weight:bold;
padding:15px;
border-radius:10px;
background:COLOR;
}
</style>
</head>
<body>
<div class="card">
<h2>Soil Moisture Monitoring</h2>
<p>Sensor Value: VALUE</p>
<div class="status">STATUS</div>
</div>
</body>
</html>
)rawliteral";

  html.replace("VALUE", String(soilValue));
  html.replace("STATUS", statusText);
  html.replace("COLOR", color);

  return html;
}

void handleRoot() {
  server.send(200, "text/html", getHTML());
}

void setup() {

  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}