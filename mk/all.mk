.PHONY: all run
all: bin/$(BINFILE) $(S)
run: bin/$(BINFILE) $(S)
	$^

.PHONY: server
server: $(P) $(wildcard static/*)
	$(PY) $<
