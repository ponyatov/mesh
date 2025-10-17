RF += ref/PcapPlusPlus/README.md
ref/PcapPlusPlus/README.md:
	$(GITREF) -b v$(PCPP_VER) https://github.com/seladb/PcapPlusPlus.git $(dir $@)

RF += ref/reciever/package.json
ref/reciever/package.json:
	$(GITREF) -b master git@gitflic.ru:dponyatov/reciever.git $(dir $@)
