SUBDIRS = externals src test
ACLOCAL_AMFLAGS = -I m4/generated

package: check
	test -d dist || mkdir dist
	(cd $(top_srcdir); git archive --format=tar HEAD) | tar -C dist -xf -
	cd dist; ./autogen.sh -v
	zip -r 440706.zip dist/
	rm -rf dist
