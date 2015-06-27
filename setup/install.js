
// exec appcmd
var shell = new ActiveXObject("Shell.Application");
var wshell = new ActiveXObject("WScript.Shell");

var appcmd = wshell.ExpandEnvironmentStrings("%windir%\\system32\\inetsrv\\appcmd.exe");

shell.ShellExecute(appcmd, "install module /name:iislua /image:\"%programfiles%\\iislua\\iislua.dll\" /commit:apphost");

// modify apphost.config
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