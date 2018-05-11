SUBDIRS=$(PLATTOP)/module/log/src \
	$(PLATTOP)/module/public/src \
    $(PLATTOP)/module/com/src

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
