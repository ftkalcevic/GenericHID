include ../build.pri

generichid:	make_exe.sh
	sh make_exe.sh $(INSTALLDIR_BIN) $(INSTALLDIR_LIB) > generichid
	chmod a+x generichid

.PHONY: install
install:	generichid 99-generichid.rules
	install -m 755 -p generichid $(INSTALLDIR_BIN)/
	install -m 644 -p 99-generichid.rules $(INSTALLDIR_UDEVRULES)

.PHONY: uninstall
uninstall:
	rm -f $(INSTALLDIR_BIN)/generichid
	rm -f $(INSTALLDIR_UDEVRULES)/99-generichid.rules

.PHONY: clean
clean:
	rm -f generichid

scripts.pro:

