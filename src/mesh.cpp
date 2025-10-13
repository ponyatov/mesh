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

Eth::Eth(int port) : Object() {  //
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
