#!/bin/bash
echo Starting Docker
(/usr/bin/docker-compose -f /home/vagrant/hipbox/docker-compose.yml up -d) && sleep 10
echo Starting Rails
cd /home/vagrant/hipbox && (/home/vagrant/.rvm/gems/ruby-2.5.1/wrappers/rails s)
