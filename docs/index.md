iislua
================

[![Build status](https://ci.appveyor.com/api/projects/status/76ulkhlfpjkqav74?svg=true)](https://ci.appveyor.com/project/shibayan/iislua)
[![Release](https://img.shields.io/github/release/shibayan/iislua.svg)](https://github.com/shibayan/iislua/releases/latest)
[![Downloads](https://img.shields.io/github/downloads/shibayan/iislua/total.svg)](https://github.com/shibayan/iislua/releases/latest)
[![License](https://img.shields.io/github/license/shibayan/iislua.svg)](https://github.com/shibayan/iislua/blob/master/LICENSE)

It brings the power of Lua scripting in your IIS.

## Install

Download MSI file from following page. (x64 version)

https://github.com/shibayan/iislua/releases

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