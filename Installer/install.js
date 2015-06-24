
var adminManager = new ActiveXObject("Microsoft.ApplicationHost.WritableAdminManager");
var configManager = adminManager.ConfigManager;

var appHostConfig = configManager.GetConfigFile("MACHINE/WEBROOT/APPHOST");
var systemWebServer = appHostConfig.RootSectionGroup.Item("system.webServer");

try {
    systemWebServer.Sections.DeleteSection("iislua");
}
catch (e) {
}

systemWebServer.Sections.AddSection("iislua");

adminManager.CommitChanges();