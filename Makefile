run:
	./jack_server/start_jack.sh
	guitarix -N -J --name=james -p 4000 &
	guitarix -N -J --name=jesse -p 4001 &
	guitarix -N -J --name=mitch -p 4002 &
	rvmsudo rails s -d -p 80 -b 0.0.0.0
	python3 jack_server/jack_server.py
