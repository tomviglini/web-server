//#include <thenext.h>
#include <server.h>
#include <string.h>


int listSockets(int argc, char *argv[]) {
    
    struct SERVER *server = _server(argv);
    struct SYSTEM *system = _system(argv);
    
    int i;
    int n;
    
    _print(system, "{\"hostname\":[");
    
    for(n = 0; n < server->listener.size; n++) {
        if(server->listener.record[n].active == 1) {
            for(i = 0; i < server->listener.record[n].host.size; i++) {
                if(i + 1 < server->listener.record[n].host.size) {
                    _printf(system, "\"%s\",", server->listener.record[n].host.record[i]->key);
                } else {
                    _printf(system, "\"%s\"", server->listener.record[n].host.record[i]->key);
                }
            }
        }
    }
    
    _print(system, "]}");
    
    return HTTP_200;
}




int action1(int argc, char *argv[]) {
    _print(_system(argv), "action1");
    return HTTP_200;
}

int action2(int argc, char *argv[]) {
    _print(_system(argv), "action2");
    return HTTP_200;
}

int action3(int argc, char *argv[]) {
    _print(_system(argv), "action3");
    return HTTP_200;
}
