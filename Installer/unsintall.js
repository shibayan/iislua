
try {
    var adminManager = new ActiveXObject("Microsoft.ApplicationHost.WritableAdminManager");
    var configManager = adminManager.ConfigManager;

    var appHostConfig = configManager.GetConfigFile("MACHINE/WEBROOT/APPHOST");
    var systemWebServer = appHostConfig.RootSectionGroup.Item("system.webServer");

    systemWebServer.Sections.DeleteSection("iislua");

    adminManager.CommitChanges();
}
catch (e) {
}