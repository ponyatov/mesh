.PHONY: ai tmp/$(APP).ai.md
ai: tmp/$(APP).ai.md

MDOC += $(HOME)/metadoc/net/book.md
MDOC += $(HOME)/metadoc/net/mesh.md
MDOC += $(HOME)/metadoc/net/lang*.md
MDOC += $(HOME)/metadoc/net/serverless.md
MDOC += $(HOME)/metadoc/net/Ethernet.md
MDOC += $(HOME)/metadoc/net/*.md

tmp/$(APP).ai.md:
	echo "# low-lever networking book" > $@
	cat $(MDOC) >> $@
#	cat README.md doc/*.md $(C) $(H) $(P) $(R) $(S) > $@ ; touch $@
