include $(TOPDIR)/rules.mk

PKG_NAME:=OpenWrtSniffer
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)


include $(INCLUDE_DIR)/package.mk

define Package/OpenWrtSniffer
	SECTION:=utils
	CATEGORY:=Utilitities
	DEPENDS+=libpcap
	TITLE:=OpenWrtSniffer -- A Sniffer of OpenWrt
endef

define Package/OpenWrtSniffer/description
	If you can't figure out what this program does,you'are probably brain-dead and need immediate medical attention

endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/OpenWrtSniffer/install
	$(INSTALL_DIR) $(1)/webroot/cgi-bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/OpenWrtSniffer $(1)/webroot/cgi-bin/OpenWrtSniffer
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/snifferd $(1)/webroot/cgi-bin/snifferd
endef

$(eval $(call BuildPackage,OpenWrtSniffer,+libpcap))
