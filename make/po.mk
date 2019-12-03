PO_SRC_PREFIX=po/
PO_BUILD_PREFIX=build/locale/
PO_SRC=ar.po \
	fr.po \
	he.po
PO=$(patsubst %.po, %.mo, $(PO_SRC))

%.mo:$(PO_SRC_PREFIX)%.po
	@mkdir -p $(PO_BUILD_PREFIX)$(patsubst $(PO_SRC_PREFIX)%,%,$(basename  $<))/LC_MESSAGES
	msgfmt $< -o $(PO_BUILD_PREFIX)$(patsubst $(PO_SRC_PREFIX)%,%,$(basename  $<))/LC_MESSAGES/irc.mo

