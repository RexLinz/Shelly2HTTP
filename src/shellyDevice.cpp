#include "shellyDevice.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <Crypto.h> // An extremely minimal crypto library for Arduino devices by Chris Ellis

// extract parameter value starting after "param" up to (excluding) delimiter
String shellyDevice::extractParam(String s, String param, char delimiter) 
{
    int _begin = s.indexOf(param);
    if (_begin == -1) 
        return ""; // not found
    return s.substring(_begin + param.length(), s.indexOf(delimiter, _begin + param.length()));
}

// calculate SHA256 hash for string s
String shellyDevice::SHA256hash(String s)
{
    SHA256 sha;
    sha.doUpdate(s.c_str());
    char hash[SHA256_SIZE];
    sha.doFinal((byte*)hash);
    String hashStr = "";
    for (int i=0; i<32; i++)
    {
        hashStr += String(((hash[i]>>4) & 0x0f), HEX) + String((hash[i] & 0x0f), HEX);
    }
    return hashStr;
}

String shellyDevice::GET(String rpcMethod)
{
    WiFiClient wifi;
    HTTPClient http;
    int httpResponseCode;
    String payload = "{}"; 
    String method = "/rpc/" + rpcMethod;

    http.begin(wifi, _server + method);
    // if the server requires authentication we will need the Authenticate header from the response
    const int NUMHEADERS = 1;
    const char* AuthHeaders[NUMHEADERS] = {"WWW-Authenticate"};
    http.collectHeaders(AuthHeaders, NUMHEADERS);
    http.setAuthorizationType("AUTH_NONE"); // force library not to try authentication (does not work with SHA256)
    // try to read from server
    httpResponseCode = http.GET();

    bool needAuth = http.hasHeader(AuthHeaders[0]) && (httpResponseCode==HTTP_CODE_UNAUTHORIZED); // server returned authentication challenge
    if ( needAuth && (_password.length()>0)) // ... and we know about password
    {
        String AuthHeader = http.header(0U); // get authentication challenge
        http.end(); // end the old request
// similar to https://forum.arduino.cc/t/arduino-web-server-http-requests-digest-authentication/531860/55
        String AuthMethod = AuthHeader.substring(0,AuthHeader.indexOf(' ')); // up to first space
        String realm = extractParam(AuthHeader, "realm=\"", '\"');
        String nonce = extractParam(AuthHeader, "nonce=\"", '\"');
        String qop   = extractParam(AuthHeader, "qop=\""  , '\"');
        String algo  = AuthHeader.substring(AuthHeader.indexOf("algorithm=")+10); // algorithm= to end

        // HA1 = SHA256(username ":" realm ":" password)
        String HA1   = SHA256hash(_user+":"+realm+":"+_password); // TODO as SHA-256
        
        // according to shelly doc HA2 = SHA256("dummy_method:dummy_uri") should work as well but does not
//        String HA2 = SHA256hash("dummy_method:dummy_uri");
        String HA2 = SHA256hash("GET:" + method);

        String nc = "1"; // constant client number
        String cnonce = String(random(556822323L)); // clients random number
        // SHA256(HA1:nonce:nc:cnonce:qop:HA2)
        String authResponse = SHA256hash(HA1+":"+nonce+":"+nc+":"+cnonce+":"+qop+":"+HA2); 

        String authString = " Digest"
            " username="   "\"" + _user  + "\""
            ", realm="     "\"" + realm  + "\""
            ", nonce="     "\"" + nonce  + "\"" 
//            ", uri="       "\"" + method + "\"" // optional, not required by shelly
            ", algorithm=" + algo + // "SHA-256"
            ", response="  "\"" + authResponse + "\""
            ", qop="       + qop + // "auth"
            ", nc="        + nc +  // 1
            ", cnonce="    + cnonce; // random
        // try with authentication added
        http.begin(wifi, _server + method);
        http.setAuthorizationType("AUTH_NONE"); // force library not to try authentication (does not work with SHA256)
        http.addHeader("Authorization", authString);
        httpResponseCode = http.GET(); // retry with authentication
    }

    // check server's response
    if (httpResponseCode == 200) // OK
        payload = http.getString();
    else // return http response code as JSON string
        payload = "{\"httpResponse\": " + String(httpResponseCode) + "}";
    http.end(); // Free resources
    return payload; // actual response or empty json object {}
}

int shellyWiFi::WiFiRSSI()
{
    String response = WiFiGetStatus();
    // returns something like {"sta_ip":"192.168.178.210","status":"got ip","ssid":"FRITZ!e24","rssi":-51}
    return extractParam(response, "\"rssi\":", '}').toInt();
}

float shellyCover::TemperatureDegC(uint8_t id)
{
    String response = CoverGetStatus(id);
    // returns something like {"id":0, ... ,"temperature":{"tC":33.1, "tF":91.6}}
    return extractParam(response, "\"tC\":", ',').toFloat();
}

float shellyCover::ActivePower(uint8_t id)
{
    String response = CoverGetStatus(id);
    // returns something like {"id":0, "source":"init", "output":false, "apower":0.0, "voltage":231.3, "current":0.000,...}
    return extractParam(response, "\"apower\":", ',').toFloat();
}

float shellySwitch::TemperatureDegC(uint8_t id)
{
    String response = SwitchGetStatus(id);
    // returns something like {"id":0, ... ,"temperature":{"tC":33.1, "tF":91.6}}
    return extractParam(response, "\"tC\":", ',').toFloat();
}

float shellySwitch::ActivePower(uint8_t id)
{
    String response = SwitchGetStatus(id);
    // returns something like {"id":0, "source":"init", "output":false, "apower":0.0, "voltage":231.3, "current":0.000,...}
    return extractParam(response, "\"apower\":", ',').toFloat();
}

float shellyTemperature::TemperatureDegC(uint8_t id)
{
    String response = TemperatureGetStatus(id);
    // returns something like {"id": 0,"tC":27.5, "tF":81.5}
    return extractParam(response, "\"tC\":", ',').toFloat();
}

float shellyEM::TotalActivePower(uint8_t id)
{
    String response = EMGetStatus(id);
// return something like {"id":0, ... ,"total_act_power":133.790,"total_aprt_power":240.765, "user_calibrated_phase":[]}
    return extractParam(response, "\"total_act_power\":", ',').toFloat();
}
