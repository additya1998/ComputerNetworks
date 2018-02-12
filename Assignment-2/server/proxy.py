import os
import re
import socket
import sys
import threading
import time
import threading
import requests

port = 21000
host = ""
threads = []
cache_index = 0
cached_files = []

def serve_request(conn, addr):
	req_data = conn.recv(1024)
	req_method = req_data.split('\r\n')[0].split(' ')[0]
	if req_method != "POST":
		file_path = req_data.split('\n\r')[0].split(' ')[1]
		host = file_path.split(':')[0] + ':' + file_path.split(':')[1]
		port = file_path.split(':')[2].split('/')[0]
		file = file_path.find(host + ':' + port)
		file = file_path[file + len(host + ':' + port):].lstrip('/')

		fd = open("t_file", "w")
		file_req = requests.get(file_path)
		file_content = file_req.content 
		fd.write(file_content)
		print file_req.headers['Cache-control']

		conn.send(file_content)


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
		threads.append(thread)
		thread.start()

	for thread in threads:
		thread.join()