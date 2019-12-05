PO_SRC_PREFIX=po/
PO_BUILD_PREFIX=build/locale/
PO_SRC=ar.po \
	fr.po \
	he.po

MO=$(patsubst %.po, $(PO_BUILD_PREFIX)/%/LC_MESSAGES/irc.mo, $(PO_SRC))

$(PO_BUILD_PREFIX)%/LC_MESSAGES/irc.mo: $(PO_SRC_PREFIX)%.po
	@mkdir -p $(dir  $@)
	@msgfmt $< -o $@
