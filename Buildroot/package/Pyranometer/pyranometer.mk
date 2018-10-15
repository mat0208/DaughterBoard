PYRANOMETER_VERSION = 1.0.0
PYRANOMETER_SITE = $(TOPDIR)/../Sources/Pyranometer
PYRANOMETER_SITE_METHOD = local

define PYRANOMETER_BUILD_CMDS
	$(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(@D)
endef

define PYRANOMETER_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/main_Pyranometer $(TARGET_DIR)/usr/bin/main_Pyranometer
	
endef

$(eval $(generic-package))
