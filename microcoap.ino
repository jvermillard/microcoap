#include <LPD8806.h>

/*
* WARNING - UDP_TX_PACKET_MAX_SIZE is hardcoded by Arduino to 24 bytes
* This limits the size of possible outbound UDP packets
*/

#include <SPI.h>
#include <Ethernet.h>
#include <stdint.h>
//#include <EthernetUdp.h>
#include "coap.h"

#define PORT 5683
static uint8_t mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x04};
IPAddress ip(10, 41, 51, 38);
IPAddress gateway(10, 41,  51, 254);
IPAddress subnet(255, 255, 255, 0);
IPAddress dnsaddy(8,8,8,8);

EthernetClient client;
EthernetUDP udp;
uint8_t packetbuf[256];
static uint8_t scratch_raw[32];
static coap_rw_buffer_t scratch_buf = {scratch_raw, sizeof(scratch_raw)};


// Number of RGB LEDs in strand:
int nLEDs = 64;

// Chose 2 pins for output; can be any valid output pins:
int dataPin  = 2;
int clockPin = 3;

// First parameter is the number of LEDs in the strand.  The LED strips
// are 32 LEDs per meter but you can extend or cut the strip.  Next two
// parameters are SPI data and clock pins:
LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);


int r1=0;
int g1=0;
int b1=0;

int r2=0;
int g2=0;
int b2=0;

extern "C" {

  void stripSet1(int r, int g, int b) {
    r1=r;
    g1=g;
    b1=b;
  }

  void stripSet2(int r, int g, int b) {
    r2=r;
    g2=g;
    b2=b;
  }
  
  int readTemp() {
    return analogRead(A5);
  }
  
  int readLum() {
    return analogRead(A4);
  }
}

void setup()
{

    int i;
    Serial.begin(9600);
    while (!Serial) 
    {
      ; // wait for serial port to connect. Needed for Leonardo only
    }
Serial.println("Hello CoAP");
Serial.flush();
    // start the Ethernet connection:
Serial.println("Configuring static IP address");   
    
    Ethernet.begin(mac, ip, dnsaddy, gateway, subnet);
    
    Serial.print("My IP address: ");
    for (i=0;i<4;i++)
    {
        Serial.print(Ethernet.localIP()[i], DEC);
        Serial.print("."); 
    }
    Serial.println();
    udp.begin(PORT);


  strip.begin();

  // Update the strip, to start they are all 'off'
  strip.show();
  
    coap_setup();
    endpoint_setup();
}

void udp_send(const uint8_t *buf, int buflen)
{
    udp.beginPacket(udp.remoteIP(), udp.remotePort());
    while(buflen--)
        udp.write(*buf++);
    udp.endPacket();
}

int it=0;
int subit=0;
void loop()
{
    int sz;
    int rc;
    coap_packet_t pkt;
    int i;
    
    if ((sz = udp.parsePacket()) > 0)
    {
        udp.read(packetbuf, sizeof(packetbuf));

        for (i=0;i<sz;i++)
        {
            Serial.print(packetbuf[i], HEX);
            Serial.print(" ");
        }
        Serial.println("");

        if (0 != (rc = coap_parse(&pkt, packetbuf, sz)))
        {
            Serial.print("Bad packet rc=");
            Serial.println(rc, DEC);
        }
        else
        {
            size_t rsplen = sizeof(packetbuf);
            coap_packet_t rsppkt;
            coap_handle_req(&scratch_buf, &pkt, &rsppkt);

            memset(packetbuf, 0, UDP_TX_PACKET_MAX_SIZE);
            if (0 != (rc = coap_build(packetbuf, &rsplen, &rsppkt)))
            {
                Serial.print("coap_build failed rc=");
                Serial.println(rc, DEC);
            }
            else
            {
                udp_send(packetbuf, rsplen);
            }
        }
    }
    
    // update led strip
    
    strip.show();
    subit++;
    if (subit >5) {
      subit=0;
      it=(it+1) % nLEDs;      
      float dr = r2 -r1 / (float) nLEDs;
      float dg = g2 -g1 / (float) nLEDs;
      float db = b2 -b1 / (float) nLEDs;
      
      for (i=0; i < strip.numPixels(); i++) {    
        
        int r = (i+it)* dr + r1;
        int g = (i+it)* dg + g1;
        int b = (i+it)* db + b1;
        strip.setPixelColor(i, strip.Color(r,g,b));
      }  
    }
    
    
   // Serial.print("IT: ");
   // Serial.println(it);
}

