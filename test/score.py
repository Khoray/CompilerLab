import os, platform, subprocess, shutil, sys

def score_compiler(arg1):
    record = {}

    is_windows = platform.system() == "Windows"

    assert(len(sys.argv) == 2)

    oftype = ""
    step = "-" + arg1
    if step == "-s0":
        oftype = "tk"
    elif step == "-s1":
        oftype = "json"
    elif step == "-s2":
        oftype = "ir"
    else:
        print("illegal input")
        exit()

    output_base = "./output/"
    ref_base = "./ref/" + arg1 + "/"

    score = 0
    total = 0

    if step == "-s0":
        for i in ["basic", "function"]:
            output_dir = output_base + i + '/'
            ref_dir = ref_base + i + '/'
            if os.path.exists(output_dir):
                files = os.listdir(output_dir)
                for file in files:
                    if not (file[-3:] == ".tk"):
                        continue
                    total += 1
                    cmd = ' '.join(["diff", ref_dir + file, output_dir + file])
                    if is_windows:
                        cmd = cmd.replace('/','\\')
                    cp = subprocess.run(cmd, shell=True, stderr=subprocess.DEVNULL, stdout=subprocess.PIPE)
                    if cp.returncode != 0:
                        record[file] = {"retval": cp.returncode, "err_detail": cp.stdout}
                    else:
                        score += 1
                        record[file] = {"retval": 0}
                    print(file, record[file])
        print("score:",score,"/",total)
    elif step == "-s1":
        for i in ["basic", "function"]:
            output_dir = output_base + i + '/'
            ref_dir = ref_base + i + '/'
            if os.path.exists(output_dir):
                files = os.listdir(output_dir)
                for file in files:
                    if not (file[-5:] == ".json"):
                        continue
                    total += 1
                    cmd = ' '.join(["diff", ref_dir + file, output_dir + file])
                    if is_windows:
                        cmd = cmd.replace('/','\\')
                    cp = subprocess.run(cmd, shell=True, stderr=subprocess.DEVNULL, stdout=subprocess.PIPE)
                    if cp.returncode != 0:
                        record[file] = {"retval": cp.returncode, "err_detail": cp.stdout}
                    else:
                        score += 1
                        record[file] = {"retval": 0}
                    print(file, record[file])
        print("score:",score,"/",total)
    else:
        print("TODO")
        exit()
        
    print(score*2)  # for PTA



if __name__ == "__main__":
    assert(len(sys.argv) == 2)
    score_compiler(sys.argv[1])