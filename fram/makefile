SUBDIRS=$(PLATHOME)/module/public/src \
		$(PLATHOME)/module/log/src \
		$(PLATHOME)/module/ini/src	\
		$(PLATHOME)/module/json/src    \
		$(PLATHOME)/module/ipc/src    \
		$(PLATHOME)/module/mempool/src    \
		$(PLATHOME)/module/threadpool/src    \
		$(PLATHOME)/module/pm/src    \
		$(PLATHOME)/module/ds/src    \
		$(PLATHOME)/module/ns/src    \
		$(PLATHOME)/module/heartbeat/src    \
		$(PLATHOME)/module/test/src 

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
