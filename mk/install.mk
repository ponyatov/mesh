.PHONY: install update
install: doc python
	$(MAKE) update
update:
	sudo apt update
	sudo apt install -uy `cat apt.Debian`
	$(PIP) install -U -r requirements.txt
