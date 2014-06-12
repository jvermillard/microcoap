
#include <stdbool.h>
#include <string.h>
#include "coap.h"

static int lightR1 = 0;
static int lightG1 = 0;
static int lightB1 = 0;

static int lightR2 = 0;
static int lightG2 = 0;
static int lightB2 = 0;

const uint16_t rsplen = 2500;
static char rsp[2500] = "";
void build_rsp(void);

#ifdef ARDUINO

#include "Arduino.h"


//static int led = 6;
void endpoint_setup(void)
{                

  
  //  pinMode(led, OUTPUT);     
    build_rsp();
}
#else
#include <stdio.h>
void endpoint_setup(void)
{
    build_rsp();
}
#endif

static const coap_endpoint_path_t path_well_known_core = {2, {".well-known", "core"}};
static int handle_get_well_known_core(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo)
{
  return coap_make_response(scratch, outpkt, (const uint8_t *)rsp, strlen(rsp), id_hi, id_lo, COAP_RSPCODE_CONTENT, COAP_CONTENTTYPE_APPLICATION_LINKFORMAT);
}

static const coap_endpoint_path_t path_lightR1 = {2, {"color1","R"}};
static const coap_endpoint_path_t path_lightG1 = {2, {"color1","G"}};
static const coap_endpoint_path_t path_lightB1 = {2, {"color1","B"}};

static const coap_endpoint_path_t path_lightR2 = {2, {"color2","R"}};
static const coap_endpoint_path_t path_lightG2 = {2, {"color2","G"}};
static const coap_endpoint_path_t path_lightB2 = {2, {"color2","B"}};


static const coap_endpoint_path_t path_temp = {2,{"analog","temperature"}};
static const coap_endpoint_path_t path_lum = {2,{"analog","luminosity"}};

static int handle_get_temp(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo)
{
    char tmp[30];
    sprintf(tmp,"%d",readTemp());
    return coap_make_response(scratch, outpkt, (const uint8_t *)&tmp, strlen(tmp), id_hi, id_lo, COAP_RSPCODE_CONTENT, COAP_CONTENTTYPE_TEXT_PLAIN);
}

static int handle_get_lum(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo)
{
    char tmp[30];
    sprintf(tmp,"%d",readLum());
    return coap_make_response(scratch, outpkt, (const uint8_t *)&tmp, strlen(tmp), id_hi, id_lo, COAP_RSPCODE_CONTENT, COAP_CONTENTTYPE_TEXT_PLAIN);
}


static int handle_get_lightR1(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo)
{
    char tmp[30];
    sprintf(tmp,"%d",lightR1);
    return coap_make_response(scratch, outpkt, (const uint8_t *)&tmp, strlen(tmp), id_hi, id_lo, COAP_RSPCODE_CONTENT, COAP_CONTENTTYPE_TEXT_PLAIN);
}

static int handle_get_lightR2(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo)
{
    char tmp[30];
    sprintf(tmp,"%d",lightR2);
    return coap_make_response(scratch, outpkt, (const uint8_t *)&tmp, strlen(tmp), id_hi, id_lo, COAP_RSPCODE_CONTENT, COAP_CONTENTTYPE_TEXT_PLAIN);
}

static int handle_get_lightG1(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo)
{
    char tmp[30];
    sprintf(tmp,"%d",lightG1);
    return coap_make_response(scratch, outpkt, (const uint8_t *)&tmp, strlen(tmp), id_hi, id_lo, COAP_RSPCODE_CONTENT, COAP_CONTENTTYPE_TEXT_PLAIN);
}

static int handle_get_lightG2(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo)
{
    char tmp[30];
    sprintf(tmp,"%d",lightG2);
    return coap_make_response(scratch, outpkt, (const uint8_t *)&tmp, strlen(tmp), id_hi, id_lo, COAP_RSPCODE_CONTENT, COAP_CONTENTTYPE_TEXT_PLAIN);
}

static int handle_get_lightB1(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo)
{
    char tmp[30];
    sprintf(tmp,"%d",lightB1);
    return coap_make_response(scratch, outpkt, (const uint8_t *)&tmp, strlen(tmp), id_hi, id_lo, COAP_RSPCODE_CONTENT, COAP_CONTENTTYPE_TEXT_PLAIN);
}

static int handle_get_lightB2(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo)
{
    char tmp[30];
    sprintf(tmp,"%d",lightB2);
    return coap_make_response(scratch, outpkt, (const uint8_t *)&tmp, strlen(tmp), id_hi, id_lo, COAP_RSPCODE_CONTENT, COAP_CONTENTTYPE_TEXT_PLAIN);
}

static int handle_put_lightR1(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo)
{
    if (inpkt->payload.len == 0)
        return coap_make_response(scratch, outpkt, NULL, 0, id_hi, id_lo, COAP_RSPCODE_BAD_REQUEST, COAP_CONTENTTYPE_TEXT_PLAIN);
    
    lightR1 = atoi(inpkt->payload.p);
    stripSet1(lightR1, lightG1, lightB1);
    
    char tmp[30];
    sprintf(tmp,"%d",lightR1);
    return coap_make_response(scratch, outpkt,  (const uint8_t *)tmp, strlen(tmp), id_hi, id_lo, COAP_RSPCODE_CHANGED, COAP_CONTENTTYPE_TEXT_PLAIN);
}

static int handle_put_lightR2(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo)
{
    if (inpkt->payload.len == 0)
        return coap_make_response(scratch, outpkt, NULL, 0, id_hi, id_lo, COAP_RSPCODE_BAD_REQUEST, COAP_CONTENTTYPE_TEXT_PLAIN);
    
    lightR2 = atoi(inpkt->payload.p);
    stripSet2(lightR2, lightG2, lightB2);
    
    char tmp[30];
    sprintf(tmp,"%d",lightR2);
    return coap_make_response(scratch, outpkt,  (const uint8_t *)tmp, strlen(tmp), id_hi, id_lo, COAP_RSPCODE_CHANGED, COAP_CONTENTTYPE_TEXT_PLAIN);
}

static int handle_put_lightG1(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo)
{
    if (inpkt->payload.len == 0)
        return coap_make_response(scratch, outpkt, NULL, 0, id_hi, id_lo, COAP_RSPCODE_BAD_REQUEST, COAP_CONTENTTYPE_TEXT_PLAIN);
    
    lightG1 = atoi(inpkt->payload.p);
    stripSet1(lightR1, lightG1, lightB1);
     
    char tmp[30];
    sprintf(tmp,"%d",lightG1);
    return coap_make_response(scratch, outpkt,  (const uint8_t *)tmp, strlen(tmp), id_hi, id_lo, COAP_RSPCODE_CHANGED, COAP_CONTENTTYPE_TEXT_PLAIN);
}

static int handle_put_lightG2(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo)
{
    if (inpkt->payload.len == 0)
        return coap_make_response(scratch, outpkt, NULL, 0, id_hi, id_lo, COAP_RSPCODE_BAD_REQUEST, COAP_CONTENTTYPE_TEXT_PLAIN);
    
    lightG2 = atoi(inpkt->payload.p);
    stripSet2(lightR2, lightG2, lightB2);
     
    char tmp[30];
    sprintf(tmp,"%d",lightG2);
    return coap_make_response(scratch, outpkt,  (const uint8_t *)tmp, strlen(tmp), id_hi, id_lo, COAP_RSPCODE_CHANGED, COAP_CONTENTTYPE_TEXT_PLAIN);
}

static int handle_put_lightB1(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo)
{
    if (inpkt->payload.len == 0)
        return coap_make_response(scratch, outpkt, NULL, 0, id_hi, id_lo, COAP_RSPCODE_BAD_REQUEST, COAP_CONTENTTYPE_TEXT_PLAIN);
    
    lightB1 = atoi(inpkt->payload.p);
    stripSet1(lightR1, lightG1, lightB1);
     
    char tmp[30];
    sprintf(tmp,"%d",lightB1);
    return coap_make_response(scratch, outpkt,  (const uint8_t *)tmp, strlen(tmp), id_hi, id_lo, COAP_RSPCODE_CHANGED, COAP_CONTENTTYPE_TEXT_PLAIN);
}

static int handle_put_lightB2(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo)
{
    if (inpkt->payload.len == 0)
        return coap_make_response(scratch, outpkt, NULL, 0, id_hi, id_lo, COAP_RSPCODE_BAD_REQUEST, COAP_CONTENTTYPE_TEXT_PLAIN);
    
    lightB2 = atoi(inpkt->payload.p);
    stripSet2(lightR2, lightG2, lightB2);
     
    char tmp[30];
    sprintf(tmp,"%d",lightB2);
    return coap_make_response(scratch, outpkt,  (const uint8_t *)tmp, strlen(tmp), id_hi, id_lo, COAP_RSPCODE_CHANGED, COAP_CONTENTTYPE_TEXT_PLAIN);
}

const coap_endpoint_t endpoints[] =
{
    {COAP_METHOD_GET, handle_get_well_known_core, &path_well_known_core, "ct=40"},
    {COAP_METHOD_GET, handle_get_lightR1, &path_lightR1, "ct=0"},
    {COAP_METHOD_PUT, handle_put_lightR1, &path_lightR1, NULL},
    {COAP_METHOD_GET, handle_get_lightG1, &path_lightG1, "ct=0"},
    {COAP_METHOD_PUT, handle_put_lightG1, &path_lightG1, NULL},
    {COAP_METHOD_GET, handle_get_lightB1, &path_lightB1, "ct=0"},
    {COAP_METHOD_PUT, handle_put_lightB1, &path_lightB1, NULL},
    {COAP_METHOD_GET, handle_get_lightR2, &path_lightR2, "ct=0"},
    {COAP_METHOD_PUT, handle_put_lightR2, &path_lightR2, NULL},
    {COAP_METHOD_GET, handle_get_lightG2, &path_lightG2, "ct=0"},
    {COAP_METHOD_PUT, handle_put_lightG2, &path_lightG2, NULL},
    {COAP_METHOD_GET, handle_get_lightB2, &path_lightB2, "ct=0"},
    {COAP_METHOD_PUT, handle_put_lightB2, &path_lightB2, NULL},
    {COAP_METHOD_GET, handle_get_temp, &path_temp,"ct=0"},
    {COAP_METHOD_GET, handle_get_lum, &path_lum,"ct=0"},

    {(coap_method_t)0, NULL, NULL, NULL}

};

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

