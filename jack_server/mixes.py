from pythonosc import udp_client
import jack, os, time


class JackHelp:
    CLIENT = None

    def init():
        JackHelp.CLIENT = jack.Client("server")
        JackHelp.CLIENT.activate()

    def connect_port(port1, port2):
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


class Mixes:
    # people = { <name>: [<headphone port left>,<headphone port right>],.. }
    # inputs = { <name>: {
    #     "port":        <audio_port> OR [<audio_port>,<audio_port>],
    #     "pan":         <L,R,C>,
    #     "record":      <True,False>,
    #     "global_mute": <True,False(default)>,
    # },.. }
    # osc_start_port   = to interact with jackminimix instances, they each need their own port
    # osc_receive_port = to send solo and mute data out
    def __init__(self, people, inputs, osc_start_port=4000, osc_ip='127.0.0.1', osc_receive_port=3002):
        self.mixes      = {}
        self.solos      = {}
        self.mutes      = {}
        self.inputs     = inputs
        self.people     = people
        self.start_port = osc_start_port
        self.ip         = osc_ip
        self.out_port   = osc_receive_port

        # set position values for inputs, this is needed by jackminimix
        for i, name in enumerate(inputs, 1):
            self.inputs[name]["pos"] = i

        for person in people:
            # for solo to work, person and input must have matching names
            self.solos[person] = False
            result = { "outports": people[person] }

            for i, name in enumerate(self.inputs):
                inp = self.inputs[name]

                # global mute is for talkback but could be for anything
                #  if global mute, add to self.mutes
                if inp.get("global_mute",False):
                    self.mutes[name] = False  # repeatedly set is not an issue
                    mute             = "global"
                else:
                    mute = False

                # if you want a mixer for a scratch recording mix, add it to list of people
                if person != "record" or inp["record"]:
                    result[name] = {
                        "vol":  0,
                        "mute": mute,
                    }

            self.mixes[person] = result

    def create_mixes(self):
        port = self.start_port
        for i, name in enumerate(self.mixes):
            mix           = self.mixes[name]
            num_inputs    = f"-c {len(mix)}"
            mix_name      = f"-n {name}_mix"
            mix_port      = f"-p {port}"
            mix_left_out  = f"-l {mix['outports'][0]}"
            mix_right_out = f"-r {mix['outports'][1]}"

            # start up instance of jackminimix
            self.mixes[name]["osc"] = udp_client.SimpleUDPClient(self.ip, port)
            os.system(f"jackminimix {num_inputs} {mix_name} {mix_port} {mix_left_out} {mix_right_out}")

            time.sleep(1) # give jackminimix a second to load

            # make all the jack connections
            for inp in mix:
                pos    = self.inputs[inp]["pos"]
                pan    = self.inputs[inp]["pan"]
                inport = self.inputs[inp]["port"]
                if type(inport) is list:
                    for p in inport:
                        if pan in ["L","C"]: JackHelp.connect_port(p,f"{name}_mix:in{pos}_left")
                        if pan in ["R","C"]: JackHelp.connect_port(p,f"{name}_mix:in{pos}_right")
                else:
                    if pan in ["L","C"]: JackHelp.connect_port(inport,f"{name}_mix:in{pos}_left")
                    if pan in ["R","C"]: JackHelp.connect_port(inport,f"{name}_mix:in{pos}_right")

            port += 1

    def process_osc(self, path, value):
        path_sp = path[1:].split('/')

        # This only processes osc that are prefaced by 'mixer'
        if path_sp[0] != "mixer": return None

        # remove 'mixer' from path
        path_sp.pop(0)

        kind, name, inp = path_sp
        pos = self.inputs[inp]["pos"]

        if kind == "vol":
            # value from rails comes in 0.0->1.0 so
            #  this gives +10/-30 db for the sliders
            value = int((value * 40) - 30)

            # we want an accurate value from slider irregardless of mute/solo
            self.mixes[name][inp][kind] = value

            # solo mutes your instrument for everyone but yourself
            if name != inp and self.solos.get(inp,False):
                value = -90 # muted

            # check if mute both local and global
            mute = self.mixes[name][inp]["mute"]
            if mute != "global" and mute:
                value = -90 # muted
            elif mute == "global" and self.mutes.get(inp,False):
                value = -90 # muted

            self.mixes[name]["osc"].send_message("/mixer/channel/set_gain", [pos, value])

        elif kind == "mute":
            if value >= .5: value = True
            else:           value = False
            self.mixes[name][inp][kind] = value

            if value: value = self.mixes[name][inp]["vol"]
            else:     value = -90

            self.mixes[name]["osc"].send_message("/mixer/channel/set_gain", [pos, value])

        elif kind == "solo":
            pass
        else:
            print("####> Error processing osc in mixer")

