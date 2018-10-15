WINDSENSOR_VERSION = 1.0.0
WINDSENSOR_SITE = $(TOPDIR)/../Sources/WindSensor
WINDSENSOR_SITE_METHOD = local

define WINDSENSOR_BUILD_CMDS
	$(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(@D)
endef

define WINDSENSOR_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/main_WindSensor $(TARGET_DIR)/usr/bin/main_WindSensor
	
endef

$(eval $(generic-package))
