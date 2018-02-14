# HTTP Proxy Server

## Usage
```sh
python2.7 server.py
python2.7 proxy.py
curl -x http://127.0.0.1:21000 http://127.0.0.1:20000/1.txt
```

## Features
* It is a simple HTTP web proxy server which is able to cache web pages. When the client makes a request, it is sent to the proxy server. The proxy server then forwards the request to the web server and the web server processes the request and sends back a response message to the proxy which then sends it to the client.
* The proxy server can cache the requests, that is, it checks if the requested object is cached (i.e. server already has the request webpage or file), and if yes, it returns the object from the cache, after verifying that the object hasn't been modified since then.
* It is a non blocking server, so, multiple clients can send requests at once.