.PHONY : install update ref gz headless
install: $(WS)_install doc ref gz
	$(MAKE) update
	$(MAKE) pcpp
update : $(WS)_update
ref    : $(RF)
gz     : $(GZ)

Debian_install:
# sudo dpkg --add-architecture i386
Debian_update:
	sudo apt update
	sudo apt install -uy `cat apt.$(WS)` $(APT)

headless: doc ref gz
	sudo apt update
	sudo apt install -uy `cat apt.$(WS).headless` $(APT)
	$(MAKE) pcpp
