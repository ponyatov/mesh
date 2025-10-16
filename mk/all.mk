.PHONY: all run watch
all: bin/$(BINFILE) $(S)
run: bin/$(BINFILE) $(S)
	sudo $^
watch: $(C) $(H) $(MK) $(CM)
	find inc lib src cmake mk | entr -r make run

.PHONY: server
server: $(P) $(wildcard static/*)
	$(PY) $<
