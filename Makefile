run: start_jack rails_detach server

start_jack:
	./jack_server/start_jack.sh

rails_detach:
	rvmsudo rails s -p 80 -b 0.0.0.0 &
	sleep 20

rails:
	rvmsudo rails s -p 80 -b 0.0.0.0

server:
	#python3 jack_server/server.py
	python3 jack_server/main.py
