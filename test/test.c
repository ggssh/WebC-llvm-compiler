str hi() {
    str a = '<h1>hello from kingtous compiler</h1>';
    ret a;
}

str currentTime(){
    long current_time = now();
    ret toStr(current_time);
}

int main() {
    str resp = currentTime();
    echo(resp);
//    echo('init...');
//    echo(hi());
//    str host = '127.0.0.1';
//    int port = 9000;
//    int core = 2;
//    echo('get server...');
//    int server_id = getServer(host, port, core);
//    echo('server id is', server_id);
//    addUrlHandler(server_id, 'GET', '/compiler','text/html', hi);
//    echo('start server in host', host, ',port is', port);
//    startServer(server_id);
    ret 0;
}
