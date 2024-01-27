from http.server import BaseHTTPRequestHandler, HTTPServer
import json


class SimpleHTTPRequestHandler(BaseHTTPRequestHandler):
    def do_POST(self):
        # 读取请求体内容
        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length).decode('utf-8')
        print(f"Content: {post_data}")

        # 发送响应
        self.send_response(200)
        self.send_header('Content-type', 'application/json')
        self.end_headers()
        self.wfile.write(json.dumps({"code": 0}).encode('utf-8'))

    def log_message(self, format, *args):
        # 打印接收到的请求的信息
        print("%s - - [%s] %s\n" %
              (self.address_string(),
               self.log_date_time_string(),
               format % args))

    def do_GET(self):
        # 处理非 POST 请求
        print("Not a POST request")
        self.send_response(200)
        self.send_header('Content-type', 'application/json')
        self.end_headers()
        self.wfile.write(json.dumps({"code": 0}).encode('utf-8'))


def run(server_class=HTTPServer, handler_class=SimpleHTTPRequestHandler):
    server_address = ('0.0.0.0', 5678)
    httpd = server_class(server_address, handler_class)
    print("Server started on 0.0.0.0:5678")
    httpd.serve_forever()


if __name__ == '__main__':
    run()
