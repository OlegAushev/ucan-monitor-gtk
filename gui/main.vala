///
///
///


extern string GIT_DESCRIBE;

// core backend
namespace Backend {

extern int main_enter();
extern void main_exit();
extern bool is_ready;

namespace Ucanopen {
extern string server_list[4];
extern string server;
extern string server_config_category;
}

}


extern bool log_get_message(string retbuf, int bufsize);


int main(string[] args) {
    var app = new CanMonitor.Application();
    return app.run(args);
}

