from pythonosc import osc_server
import re, threading

class OSC:
    osc = {}

    def run(grep="/*", port=3001, ip='0.0.0.0'):
        from pythonosc import dispatcher

        disp = dispatcher.Dispatcher()
        disp.map(grep, OSC._callback)
        server = osc_server.ThreadingOSCUDPServer((ip, port), disp)
        threading.Thread(target=server.serve_forever).start()

    def get(name):
        if name in OSC.osc:
            return OSC.osc[name]
        else:
            return 0

    def set(name, value):
        OSC.osc[name] = value

    def set_grep(grep, value):
        for key in OSC.osc:
            if bool(re.search(grep,key)):
                OSC.set(key,value)

    def _callback(path, value):
        path = path[1:]
        OSC.set(path, value)
        if "vol" in path:
            OSC.set(path, int((value * 40) - 30))
        else:
            if value >= .5: OSC.set(path, 1)
            else:           OSC.set(path, 0)
        # print(f"path: {path} | value: {value}")
