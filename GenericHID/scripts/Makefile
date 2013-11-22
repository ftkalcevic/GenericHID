include ../build.pri

generichid:	make_exe.sh
	sh make_exe.sh $(INSTALLDIR_BIN) $(INSTALLDIR_LIB) > generichid
	chmod a+x generichid

.PHONY: install
install:	generichid
	install -m 755 -p generichid $(INSTALLDIR_BIN)/

.PHONY: uninstall
uninstall:
	rm -f $(INSTALLDIR_BIN)/generichid

.PHONY: clean
clean:
	rm -f generichid

scripts.pro:

