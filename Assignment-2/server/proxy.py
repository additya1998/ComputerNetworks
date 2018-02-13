import os
import re
import socket
import sys
import time
import threading

port = 21000
host = ''
cache_index = 0
cached_files = {}

def requestWebsite(url_path, req_data, url_hex):
	port_no = ''
	host = ''
	file = ''

	colon_count = 0
	slash_count = 0
	for c in url_path:
		if c == ':':
			colon_count += 1
		elif c == '/' and slash_count < 3:
			slash_count += 1
		elif colon_count == 1 and slash_count == 2:
			host += c
		elif colon_count == 2 and slash_count == 2:
			port_no += c
		elif slash_count == 3:
			file += c

	if port_no == '':
		port_no = '80'
	port_no = int(port_no)

	req_data = req_data.split(' ')
	req_data[1] = '/' + file
	req_data = ' '.join(req_data)

	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.settimeout(5000)
	s.connect((host, port_no))
	s.sendall(req_data)

	fd = open(url_hex, "w")

	while True:
		data = s.recv(262144)
		if data != '':
			fd.write(data)
		else:
			break

	fd.close()

def serve_request(conn, addr):
	req_data = conn.recv(262144)
	req_lines = req_data.splitlines()

	if len(req_lines) == 0:
		print "Empty Request"
	else:
		req_method = req_lines[0].split(' ')[0]

		if req_method == "GET" and len(req_lines[0].split(' ')) == 3:
			url_path = req_lines[0].split(' ')[1]
			url_hex = url_path.encode("hex")

			requestWebsite(url_path, req_data, url_hex)

			fd = open(url_hex, "r")

			while True:
				data = fd.read(262144)
				if data != '':
					conn.send(data)
				else:
					break

			fd.close()


		else:
			print "Invalid Request"

	conn.close()		

if __name__ == "__main__":

	sock = socket.socket()
	sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	sock.bind((host, port))
	sock.listen(5)

	while (1):
		conn, addr = sock.accept()
		thread = threading.Thread(target=serve_request, args=(conn, addr))
		thread.start()