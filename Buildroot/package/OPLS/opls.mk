OPLS_VERSION = 1.0.0
OPLS_SITE = $(TOPDIR)/../Sources/OPLS
OPLS_SITE_METHOD = local

define OPLS_BUILD_CMDS
	$(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(@D)
endef

define OPLS_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/main_OPLS $(TARGET_DIR)/usr/bin/main_OPLS
	
endef

$(eval $(generic-package))
