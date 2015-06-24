
// exec appcmd
var shell = new ActiveXObject("Shell.Application");
var wshell = new ActiveXObject("WScript.Shell");

var appcmd = wshell.ExpandEnvironmentStrings("%windir%\\system32\\inetsrv\\appcmd.exe");

shell.ShellExecute(appcmd, "uninstall module iislua /commit:apphost");

// modify apphost.config
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