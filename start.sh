#!/bin/bash
echo Starting Docker
(/usr/bin/docker-compose -f /home/vagrant/hipbox/docker-compose.yml up -d) && sleep 10
echo Starting Rails
cd /home/vagrant/hipbox && (sudo rvmsudo /home/vagrant/.rvm/gems/ruby-2.5.1/wrappers/rails s -b 0.0.0.0 -p 80)
