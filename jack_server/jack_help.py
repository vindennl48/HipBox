import jack, time


class JackHelp:
    CLIENT = None

    def init():
        JackHelp.CLIENT = jack.Client("server")
        JackHelp.CLIENT.activate()

    def connect_port(port1, port2):
        if JackHelp.CLIENT is None:
            JackHelp.init()

        success = False
        i = 0
        while not success and i < 20:
            try:
                JackHelp.CLIENT.connect(port1,port2)
                success = True
            except:
                print(f"####> error trying to connect ports attempt {i}-20: {port1} -> {port2}")
                i += 1
                time.sleep(1)
        if i >= 20:
            raise Exception(f">>>>> Can not connect ports in 20 attempts: {port1} -> {port2}")
        else:
            print(f"----> ports successfully connected: {port1} -> {port2}")
