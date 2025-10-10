.PHONY : install update ref gz
install: $(WS)_install doc ref gz python
	$(MAKE) update
	$(MAKE) pcpp
update : $(WS)_update
	$(PIP) install -U -r requirements.txt
ref    : $(RF)
gz     : $(GZ)

Debian_install:
# sudo dpkg --add-architecture i386
Debian_update:
	sudo apt update
	sudo apt install -uy `cat apt.$(WS)` $(APT)
