PCPP_CFG += -DPCAPPP_BUILD_EXAMPLES=OFF
PCPP_CFG += -DPCAPPP_BUILD_TESTS=OFF
PCPP_CFG += -DPCAPPP_BUILD_TUTORIALS=OFF
PCPP_CFG += -DBUILD_SHARED_LIBS=OFF
PCPP_CFG += -DPCAPPP_USE_DPDK=ON
PCPP_CFG += -DPCAPPP_USE_XDP=OFF
PCPP_CFG += -DPCAPPP_BUILD_PCAPPP=ON
# -msse4.1
PCPP_CFG += -DCMAKE_C_FLAGS="-march=native"
PCPP_CFG += -DCMAKE_CXX_FLAGS="-march=native"
PCPP_CFG += -DPCAPPP_INSTALL_CMAKEDIR=$(LIB)/pcpp/cmake
PCPP_CFG += -DPCAPPP_INSTALL_CMAKEDIR=$(LIB)/pcpp/include

.PHONY: pcpp
pcpp: lib/pcpp/setup_dpdk.py
# rm -rf lib/pcpp tmp/pcpp ; time make pcpp
lib/pcpp/setup_dpdk.py: tmp/pcpp/setup_dpdk.py
	sed '1s/python/python3/' $< > $@ ; chmod +x $@
tmp/pcpp/setup_dpdk.py: lib/pcpp/lib/libPacket++.a
lib/pcpp/lib/libPacket++.a: ref/PcapPlusPlus/README.md
	cmake $(PCPP_CFG) -S $(dir $<) -B tmp/pcpp --install-prefix=$(LIB)/pcpp
	cmake --build   tmp/pcpp -j
	cmake --install tmp/pcpp
