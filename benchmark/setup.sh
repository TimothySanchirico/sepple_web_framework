
# We use CPU 3 for all of our benchmarks
# 1. Disable CPU scaling
# 2. Disable hyperthreading
# 3. isolcpus cpu 3 so the kernel doesnt run anything there

# Disable the cpu scaling stuff
sudo cpupower 3 frequency-set --governor performance

# turn off hype threading on cpu3 (cpu7 is the ht partner)
sudo bash -c 'echo 0 > /sys/devices/system/cpu/cpu7/online'
