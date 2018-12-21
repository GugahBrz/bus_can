# bus_can

le fichier representant la carte: /dev/ttyACM*

le fichier representant la sonde: /dev/pcanusb32

### liaison serie
#### Pour la carte Nucleo-F411 / F446 :
> openocd -s /local/sdk_elec/openocd/share/openocd/scripts -f board/st_nucleo_f4.cfg -c init  

#### Pour la carte Nucleo-F103 :
> openocd -s /local/sdk_elec/openocd/share/openocd/scripts -f board/st_nucleo_f10

#### pipocom
> picocom -b 115200 -r -l /dev/ttyACM*

### eclipse
> /local/sdk_elec/eclipse/eclipse 

### qt
> /local/qtcreator*/bin/qtcreator

programme de base dans le projet qt en c++

> Tacos > Kebab
