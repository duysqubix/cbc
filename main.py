from pathlib import Path 
import json

"""
byte struct per line 
0x123456789 a b c d e f h l pc sp 0x123456789 a b c d e f h l pc sp 0x987654321 (newline)
"""

default_dir = Path("/home/duys/.repos/GameboyCPUTests/v2")

def parse_test(test):

    initial = test["initial"]
    final = test["final"]
    name = test["name"]

    ram_initial = initial["ram"]
    ram_final = final["ram"]

    # print(ram_initial)
    # print(ram_final)
    names = name.split(" ")
    opcode = int(names[0], 16)
    v1 = int(names[1], 16)
    v2 = int(names[2], 16)

    a_i = int(initial["a"])
    b_i = int(initial["b"])
    c_i = int(initial["c"])
    d_i = int(initial["d"])
    e_i = int(initial["e"])
    f_i = int(initial["f"])
    h_i = int(initial["h"])
    l_i = int(initial["l"])
    pc = int(initial["pc"]).to_bytes(2, "big")
    sp = int(initial["sp"]).to_bytes(2, "big")

    ram_values = bytearray()
    for pair in ram_initial:
        addr = int(pair[0])
        val = int(pair[1])
        ram_values.extend(
            (addr.to_bytes(2, "big") + val.to_bytes(1, "big"))
        )
        # print(addr.to_bytes(2, "big"), val.to_bytes(1, "big"))
    if len(ram_initial) < 6:
        remaining = 6 - len(ram_initial)
        ram_values.extend(b"\x00\x00\x00"*remaining)

    
    a_f = int(final["a"])
    b_f = int(final["b"])
    c_f = int(final["c"])
    d_f = int(final["d"])
    e_f = int(final["e"])
    f_f = int(final["f"])
    h_f = int(final["h"])
    l_f = int(final["l"])
    pc_f = int(final["pc"]).to_bytes(2, "big")
    sp_f = int(final["sp"]).to_bytes(2, "big")

    ram_values_final = bytearray()
    for pair in ram_final:
        addr = int(pair[0])
        val = int(pair[1])
        ram_values_final.extend(
            (addr.to_bytes(2, "big") + val.to_bytes(1, "big"))
        )

    if len(ram_final) < 6:
        remaining = 6 - len(ram_final)
        ram_values_final.extend(b"\x00\x00\x00"*remaining)

    b = bytearray()
    line1 = [opcode, v1, v2, a_i, b_i, c_i, d_i, e_i, f_i, h_i, l_i, pc[0], pc[1], sp[0], sp[1]]
    line1_2 = ram_values 
    line2 = [a_f, b_f, c_f, d_f, e_f, f_f, h_f, l_f, pc_f[0], pc_f[1], sp_f[0], sp_f[1]]
    line2_2 = ram_values_final

    if b"\xbe\xef" in line1 or b"\xbe\xef" in line2:
        print(f"Error in {name}")
        exit(1)

    b.extend(line1)
    b.extend(line1_2)
    b.extend(line2)
    b.extend(line2_2)

    # Path("test.bin").write_bytes(b)
    return b
    
    
    
import sys 
import os 


def main():
    filename = os.environ.get("FILENAME")
    bin_content= bytearray()
    for file in default_dir.glob("*.json"):
        if filename and file.name != filename:
            continue
        print(f"Processing {file.name}")
        jdata = json.loads(file.read_bytes())
        for test in jdata:
            b = parse_test(test)
            bin_content.extend(b)
            
        #     break    
        # break
    Path(f"main.bin").write_bytes(bin_content)
    print(f"Wrote {len(bin_content)} bytes to main.bin")
    


if __name__ == "__main__":
    main()
