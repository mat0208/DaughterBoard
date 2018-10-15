LIDAR_VERSION = 1.0.0
LIDAR_SITE = $(TOPDIR)/../Sources/Lidar
LIDAR_SITE_METHOD = local

define LIDAR_BUILD_CMDS
	$(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(@D)
endef

define LIDAR_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/main_Lidar $(TARGET_DIR)/usr/bin/main_Lidar
	
endef

$(eval $(generic-package))
