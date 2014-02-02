TEXI_SOURCE=boueux.texi
IMG_DIR=img

PDF_BUILD_DIR=boueux.t2d
HTML_DIR=boueux

all: html pdf

pdf: $(TEXI_SOURCE)
	texi2dvi --build=tidy --build-dir=$(PDF_BUILD_DIR) --pdf $(TEXI_SOURCE)
	
html: $(TEXI_SOURCE)
	makeinfo --output=$(HTML_DIR) --html $(TEXI_SOURCE)
	cp -r $(IMG_DIR) $(HTML_DIR)

clean:
	rm -rf $(PDF_BUILD_DIR)
