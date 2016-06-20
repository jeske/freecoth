include $(top_srcdir)/Makefile.am.include

SUBDIRS = freecloth
EXTRA_DIST = Makefile.am.include

.PHONY: doc docPublic docInternal

doc: docPublic docInternal

docPublic:
	doxygen docPublicConfig
docInternal:
	doxygen docInternalConfig
