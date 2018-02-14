import os
import socket
import hashlib
import time
import threading
import sys
from collections import OrderedDict

port = 21000
host = ''
cached_files = OrderedDict()

def requestWebsite(url_path, req_data, url_hex):
	port_no = ''
	host = ''
	file = ''

	print [url_path, url_hex]

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
	sent_if_modified = False

	if url_path in cached_files and req_data.lower().find('If-Modified-Since:') == -1:
		req_data = req_data.split(' ')
		temp_data = req_data[2].split('\r\n', 1)
		req_data[2] = temp_data[0] + '\nIf-Modified-Since: ' + cached_files[url_path] + '\n' + temp_data[1]
		req_data = ' '.join(req_data)
		sent_if_modified = True

	print(req_data)

	print('\n\nEnd of Request\n\n')

	try:
		s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		s.settimeout(50)
		s.connect((host, port_no))
		s.sendall(req_data)
	except:
		sys.exit(0)

	data = s.recv(262144)
	lines = data.splitlines()

	print(data)

	print('\n\nEnd of Response\n\n')

	if sent_if_modified and lines[0].split(' ')[1] == '304':
		return

	try:
		fd = open('temp' + str(threading.current_thread().ident), "w")
		fd.write(data)
	except:
		print "Error writing to file"
		sys.exit(0)

	while True:
		try:
			data = s.recv(262144)
			if data != '':
				fd.write(data)
			else:
				break
		except:
			print "Error"
			sys.exit(0)

	fd.close()

	try:
		fd = open('temp' + str(threading.current_thread().ident), "r")
	except:
		print "Error processing file"
		sys.exit(0)

	for line in fd:
		if line == '':
			break
		if line.lower().find("cache-control:") == 0 and line.lower().find('no-cache') == -1:
			if url_path in cached_files:
				del cached_files[url_path]
			cached_files[url_path] = time.strftime("%a, %d %b %Y %H:%M:%S GMT", time.gmtime())
			if len(cached_files) > 3:
				cached_files.popitem(last=False)

	fd.close()
	os.rename('temp' + str(threading.current_thread().ident), url_hex)

def serve_request(conn, addr):
	try:
		req_data = conn.recv(262144)
	except:
		print "Problem receiving request data"
		sys.exit(0)

	req_lines = req_data.splitlines()

	if len(req_lines) == 0:
		try:
			conn.send('HTTP/1.1 400 Bad Request\r\n\r\n')
		except:
			print "Error"
			sys.exit(0)

	else:
		req_method = req_lines[0].split(' ')[0]

		if req_method == "GET" and len(req_lines[0].split(' ')) == 3:
			url_path = req_lines[0].split(' ')[1]
			url_hex = 'file' + hashlib.md5(url_path).hexdigest()

			requestWebsite(url_path, req_data, url_hex)

			try:
				fd = open(url_hex, "r")
			except IOError:
				conn.send('HTTP/1.1 500 Internal Server Error\r\n\r\n')
				sys.exit(0)

			while True:
				try:
					data = fd.read(262144)
					if data != '':
						conn.send(data)
					else:
						break
				except:
					print "Error processing request"
					sys.exit(0)

			fd.close()

		else:
			try:
				conn.send('HTTP/1.1 400 Bad Request\r\n\r\n')
			except:
				print "Error"
				sys.exit(0)

	conn.close()		

if __name__ == "__main__":

	try:
		sock = socket.socket()
		sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
		sock.bind((host, port))
		sock.listen(5)
	except:
		print "Cannot initialise socket"
		sys.exit(0)

	while (1):
		try:
			conn, addr = sock.accept()
			thread = threading.Thread(target=serve_request, args=(conn, addr))
			thread.start()
		except:
			print "Could not accept request"