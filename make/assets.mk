ASSETS_SRC_PREFIX=assets/
ASSETS_BUILD_PREFIX=build/ui/assets/
ASSETS_SRC=icons8-annuler-16.png \
			icons8-ok-16.png \
			icons8-mode-veille-16.png \
			icons8-grand-hashtag-40.png \
			icons8-haute-priorité-100.png \
			icons8-info-carré-16.png \
			icons8-dormir-16.png \
			icons8-importer-24.png
ASSETS=$(patsubst %, $(ASSETS_BUILD_PREFIX)%, $(ASSETS_SRC))

$(ASSETS_BUILD_PREFIX)%: $(ASSETS_SRC_PREFIX)%
	@mkdir -p $(ASSETS_BUILD_PREFIX)
	@cp -f $< $@
