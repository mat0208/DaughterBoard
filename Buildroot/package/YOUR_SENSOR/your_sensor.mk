YOUR_SENSOR_VERSION = 1.0.0
YOUR_SENSOR_SITE = $(TOPDIR)/../Sources/YOUR_SENSOR
YOUR_SENSOR_SITE_METHOD = local

define YOUR_SENSOR_BUILD_CMDS
	$(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(@D)
endef

define YOUR_SENSOR_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/main_YOUR_SENSOR $(TARGET_DIR)/usr/bin/main_YOUR_SENSOR
	
endef

$(eval $(generic-package))
