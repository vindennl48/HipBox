run: start_jack rails server

start_jack:
	./jack_server/start_jack.sh

rails:
	rvmsudo rails s -p 80 -b 0.0.0.0 &

server:
	python3 jack_server/server.py
