#!/usr/bin/lua

require "cjson"

message = "";

function main(buff)
    success();
end

function success()
    local html = "<html><h1>It works!</h1></html>";
    message = string.format("HTTP/1.1 200 0K \r\nServer: my server\r\nContent-Type: text/html\r\nContent-Length:%d\r\n\r\n%s\r\n", string.len(html), html);

end

function fail()
    local html = "<html><h1>It works!</h1></html>";
    message = string.format("HTTP/1.1 200 0K \r\nServer: my server\r\nContent-Type: text/html\r\nContent-Length:%d\r\n\r\n%s\r\n", string.len(html), html);

end
