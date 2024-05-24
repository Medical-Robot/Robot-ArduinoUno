/*
  Network conversions

  This example tests the implementation of the htonX() and ntohX functions 
  of the netconv library.

  The sketch can be viewed as a unit test, although you have to check the
  correctness of the results for yourself.
    
  This example code is in the public domain.
*/

#include <netconv.h>

uint8_t tempbuf[8];
uint8_t hostbytes[8];
uint8_t netbytes[8];

// unsigned long long
uint64_t ll1 = 0;
uint64_t ll2 = 0;

// unsigned long
uint32_t l1 = 0;
uint32_t l2 = 0;

// unsigned short
uint16_t s1 = 0;
uint16_t s2 = 0;

// float (32 bit)
float f1 = 0.0f;
float f2 = 0.0f;

// some fixed number for testing
const uint64_t testlonglong1 = 0x1122334455667788;
const uint32_t testlong1     = 0xA1B2C3D4; 
const uint16_t testshort1    = 0xE5F6;

// pi, 0x40490FDB in interchange format
const float testfloat1       = 3.14159274101257324;

// one third, 0x3EAAAAAB in interchange format
const float testfloat2       = 1.0f / 3.0f;

// pi, 0x4009 21FB 5444 2D18 in interchange format
const double testdouble1     = 3.14159265358979323846264338327950288419716939937510;


// helper function template to be able to convert any type of number to an array of bytes
template <typename T>
size_t number_to_bytearray(T n, uint8_t buffer[]) {
    size_t s = sizeof(T);
    memcpy(buffer, &n, s);
    return s;
}

// helper function template to be able to convert an array of bytes back to any type of number
template <typename T>
T bytearray_to_number(uint8_t buffer[]) {
    T retval;
    memcpy(&retval, buffer, sizeof(T));
    return retval;
}

// helper funtion to pretty-print uint8_t arrays
void print_buffer(uint8_t buffer[], size_t buffersize, bool newline=true) {
    for (size_t i=0; i<buffersize; i++) {
        if (buffer[i] < 16) Serial.print("0");
        Serial.print(buffer[i], HEX);
        Serial.print(F(" "));
    }
    if (newline) Serial.println();
}

void setup() {

    size_t s;
    
    Serial.begin(115200);
    while (!Serial);
    
    Serial.println(F("\n\nStarting conversion tests..."));

    
    Serial.print(F("__SIZEOF_FLOAT__:  "));
    Serial.println(__SIZEOF_FLOAT__, DEC);
    Serial.print(F("__SIZEOF_DOUBLE__: "));
    Serial.println(__SIZEOF_DOUBLE__, DEC);

    if (__SIZEOF_DOUBLE__ != 8) {
        Serial.println(F("Platform does not support 'real' double precision numbers. htond() and ntohd() are not available."));
    }
    
    #if ( __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ )
    Serial.println(F("This processor uses little-endian integers. Expect byte swaps when converting to network byte order and back."));
    #else
    Serial.println(F("This processor uses big-endian integers. Integers are already in the correct network byte order."));
    #endif

    #if (__FLOAT_WORD_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    Serial.println(F("This processor uses little-endian floats. Expect byte swaps when converting to network byte order and back."));
    #else
    Serial.println(F("This processor uses big-endian floats (sign byte at lowest memory address). Numbers are already in the correct network byte order."));
    #endif

    /* htonl(testlonglong1) */
    
    Serial.println();
    Serial.println(F("testlonglong1:"));

    //print original value
    Serial.print(F("original value (in HEX):                          0x"));
//    Serial.print(testlonglong1, HEX); // Serial.print() fpr uint64_t is not implemented
    Serial.println();

    Serial.println(F("Convert original value to array of bytes - keeping the byte order!"));
    s = number_to_bytearray<uint64_t>(testlonglong1, hostbytes);
    Serial.print(F("original value in byte representation (*hostbytes): "));
    print_buffer(hostbytes, s, true);

    Serial.println(F("Convert single number in host byte order to single number in network byte order."));
    Serial.print(F("value of htonll(x) in byte representation:          "));
    ll1 = htonll(testlonglong1);
    s = number_to_bytearray<uint64_t>(ll1, tempbuf);
    print_buffer(tempbuf, s, true);

    Serial.println(F("Convert a single number in host byte order, write to array of bytes in network byte order."));
    Serial.print(F("netbytes after htonll(x, *netbytes):                "));
    htonll(testlonglong1, netbytes);
    print_buffer(netbytes, s, true);

    Serial.println(F("Convert array of bytes in host byte order, return single number in network byte order."));
    Serial.print(F("value of htonll(*hostbytes) in byte representation: "));
    ll2 = htonll(hostbytes);
    s = number_to_bytearray<uint64_t>(ll2, tempbuf);
    print_buffer(tempbuf, s, true);
    

   /* ntohl(testlonglong1) */

    Serial.println(F("\nConverting back..."));
   
    Serial.println(F("Convert single number in network byte order to single number in host byte order."));
    Serial.print(F("value of ntohll(x) in byte representation:          "));
    ll2 = ntohll(ll1);
    s = number_to_bytearray<uint64_t>(ll2, tempbuf);
    print_buffer(tempbuf, s, true);

    Serial.println(F("Convert a single number in network byte order, write to array of bytes in host byte order."));
    Serial.print(F("hostbytes after ntohll(x, *netbytes):               "));
    ntohll(ll1, hostbytes);
    print_buffer(hostbytes, s, true);

    Serial.println(F("Convert array of bytes in network byte order, return single number in host byte order."));
    Serial.print(F("value of ntohll(*netbytes) in byte representation:  "));
    ll2 = ntohll(netbytes);
    s = number_to_bytearray<uint64_t>(ll2, tempbuf);
    print_buffer(tempbuf, s, true);

    /* htonl(testlong1) */
    
    Serial.println();
    Serial.println(F("testlong1:"));

    //print original value
    Serial.print(F("original value (in HEX):                          0x"));
    Serial.println(testlong1, HEX);

    Serial.println(F("Convert original value to array of bytes - keeping the byte order!"));
    s = number_to_bytearray<uint32_t>(testlong1, hostbytes);
    Serial.print(F("original value in byte representation (*hostbytes): "));
    print_buffer(hostbytes, s, true);

    Serial.println(F("Convert single number in host byte order to single number in network byte order."));
    Serial.print(F("value of htonl(x) in byte representation:           "));
    l1 = htonl(testlong1);
    s = number_to_bytearray<uint32_t>(l1, tempbuf);
    print_buffer(tempbuf, s, true);

    Serial.println(F("Convert a single number in host byte order, write to array of bytes in network byte order."));
    Serial.print(F("netbytes after htonl(x, *netbytes):                 "));
    htonl(testlong1, netbytes);
    print_buffer(netbytes, s, true);

    Serial.println(F("Convert array of bytes in host byte order, return single number in network byte order."));
    Serial.print(F("value of htonl(*hostbytes) in byte representation:  "));
    l2 = htonl(hostbytes);
    s = number_to_bytearray<uint32_t>(l2, tempbuf);
    print_buffer(tempbuf, s, true);
    

   /* ntohl(testlong1) */

    Serial.println(F("\nConverting back..."));
   
    Serial.println(F("Convert single number in network byte order to single number in host byte order."));
    Serial.print(F("value of ntohl(x) in byte representation:           "));
    l2 = ntohl(l1);
    s = number_to_bytearray<uint32_t>(l2, tempbuf);
    print_buffer(tempbuf, s, true);

    Serial.println(F("Convert a single number in network byte order, write to array of bytes in host byte order."));
    Serial.print(F("hostbytes after ntohl(x, *netbytes):                "));
    ntohl(l1, hostbytes);
    print_buffer(hostbytes, s, true);

    Serial.println(F("Convert array of bytes in network byte order, return single number in host byte order."));
    Serial.print(F("value of ntohl(*netbytes) in byte representation:   "));
    l2 = ntohl(netbytes);
    s = number_to_bytearray<uint32_t>(l2, tempbuf);
    print_buffer(tempbuf, s, true);

    /* htons(testshort1) */

    Serial.println();
    Serial.println(F("testshort1:"));

    //print original value
    Serial.print(F("original value (in HEX):                          0x"));
    Serial.println(testshort1, HEX);

    Serial.println(F("Convert original value to array of bytes - keeping the byte order!"));
    s = number_to_bytearray<uint16_t>(testshort1, hostbytes);
    Serial.print(F("original value in byte representation (*hostbytes): "));
    print_buffer(hostbytes, s, true);

    Serial.println(F("Convert single number in host byte order to single number in network byte order."));
    Serial.print(F("value of htons(x) in byte representation:           "));
    s1 = htons(testshort1);
    s = number_to_bytearray<uint16_t>(s1, tempbuf);
    print_buffer(tempbuf, s, true);

    Serial.println(F("Convert a single number in host byte order, write to array of bytes in network byte order."));
    Serial.print(F("netbytes after htons(x, *netbytes):                 "));
    htons(testshort1, netbytes);
    print_buffer(netbytes, s, true);

    Serial.println(F("Convert array of bytes in host byte order, return single number in network byte order."));
    Serial.print(F("value of htons(*hostbytes) in byte representation:  "));
    s2 = htons(hostbytes);
    s = number_to_bytearray<uint16_t>(s2, tempbuf);
    print_buffer(tempbuf, s, true);  

   /* ntohl(testshort1) */

    Serial.println(F("\nConverting back..."));
   
    Serial.println(F("Convert single number in network byte order to single number in host byte order."));
    Serial.print(F("value of ntohs(x) in byte representation:           "));
    s2 = ntohs(s1);
    s = number_to_bytearray<uint16_t>(s2, tempbuf);
    print_buffer(tempbuf, s, true);

    Serial.println(F("Convert a single number in network byte order, write to array of bytes in host byte order."));
    Serial.print(F("hostbytes after ntohs(x, *netbytes):                "));
    ntohs(s1, hostbytes);
    print_buffer(hostbytes, s, true);

    Serial.println(F("Convert array of bytes in network byte order, return single number in host byte order."));
    Serial.print(F("value of ntohs(*netbytes) in byte representation:   "));
    l2 = ntohs(netbytes);
    s = number_to_bytearray<uint16_t>(s2, tempbuf);
    print_buffer(tempbuf, s, true);
    
    /* htonf(testfloat1) */

    Serial.println();
    Serial.println(F("testfloat1:"));

    //print original value
    Serial.print(F("original value:                                     "));
    Serial.println(testfloat1, 8);
    Serial.println(F("Should be 0x40490FDB in network interchange format."));

    Serial.println(F("Convert original value to array of bytes - keeping the byte order!"));
    s = number_to_bytearray<float>(testfloat1, hostbytes);
    Serial.print(F("original value in byte representation (*hostbytes): "));
    print_buffer(hostbytes, s, true);

    Serial.println(F("Convert single number in host byte order to single number in network byte order."));
    Serial.print(F("value of htonf(x) in byte representation:           "));
    l1 = htonf(testfloat1); // result is an uint32_t!
    s = number_to_bytearray<uint32_t>(l1, tempbuf);
    print_buffer(tempbuf, s, true);

    Serial.println(F("Convert a single number in host byte order, write to array of bytes in network byte order."));
    Serial.print(F("netbytes after htonf(x, *netbytes):                 "));
    htonf(testfloat1, netbytes);
    print_buffer(netbytes, s, true);

    Serial.println(F("Convert array of bytes in host byte order, return single number in network byte order."));
    Serial.print(F("value of htonf(*hostbytes) in byte representation:  "));
    l2 = htonf(hostbytes); // result is an uint32_t!
    s = number_to_bytearray<uint32_t>(l2, tempbuf);
    print_buffer(tempbuf, s, true);  


   /* ntohl(testfloat1) */

    Serial.println(F("\nConverting back..."));
   
    Serial.println(F("Convert single number in network byte order to single number in host byte order."));
    Serial.print(F("value of ntohf(x) in byte representation:           "));
    f1 = ntohf(l1);
    s = number_to_bytearray<float>(f1, tempbuf);
    print_buffer(tempbuf, s, true);

    Serial.println(F("Convert a single number in network byte order, write to array of bytes in host byte order."));
    Serial.print(F("hostbytes after ntohf(x, *hostbytes):               "));
    ntohf(l1, hostbytes);
    print_buffer(hostbytes, s, true);

    Serial.println(F("Convert array of bytes in network byte order, return single number in host byte order."));
    Serial.print(F("value of ntohf(*netbytes) in byte representation:   "));
    f2 = ntohf(netbytes);
    s = number_to_bytearray<float>(f2, tempbuf);
    print_buffer(tempbuf, s, true);
    
    /* htonf(testfloat2) */

    Serial.println();
    Serial.println(F("testfloat2:"));

    //print original value
    Serial.print(F("original value:                                     "));
    Serial.println(testfloat2, 8);
    Serial.println(F("Should be 0x3EAAAAAB in network interchange format."));

    Serial.println(F("Convert original value to array of bytes - keeping the byte order!"));
    s = number_to_bytearray<float>(testfloat2, hostbytes);
    Serial.print(F("original value in byte representation (*hostbytes): "));
    print_buffer(hostbytes, s, true);

    Serial.println(F("Convert single number in host byte order to single number in network byte order."));
    Serial.print(F("value of htonf(x) in byte representation:           "));
    l1 = htonf(testfloat2); // result is an uint32_t!
    s = number_to_bytearray<uint32_t>(l1, tempbuf);
    print_buffer(tempbuf, s, true);

    Serial.println(F("Convert a single number in host byte order, write to array of bytes in network byte order."));
    Serial.print(F("netbytes after htonf(x, *netbytes):                 "));
    htonf(testfloat2, netbytes);
    print_buffer(netbytes, s, true);

    Serial.println(F("Convert array of bytes in host byte order, return single number in network byte order."));
    Serial.print(F("value of htonf(*hostbytes) in byte representation:  "));
    l2 = htonf(hostbytes); // result is an uint32_t!
    s = number_to_bytearray<uint32_t>(l2, tempbuf);
    print_buffer(tempbuf, s, true);  


   /* ntohl(testfloat2) */

    Serial.println(F("\nConverting back..."));
   
    Serial.println(F("Convert single number in network byte order to single number in host byte order."));
    Serial.print(F("value of ntohf(x) in byte representation:           "));
    f1 = ntohf(l1);
    s = number_to_bytearray<float>(f1, tempbuf);
    print_buffer(tempbuf, s, true);

    Serial.println(F("Convert a single number in network byte order, write to array of bytes in host byte order."));
    Serial.print(F("hostbytes after ntohf(x, *hostbytes):               "));
    ntohf(l1, hostbytes);
    print_buffer(hostbytes, s, true);

    Serial.println(F("Convert array of bytes in network byte order, return single number in host byte order."));
    Serial.print(F("value of ntohf(*netbytes) in byte representation:   "));
    f2 = ntohf(netbytes);
    s = number_to_bytearray<float>(f2, tempbuf);
    print_buffer(tempbuf, s, true);


    #if (__SIZEOF_DOUBLE__ == 8)

    /* htond(testdouble1) */

    Serial.println();
    Serial.println(F("testdouble1:"));

    //print original value
    Serial.print(F("original value:                                     "));
    Serial.println(testdouble1, 16);
    Serial.println(F("Should be  0x(4009 21FB 5444 2D18) in network interchange format."));

    Serial.println(F("Convert original value to array of bytes - keeping the byte order!"));
    s = number_to_bytearray<double>(testdouble1, hostbytes);
    Serial.print(F("original value in byte representation (*hostbytes): "));
    print_buffer(hostbytes, s, true);

    Serial.println(F("Convert single number in host byte order to single number in network byte order."));
    Serial.print(F("value of htond(x) in byte representation:           "));
    ll1 = htond(testdouble1); // result is an uint64_t!
    s = number_to_bytearray<uint64_t>(ll1, tempbuf);
    print_buffer(tempbuf, s, true);

    Serial.println(F("Convert a single number in host byte order, write to array of bytes in network byte order."));
    Serial.print(F("netbytes after htond(x, *netbytes):                 "));
    htond(testdouble1, netbytes);
    print_buffer(netbytes, s, true);

    Serial.println(F("Convert array of bytes in host byte order, return single number in network byte order."));
    Serial.print(F("value of htond(*hostbytes) in byte representation:  "));
    ll2 = htond(hostbytes); // result is an uint64_t!
    s = number_to_bytearray<uint64_t>(ll2, tempbuf);
    print_buffer(tempbuf, s, true);  

       /* ntohd(testdouble1) */

    Serial.println(F("\nConverting back..."));

    double d1 = 0.0;
    double d2 = 0.0;

    Serial.println(F("Convert single number in network byte order to single number in host byte order."));
    Serial.print(F("value of ntohd(x) in byte representation:           "));
    d1 = ntohd(ll1);
    s = number_to_bytearray<double>(d1, tempbuf);
    print_buffer(tempbuf, s, true);

    Serial.println(F("Convert a single number in network byte order, write to array of bytes in host byte order."));
    Serial.print(F("hostbytes after ntohd(x, *hostbytes):               "));
    ntohd(ll1, hostbytes);
    print_buffer(hostbytes, s, true);

    Serial.println(F("Convert array of bytes in network byte order, return single number in host byte order."));
    Serial.print(F("value of ntohd(*netbytes) in byte representation:   "));
    d2 = ntohd(netbytes);
    s = number_to_bytearray<double>(d2, tempbuf);
    print_buffer(tempbuf, s, true);
    
    #else
    Serial.println(F("\nNot testing hotnd()/ntohd(), double precision floating point numbers are not supported."));
    #endif
    
} // end of setup()

void loop() {
    
}
