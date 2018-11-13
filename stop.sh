#!/bin/bash
cd /home/vagrant/hipbox
sudo pkill ruby
/usr/bin/docker-compose down
sudo rm -f /home/vagrant/hb.log /home/vagrant/hb_rails.log /home/vagrant/hb_docker.log /home/vagrant/log.txt
