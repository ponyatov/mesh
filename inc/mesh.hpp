#pragma once

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

/// @defgroup REPL REPL

/// @defgroup main main
/// @brief system startup
/// @ingroup REPL
/// @{
extern int main(int argc, char *argv[]);  ///< POSIX entry point
extern void arg(int argc, char *argv);    ///< process command line argument
/// @}

/// @defgroup gc gc
/// @brief Garbage collection
/// @ingroup memory

/// @defgroup vm VM
/// @brief FORTH Virtual Machine (for scripting)

/// @defgroup object object
/// @brief object graph

/// @brief root object graph class\
/// @ingroup object
class Object {
    std::string tag();
    virtual std::string val();

   public:
    /// @ingroup gc
    /// reference counter
    size_t ref;
    Object();
    virtual ~Object();
    virtual std::string dump(std::string prefix = "");
};

/// @defgroup prim primitive
/// @ingroup object

/// @brief primitive types
/// @ingroup prim
class Primitive : public Object {
   public:
    Primitive();
};

/// @brief integer number
/// @ingroup prim
class Int : public Primitive {
   public:
    int value;
    Int(char *V);
    std::string val();
};

/// @brief floating point number
/// @ingroup prim
class Num : public Primitive {
    float value;

   public:
    Num(char *V);
    std::string val();
};

/// @defgroup config config
/// @brief @ref memory sizes
/// @ingroup vm
/// @{

/// @brief @ref M size
#define Msz 0x10000
/// @brief @ref R size
#define Rsz 0x100
/// @brief @ref D size
#define Dsz 0x10

/// @}

/// @defgroup type type
/// @brief @ref vm specific types
/// @ingroup vm
/// @{
typedef uint8_t byte;   ///< single byte
typedef uint16_t addr;  ///< short @ref M address
typedef Object *cell;   ///< storage element type
/// @}

/// @defgroup memory memory
/// @ingroup vm
/// @{

extern cell M[Msz];  ///< main memory
extern addr Cp;      ///< compiler pointer
extern addr R[Rsz];  ///< return stack
extern byte Rp;      ///< @ref R pointer
extern cell D[Dsz];  ///< data stack
extern byte Dp;      ///< @ref D pointer

/// @}

/// #defgroup command command
/// @ingroup vm
/// @{

/// @name debug
/// @{
extern void dump();  ///< `( -- )` dump @ref vm state

/// @name stack
/// @{
extern void push(Object *o);  ///< `( -- o )` push to @ref D
extern Object *pop();         ///< `( o -- )` pop from @ref D
extern Object *top();         ///< `( o -- o )` get @ref D top
extern void clear();          ///< `( ... -- )` clear @ref D

extern void dup();    ///< `DUP `( a -- a )`
extern void drop();   ///< `DROP `( a b -- a )`
extern void press();  ///< `PRESS `( a b -- b )`
extern void swap();   ///< `SWAP `( a b -- b a )`
extern void over();   ///< `OVER `( a b -- a b a )`

/// @}
/// @}

/// @defgroup io io
/// @ingroup object
/// @{
class IO : public Object {
   public:
    IO();
};
/// @}

/// @defgroup net net
/// @ingroup io
/// @brief raw networking
#include "DpdkDevice.h"
#include "DpdkDeviceList.h"
#include "EthLayer.h"
#include "IPv4Layer.h"
#include "IpAddress.h"
#include "MacAddress.h"
#include "Packet.h"
#include "PayloadLayer.h"
#include "PcapLiveDeviceList.h"
#include "RawPacket.h"
#include "UdpLayer.h"
/// @{

#define DEFAULT_MBUF_POOL_SIZE (0x10000 - 1)

#define SENDMAC1 "e8:eb:d3:93:42:98"
#define SENDMAC2 "e8:eb:d3:93:42:99"
#define SENDIP "10.120.101.111"
#define SEND_INTERVAL_MS 1111 /* ms */

#define RECVMAC "e8:eb:d3:93:42:91"
#define BROADCAST "ff:ff:ff:ff:ff:ff"
#define RECVIP "10.120.101.11"

/// @defgroup wireshark wireshark
/// @brief remote Wireshark UDP
/// @{
#define WSIP "10.110.21.104"
#define WSPORT "12345"
//// @}

/// @brief NIC representation
class Eth : public IO {
    static bool initialized;

    static const uint32_t mBufPoolSize = DEFAULT_MBUF_POOL_SIZE;

    int id;               ///< DPDK port id (for opened @ref dev)
    std::string name;     ///< DPDK port name
    int mtu;              ///< MTU packet size
    std::string pmdname;  ///< driver name
    int pmdtype;          ///< driver id

    bool up;      ///< @ref linkStatus -> linkUp
    int speed;    ///< @ref linkStatus -> linkSpeedMbps
    bool duplex;  ///< @ref linkStatus -> linkDuplex
    pcpp::DpdkDevice::LinkStatus linkStatus;  ///< link status info

   public:
    pcpp::DpdkDevice *dev;                   ///< DPDK device id
    Eth(int port);                           ///< create nic with DPDK index
    static bool init();                      // int argc, char *argv[]);
    static void list();                      ///< list available nic's
    void open();                             ///< open port with Rx/Tx
    void close();                            ///< shutdown
    pcpp::DpdkDevice::LinkStatus &status();  ///< update @ref linkStatus
    std::string val();                       ///<
};

class Recv : public IO, pcpp::DpdkWorkerThread {
    Eth *_eth;
    pcpp::DpdkDevice *_dev;
    uint32_t _coreId;
    bool _stop;

   public:
    Recv(Eth *);
    bool run(uint32_t coreId);
    void stop();
    uint32_t getCoreId() const;
};

class Send : public IO, pcpp::DpdkWorkerThread {
    Eth *_eth;
    pcpp::DpdkDevice *_dev;
    uint32_t _coreId;
    bool _stop;

   public:
    Send(Eth *);
    bool run(uint32_t coreId);
    void stop();
    uint32_t getCoreId() const;
};

/// @}

/// @defgroup skelex skelex
/// @brief syntax parser
/// @ingroup REPL
/// @{
extern int yylex();                    ///< lexer
extern int yylineno;                   ///< current file line number
extern char *yyfile;                   ///< current file name
extern FILE *yyin;                     ///< current file handler
extern char *yytext;                   ///< lexeme (token) string value
extern int yyparse();                  ///< parser
extern void yyerror(const char *msg);  ///< syntax error callback
#include "mesh.yacc.hpp"
#define TOKEN(C, X)               \
    {                             \
        yylval.o = new C(yytext); \
        return X;                 \
    }
/// @}

/// @defgroup data data
/// @brief customer data model
/// @{

class SensorGroup;

/// @brief customer sensor
class Sensor {
    SensorGroup *group;        ///< exclusive group membering
    void add(SensorGroup *g);  ///< add sensor to group
    void rm(SensorGroup *g);   ///< remove sensor from group
};

/// @brief burst group
class SensorGroup {
    std::vector<Sensor *> sensors;     ///< group members
    std::chrono::milliseconds period;  ///< data capture period (burst send)

    void add(Sensor *s);  ///< add sensor to group
    void rm(Sensor *s);   ///< remove sensor from group
};

/// @brief large binary image of raw data
/// @details must be splitted into UDP packages bursts
class BLOB {
    size_t size;          ///< data size, bytes
    SensorGroup *source;  ///< data source
};

/// @}
