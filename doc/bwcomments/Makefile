.SUFFIXES: .tex .dvi .html .ps .pdf .bbl .eps .pnm .png .epssrc

LATEX=latex
BIBTEX=bibtex
PDFLATEX=pdflatex
DVIPS=dvips
PS2PDF=ps2pdf14

DVIPSFLAGS=-f -Ppdf -G0
PS2PDFFLAGS=-dCompatibilityLevel=1.3 -sColorImageFilter=FlateEncode -sGrayImageFilter=FlateEncode -dEmbedAllFonts=true
PS2PDFFLAGS_PRINT=$(PS2PDFFLAGS) -r600 -dAutoFilterColorImages=false -dAutoFilterGrayImages=false -dPDFSETTINGS=/prepress
PS2PDFFLAGS_WEB=$(PS2PDFFLAGS) -r300 -dAutoFilterColorImages=true -dAutoFilterGrayImages=true


# Optional: -html_version 4.0,math
# ... will use textual representations where possible


# Note: you'll still need to hand-tweak the screenshots to get good results.
# They're non-antialiased in the stupid EPS files that get used.
HTMLFLAGS= \
        -show_section_numbers \
	-info 0 \
	-address 0 \
	-no_math -html_version 4.0 \
	-scalable_fonts \
	-antialias -antialias_text \
	-white

SRCTEX= \
    report.tex

IMGDVI=

IMGPDF=

report.dvi: $(SRCTEX) $(IMGDVI) report.bib
report.pdf: $(SRCTEX) $(IMGPDF) report.bib

default: report_letter.ps
all: report_print.pdf report_web_a4.pdf report_web_letter.pdf

.eps.pdf:
	epstopdf -o=$*.pdf $*.eps

.tex.dvi:
	$(LATEX) $*
	$(BIBTEX) $*
	$(LATEX) $*
	$(LATEX) $*

report_a4.ps: report.dvi
	$(DVIPS) report $(DVIPSFLAGS) -t a4 -o $*.ps

report_letter.ps: report.dvi
	$(DVIPS) report $(DVIPSFLAGS) -t letter -o $*.ps

report_print.pdf: report_a4.ps
	$(PS2PDF) $(PS2PDFFLAGS_PRINT) -sPAPERSIZE=a4 report_a4.ps $*.pdf
report_print_letter.pdf: report_letter.ps
	$(PS2PDF) $(PS2PDFFLAGS_PRINT) report_letter.ps $*.pdf
report_web_a4.pdf: report_a4.ps
	$(PS2PDF) $(PS2PDFFLAGS_WEB) -sPAPERSIZE=a4 report_a4.ps $*.pdf
report_web_letter.pdf: report_letter.ps
	$(PS2PDF) $(PS2PDFFLAGS_WEB) report_letter.ps $*.pdf

report-chaps.html: $(SRCTEX) $(IMGPDF) report.bib
	mkdir $*
	latex2html $(HTMLFLAGS) -split 4 -dir $* report
	touch $*.html
	cp *.gif $*
	rm -f $*/$*.html
	chmod a+rx $*/*.html $*/*.gif $*/*.css
report-single.html: $(SRCTEX) $(IMGPDF) report.bib
	mkdir $*
	latex2html $(HTMLFLAGS) -split 0 -dir $* report
	touch $*.html
	cp *.gif $*
	rm -f $*/$*.html
	chmod a+rx $*/*.html $*/*.gif $*/*.css

.tex.html:
# pstoimg colour depth: 24-bit
	latex2html $(HTMLFLAGS) $*
	touch $*.html
	cp *.gif $*
	rm -f $*/$*.html
	chmod a+rx $*/*.html $*/*.gif $*/*.css

.tex.pdf:
	$(PDFLATEX) $*
	$(BIBTEX) $*
	$(PDFLATEX) $*
	$(PDFLATEX) $*

.epssrc.eps:
	cp $*.epssrc $*.eps

.pnm.eps:
	convert $*.pnm $*.eps

.pnm.png:
	convert $*.pnm $*.png

clean:
	rm -f *.html *.dvi *.pdf *.ps *.aux *.log *.lbl *.eps *.png *.bbl *.blg *.brf
