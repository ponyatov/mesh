#include "mesh.hpp"

int main(int argc, char *argv[]) {  //
    arg(0, argv[0]);
    for (int i = 1; i < argc; i++) {  //
        arg(i, argv[i]);
    }
}

void arg(int argc, char *argv) {  //
    std::cerr << "argv[" << argc << "] = <" << argv << "]\n";
    if (argc) {
        yyfile = argv;
        assert(yyin = fopen(yyfile, "r"));
        yyparse();
        fclose(yyin);
        yyfile = nullptr;
    }
}

void yyerror(const char *msg) {  //
    std::cerr << "\n\n"
              << yyfile << ":" << yylineno << " " << msg << " [" << yytext
              << "]\n\n";
    exit(-1);
}

cell D[Dsz];
byte Dp = 0;

void dump() {  //
    std::cerr << "D:[";
    for (int i = 0; i < Dp; i++) std::cerr << D[i]->dump(" ");
    std::cerr << " ]\n";
}

void push(Object *o) {
    assert(Dp < Dsz);
    D[Dp++] = o;
    o->ref++;
}

Object *pop() {
    assert(Dp > 0);
    assert(Dp < Dsz);
    Object *o = D[--Dp];
    assert(o->ref);
    o->ref--;
    return o;
}

Object *top() {
    assert(Dp);
    assert(Dp < Dsz);
    Object *o = D[Dp - 1];
    assert(o->ref);
    return o;
}

void clear() {
    for (int i = 0; i < Dp; i++) pop();
}

extern void dup() {  //
    push(top());
}

extern void drop() {  //
    pop();
}

extern void press() {  //
    Object *o = pop();
    pop();
    push(o);
}

extern void swap() {  //
    Object *b = pop();
    Object *a = pop();
    push(b);
    push(a);
}

extern void over() {  //
    push(D[Dp - 2]);
}

Object::Object() : ref(0) {}

Object::~Object() { assert(!ref); }

#include <cxxabi.h>

std::string Object::tag() {
    std::string ret =
        abi::__cxa_demangle(typeid(*this).name(), NULL, NULL, nullptr);
    for (char &c : ret) c = tolower(c);
    return ret;
}

std::string Object::val() { return ""; }

std::string Object::dump(std::string prefix) {
    std::ostringstream os;
    os << prefix << tag() << ':' << val();
    return os.str();
}

Primitive::Primitive() : Object() {}

Int::Int(char *V) : Primitive(), value(atoi(V)) {}
Num::Num(char *V) : Primitive(), value(atof(V)) {}

std::string Int::val() {
    std::ostringstream os;
    os << value;
    return os.str();
}

std::string Num::val() {
    std::ostringstream os;
    os << value;
    return os.str();
}

IO::IO() : Object() {}

bool Eth::init() {  // int argc, char *argv[]) {  //
    pcpp::CoreMask coreMask = pcpp::getCoreMaskForAllMachineCores();
    // rte_eal_init(argc, argv);
    assert(pcpp::DpdkDeviceList::initDpdk(coreMask, Eth::mBufPoolSize));
    return true;
}

bool Eth::initialized = false;

void Eth::list() {
    // PCAP
    pcpp::PcapLiveDeviceList &pcapDeviceList =
        pcpp::PcapLiveDeviceList::getInstance();
    const std::vector<pcpp::PcapLiveDevice *> &devices =
        pcapDeviceList.getPcapLiveDevicesList();
    std::cerr << "\nPCAP:";
    for (auto *dev : devices) {
        auto mtu = dev->getMtu();
        if (mtu) {
            std::cerr << "\n\t" << dev->getName()              //
                      << " ( " << dev->getDesc() << " )"       //
                      << "\n\t\tMAC:" << dev->getMacAddress()  //
                      << "\tMTU:" << mtu                       //
                      << "\n\t\tIP:" << dev->getIPv4Address()  //
                      << "\tGW:" << dev->getDefaultGateway()   //
                ;
        }
    }
    // DPDK
    if (!Eth::initialized) Eth::initialized = Eth::init();
    auto dpdkDeviceList =
        pcpp::DpdkDeviceList::getInstance().getDpdkDeviceList();
    std::cerr << "\nDPDK:";
    for (auto &dev : dpdkDeviceList) {  //
        auto mtu = dev->getMtu();
        if (mtu) {
            std::cerr << "\n\tPORT:" << dev->getDeviceId()      //
                      << " ( " << dev->getDeviceName() << " )"  //
                      << "\n\t\tMAC:" << dev->getMacAddress()   //
                      << "\n\t\tPCI:" << dev->getPciAddress()   //
                      << "\n\t\tPMD:" << dev->getPMDName()      //
                      << "/" << dev->getPMDType()               //
                      << "\n\t\tMTU:" << mtu                    //
                ;
        }
    }
    //
    std::cerr << "\n";
}

Eth::Eth(int port) : IO() {  //
    std::cerr << "\nopening port:" << port;
    dev = pcpp::DpdkDeviceList::getInstance().getDeviceByPort(port);
    assert(dev != nullptr);
    id = dev->getDeviceId();
    name = dev->getDeviceName();
    pmdname = dev->getPMDName();
    pmdtype = dev->getPMDType();
    assert(mtu = dev->getMtu());
    std::cerr << "\n\tID:" << id                        //
              << "\tNAME:" << name                      //
              << "\tMTU:" << mtu                        //
              << "\tPMD:" << pmdname << '/' << pmdtype  //
              << "\n";
    open();
}

void Eth::close() {
    dev->close();  // dev = nullptr;
}

void Eth::open() {        //
    assert(dev->open());  // dev->openMultiQueues(1, 1);
    status();
}

pcpp::DpdkDevice::LinkStatus &Eth::status() {
    dev->getLinkStatus(linkStatus);
    up = linkStatus.linkUp;
    speed = linkStatus.linkSpeedMbps;
    duplex =
        (linkStatus.linkDuplex == pcpp::DpdkDevice::LinkStatus::FULL_DUPLEX);
    return linkStatus;
}

std::string Eth::val() {
    std::ostringstream os;
    os << id << '/' << name << "#mtu:" << mtu << "#up:" << up
       << "#duplex:" << duplex << "#speed:" << speed;
    return os.str();
}

Recv::Recv(Eth *eth) {
    _eth = eth;
    _dev = eth->dev;
}

Send::Send(Eth *eth) {
    _eth = eth;
    _dev = eth->dev;
}

bool Recv::run(uint32_t coreId) {
    _coreId = coreId;
    _stop = false;

    const int MBUF_SZ = 1;  // 64

    pcpp::MBufRawPacket *mbufArr[MBUF_SZ] = {};

    std::cerr << "\nrecv:" << "\n";

    while (!_stop) {
        uint16_t numOfPackets = _dev->receivePackets(mbufArr, MBUF_SZ, 0);
        if (numOfPackets) {
            std::cerr << "\npackets:" << numOfPackets << "\n";
            stop();
        }
    }

    std::cerr << "\n";
    return true;
}

bool Send::run(uint32_t coreId) {
    _coreId = coreId;
    _stop = false;

    const int MBUF_SZ = 1;  // 64

    pcpp::MBufRawPacket *mbufArr[MBUF_SZ] = {};

    std::cerr << "\nsend:" << "\n";

    pcpp::Packet packet(0x11);

    pcpp::EthLayer eth_arp(pcpp::MacAddress(RECVMAC),
                           pcpp::MacAddress(BROADCAST));

    pcpp::EthLayer eth_layer(pcpp::MacAddress(RECVMAC),
                             pcpp::MacAddress(BROADCAST));

    packet.addLayer(&eth_layer);

    pcpp::IPv4Layer ipv4_layer(pcpp::IPv4Address(SENDIP),
                               pcpp::IPv4Address(RECVIP));
    ipv4_layer.getIPv4Header()->ipId =
        pcpp::hostToNet16(4000);                  // multipart package
    ipv4_layer.getIPv4Header()->timeToLive = 11;  // shorter path

    packet.addLayer(&ipv4_layer);

    pcpp::UdpLayer udp_layer(12345, 54321);
    // udp_layer.getUdpHeader()->length = pcpp::hostToNet16(1234);
    packet.addLayer(&udp_layer);

    // packet.computeCalculateFields();

#ifdef SEND_INTERVAL_NS
    auto interval = std::chrono::nanoseconds(SEND_INTERVAL_NS);
    auto next_time = std::chrono::high_resolution_clock::now() + interval;
#else
    auto interval = std::chrono::milliseconds(SEND_INTERVAL_MS);
    auto next_time = std::chrono::system_clock::now() + interval;
#endif  // SEND_INTERVAL_NS

    size_t counter = 0;  /// sent packet autocounter
    pcpp::PayloadLayer payloadLayer((uint8_t*)&counter, sizeof(counter));
    packet.addLayer(&payloadLayer);

    while (!_stop) {
        // auto next_packet = packet.clone();
        // next_packet.add(counter++);
        packet.computeCalculateFields();
        uint16_t numOfPackets = _dev->sendPacket(packet);
        // if (numOfPackets) {
        //     std::cerr << "\npackets:" << numOfPackets << "\n";
        //     stop();
        // }
        //
        std::cerr << counter++ << ' ';
        //
        std::this_thread::sleep_until(next_time);
        next_time += interval;
    }

    std::cerr << "\n";
    return true;
}

uint32_t Recv::getCoreId() const { return _coreId; }
uint32_t Send::getCoreId() const { return _coreId; }

void Recv::stop() { _stop = true; }
void Send::stop() { _stop = true; }
