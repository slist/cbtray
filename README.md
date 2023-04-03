# Carbon Black Cloud systray for Linux

## What can it do? 

* Monitor /var/log/syslog to find Carbon Black events

* Show message when a new log is found in syslog

* Display all Carbon Black messages in the systray

## How to build on Linux CentOS 7

### Install required packages and source code
``` sh
sudo yum install qt5-qtbase qt5-qtbase-devel vim-X11 gcc-c++ qt5-linguist git ImageMagick
git clone https://github.com/slist/cbtray.git
```

### Build
``` sh
cd cbtray
qmake-qt5
make
``` 

### Install

``` sh
sudo cp cbtray /usr/local/bin/
sudo convert images/cbtray.png -resize 24x24 /usr/share/icons/hicolor/24x24/apps/cbtray.png
sudo chmod +r /usr/share/icons/hicolor/24x24/apps/cbtray.png
mkdir -p ~/.config/autostart
cat <<EOF > ~/.config/autostart/cbtray.desktop 
[Desktop Entry]
Type=Application
Name=cbtray
Exec=/usr/local/bin/cbtray
Icon=cbtray
Comment=Carbon Black systray agent
EOF
``` 
Logout / Login in your Gnome desktop environment.


## How to build on Linux Ubuntu 22.04 LTS

### Install required packages and source code
``` sh
sudo apt install -y qtcreator qtbase5-dev qt5-qmake imagemagick
git clone https://github.com/slist/cbtray.git
```

### Build
``` sh
cd cbtray
qmake
make
``` 

### Install

``` sh
sudo cp cbtray /usr/local/bin/
sudo convert images/cbtray.png -resize 24x24 /usr/share/icons/hicolor/24x24/apps/cbtray.png
sudo chmod +r /usr/share/icons/hicolor/24x24/apps/cbtray.png
mkdir -p ~/.config/autostart
cat <<EOF > ~/.config/autostart/cbtray.desktop 
[Desktop Entry]
Type=Application
Name=cbtray
Exec=/usr/local/bin/cbtray
Icon=cbtray
Comment=Carbon Black systray agent
EOF
``` 
Logout / Login in your Gnome desktop environment.

## Test

You can test cbtray with cctest (Carbon Black EICAR file for Linux):
``` sh
wget https://github.com/slist/LinuxMalware/raw/main/cctest
chmod +x cctest
./cctest
./cctest
```



## Uninstall

You can remove it at any time with:
``` sh
rm ~/.config/autostart/cbtray.desktop
```

or using Gnome UI
``` sh
gnome-session-properties 
```


## Resources

* Developer Network website: https://developer.carbonblack.com - contains reference documentation, video tutorials, and how-to guides on how to integrate with Carbon Black products
* CbAPI Python module: https://cbapi.readthedocs.io - our Python module that makes interfacing with the Carbon Black APIs easy. Check it out if you're using Python to interface with Cb products.
* CbAPI Python module source code: https://github.com/carbonblack/cbapi-python - the source code to the CbAPI Python module, including example scripts

## Support

If you have questions, don't hesitate to contact me at slist@vmware.com

## License

Use of this software is governed by the license found in LICENSE.md.
