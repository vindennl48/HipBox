from pythonosc import udp_client
from jack_help import JackHelp
import os, time


class Mixes:
    # people = { <name>: [<headphone port left>,<headphone port right>] OR [],.. }
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

        # self.mixes = {
        #     "mitch": {
        #         "james": {
        #             "vol":  0,
        #             "mute": false,
        #         },
        #         "jesse": {
        #             "vol":  0,
        #             "mute": false,
        #         },
        #     }
        # } ...
        for person in people:
            # for solo to work, person and input must have matching names
            self.solos[person] = False
            result             = {}

            # only create outports if they are included in args
            if people[person]:
                result["outports"] = people[person]
                result["hp"]       = 0

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
                # if (person == "record" and inp["record"]) or person != "record":
                result[name] = {
                    "vol":  0,
                    "mute": mute,
                }

            self.mixes[person] = result

        self.create_mixes()


    def create_mixes(self):
        port = self.start_port
        for i, name in enumerate(self.mixes):
            mix           = self.mixes[name]
            num_inputs    = f"-c {len(self.inputs)}"
            mix_name      = f"-n {name}_mix"
            mix_port      = f"-p {port}"
            mix_left_out  = f"-l {mix['outports'][0]}" if "outports" in mix else ""
            mix_right_out = f"-r {mix['outports'][1]}" if "outports" in mix else ""

            # start up instance of jackminimix
            client = udp_client.SimpleUDPClient(self.ip, port)
            self.mixes[name]["osc"] = client
            os.system(f"jackminimix {num_inputs} {mix_name} {mix_port} {mix_left_out} {mix_right_out} &")

            time.sleep(1) # give jackminimix a second to load

            # make all the jack connections
            for inp in self.inputs:
                pos    = self.inputs[inp]["pos"]
                pan    = self.inputs[inp]["pan"]
                inport = self.inputs[inp]["port"]

                if name != "record" or (name == "record" and self.inputs[inp]["record"]):
                    if name == "record":
                        client.send_message(f"/mixer/channel/set_gain", [pos, 0])
                    if type(inport) is list:
                        for p in inport:
                            if pan in ["L","C"]: JackHelp.connect_port(p,f"{name}_mix:in{pos}_left")
                            if pan in ["R","C"]: JackHelp.connect_port(p,f"{name}_mix:in{pos}_right")
                    else:
                        if pan in ["L","C"]: JackHelp.connect_port(inport,f"{name}_mix:in{pos}_left")
                        if pan in ["R","C"]: JackHelp.connect_port(inport,f"{name}_mix:in{pos}_right")

            port += 1


    # /vol/mitch/james
    # /mute/mitch/james/toggle
    def process_osc(self, path, value):
        path_sp = path[1:].split('/')

        # This only processes osc that are prefaced by 'mixer'
        if path_sp[0] != "mixer": return 0

        # remove 'mixer' from path
        path_sp.pop(0)

        # handle different length paths
        if len(path_sp) == 3:
            kind, name, inp = path_sp
            toggle = False
        elif len(path_sp) == 4:
            kind, name, inp, toggle = path_sp

        # /vol/mitch/hp
        if kind == "vol":
            if inp == "hp":
                self.mixes[name]["hp"] = value
            else:
                self.mixes[name][inp][kind] = value

            self.set_gain(name, inp)

        elif kind == "mute":
            mute = self.mixes[name][inp][kind]

            if toggle:
                if mute == "global":
                    value = False if self.mutes[inp] else True
                else:
                    value = False if self.mixes[name][inp][kind] else True
            else:
                if value >= .5: value = True
                else:           value = False

            if mute == "global":
                self.mutes[inp] = value
            else:
                self.mixes[name][inp][kind] = value

            self.set_gain(name, inp)

        elif kind == "solo":
            # Solo only works if the instrument matches the person
            if name != inp: return 1

            if toggle:
                value = False if self.solos[name] else True
            else:
                if value >= .5: value = True
                else:           value = False

            self.solos[name] = value

            for mix_name in self.mixes:
                self.set_gain(mix_name, inp)

        elif kind == "record":
            print("----> Ignoring kind:'record' in mixes:process_osc()")

        else:
            print("####> Error processing osc in mixer")

        return 1


    def set_gain(self, mix_name, input_name):
        # dont process anything for the record mix
        if mix_name == "record": return None

        if input_name == "hp":
            value = self.midi_to_db(self.mixes[mix_name]["hp"])
            pos = 0
        else:
            value = self.midi_to_db(self.mixes[mix_name][input_name]["vol"])
            pos = self.inputs[input_name]["pos"]

            # solo mutes your instrument for everyone but yourself
            if mix_name != input_name and self.solos.get(input_name,False):
                value = -90 # muted

            # check if mute both local and global
            mute = self.mixes[mix_name][input_name]["mute"]
            if mute != "global" and mute:
                value = -90 # muted
            elif mute == "global" and self.mutes.get(input_name,False):
                value = -90 # muted

        self.mixes[mix_name]["osc"].send_message("/mixer/channel/set_gain", [pos, value])

    def midi_to_db(self, value):
        # this will give a +10 -> -40 db spread
        result = (0.3149606 * value) - 30
        return result

    def get_output_names(self, mix_name):
        if mix_name in self.mixes:
            return [f"{mix_name}:out_left",f"{mix_name}:out_right"]
        else:
            return []
