# HipBox v7.0.0-Beta
The newest version of HipBox.  Rails frontend, C++ Backend.

## Installation Instructions

### Jack2
First, let's install `jack2`.  You might need to switch from Jack1 to Jack2:
```sh
sudo pacman -S --needed jack2
sudo pacman -S --needed realtime-privileges python-dbus cadence
shutdown -r now
```
Next, we need to add the correct groups to the current user:
```sh
sudo usermod -aG audio,realtime $USER
shutdown -r now
```
Lastly, we need to boot up `cadence` and set the jack driver to asla. (this is supposed to be done by `jack_control` inside rails but for what ever reason it is failing at the moment.  Because of that, this is the current work-around)
 - Open Cadence
 - Click on the configure button under 'JACK Status'
 - Click 'ASLA' under the driver list on the left side
 - Set 'Sample Rate' to 44100
 - Set 'Buffer Size' to 128
 - Set 'Periods/Buffer' to 2
 - Press 'OK' button.

You can now exit out of `cadence` and you will never need to open this ever again.

### Ruby on Rails
 - Goto rvm.io and follow the install instructions for ruby and rails.

### Postgresql
```sh
sudo pacman -S --needed postgresql
su
su -l postgres
initdb --locale $LANG -E UTF8 -D '/var/lib/postgres/data'
```
Then exit back to the regular user and start the service:
```sh
sudo systemctl enable postgresql
sudo systemctl start postgresql
```
Now we can add our username and default database
```sh
su
su -l postgres
createuser --interactive #install for your username
createdb <your user name>
```
Exit back to the regular user again and you should be complete.
To test if it works, try `psql` and see if you can log in.

### C++ Compiler and Dev Tools
```sh
sudo pacman -S --needed base-devel  #use default option 'default=all'
sudo pacman -S --needed libsndfile liblo
```
After installing the above, we need to clone the repo and compile the server.
```sh
git clone https://github.com/vindennl48/hipbox
cd hipbox
git checkout v7.0.0
cd aem
make
```

## That's It
Just bundle install rails, create and seed the database, and start it up:
```sh
bundle install
rails db:create db:migrate db:seed
rails s -b 0.0.0.0
```

