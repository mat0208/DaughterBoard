HANDLER_VERSION = 1.0.0
HANDLER_SITE = $(TOPDIR)/../Sources/Handler
HANDLER_SITE_METHOD = local

define HANDLER_BUILD_CMDS
	$(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(@D)
endef

define HANDLER_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/main_Handler $(TARGET_DIR)/usr/bin/main_Handler
	
endef

$(eval $(generic-package))
