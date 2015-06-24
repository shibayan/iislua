lua-iis-module
================

It brings the power of Lua scripting in your IIS.

## Install

install iislua module.

```
appcmd install module /name:iislua /image:C:\..\iislua.dll /add:true /lock:true
```

modify applicationHost.config

```xml
<sectionGroup name="system.webServer">
  <section name="iislua" overrideModeDefault="Allow" />
</sectionGroup>
```

## Configuration

### Web.config

```xml
<?xml version="1.0" encoding="utf-8"?>
<configuration>
  <system.webServer>
    <iislua>
      <beginRequest scriptPath="C:\inetpub\beginrequest.lua" />
      <mapPath scriptPath="C:\inetpub\mappath.lua" />
    </iislua>
  </system.webServer>
</configuration>
```

## Usage

### Return status code

```lua
iis.exit(404)
```

### Redirect

```lua
iis.redirect("http://buchizo.wordpress.com/")
```

### Refuse http method

```lua
if iis.req.http_method() ~= "POST" then
    iis.exit(405)
end
```

### Cross domain access control

```lua
req_headers = iis.req.get_headers()

if req_headers["Origin"] ~= nil then
    iis.resp.set_header("Access-Control-Allow-Origin", req_headers["Origin"])
end
```

### Rewrite to other url

```lua
if iis.req.get_url() == "/" then
   iis.exec("/iisstart.htm")
end
```