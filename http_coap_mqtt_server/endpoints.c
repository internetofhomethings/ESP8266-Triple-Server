#include <stdbool.h>
#include <string.h>
#include "coap.h"

#define LED_IND 16      // LED used for server command testing
#define LOW 0
#define HIGH 1

//Server actions
#define SET_LED_OFF 0
#define SET_LED_ON  1
#define Get_SENSORS 2
#define INVALID_REQUEST 99

static char light[10] = "000";
static char rqst[256] = "";
static char replystring[256];

const uint16_t rsplen = 1500;
static char rsp[1500] = "";

#include <stdio.h>
void blinkLed(int nblink);
static int led = LED_IND;

//////////////////////////////////////////////////////////////////////////
// Local function prototypes
//////////////////////////////////////////////////////////////////////////
void endpoint_setup(void);
int numbers_only(const char *s);
void build_rsp(void);
static int handle_get_well_known_core(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo);
static int handle_get_light_blink(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo);
static int handle_put_light_blink(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo);
static int handle_get_request(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo);
static int handle_put_request(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo);
static int handle_get_light(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo);
static int handle_put_light(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo);
static int handle_get_light(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo);
static int handle_put_light(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo);


//////////////////////////////////////////////////////////////////////////
// Endpoint Setup
//////////////////////////////////////////////////////////////////////////
void endpoint_setup(void)
{
     build_rsp();
}

//////////////////////////////////////////////////////////////////////////
// Return 1 if all numbers, 0 if not
//////////////////////////////////////////////////////////////////////////
int numbers_only(const char *s)
{
    while (*s) {
        if (isdigit(*s++) == 0) return 0;
    }
    return 1;
}

//////////////////////////////////////////////////////////////////////////
// Define URI path for all CoAP Methods for this Server
//////////////////////////////////////////////////////////////////////////
static const coap_endpoint_path_t path_well_known_core = {2, {".well-known", "core"}};
static const coap_endpoint_path_t path_light_blink = {1, {"light_blink"}};
static const coap_endpoint_path_t path_request = {1, {"request"}};
static const coap_endpoint_path_t path_light = {1, {"light"}};

//////////////////////////////////////////////////////////////////////////
// Define all CoAP Methods for this Server
//////////////////////////////////////////////////////////////////////////
const coap_endpoint_t endpoints[] =
{
    {COAP_METHOD_GET, handle_get_well_known_core, &path_well_known_core, "ct=40"},
    {COAP_METHOD_GET, handle_get_light, &path_light, "ct=0"},
    {COAP_METHOD_GET, handle_get_light_blink, &path_light_blink, "ct=0"},
    {COAP_METHOD_GET, handle_get_request, &path_request, "ct=0"},
    {COAP_METHOD_PUT, handle_put_request, &path_request, NULL},
    {COAP_METHOD_PUT, handle_put_light, &path_light, NULL},
    {COAP_METHOD_PUT, handle_put_light_blink, &path_light_blink, NULL},
    {(coap_method_t)0, NULL, NULL, NULL}
};

//////////////////////////////////////////////////////////////////////////
// Method function:  get_well_known_core
//////////////////////////////////////////////////////////////////////////
static int handle_get_well_known_core(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo)
{
    return coap_make_response(scratch, outpkt, (const uint8_t *)rsp, strlen(rsp), id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CONTENT, COAP_CONTENTTYPE_APPLICATION_LINKFORMAT);
}

//////////////////////////////////////////////////////////////////////////
// Method function:  get_light_blink
//////////////////////////////////////////////////////////////////////////
static int handle_get_light_blink(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo)
{
    return coap_make_response(scratch, outpkt, (const uint8_t *)&light, 1, id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CONTENT, COAP_CONTENTTYPE_TEXT_PLAIN);
}

//////////////////////////////////////////////////////////////////////////
// Method function:  put_light_blink
//////////////////////////////////////////////////////////////////////////
static int handle_put_light_blink(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo)
{
    int i,nblink;
    if (inpkt->payload.len == 0)
    {
        return coap_make_response(scratch, outpkt, NULL, 0, id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_BAD_REQUEST, COAP_CONTENTTYPE_TEXT_PLAIN);
    }
    if(inpkt->payload.len<9)
    {
        strcpy(light,(const char *)&inpkt->payload.p[0]);
    }
    if(numbers_only(light)) {
        nblink = atoi(inpkt->payload);
        blinkLed(nblink); //Blink Led nblink times using non-blocking timer
    }
    return coap_make_response(scratch, outpkt, (const uint8_t *)&light[0], inpkt->payload.len, id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CHANGED, COAP_CONTENTTYPE_TEXT_PLAIN);
}

//////////////////////////////////////////////////////////////////////////
// Method function:  get_request
//////////////////////////////////////////////////////////////////////////
static int handle_get_request(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo)
{
    return coap_make_response(scratch, outpkt, (const uint8_t *)&rqst, strlen(rqst), id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CONTENT, COAP_CONTENTTYPE_TEXT_PLAIN);
}

//////////////////////////////////////////////////////////////////////////
// Method function:  put_request
//////////////////////////////////////////////////////////////////////////
static int handle_put_request(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo)
{
    if (inpkt->payload.len == 0)
        return coap_make_response(scratch, outpkt, NULL, 0, id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_BAD_REQUEST, COAP_CONTENTTYPE_TEXT_PLAIN);

    strncpy(rqst,(const char *)&inpkt->payload.p[0],inpkt->payload.len);

    // Process request & get reply string
    ProcessCoAPrequest(rqst,&replystring[0]);
   
    return coap_make_response(scratch, outpkt, (const uint8_t *)&replystring[0], strlen(replystring), id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CHANGED, COAP_CONTENTTYPE_TEXT_PLAIN);
}

//////////////////////////////////////////////////////////////////////////
// Method function:  get_light
//////////////////////////////////////////////////////////////////////////
static int handle_get_light(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo)
{
    return coap_make_response(scratch, outpkt, (const uint8_t *)&light, 1, id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CONTENT, COAP_CONTENTTYPE_TEXT_PLAIN);
}

//////////////////////////////////////////////////////////////////////////
// Method function:  put_light
//////////////////////////////////////////////////////////////////////////
static int handle_put_light(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo)
{
    if (inpkt->payload.len == 0)
        return coap_make_response(scratch, outpkt, NULL, 0, id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_BAD_REQUEST, COAP_CONTENTTYPE_TEXT_PLAIN);
    if (inpkt->payload.p[0] == '1')
    {
        light[0] = '1';
        digitalWrite(led, HIGH);
        //printf("ON\n");

        return coap_make_response(scratch, outpkt, (const uint8_t *)&light, 1, id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CHANGED, COAP_CONTENTTYPE_TEXT_PLAIN);
    }
    else
    {
        light[0] = '0';
        digitalWrite(led, LOW);
        //printf("OFF\n");

        return coap_make_response(scratch, outpkt, (const uint8_t *)&light, 1, id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CHANGED, COAP_CONTENTTYPE_TEXT_PLAIN);
    }
}

void build_rsp(void)
{
    uint16_t len = rsplen;
    const coap_endpoint_t *ep = endpoints;
    int i;

    len--; // Null-terminated string

    while(NULL != ep->handler)
    {
        if (NULL == ep->core_attr) {
            ep++;
            continue;
        }

        if (0 < strlen(rsp)) {
            strncat(rsp, ",", len);
            len--;
        }

        strncat(rsp, "<", len);
        len--;

        for (i = 0; i < ep->path->count; i++) {
            strncat(rsp, "/", len);
            len--;

            strncat(rsp, ep->path->elems[i], len);
            len -= strlen(ep->path->elems[i]);
        }

        strncat(rsp, ">;", len);
        len -= 2;

        strncat(rsp, ep->core_attr, len);
        len -= strlen(ep->core_attr);

        ep++;
    }
}

