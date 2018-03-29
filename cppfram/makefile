SUBDIRS=$(PLATTOP)/public/src

.PHONY: all
all:
	@list='$(SUBDIRS)'; for subdir in $$list; do \
	echo "Clean in $$subdir";\
	cd $$subdir && $(MAKE);\
	done

.PHONY: clean
clean:
	@list='$(SUBDIRS)'; for subdir in $$list; do \
	echo "Clean in $$subdir";\
	cd $$subdir && $(MAKE) clean;\
	done
