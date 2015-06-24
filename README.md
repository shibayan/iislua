lua-iis-module
================

It brings the power of Lua scripting in your IIS.

## Configuration

### Sample Web.config

```
<?xml version="1.0" encoding="UTF-8"?>
<configuration>
  <system.webServer>
    <iislua beginRequest="C:\inetpub\beginrequest.lua" mapPath="C:\inetpub\mappath.lua" />
  </system.webServer>
</configuration>
```

## Usage

### Return status code

```
iis.exit(404)
```

### Redirect

```
iis.redirect("http://buchizo.wordpress.com/")
```

### Refuse http method

```
if iis.req.http_method() ~= "POST" then
    iis.exit(405)
end
```

### Cross domain access control

```
req_headers = iis.req.get_headers()

if req_headers["Origin"] ~= nil then
    iis.resp.set_header("Access-Control-Allow-Origin", req_headers["Origin"])
end
```

### Rewrite to other url

```
if iis.req.get_url() == "/" then
   iis.exec("/iisstart.htm")
end
```