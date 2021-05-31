# Jetson smart fan
an open source pwm fan program for nvidia jetson devices.
jetson smart fan is implemented in c++ language and still under dev. Pull requests are welcome.

# Sync code
Using git and run 
```
git clone https://github.com/kiah2008/jetson_smart_fan.git
```

# Installation
## compile

you can build it in jetson directly.
PS. you can download cross-compile toolchain and using aarch64 gcc to compile and build the target.

using cmake to help the construction.

command as below:

```
mkdir build
cd build && cmake .. -DCMAKE_INSTALL_PREFIX=/etc/jetson_smart_fan
make
sudo make install
```

# Configure
see more details  in config/pwm_fan.json
```
    [
        {
            "temp_thsd": 25,
            "sample_interval": 1.0,
            "dev_level": 0
        },
        {
            "temp_thsd": 38.5,
            "sample_interval": 0.8,
            "dev_level": 45
        },
        {
            "temp_thsd": 42,
            "sample_interval": 0.5,
            "dev_level": 55
        },
        {
            "temp_thsd": 46,
            "sample_interval": 0.5,
            "dev_level": 90
        },
        {
            "temp_thsd": 55,
            "sample_interval": 0.25,
            "dev_level": 120
        },
        {
            "temp_thsd": 60,
            "sample_interval": 0.25,
            "dev_level": 150
        }
        ,
        {
            "temp_thsd": 100,
            "sample_interval": 0.25,
            "dev_level": 255
        }
    ]
```

# Run
## start service
after install, the service unit will be under /lib/systemd folders.
you can start the service by manual.

```
sudo systemctl daemon-reload
sudo systemctl start jetson_smart_fan
```
## start service when booting
```
sudo systemctl enable jetson_smart_fan
```

## check service status

```
kiah@kiah-jetson:/etc/jetson_smart_fan$ systemctl status jetson_smart_fan
● jetson_smart_fan.service - jetson_stats service
   Loaded: loaded (/lib/systemd/system/jetson_smart_fan.service; enabled; vendor preset: enabled)
   Active: active (running) since Mon 2021-05-31 15:55:30 CST; 32min ago
 Main PID: 16306 (jetson_smart_fa)
    Tasks: 2 (limit: 4177)
   CGroup: /system.slice/jetson_smart_fan.service
           └─16306 /etc/jetson_smart_fan/jetson_smart_fan -s -f /etc/jetson_smart_fan/pwm_fan.json

5月 31 15:55:30 kiah-jetson jetson_smart_fan[16306]: using rec {38.500000c,0.800000s,45} w/ temp 38.000000
5月 31 15:55:30 kiah-jetson jetson_smart_fan[16306]: using dev level 45
5月 31 16:04:58 kiah-jetson jetson_smart_fan[16306]: using rec {42.000000c,0.500000s,60} w/ temp 40.000000
5月 31 16:04:58 kiah-jetson jetson_smart_fan[16306]: using dev level 60
5月 31 16:07:00 kiah-jetson jetson_smart_fan[16306]: using rec {38.500000c,0.800000s,45} w/ temp 38.000000
5月 31 16:07:00 kiah-jetson jetson_smart_fan[16306]: using dev level 45
5月 31 16:24:27 kiah-jetson jetson_smart_fan[16306]: using rec {42.000000c,0.500000s,60} w/ temp 40.000000
5月 31 16:24:27 kiah-jetson jetson_smart_fan[16306]: using dev level 60
5月 31 16:28:06 kiah-jetson jetson_smart_fan[16306]: using rec {38.500000c,0.800000s,45} w/ temp 38.000000
5月 31 16:28:06 kiah-jetson jetson_smart_fan[16306]: using dev level 45
```


# SOURCE CODE CONTRIBUTION

Your contribution to jetson smart fan Project is much appreciated.
Please send patches to me through GitHub.