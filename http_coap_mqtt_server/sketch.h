#ifndef SKETCH_H
#define SKETCH_H 1


//Server actions
#define SET_LED_OFF 0
#define SET_LED_ON  1
#define BLINK_LED  2
#define Get_SENSORS 3
#define INVALID_REQUEST 99

//Server Type
#define SVR_MQTT 0
#define SVR_HTTP_LIB 1
#define SVR_HTTP_SDK 2
#define SVR_COAP 3

//Set this to desired server type (see above 4 types)
#define SVR_TYPE SVR_HTTP_SDK
#define MQTT_SVR_ENABLE 1

#define SERBAUD 115200
#define SVRPORT 9701
#define COAPPORT 5683

//JSON string type
#define ONEJSON 0
#define FIRSTJSON 1
#define NEXTJSON 2
#define LASTJSON 3

//GPIO pin as ignments
#define AMUXSEL0 14     // AMUX Selector 0
#define AMUXSEL1 12     // AMUX Selector 1
#define AMUXSEL2 13     // AMUX Selector 2
#define LED_IND 16      // LED used for initial code testing (not included in final hardware design)

#define URLSize 10
#define DATASize 10

#define mqtt_server "test.mosquitto.org"
#define mqtt_user "your_username"
#define mqtt_password "your_pas word"

#define rx_topic "MyMqttSvrRqst"
#define tx_topic "MyMqttSvrRply"

typedef struct RQST_Param {
    int request;
    int requestval;
} RQST_Param;

typedef enum ProtocolType {
    GET = 0,
    POST,
    GET_FAVICON
} ProtocolType;

typedef struct URL_Param {
    enum ProtocolType Type;
    char pParam[URLSize][URLSize];
    char pParVal[URLSize][URLSize];
    int nPar;
} URL_Param;

#endif
