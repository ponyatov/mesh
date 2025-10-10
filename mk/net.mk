PCPP_CFG += -DPCAPPP_BUILD_EXAMPLES=OFF
PCPP_CFG += -DPCAPPP_BUILD_TESTS=OFF
PCPP_CFG += -DPCAPPP_BUILD_TUTORIALS=OFF
PCPP_CFG += -DBUILD_SHARED_LIBS=OFF
PCPP_CFG += -DPCAPPP_USE_DPDK=ON
PCPP_CFG += -DPCAPPP_USE_XDP=OFF
PCPP_CFG += -DPCAPPP_BUILD_PCAPPP=ON

.PHONY: pcpp
pcpp: ref/PcapPlusPlus/README.md
	cmake $(PCPP_CFG) -S $(dir $<) -B tmp/$@ --install-prefix=$(LIB)/pcpp
	cmake --build   tmp/$@
	cmake --install tmp/$@

# # components: need examples for testing DPDK installed ok
# PCPP_CFG += -DPCAPPP_BUILD_EXAMPLES=ON
# PCPP_CFG += -DPCAPPP_BUILD_TESTS=OFF
# PCPP_CFG += -DPCAPPP_BUILD_TUTORIALS=OFF
# # pcpp
# PCPP_CFG += -DBUILD_SHARED_LIBS=OFF
# PCPP_CFG += -DPCAPPP_BUILD_PCAPPP=ON
# PCPP_CFG += -DPCAPPP_INSTALL_INCLUDEDIR=include
# PCPP_CFG += -DPCAPPP_USE_DPDK=ON
# # DPDK_ROOT = /opt/dpdk/dpdk-stable-$(DPDK_VER)/mydpdk
# # PCPP_CFG += -DDPDK_ROOT=$(DPDK_ROOT)
# PCPP_CFG += -DDPDK_VER=$(DPDK_VER) -DDPDK_DEBUG=YES
# # PCPP_CFG += -DDPDK_INCLUDE_DIRS=$(DPDK_ROOT)/include
# # PCPP_CFG += -DCMAKE_C_FLAGS="-march=native -msse4.1"
# # PCPP_CFG += -DCMAKE_CXX_FLAGS="-march=native -msse4.1"
# # # # PCPP_CFG += -DLIGHT_PCAPNG_ZSTD=ON
# # # # PCPP_CFG += -DPCAPPP_USE_XDP=ON
# # # # PCPP_CFG += -DPCAPPP_LOG_LEVEL=1

# .PHONY: pcpp
# pcpp: lib/pcpp/setup_dpdk.py lib/pcpp/lib/libPacket++.a
# # 	rm -rf lib/pcpp tmp/pcpp ; time make pcpp
# lib/pcpp/setup_dpdk.py: tmp/pcpp/setup_dpdk.py
# 	sed '1s/python/python3/' $< > $@ ; chmod +x $@
# tmp/pcpp/setup_dpdk.py: lib/pcpp/lib/libPacket++.a
# lib/pcpp/lib/libPacket++.a: ref/PcapPlusPlus/README.md
# 	cmake $(PCPP_CFG) -S $(dir $<) -B tmp/pcpp --install-prefix=$(LIB)/pcpp
# 	cmake --build   tmp/pcpp -j
# 	cmake --install tmp/pcpp
