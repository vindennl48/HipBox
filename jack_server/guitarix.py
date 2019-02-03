from jack_help import JackHelp
import os, time


class Guitarix:
    # inputs = { <name>: {"port":<audio port>,"midi_port"(only if needed):<midi port>} }
    def __init__(self, inputs, start_port=5000, isHeadless=True):
        self.inputs     = inputs
        self.start_port = start_port

        for i, name in enumerate(inputs):
            inport      = inputs[name]["port"]
            headless    = f"-N" if isHeadless else ""
            no_connect  = f"-J"
            multiclient = f"-D"
            amp_name    = f"--name={name}_guitarix"
            port        = f"-p {start_port+i}"

            os.system(f"guitarix {headless} {no_connect} {multiclient} {amp_name} {port} &")

            time.sleep(1) # give guitarix a second to load
            JackHelp.connect_port(inport,f"{name}_guitarix:in_0")

            if "midi_port" in inputs[name]:
                midi_port = inputs[name]["midi_port"]
                JackHelp.connect_port(midi_port,f"{name}_guitarix:midi_in_1")

    def get_output_name(self, amp_name):
        if amp_name in self.inputs:
            return f"{amp_name}_guitarix:out_0"
        else:
            return ""
