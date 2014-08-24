import filecmp
import subprocess

tests = ["test1", "test2"]

def get_file_names(tests):
    return [ dict(input=t+'_input', output= t+'_output') for t in tests]

if __name__ == "__main__":
    files = get_file_names(tests)

    subprocess.call("g++ main.cpp", shell=True)

    for file in files:
        ret = subprocess.call("./a.out " + file['input'] + " > output", shell=True)
        print file['input'], filecmp.cmp(file['output'], 'output')

    subprocess.call("rm output a.out", shell=True)
