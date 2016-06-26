import subprocess

print "hello MuddyGB-DSD"

subprocess.call(["make"])

subprocess.call(["vba", "muddygb_dsd.gb"])


